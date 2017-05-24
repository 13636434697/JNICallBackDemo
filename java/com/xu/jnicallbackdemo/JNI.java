package com.xu.jnicallbackdemo;


import android.content.Context;
import android.widget.Toast;

public class JNI {

    static{
        System.loadLibrary("callback");
    }
    /*
     * 特别注意，在mainactivity里面添加本地方法会空指针，因为这个类不是new出来的，是系统自动创建的，会传进来上下文，
     * 没有传上下问，所以空指针，所以mainactivity创建jni的时候，在构造的时候传一个上下文
     *
     * 回调操作界面的时候要注意，要么直接用jni穿过来的activity的对象，本地方法和操作的对象都写在activity里，要么写jni里面的时候要传上下文
     * 当回调的方法跟本地方法不再一个类里面，需要通过刚创建的字节码对象手动创建一个java对象，在通过这个对象来回调java方法
     * */
    private Context mContext;
    public JNI(Context context){
        mContext = context;
    }
    //通过这个方法调用hellofromJava，通过JNI调用C代码，在通过C的方式通过反射的方式在回调到helloFromJava
    //返回的是void
    public native void callbackvoidmethod();
    //返回的是int值
    public native void callbackintmethod();
    //返回字符串
    public native void callbackStringmethod();
    //回调toast方法，字符串通过土司吐出来
    public native void callbackShowToast();

    //C调用java空方法
    public void helloFromJava(){
        System.out.println("hello from java");
    }
    //C调用java中的带两个int参数的方法
    public int add(int x,int y) {
        return x+y;
    }
    //C调用java中参数为string的方法
    public void printString(String s){
        System.out.println(s);
    }
    /*
     * 特别注意，在mainactivity里面添加本地方法会空指针，因为这个类不是new出来的，是系统自动创建的，会传进来上下文，
     * 没有传上下问，所以空指针，所以mainactivity创建jni的时候，在构造的时候传一个上下文
     *
     * 回调操作界面的时候要注意，要么直接用jni穿过来的activity的对象，本地方法和操作的对象都写在activity里，要么写jni里面的时候要传上下文
     * 当回调的方法跟本地方法不再一个类里面，需要通过刚创建的字节码对象手动创建一个java对象，在通过这个对象来回调java方法
     * */
    public void showToast(String s){
        Toast.makeText(mContext, s, Toast.LENGTH_SHORT).show();
    }
}
