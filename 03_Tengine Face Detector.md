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
&emsp;&emsp;简单说来就是这样：java层新建一个mat数据，然后通过调用JNI接口函数传入mat.getNativeObjAddr()就可以将mat数据传进函数，例如：<br>
```
if(RunMobilenet("/data/local/tmp/myface.png")>0)
```          
顾名思义getNativeObjAddr()就是获取本地对象的地址，返回的是一个jlong类型的数据，所以下面我们在JNI进行调用的时候格式是这样的，<br>
activity文件申明：<br>
```
public native float RunMobilenet(long mat);
```
JNI函数定义：<br>
```
JNIEXPORT jfloat JNICALL
Java_com_tengine_openailab_mobilenet_MainActivity_RunMobilenet(
        JNIEnv *env,
        jobject /* this */, jlong mat) {
    cv::Mat javamat = (*(cv::Mat*)mat);
    if( tengine_wrapper->RunMobilenet(javamat) > 0 )
        return 1;
    return 0;
}
```
这里稍微解释一下这句：cv::Mat javamat = (*(cv::Mat*)mat);<br>
jlong mat是一个地址，先将它转换成mat类型指针，然后取该地址的值作为JNI里面mat变量的值，这就实现了mat数据的传递。<br>
&emsp;&emsp;然后我们发现这个新的javamat又被传递到C++/C里面的一个函数叫做RunMobilenet()，下面我们就去看看C++里面是怎么写的。这个RunMobilenet()是class TengineWrapper里面的一个函数，定义在Tengine_Wrapper.h里面。具体的函数在Tengine_Wrapper.cpp里面。<br>
```
float TengineWrapper::RunMobilenet(cv::Mat image)
```
这里就可以看到，它的输入就是一个mat类型的数据。至此就讲完了整个接口的调用情况。<br>
3、底层C++/C函数<br>
这部分就是底层代码啦，主要就在cpp文件夹里，里面是Tengine框架的调用，我们的数据图像处理就会在这里使用。在第二部分将会重点讲述这部分内容。<br>

二、Tengine使用
---
大致上的调用流程如下：
'调用 init_tengine_library 函数初始化'
'调用 load_model 函数载入训练好的模型'
'这里需要指定是哪个框架的模型，如tensorflow、caffe、mxnet、onnx；
需要在先前的安装中配置 makefile.config 文件，解除 CONFIG_XXX_SERIALIZER=y 行的注释来添加对 XXX 模型的支持
调用 create_runtime_graph 函数创建图
调用 get_graph_input_tensor 获取输入Tensor并用 set_tensor_shape 设置输入Tensor的shape
调用 prerun_graph 函数预启动图
调用 get_graph_output_tensor 获取输出Tensor并用 get_tensor_buffer_size 获取输出的shape
向 input_data 写入输入的数据，并调用 set_tensor_buffer 把数据转移到输入Tensor上
调用 run_graph 运行图（做一次前向传播）
调用 get_graph_output_tensor 获取输出Tensor并用 get_tensor_buffer 取得缓冲区上的数据
最后在退出程序前依次释放各个申请的动态空间

他们所对应的代码如下，基本都在Tengine_Wrapper.cpp里面：







三、Tengine_FaceDetector
---
最后再讲述一下这个Tengine_FaceDetector的基本流程吧：<br>
（1）Java层使用一个org.opencv.android.JavaCameraView布局来调用摄像头获取图像<br>
（2）使用opencv自带的分类器实现人脸框图<br>
（3）将人脸框图灰度图传给底层使用tensorflow模型（model3model.pb文件）输出人脸的15个特征点坐标，15个特征点坐标在模型输出的 1*30数组元素里面。分别是x1 y1 x2 y2 ......<br>
（4）得到15个特征点的数据以后可以在java层画出特征点位置。<br>
