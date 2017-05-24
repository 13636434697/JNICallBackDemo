#include <jni.h>
#include <stdlib.h>
#include <android/log.h>
#define LOG_TAG "System.out"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)


/**
 * 这是一个工具方法
 * 把一个jstring转换成一个c语言的char* 类型.
 */
char* _JString2CStr(JNIEnv* env, jstring jstr) {
    //先声明了一个char类型的指针，指向了null就是0，就是一个没有用的地址
    char* rtn = NULL;
    //反射找到字节码，String这个类的字节码对象
    //接收的是string类型，所以把gb2312，转换成char*类型，用这个方法
    jclass clsstring = (*env)->FindClass(env, "java/lang/String");
    //把GB2312转换成了jstring类型
    jstring strencode = (*env)->NewStringUTF(env,"GB2312");
    //通过字节码拿到String的一些方法getBytes的方法。有四个方法，因为有方法签名，所以就能确定，
    // 这里找的是：中括号是数组，B是byte数组，接收的参数是string类型。
    jmethodID mid = (*env)->GetMethodID(env, clsstring, "getBytes", "(Ljava/lang/String;)[B");
    //因为返回值是数组，数组在C里面属于object，所以都调用这个方法，传进来java字符串，就能调用找到的getbyte方法，
    //mid是刚才的方法名，strencode是刚才的gb2312，
    // 拿到的返回值是jbyteArray转换成byte数组，因为c的插入类型是1个字节，javabyte也是一个
    jbyteArray barr = (jbyteArray)(*env)->CallObjectMethod(env, jstr, mid, strencode); // String .getByte("GB2312");
    //获取的数组的长度
    jsize alen = (*env)->GetArrayLength(env, barr);
    //拿到数组元素的首地址，jbyte就是char，转化成char类型的指针，c的字符串就可以用char指针来表示
    jbyte* ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);
    //如果这个字符串是有内容的
    if(alen > 0) {
        //申请一块堆内存，用来存放要生成c的字符串，alen是java字符串的长度+1是用来存放c的字符数组的结束符
        rtn = (char*)malloc(alen+1); //"\0"
        //内存拷贝，把byte数组的内容拷贝到rtn申请的堆内存中（参数：申请堆内存的指针，要拷贝进来btye数组的指针，要拷贝进来的数组的长度）
        memcpy(rtn, ba, alen);
        //结束符放进来
        rtn[alen]=0;
    }
    //最后释放一下
    (*env)->ReleaseByteArrayElements(env, barr, ba,0);
    //里面就有内容之后返回出去
    return rtn;
}


//第二个参数，是调用本地函数的javah类对象，clazz就是jni对象，可以直接调用方法，不用通过字节码对象创建object
JNIEXPORT void JNICALL Java_com_xu_jnicallbackdemo_JNI_callbackvoidmethod(JNIEnv * env, jobject clazz){
    //jclass      (*FindClass)(JNIEnv*, const char*);
    //① 获取字节码对象,(具体的类)
    jclass claz = (*env)->FindClass(env,"com/xu/jnicallbackdemo/JNI");
    //②获取Method对象
    //jmethodID   (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);（回调方法名，方法的签名：到classz目录，javap -s,因为java里面可以重名的）
    jmethodID methodID =(*env)->GetMethodID(env,claz,"helloFromJava","()V");

    //③通过字节码对象创建一个Object，
    //由于本地方法，回调的javah方法都写在了jni这个类里，在调用调方法的时候，jni就传递的对象进来，
    //而这个对象正好是jni的对象，反射的方法正好是jni的实例方法，不用在去创建一个对象了，直接传进来了

    //④通过对象调用方法
    //void        (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
    //因为要调用的方法，没有返回的，所以调用这个方法就可以了
    //参数，env指针，clazz调用方法的javah对象，用哪个对象来调用这个method，上面已经创建好了，。。。调用这个方法时候传递的参数
    (*env)->CallVoidMethod(env,clazz,methodID);
}

JNIEXPORT void JNICALL Java_com_xu_jnicallbackdemo_JNI_callbackintmethod  (JNIEnv * env, jobject clazz){
    //① 获取字节码对象
    jclass claz =(*env)->FindClass(env,"com/xu/jnicallbackdemo/JNI");
    //②获取Method对象(第三个方法名，第四个方法签名)
    jmethodID methodID = (*env)->GetMethodID(env,claz,"add","(II)I");
    //jint        (*CallIntMethod)(JNIEnv*, jobject, jmethodID, ...);
    //调用方法，调用int的方法，接收参数后面2个参数是jint就是int
    int result =(*env)->CallIntMethod(env,clazz,methodID,3,4);
    LOGD("result = %d",result);
}

JNIEXPORT void JNICALL Java_com_xu_jnicallbackdemo_JNI_callbackStringmethod (JNIEnv * env, jobject clazz){
    //① 获取字节码对象
    jclass claz =(*env)->FindClass(env,"com/xu/jnicallbackdemo/JNI");
    //② 获取Method对象
    jmethodID methodid =(*env)->GetMethodID(env,claz,"printString","(Ljava/lang/String;)V");

    jstring result =(*env)->NewStringUTF(env,"hello from c");
    //还是void空的CallVoidMethod，因为需要string，所以需要转一下
    (*env)->CallVoidMethod(env,clazz,methodid,result);
}

/*
 * 特别注意，在mainactivity里面添加本地方法会空指针，因为这个类不是new出来的，是系统自动创建的，会传进来上下文，
 * 没有传上下问，所以空指针，所以mainactivity创建jni的时候，在构造的时候传一个上下文
 *
 * 回调操作界面的时候要注意，要么直接用jni穿过来的activity的对象，本地方法和操作的对象都写在activity里，要么写jni里面的时候要传上下文
 * 当回调的方法跟本地方法不再一个类里面，需要通过刚创建的字节码对象手动创建一个java对象，在通过这个对象来回调java方法
 * */
JNIEXPORT void JNICALL Java_com_xu_jnicallbackdemo_JNI_callbackShowToast (JNIEnv * env, jobject clazz){
    //通过的反射的方式找到了字节码，用类对象调用了toast方法
    jclass claz =(*env)->FindClass(env,"com/xu/jnicallbackdemo/JNI");
    //方法签名，通过方法签名就能确定方法，接收一个string类型的参数，返回值是void（最后哪个参数）
    jmethodID methodid =(*env)->GetMethodID(env,claz,"showToast","(Ljava/lang/String;)V");
    //注释掉的前面几个方法都没有用，这里用到了，就是方法和本地方法不再一个类
    //jobject     (*AllocObject)(JNIEnv*, jclass);
    //通过字节码对象创建 java对象 在这儿就是创建了mainactivity的对象
    //jobject obj =(*env)->AllocObject(env,claz);
    jstring result =(*env)->NewStringUTF(env,"hello from c");
    //void        (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
    //调用方法：参数2应该传obj是对象，其他都一样
    //(*env)->CallVoidMethod(env,clazz,methodid,result);
    (*env)->CallVoidMethod(env,clazz,methodid,result);
}

