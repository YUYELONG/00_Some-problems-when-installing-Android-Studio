Tengine Face Detector(20181223)
===

Preface: 在之前学习了Tengine在安卓上的相关配置(01_Tengine_Android)和Android Studio上使用OpenCV之后终于步入正题，进入我们这个比赛的内容，检测人脸的特征点。之前其实都没有好好看过代码，所以这篇文章就作为Tengine_FaceDetector的代码解读吧！其实东西做完了却不知从哪里说起，回想起来这几天做的很多事情，却非常杂乱，行文之间若有逻辑不清之处还望海涵。<br>

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
