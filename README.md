Summary of Arm Competition
===

Abstract: 这是一篇关于这次Arm比赛的文章，我想解释一下发在Github上的几篇文章的思路；补充一下使用Tengine过程中遇到的问题，最后讲讲这个比赛还有的不足之处和一些结果吧。

一、几篇文章的思路：
---
实际上之前几篇文章都是按照这个比赛的完成顺序来的，在03当中讲述了整个Tengine_FaceDetector的框架，这个框架的顺序也正好是按照这个比赛的完成顺序与文章的顺序来的：<br>
（1）首先你要做安卓肯定还是要用到Android Studio的吧，所以00讲述了在安装Android Studio上的一些问题；<br>
（2）然后尝试用Tengine在Android上的部署，所以有01这篇文章，但是并没有用到人脸检测；<br>
（3）有一天队友告诉我要用到OpenCV库，于是又学习了一下OpenCV在Android上面的装载，所以有02这篇文章<br>
（4）最后我们将之前的总和起来就实现了03_Tengine_FaceDetector<br>
（5）04其实就是我们对这场比赛的总结了！<br>

 00_Some_problems_when_installing_Android_Studio<br>
 01_Tengine_Android<br>
 02_Android_OpenCV<br>
 03_Tengine_FaceDetector<br>
 04_Summary_of_Competition<br>
 
二、整体框架
---
那我们就从这个项目的最终完成版说起吧，之前说了本文主要以解读代码为主，那就先来看看整个项目工程的框架。主要是分成这么几部分，我们一一介绍：<br>
1、Android平台的java语言，这部分主要是对安卓手机界面进行编程，看着《第一行代码》一步步走下去就行了，这部分算是顶层代码吧~，主要就是activity文件以及界面xml文件。<br>

2、JNI接口，这是个啥玩意呢？请看词条：https://baike.so.com/doc/508314-538197.html<br>
意思也就是说，JNI可以当做一个接口，使得Java语言能够调用底层的C++/C语言，也可以是其他语言。要使用这些接口需要首先进行函数申明，一般是在MainActivity类的最后；然后呢这些函数就链接到一个叫做native-lib.cpp的文件当中；这里其实你就会发现一些东西，比如在JNI里面函数的定义与申明的不同，首先是数据类型的转换，java与JNI当中是一一对应的；这里没有提到数组，其实JNI里面有特定的数组格式：比如jobjectArray、jbooleanArray、jbyteArray、jcharArray、jshortArray、jintArray、jlongArray、jfloatArray、jdoubleArray等，还有jclass、jstring、jthrowable等。那么其实这里就有个问题了，为什么顶层的java语言能够调用JNI接口文件呢？在MainActivity的一开始就load了这个名字叫做native-lib的库。所以他们能够链接起来。<br>

3、底层C++/C函数<br>
这部分就是底层代码啦，主要就在cpp文件夹里，里面是Tengine框架的调用，我们的数据图像处理就会在这里使用。在第二部分将会重点讲述这部分内容。<br>

三、Tengine使用
---
调用 init_tengine_library 函数初始化<br>
调用 load_model 函数载入训练好的模型<br>
这里需要指定是哪个框架的模型，如tensorflow、caffe、mxnet、onnx；<br>
需要在先前的安装中配置 makefile.config 文件，解除 CONFIG_XXX_SERIALIZER=y 行的注释来添加对 XXX 模型的支持<br>
调用 create_runtime_graph 函数创建图<br>
调用 get_graph_input_tensor 获取输入Tensor并用 set_tensor_shape 设置输入Tensor的shape<br>
调用 prerun_graph 函数预启动图<br>
调用 get_graph_output_tensor 获取输出Tensor并用 get_tensor_buffer_size 获取输出的shape<br>
向 input_data 写入输入的数据，并调用 set_tensor_buffer 把数据转移到输入Tensor上<br>
调用 run_graph 运行图（做一次前向传播）<br>
调用 get_graph_output_tensor 获取输出Tensor并用 get_tensor_buffer 取得缓冲区上的数据<br>
最后在退出程序前依次释放各个申请的动态空间<br>

四、Tengine_FaceDetector
---
最后再讲述一下这个Tengine_FaceDetector的基本流程吧：<br>
（1）Java层使用一个org.opencv.android.JavaCameraView布局来调用摄像头获取图像<br>
（2）使用opencv自带的分类器实现人脸框图<br>
（3）将人脸框图灰度图传给底层使用tensorflow模型（model3model.pb文件）输出人脸的15个特征点坐标，15个特征点坐标在模型输出的 1*30数组元素里面。分别是x1 y1 x2 y2 ......<br>
（4）得到15个特征点的数据以后可以在java层画出特征点位置。<br>

五、关于Code
---
(1)activity_main.xml文件：手机app的页面布局文件<br>
(2)MainActivity.java文件：主活动java文件<br>
(3)model3model.pb文件：tensorflow模型文件<br>
(4)native-lib.cpp文件：JNI文件<br>
(5)tengine_c_api.h文件：Tengine API库文件<br>
(6)Tengine_Wrapper.cpp文件：Tengine_Wrapper调用具体函数<br>
(7)Tengine_Wrapper.h文件：Tengine_Wrapper库文件<br>
