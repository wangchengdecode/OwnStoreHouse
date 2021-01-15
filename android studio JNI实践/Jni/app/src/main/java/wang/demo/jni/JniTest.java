package wang.demo.jni;

public class JniTest {

    static {
        System.loadLibrary("jni-demo");
    }
    public static native String hello();
}
