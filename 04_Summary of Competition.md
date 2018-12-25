04 Summary of Competition(20181224)
===


Abstract: 这是最后一篇关于这次Arm比赛的文章，作为总结，我想解释一下之前的几篇文章的思路，还要补充一下使用Tengine过程中遇到的问题，最后讲讲这个比赛感觉上还有的不足之处和一些结果吧。

一、之前文章的思路：
实际上之前几篇文章都是按照这个比赛的完成顺序来的，在03当中讲述了整个Tengine_FaceDetector的框架，这个框架的顺序也正好是按照这个比赛的完成顺序与文章的顺序来的：
（1）首先你要做安卓肯定还是要用到Android Studio的吧，所以00讲述了在安装Android Studio上的一些问题；
（2）然后尝试用Tengine在Android上的部署，所以有01这篇文章，但是用的模型是不一样的，是物体分类的模型，并没有用到人脸检测；
（3）有一天队友告诉我人脸检测要用到OpenCV库，于是又学习了一下OpenCV在Android上面的装载，所以有02这篇文章讲述了Android_OpenCV
（4）最后我们将之前的总和起来就实现了03_Tengine_FaceDetector

二、补充使用Tengine过程中遇到的问题
1、模型load不成功
一开始load模型的时候发现不成功，究其原因原来是模型里面有些函数还不支持，tips和Github上有一些框架所支持的OP列表，除此之外还有一些函数虽然没有列出但是Tengine还是支持的，比如tensorflow的Reshape函数。
这里也介绍一个查看模型结构的利器（此处感谢郑楷）：
https://github.com/lutzroeder/netron
