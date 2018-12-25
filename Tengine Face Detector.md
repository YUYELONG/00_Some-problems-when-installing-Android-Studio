Tengine Face Detector(20181223)
===

&emsp;&emsp;Preface: 在之前学习了Tengine在安卓上的相关配置(01_Tengine_Android)和Android Studio上使用OpenCV之后终于步入正题，进入我们这个比赛的内容，检测人脸的特征点。之前其实都没有好好看过代码，所以这篇文章就作为Tengine_FaceDetector的代码解读吧！其实东西做完了却不知从哪里说起，回想起来这几天做的很多事情，却非常杂乱，行文之间若有逻辑不清之处还望海涵。<br>

一、整体框架
---
&emsp;&emsp;那我们就从这个项目的最终完成版说起吧，之前说了本文主要以解读代码为主，那就先来看看整个项目工程的框架。主要是分成这么几部分，我们一一介绍：<br>
1、Android平台的java语言，这部分主要是对安卓手机界面进行编程，看着《第一行代码》一步步走下去就行了，这部分算是顶层代码吧~，主要就是activity文件以及界面xml文件。<br>
2、JNI接口，这是个啥玩意呢？请看词条：https://baike.so.com/doc/508314-538197.html<br>
意思也就是说，JNI可以当做一个接口，使得Java语言能够调用底层的C++/C语言，也可以是其他语言。要使用这些接口需要首先进行函数申明，一般是在MainActivity类的最后：<br>
```
public native String stringFromJNI();
public native int TengineWrapperInit();
public native float RunMobilenet(String file);
public native float TengineWrapperGetTop1();
```
然后呢这些函数就链接到一个叫做native-lib.cpp的文件当中：  
```
JNIEXPORT jint JNICALL
Java_com_tengine_openailab_mobilenet_MainActivity_TengineWrapperInit(
        JNIEnv *env,
        jobject /* this */) {
    tengine_wrapper = new TengineWrapper();
    if(!tengine_wrapper)
        return 1;
    int ret = tengine_wrapper->InitMobilenet();
    if( ret!= 0) {
        return ret;
    }
    return 0;
}
```
这里其实你就会发现一些东西，比如在JNI里面函数的定义与申明的不同，首先是数据类型的转换，java与JNI当中是一一对应的。JNI里面有特定的数组格式：比如jobjectArray、jbooleanArray、jbyteArray、jcharArray、jshortArray、jintArray、jlongArray、jfloatArray、jdoubleArray等，还有jclass、jstring、jthrowable等。那么其实这里就有个问题了，为什么顶层的java语言能够调用JNI接口文件呢？这就是下面这段话的功劳：
```
public class MainActivity extends Activity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
```
看到了吧这里在MainActivity的一开始就load了这个名字叫做native-lib的库。所以他们能够链接起来。  
&emsp;&emsp;那么在做这个项目的时候呢就有一个问题出现了：顶层activity文件中如果我要输入一个图片文件如Mat格式的数据到底层去处理，数据接口应该怎么写呢？这个问题曾经困扰过，因为一开始的时候我是用Mat格式的数据直接传的，后来发现这样传递数据根本没有传到底层，而且还会导致app闪退，幸好这个已经有前人做过了，下面这篇博文就讲述了java层到JNI的Mat数据的接口处理：<br>
https://blog.csdn.net/brcli/article/details/76407986<br>
https://blog.csdn.net/pplxlee/article/details/52713311<br>
&emsp;&emsp;简单说来就是这样：java层新建一个mat数据，然后通过调用JNI接口函数传入mat.getNativeObjAddr()就可以将mat数据传进函数，






