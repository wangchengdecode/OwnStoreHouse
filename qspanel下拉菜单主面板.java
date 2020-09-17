/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.systemui.qs;

import static android.content.res.Configuration.ORIENTATION_LANDSCAPE;
import static com.android.systemui.qs.tileimpl.QSTileImpl.getColorForState;

import android.annotation.Nullable;
import android.content.ComponentName;
import android.content.Context;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.media.AudioAttributes;
import android.media.AudioManager;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.metrics.LogMaker;
import android.net.Uri;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings;
import android.service.quicksettings.Tile;
import android.util.AttributeSet;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.content.BroadcastReceiver;
import android.content.IntentFilter;
import android.content.Intent;
import android.content.ContentResolver;
import com.android.internal.logging.MetricsLogger;
import com.android.internal.logging.nano.MetricsProto.MetricsEvent;
import com.android.settingslib.Utils;
import com.android.systemui.Dependency;
import com.android.systemui.R;
import com.android.systemui.plugins.qs.DetailAdapter;
import com.android.systemui.plugins.qs.QSTile;
import com.android.systemui.plugins.qs.QSTileView;
import com.android.systemui.qs.QSHost.Callback;
import com.android.systemui.qs.customize.QSCustomizer;
import com.android.systemui.qs.external.CustomTile;
import com.android.systemui.settings.BrightnessController;
import com.android.systemui.settings.ToggleSeekBar;
import com.android.systemui.settings.ToggleSlider;
import com.android.systemui.settings.ToggleSliderView;
import com.android.systemui.statusbar.policy.BrightnessMirrorController;
import com.android.systemui.statusbar.policy.BrightnessMirrorController.BrightnessMirrorListener;
import com.android.systemui.tuner.TunerService;
import com.android.systemui.tuner.TunerService.Tunable;

import java.util.ArrayList;
import java.util.Collection;
import android.database.ContentObserver;
import android.provider.Settings;

/** View that represents the quick settings tile panel (when expanded/pulled down). **/
public class QSPanel extends LinearLayout implements Tunable, Callback, BrightnessMirrorListener {
    // REQ103,systemui:ui,wmd,2020.0323
    private static final String TAG = QSPanel.class.getSimpleName();

    public static final String QS_SHOW_BRIGHTNESS = "qs_show_brightness";
    public static final String QS_SHOW_HEADER = "qs_show_header";
    private static final String VOLUME_CHANGED_ACTION = "android.media.VOLUME_CHANGED_ACTION";
    ToggleSliderView mMusicslider,mNoticicationslider;
    protected final Context mContext;
    protected final ArrayList<TileRecord> mRecords = new ArrayList<>();
    protected final View mBrightnessView;
    // REQ103,systemui:ui,wmd,2020.0323
    protected final View mVolumeView;
    protected final View mNotiVolumeView;
    private AudioManager mAudioManager;
    private Ringtone mRingtone;
    private Ringtone mNotiRingtone;
    private Uri mDefaultUri;
    private final H mHandler = new H();
    private final MetricsLogger mMetricsLogger = Dependency.get(MetricsLogger.class);
    private final QSTileRevealController mQsTileRevealController;

    protected boolean mExpanded;
    protected boolean mListening;

    private QSDetail.Callback mCallback;
    private BrightnessController mBrightnessController;
    protected QSTileHost mHost;

    protected QSSecurityFooter mFooter;
    private PageIndicator mPanelPageIndicator;
    private PageIndicator mFooterPageIndicator;
    private boolean mGridContentVisible = true;
    protected QSTileLayout mTileLayout;

    private QSCustomizer mCustomizePanel;
    private Record mDetailRecord;

    private BrightnessMirrorController mBrightnessMirrorController;
    private View mDivider;

    // PLM14161, ZFY SILENT MODE, wmd, 200622
    final static Uri ZFY_SILENT_MODE_URI = Settings.Global.getUriFor("persist.sys.zfy.silent");

    public QSPanel(Context context) {
        this(context, null);
    }

    public QSPanel(Context context, AttributeSet attrs) {
        super(context, attrs);
        mContext = context;

        setOrientation(VERTICAL);

        mBrightnessView = LayoutInflater.from(mContext).inflate(
            R.layout.quick_settings_brightness_dialog, this, false);
        ((TextView)mBrightnessView.findViewById(R.id.slider_tips)).setText(mContext.getResources().getString(R.string.brightness_slider_tips));
        addView(mBrightnessView);

        // REQ103,systemui:ui,wmd,2020.0323
        mAudioManager = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
        mDefaultUri =Uri.parse(ContentResolver.SCHEME_ANDROID_RESOURCE + "://"
                + getContext().getPackageName()
                + "/" + R.raw.common_ding);
        mRingtone = RingtoneManager.getRingtone(mContext, mDefaultUri);
        mNotiRingtone = RingtoneManager.getRingtone(mContext, mDefaultUri);
        mRingtone.setStreamType(AudioManager.STREAM_MUSIC);
        mNotiRingtone.setStreamType(AudioManager.STREAM_NOTIFICATION);
        int maxVolume = mAudioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
        int maxNotiVolume = mAudioManager.getStreamMaxVolume(AudioManager.STREAM_NOTIFICATION);
        mVolumeView = LayoutInflater.from(mContext).inflate(
                R.layout.quick_settings_brightness_dialog, this, false);
        // REQ232,system:status bar icons,wmd,2020.04
        ((ImageView) mVolumeView.findViewById(R.id.brightness_icon)).setImageResource(R.drawable.ic_qs_volume_icon_left);
        ((ImageView) mVolumeView.findViewById(R.id.brightness_icon_right)).setImageResource(R.drawable.ic_qs_volume_icon_right);
        ((TextView)mVolumeView.findViewById(R.id.slider_tips)).setText(mContext.getResources().getString(R.string.volume_slider_tips));
        addView(mVolumeView);

        mNotiVolumeView = LayoutInflater.from(mContext).inflate(
                R.layout.quick_settings_brightness_dialog, this, false);
        ((ImageView) mNotiVolumeView.findViewById(R.id.brightness_icon)).setImageResource(R.drawable.ic_qs_volume_icon_left);
        ((ImageView) mNotiVolumeView.findViewById(R.id.brightness_icon_right)).setImageResource(R.drawable.ic_qs_volume_icon_right);
        ((TextView)mNotiVolumeView.findViewById(R.id.slider_tips)).setText(mContext.getResources().getString(R.string.notivolume_slider_tips));
        addView(mNotiVolumeView);
        mMusicslider = mVolumeView.findViewById(R.id.brightness_slider);
        mMusicslider.setMax(maxVolume);//拖动条平滑处理
        mMusicslider.setOnChangedListener(new ToggleSlider.Listener(){
            @Override
            public void onInit(ToggleSlider control){
                // do nothing
            }
            @Override
            public void onChanged(ToggleSlider slider, boolean tracking, boolean automatic, int value,
                           boolean stopTracking){
                mHandler.removeMessages(H.STOP_RINGTONE_SAMPLE);
                if(stopTracking){
                    mHandler.sendEmptyMessage(H.START_RINGTONE_SAMPLE);
//                    Log.d(TAG, "ToggleSliderView onChanged value:" + value);
                    mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC, value, 0);
                }
            }
        });
        int currVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
        mMusicslider.setValue(currVolume);

        mNoticicationslider  = mNotiVolumeView.findViewById(R.id.brightness_slider);
        mNoticicationslider.setMax(maxNotiVolume);
        mNoticicationslider.setOnChangedListener(new ToggleSlider.Listener(){
            @Override
            public void onInit(ToggleSlider control){
                // do nothing
            }
            @Override
            public void onChanged(ToggleSlider slider, boolean tracking, boolean automatic, int value,
                                  boolean stopTracking){
                mHandler.removeMessages(H.STOP_NOTIRINGTONE_SAMPLE);
                if(stopTracking){
                    mHandler.sendEmptyMessage(H.START_NOTIRINGTONE_SAMPLE);
//                    Log.d(TAG, "ToggleSliderView onChanged value:" + value);
                    mAudioManager.setStreamVolume(AudioManager.STREAM_NOTIFICATION, value, 0);
                }
            }
        });
        int currNotiVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_NOTIFICATION);
        mNoticicationslider.setValue(currNotiVolume);

        IntentFilter filter = new IntentFilter() ;
        filter.addAction("android.media.VOLUME_CHANGED_ACTION") ;
        context.registerReceiver(mVolumeReceiver, filter) ;
        mTileLayout = (QSTileLayout) LayoutInflater.from(mContext).inflate(
                R.layout.qs_paged_tile_layout, this, false);
        mTileLayout.setListening(mListening);
        addView((View) mTileLayout);

        mPanelPageIndicator = (PageIndicator) LayoutInflater.from(context).inflate(
                R.layout.qs_page_indicator, this, false);
        addView(mPanelPageIndicator);

        ((PagedTileLayout) mTileLayout).setPageIndicator(mPanelPageIndicator);
        mQsTileRevealController = new QSTileRevealController(mContext, this,
                (PagedTileLayout) mTileLayout);

        addDivider();

        mFooter = new QSSecurityFooter(this, context);
        addView(mFooter.getView());

        mBrightnessController = new BrightnessController(getContext(),
                findViewById(R.id.brightness_icon),
                findViewById(R.id.brightness_slider));

        updateResources();

        // PLM14161, ZFY SILENT MODE, wmd, 200622
        updateViewsForZFYSilentMode();
        mContext.getContentResolver().registerContentObserver(ZFY_SILENT_MODE_URI, false,
                mZFYSilentModeObserver);

    }

    // PLM14161, ZFY SILENT MODE, wmd, 200622
    private void updateViewsForZFYSilentMode(){
        if(1 == Settings.Global.getInt(mContext.getContentResolver(), "persist.sys.zfy.silent",0)){
            mHandler.obtainMessage(H.SET_ZFY_SILENT_MODE_EBNALED).sendToTarget();
        }else{
            mHandler.obtainMessage(H.SET_ZFY_SILENT_MODE_DISABLED).sendToTarget();
        }
    }

    private ContentObserver mZFYSilentModeObserver = new ContentObserver(null) {
        @Override
        public void onChange(boolean selfChange) {
            super.onChange(selfChange);
            updateViewsForZFYSilentMode();
        }
    };

    protected final BroadcastReceiver mVolumeReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals("android.media.VOLUME_CHANGED_ACTION")) {
                int currVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_MUSIC);//
                mMusicslider.setValue(currVolume);
            }
            if (intent.getAction().equals("android.media.VOLUME_CHANGED_ACTION")) {
                int currVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_NOTIFICATION);//
                mNoticicationslider.setValue(currVolume);
            }
        }
    };

    protected void addDivider() {
        mDivider = LayoutInflater.from(mContext).inflate(R.layout.qs_divider, this, false);
        mDivider.setBackgroundColor(Utils.applyAlpha(mDivider.getAlpha(),
                getColorForState(mContext, Tile.STATE_ACTIVE)));
        addView(mDivider);
    }

    public View getDivider() {
        return mDivider;
    }

    public View getPageIndicator() {
        return mPanelPageIndicator;
    }

    public QSTileRevealController getQsTileRevealController() {
        return mQsTileRevealController;
    }

    public boolean isShowingCustomize() {
        return mCustomizePanel != null && mCustomizePanel.isCustomizing();
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        final TunerService tunerService = Dependency.get(TunerService.class);
        tunerService.addTunable(this, QS_SHOW_BRIGHTNESS);

        if (mHost != null) {
            setTiles(mHost.getTiles());
        }
        if (mBrightnessMirrorController != null) {
            mBrightnessMirrorController.addCallback(this);
        }
    }

    @Override
    protected void onDetachedFromWindow() {
        Dependency.get(TunerService.class).removeTunable(this);
        if (mHost != null) {
            mHost.removeCallback(this);
        }
        for (TileRecord record : mRecords) {
            record.tile.removeCallbacks();
        }
        if (mBrightnessMirrorController != null) {
            mBrightnessMirrorController.removeCallback(this);
        }
        super.onDetachedFromWindow();
    }

    @Override
    public void onTilesChanged() {
        setTiles(mHost.getTiles());
    }

    @Override
    public void onTuningChanged(String key, String newValue) {
        if (QS_SHOW_BRIGHTNESS.equals(key)) {
            updateViewVisibilityForTuningValue(mBrightnessView, newValue);
        }
    }

    private void updateViewVisibilityForTuningValue(View view, @Nullable String newValue) {
        view.setVisibility(newValue == null || Integer.parseInt(newValue) != 0 ? VISIBLE : GONE);
    }

    public void openDetails(String subPanel) {
        QSTile tile = getTile(subPanel);
        showDetailAdapter(true, tile.getDetailAdapter(), new int[]{getWidth() / 2, 0});
    }

    private QSTile getTile(String subPanel) {
        for (int i = 0; i < mRecords.size(); i++) {
            if (subPanel.equals(mRecords.get(i).tile.getTileSpec())) {
                return mRecords.get(i).tile;
            }
        }
        return mHost.createTile(subPanel);
    }

    public void setBrightnessMirror(BrightnessMirrorController c) {
        if (mBrightnessMirrorController != null) {
            mBrightnessMirrorController.removeCallback(this);
        }
        mBrightnessMirrorController = c;
        if (mBrightnessMirrorController != null) {
            mBrightnessMirrorController.addCallback(this);
        }
        updateBrightnessMirror();
    }

    @Override
    public void onBrightnessMirrorReinflated(View brightnessMirror) {
        updateBrightnessMirror();
    }

    View getBrightnessView() {
        return mBrightnessView;
    }

    public void setCallback(QSDetail.Callback callback) {
        mCallback = callback;
    }

    public void setHost(QSTileHost host, QSCustomizer customizer) {
        mHost = host;
        mHost.addCallback(this);
        setTiles(mHost.getTiles());
        mFooter.setHostEnvironment(host);
        mCustomizePanel = customizer;
        if (mCustomizePanel != null) {
            mCustomizePanel.setHost(mHost);
        }
    }

    /**
     * Links the footer's page indicator, which is used in landscape orientation to save space.
     *
     * @param pageIndicator indicator to use for page scrolling
     */
    public void setFooterPageIndicator(PageIndicator pageIndicator) {
        if (mTileLayout instanceof PagedTileLayout) {
            mFooterPageIndicator = pageIndicator;
            updatePageIndicator();
        }
    }

    private void updatePageIndicator() {
        if (mTileLayout instanceof PagedTileLayout) {
            // If we're in landscape, and we have the footer page indicator (which we should if the
            // footer has been initialized & linked), then we'll show the footer page indicator to
            // save space in the main QS tile area. Otherwise, we'll use the default one under the
            // tiles/above the footer.
            boolean shouldUseFooterPageIndicator =
                    getResources().getConfiguration().orientation == ORIENTATION_LANDSCAPE
                            && mFooterPageIndicator != null;

            mPanelPageIndicator.setVisibility(View.GONE);
            if (mFooterPageIndicator != null) {
                mFooterPageIndicator.setVisibility(View.GONE);
            }

            ((PagedTileLayout) mTileLayout).setPageIndicator(
                    shouldUseFooterPageIndicator ? mFooterPageIndicator : mPanelPageIndicator);
        }
    }

    public QSTileHost getHost() {
        return mHost;
    }

    public void updateResources() {
        final Resources res = mContext.getResources();
        setPadding(0, res.getDimensionPixelSize(R.dimen.qs_panel_padding_top), 0, res.getDimensionPixelSize(R.dimen.qs_panel_padding_bottom));

        updatePageIndicator();

        if (mListening) {
            refreshAllTiles();
        }
        if (mTileLayout != null) {
            mTileLayout.updateResources();
        }
    }

    @Override
    protected void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        mFooter.onConfigurationChanged();

        updateBrightnessMirror();
    }

    public void updateBrightnessMirror() {
        if (mBrightnessMirrorController != null) {
            ToggleSliderView brightnessSlider = findViewById(R.id.brightness_slider);
            ToggleSliderView mirrorSlider = mBrightnessMirrorController.getMirror()
                    .findViewById(R.id.brightness_slider);
            brightnessSlider.setMirror(mirrorSlider);
            brightnessSlider.setMirrorController(mBrightnessMirrorController);
        }
    }

    public void onCollapse() {
        if (mCustomizePanel != null && mCustomizePanel.isShown()) {
            mCustomizePanel.hide(mCustomizePanel.getWidth() / 2, mCustomizePanel.getHeight() / 2);
        }
    }

    public void setExpanded(boolean expanded) {
        if (mExpanded == expanded) return;
        mExpanded = expanded;
        if (!mExpanded && mTileLayout instanceof PagedTileLayout) {
            ((PagedTileLayout) mTileLayout).setCurrentItem(0, false);
        }
        mMetricsLogger.visibility(MetricsEvent.QS_PANEL, mExpanded);
        if (!mExpanded) {
            closeDetail();
        } else {
            logTiles();
        }
    }

    public void setPageListener(final PagedTileLayout.PageListener pageListener) {
        if (mTileLayout instanceof PagedTileLayout) {
            ((PagedTileLayout) mTileLayout).setPageListener(pageListener);
        }
    }

    public boolean isExpanded() {
        return mExpanded;
    }

    public void setListening(boolean listening) {
        if (mListening == listening) return;
        mListening = listening;
        if (mTileLayout != null) {
            mTileLayout.setListening(listening);
        }
        mFooter.setListening(mListening);
        if (mListening) {
            refreshAllTiles();
        }
        if (mBrightnessView.getVisibility() == View.VISIBLE) {
            if (listening) {
                mBrightnessController.registerCallbacks();
            } else {
                mBrightnessController.unregisterCallbacks();
            }
        }
    }

    public void refreshAllTiles() {
        mBrightnessController.checkRestrictionAndSetEnabled();
        for (TileRecord r : mRecords) {
            r.tile.refreshState();
        }
        mFooter.refreshState();
    }

    public void showDetailAdapter(boolean show, DetailAdapter adapter, int[] locationInWindow) {
        int xInWindow = locationInWindow[0];
        int yInWindow = locationInWindow[1];
        ((View) getParent()).getLocationInWindow(locationInWindow);

        Record r = new Record();
        r.detailAdapter = adapter;
        r.x = xInWindow - locationInWindow[0];
        r.y = yInWindow - locationInWindow[1];

        locationInWindow[0] = xInWindow;
        locationInWindow[1] = yInWindow;

        showDetail(show, r);
    }

    protected void showDetail(boolean show, Record r) {
        mHandler.obtainMessage(H.SHOW_DETAIL, show ? 1 : 0, 0, r).sendToTarget();
    }

    public void setTiles(Collection<QSTile> tiles) {
        setTiles(tiles, false);
    }

    public void setTiles(Collection<QSTile> tiles, boolean collapsedView) {
        if (!collapsedView) {
            mQsTileRevealController.updateRevealedTiles(tiles);
        }
        for (TileRecord record : mRecords) {
            mTileLayout.removeTile(record);
            record.tile.removeCallback(record.callback);
        }
        mRecords.clear();
        for (QSTile tile : tiles) {
            addTile(tile, collapsedView);
        }
    }

    protected void drawTile(TileRecord r, QSTile.State state) {
        r.tileView.onStateChanged(state);
    }

    protected QSTileView createTileView(QSTile tile, boolean collapsedView) {
        return mHost.createTileView(tile, collapsedView);
    }

    protected boolean shouldShowDetail() {
        return mExpanded;
    }

    protected TileRecord addTile(final QSTile tile, boolean collapsedView) {
        final TileRecord r = new TileRecord();
        r.tile = tile;
        r.tileView = createTileView(tile, collapsedView);
        final QSTile.Callback callback = new QSTile.Callback() {
            @Override
            public void onStateChanged(QSTile.State state) {
                drawTile(r, state);
            }

            @Override
            public void onShowDetail(boolean show) {
                // Both the collapsed and full QS panels get this callback, this check determines
                // which one should handle showing the detail.
                if (shouldShowDetail()) {
                    QSPanel.this.showDetail(show, r);
                }
            }

            @Override
            public void onToggleStateChanged(boolean state) {
                if (mDetailRecord == r) {
                    fireToggleStateChanged(state);
                }
            }

            @Override
            public void onScanStateChanged(boolean state) {
                r.scanState = state;
                if (mDetailRecord == r) {
                    fireScanStateChanged(r.scanState);
                }
            }

            @Override
            public void onAnnouncementRequested(CharSequence announcement) {
                if (announcement != null) {
                    mHandler.obtainMessage(H.ANNOUNCE_FOR_ACCESSIBILITY, announcement)
                            .sendToTarget();
                }
            }
        };
        r.tile.addCallback(callback);
        r.callback = callback;
        r.tileView.init(r.tile);
        r.tile.refreshState();
        mRecords.add(r);

        if (mTileLayout != null) {
            mTileLayout.addTile(r);
        }

        return r;
    }


    public void showEdit(final View v) {
        v.post(new Runnable() {
            @Override
            public void run() {
                if (mCustomizePanel != null) {
                    if (!mCustomizePanel.isCustomizing()) {
                        int[] loc = new int[2];
                        v.getLocationInWindow(loc);
                        int x = loc[0] + v.getWidth() / 2;
                        int y = loc[1] + v.getHeight() / 2;
                        mCustomizePanel.show(x, y);
                    }
                }

            }
        });
    }

    public void closeDetail() {
        if (mCustomizePanel != null && mCustomizePanel.isShown()) {
            // Treat this as a detail panel for now, to make things easy.
            mCustomizePanel.hide(mCustomizePanel.getWidth() / 2, mCustomizePanel.getHeight() / 2);
            return;
        }
        showDetail(false, mDetailRecord);
    }

    public int getGridHeight() {
        return getMeasuredHeight();
    }

    protected void handleShowDetail(Record r, boolean show) {
        if (r instanceof TileRecord) {
            handleShowDetailTile((TileRecord) r, show);
        } else {
            int x = 0;
            int y = 0;
            if (r != null) {
                x = r.x;
                y = r.y;
            }
            handleShowDetailImpl(r, show, x, y);
        }
    }

    private void handleShowDetailTile(TileRecord r, boolean show) {
        if ((mDetailRecord != null) == show && mDetailRecord == r) return;

        if (show) {
            r.detailAdapter = r.tile.getDetailAdapter();
            if (r.detailAdapter == null) return;
        }
        r.tile.setDetailListening(show);
        int x = r.tileView.getLeft() + r.tileView.getWidth() / 2;
        int y = r.tileView.getDetailY() + mTileLayout.getOffsetTop(r) + getTop();
        handleShowDetailImpl(r, show, x, y);
    }

    private void handleShowDetailImpl(Record r, boolean show, int x, int y) {
        setDetailRecord(show ? r : null);
        fireShowingDetail(show ? r.detailAdapter : null, x, y);
    }

    protected void setDetailRecord(Record r) {
        if (r == mDetailRecord) return;
        mDetailRecord = r;
        final boolean scanState = mDetailRecord instanceof TileRecord
                && ((TileRecord) mDetailRecord).scanState;
        fireScanStateChanged(scanState);
    }

    void setGridContentVisibility(boolean visible) {
        int newVis = visible ? VISIBLE : INVISIBLE;
        setVisibility(newVis);
        if (mGridContentVisible != visible) {
            mMetricsLogger.visibility(MetricsEvent.QS_PANEL, newVis);
        }
        mGridContentVisible = visible;
    }

    private void logTiles() {
        for (int i = 0; i < mRecords.size(); i++) {
            QSTile tile = mRecords.get(i).tile;
            mMetricsLogger.write(tile.populate(new LogMaker(tile.getMetricsCategory())
                    .setType(MetricsEvent.TYPE_OPEN)));
        }
    }

    private void fireShowingDetail(DetailAdapter detail, int x, int y) {
        if (mCallback != null) {
            mCallback.onShowingDetail(detail, x, y);
        }
    }

    private void fireToggleStateChanged(boolean state) {
        if (mCallback != null) {
            mCallback.onToggleStateChanged(state);
        }
    }

    private void fireScanStateChanged(boolean state) {
        if (mCallback != null) {
            mCallback.onScanStateChanged(state);
        }
    }

    public void clickTile(ComponentName tile) {
        final String spec = CustomTile.toSpec(tile);
        final int N = mRecords.size();
        for (int i = 0; i < N; i++) {
            if (mRecords.get(i).tile.getTileSpec().equals(spec)) {
                mRecords.get(i).tile.click();
                break;
            }
        }
    }

    QSTileLayout getTileLayout() {
        return mTileLayout;
    }

    QSTileView getTileView(QSTile tile) {
        for (TileRecord r : mRecords) {
            if (r.tile == tile) {
                return r.tileView;
            }
        }
        return null;
    }

    public QSSecurityFooter getFooter() {
        return mFooter;
    }

    public void showDeviceMonitoringDialog() {
        mFooter.showDeviceMonitoringDialog();
    }

    public void setMargins(int sideMargins) {
        for (int i = 0; i < getChildCount(); i++) {
            View view = getChildAt(i);
            if (view != mTileLayout) {
                LayoutParams lp = (LayoutParams) view.getLayoutParams();
                lp.leftMargin = sideMargins;
                lp.rightMargin = sideMargins;
            }
        }
    }

    private class H extends Handler {
        private static final int SHOW_DETAIL = 1;
        private static final int SET_TILE_VISIBILITY = 2;
        private static final int ANNOUNCE_FOR_ACCESSIBILITY = 3;
        // REQ103,systemui:volume
        private static final int STOP_RINGTONE_SAMPLE = 4;
        private static final int START_RINGTONE_SAMPLE = 5;
        // PLM14161, ZFY SILENT MODE, wmd, 200622
        private static final int SET_ZFY_SILENT_MODE_EBNALED = 6;
        private static final int SET_ZFY_SILENT_MODE_DISABLED = 7;
        //wc
        private static final int START_NOTIRINGTONE_SAMPLE = 8;
        private static final int STOP_NOTIRINGTONE_SAMPLE = 4;

        @Override
        public void handleMessage(Message msg) {
            if (msg.what == SHOW_DETAIL) {
                handleShowDetail((Record) msg.obj, msg.arg1 != 0);
            } else if (msg.what == ANNOUNCE_FOR_ACCESSIBILITY) {
                announceForAccessibility((CharSequence) msg.obj);
            }
            // REQ103,systemui:volume
            else if(msg.what == STOP_RINGTONE_SAMPLE){
                if (mRingtone != null) {
                    mRingtone.stop();
                }
            }
            else if(msg.what == STOP_NOTIRINGTONE_SAMPLE)
            {
                if(mNotiRingtone != null )
                {
                    mNotiRingtone.stop();
                }
            }
            else if(msg.what == START_RINGTONE_SAMPLE) {
                if (mRingtone != null) {
                    try {
                        mRingtone.setAudioAttributes(new AudioAttributes.Builder(mRingtone
                                .getAudioAttributes())
                                .setFlags(AudioAttributes.FLAG_BYPASS_MUTE)
                                .build());
                        mRingtone.play();
                        Log.d(TAG, "RINGTONE PLAYING ");
                        mHandler.sendEmptyMessageDelayed(STOP_RINGTONE_SAMPLE, 3000);
                    } catch (Throwable e) {
                        Log.w(TAG, "Error playing ringtone, stream " + AudioManager.STREAM_MUSIC, e);
                    }
                }
            }
            else if(msg.what == START_NOTIRINGTONE_SAMPLE) {
                if (mNotiRingtone != null) {
                    try {
                        mNotiRingtone.setAudioAttributes(new AudioAttributes.Builder(mNotiRingtone
                                .getAudioAttributes())
                                .setFlags(AudioAttributes.FLAG_BYPASS_MUTE)
                                .build());
                        mNotiRingtone.play();
                        Log.d(TAG, "NOTIRINGTONE PLAYING ");
                        mHandler.sendEmptyMessageDelayed(STOP_NOTIRINGTONE_SAMPLE, 3000);
                    } catch (Throwable e) {
                        Log.w(TAG, "Error playing ringtone, stream " + AudioManager.STREAM_NOTIFICATION, e);
                    }
                }
            }

            // PLM14161, ZFY SILENT MODE, wmd, 200622
            else if(msg.what == SET_ZFY_SILENT_MODE_EBNALED){
                Log.d(TAG, "updateViewsForZFYSilentMode zfy silent: true");
                ToggleSliderView toggleSliderView = (ToggleSliderView)mBrightnessView.findViewById(R.id.brightness_slider);
                if(toggleSliderView != null) {
                    ToggleSeekBar wrapper = (ToggleSeekBar)toggleSliderView.findViewById(R.id.slider);
                    if(wrapper != null) {
                        wrapper.setAlpha(0.3f);
                        wrapper.setEnabled(false);
                        wrapper.setClickable(false);
                    }
                }
                toggleSliderView = (ToggleSliderView)mVolumeView.findViewById(R.id.brightness_slider);
                if(toggleSliderView != null) {
                    ToggleSeekBar wrapper = (ToggleSeekBar)toggleSliderView.findViewById(R.id.slider);
                    if(wrapper != null) {
                        wrapper.setAlpha(0.3f);
                        wrapper.setEnabled(false);
                        wrapper.setClickable(false);
                    }
                }
                toggleSliderView = (ToggleSliderView)mNotiVolumeView.findViewById(R.id.brightness_slider);
                if(toggleSliderView != null) {
                    ToggleSeekBar wrapper = (ToggleSeekBar)toggleSliderView.findViewById(R.id.slider);
                    if(wrapper != null) {
                        wrapper.setAlpha(0.3f);
                        wrapper.setEnabled(false);
                        wrapper.setClickable(false);
                    }
                }
            }else if(msg.what == SET_ZFY_SILENT_MODE_DISABLED){
                Log.d(TAG, "updateViewsForZFYSilentMode zfy silent: false");
                ToggleSliderView toggleSliderView = (ToggleSliderView)mBrightnessView.findViewById(R.id.brightness_slider);
                if(toggleSliderView != null) {
                    ToggleSeekBar wrapper = (ToggleSeekBar)toggleSliderView.findViewById(R.id.slider);
                    if(wrapper != null) {
                        wrapper.setAlpha(1.0f);
                        wrapper.setEnabled(true);
                        wrapper.setClickable(true);
                    }
                }
                toggleSliderView = (ToggleSliderView)mVolumeView.findViewById(R.id.brightness_slider);
                if(toggleSliderView != null) {
                    ToggleSeekBar wrapper = (ToggleSeekBar)toggleSliderView.findViewById(R.id.slider);
                    if(wrapper != null) {
                        wrapper.setAlpha(1.0f);
                        wrapper.setEnabled(true);
                        wrapper.setClickable(true);
                    }
                }
                toggleSliderView = (ToggleSliderView)mNotiVolumeView.findViewById(R.id.brightness_slider);
                if(toggleSliderView != null) {
                    ToggleSeekBar wrapper = (ToggleSeekBar)toggleSliderView.findViewById(R.id.slider);
                    if(wrapper != null) {
                        wrapper.setAlpha(1.0f);
                        wrapper.setEnabled(true);
                        wrapper.setClickable(true);
                    }
                }
            }
        }
    }

    protected static class Record {
        DetailAdapter detailAdapter;
        int x;
        int y;
    }

    public static final class TileRecord extends Record {
        public QSTile tile;
        public com.android.systemui.plugins.qs.QSTileView tileView;
        public boolean scanState;
        public QSTile.Callback callback;
    }

    public interface QSTileLayout {
        void addTile(TileRecord tile);

        void removeTile(TileRecord tile);

        int getOffsetTop(TileRecord tile);

        boolean updateResources();

        void setListening(boolean listening);

        default void setExpansion(float expansion) {}
    }
}
