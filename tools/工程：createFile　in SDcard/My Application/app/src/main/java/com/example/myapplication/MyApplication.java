package com.example.myapplication;

import android.app.Activity;
import android.os.Bundle;
import android.os.StatFs;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;
import java.io.File;

public class MyApplication extends Activity  {

    private final static String TAG = "MyApplication";
    private Button bt1,bt2;
    private final static int FILECOUNT = 15;
    private String rootDir = "/storage/9016-4EF8";
    private TextView tv1_t,tv2_t,tv3,before,after;
    private Spinner spinner1,spinner2;
    private  final File sdCardDir = new File("storage/9016-4EF8/DCIM/000000/1");
    CreateFile cf = new CreateFile();

    // The first rear facing camera
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        bindview();
        init();
        update("初始化剩余：");

        bt1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                File[] filenames = sdCardDir.listFiles();
                before.setText("删除前总剩余： "+getemptyGB()+"Gb");
                if(filenames.length == 0)
                    Toast.makeText(getBaseContext(),"file not exist，add first",Toast.LENGTH_SHORT).show();
                else if(filenames.length >= FILECOUNT){
                    Log.d(TAG,"-----------------------------------------------------------------------------------------");
                    for(int i =0 ; i < FILECOUNT; i ++) {
                        String str = getemptyGB();
                        filenames[i].delete();
                        tv2_t.setText("共15个已删除文件！");
                        Log.d(TAG,filenames[i].toString()+"文件被已删除！");
                        Log.d(TAG,"之前："+str+"Gb");
                        Log.d(TAG,"删除后剩余："+getemptyGB()+"Gb");
                    }
                }
                else{
                    Log.d(TAG,"-----------------------------------------------------------------------------------------");
                    for(int i =0 ; i < filenames.length; i ++) {
                        String str = getemptyGB();
                        filenames[i].delete();
                        tv2_t.setText("共"+filenames.length+"个已删除文件！");
                        Log.d(TAG,filenames[i].toString()+"文件被已删除！");
                        Log.d(TAG,"之前："+str+"Gb");
                        Log.d(TAG,"删除后剩余："+getemptyGB()+"Gb");
                    }
                }
                update("删除后总剩余：");
            }

        });
        bt2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final int spase = Integer.parseInt(spinner1.getSelectedItem().toString());
                final String unit = spinner2.getSelectedItem().toString();
                tv2_t.setText("增加中...");
                tv2_t.setTextColor(getResources().getColor(R.color.red));
                before.setText("增加前剩余： "+getemptyGB()+"Gb");
                new Thread(){
                    @Override
                    public void run()
                    {
                        super.run();
                        Log.d(TAG,"--------------------------------------------------------------------------------------------");
                        for( int i =0 ;i < FILECOUNT ; i ++ ) {
                            switch (unit) {
                                case "KB":
                                    String str1 = getemptyGB();
                                    String str2 = "storage/9016-4EF8/DCIM/000000/1/"+spase + unit +
                                            "(" + System.currentTimeMillis() % 1000+").mp4";
                                    cf.createFile(str2, spase, CreateFile.FileUnit.KB);
                                    Log.d(TAG,str2+"文件已添加!");
                                    Log.d(TAG,"之前："+str1+"Gb");
                                    Log.d(TAG,"添加之后剩余："+getemptyGB()+"Gb");
                                    break;
                                case "MB":
                                    String str3 = getemptyGB();
                                    String str4 = "storage/9016-4EF8/DCIM/000000/1/"+spase + unit +
                                            "(" + System.currentTimeMillis() % 1000+").mp4";
                                    cf.createFile(str4, spase, CreateFile.FileUnit.MB);
                                    Log.d(TAG,str4+"文件已添加!");
                                    Log.d(TAG,"之前："+str3+"Gb");
                                    Log.d(TAG,"添加之后剩余："+getemptyGB()+"Gb");
                                    break;
                                case "GB":
                                    String str5 = getemptyGB();
                                    String str6 = "storage/9016-4EF8/DCIM/000000/1/"+spase + unit +
                                            "(" + System.currentTimeMillis() % 1000+").mp4";
                                    cf.createFile(str6, spase, CreateFile.FileUnit.GB);
                                    Log.d(TAG,str6+"文件已添加!");
                                    Log.d(TAG,"之前："+str5+"Gb");
                                    Log.d(TAG,"添加之后剩余："+getemptyGB()+"Gb");
                                    break;
                                default:
                                    break;
                            }
                        }
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    tv2_t.setText("10个"+spase + unit + "大小文件增加完毕！");
                                    tv2_t.setTextColor(getResources().getColor(R.color.green));
                                    update("增加后剩余：");
                                }
                            });

                    }
                }.start();
//
            }
        });


    }

    public String getemptyGB(){
        try{
            StatFs file = new StatFs(rootDir);
            long tsize = file.getAvailableBlocksLong() * file.getBlockSizeLong();
            float gbsize = (float)tsize / (1024 * 1024 * 1024);
            return String.format("%.2f",gbsize);
        }catch (Exception e)
        {

        }
        return "-1";
    }

    public void init()
    {
        if(!sdCardDir.exists())
            sdCardDir.mkdir();
    }

    public void bindview()
    {
        bt1 = findViewById(R.id.bt1);
        tv1_t = findViewById(R.id.bt1_tip);
        tv2_t = findViewById(R.id.bt2_tip);
        bt2 = findViewById(R.id.bt2);
        tv3 = findViewById(R.id.tv3);
        before = findViewById(R.id.before);
        after = findViewById(R.id.after);
        spinner1 = findViewById(R.id.spinner1);
        spinner2 = findViewById(R.id.spinner2);
    }

    public void update(String s)
    {
        String str =" ";
        File[] filenames = sdCardDir.listFiles();
        after.setText(s+getemptyGB()+"Gb");
        tv1_t.setText("目录下目前文件数："+filenames.length+"个");
        for(int i = 0 ; i < filenames.length ; i ++)
        {
            str = str + filenames[i].toString().replace("storage/9016-4EF8","") + "\n";
        }
        tv3.setText(str);
    }

}
