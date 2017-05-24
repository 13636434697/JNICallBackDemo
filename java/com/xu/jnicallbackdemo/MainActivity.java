package com.xu.jnicallbackdemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
/*
* c回调java的方法，后台耗时操作的通知
*
* */
public class MainActivity extends AppCompatActivity {
    JNI jni;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        jni = new JNI(getApplicationContext());
    }

    public void callbackvoid(View v){
        jni.callbackvoidmethod();
    }

    public void callbackint(View v){
        jni.callbackintmethod();
    }

    public void callbackString(View v){
        jni.callbackStringmethod();
    }

    public void callbackshowtoast(View v){
        jni.callbackShowToast();
    }
    //public native void callbackShowToast();

}