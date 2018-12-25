01 Tengine_Android（20181205）
===

1、open the existing project<br>
2、运行可能会显示：NDK not configured.<br>
这是由于SDK更新的问题，因为在SDK manager当中显示的就是在dl.google.com上下载的SDK更新，实际上我们用的都是国内网站，至于怎么上google进行下载也不清楚。<br>

根据网上的一些讨论和博客发现遇到这个问题可以尝试修改一下hosts，这与翻墙可能有关。<br>
https://blog.csdn.net/xwq911/article/details/41823947<br>

3、运行时可能还会显示错误：Failed to find Cmake.<br>
这也是更新问题，SDK manager 当中需要安装这几个更新：NDK,LLDB,CMake<br>

https://blog.csdn.net/weixin_41189380/article/details/79049045<br>
（据说gradle failed是经常的事情，还有上述更新的问题与网络有关，多试几次就过去了）<br>

4、以上是在configure build上遇到的问题，上述解决办法可以排除这几个问题configure build成功，但是在Run tasks的时候又会遇到不同的问题：<br>


阅读错误提示以后发现这是大概由于缺少了libtengine.so文件，于是乎我们就要去阅读一下tengine的东西：<br>
build_android文档链接如下：<br>
https://github.com/OAID/Tengine/blob/master/doc/build_android.md<br>

根据我的理解，按照上述说明结合例程大致意思是这样：<br>
（1）首先需要编译Tengine（这部分需要在linux系统下面操作，我的ubuntu系统尚且搞	不懂，所以下面说的so文件是从公司那里拿的开源文件），生成一个libtengine.so	文件，将其放在以下目录：<br>
project目录/app/src/main/jnilibs/libtengine.so<br>
这样以后可以将整个工程文件编译成功，包括Run tasks部分<br>

（2）其次，有一些文件需要准备：<br>
【1】生成的模型文件：frozen_mobilenet_v1_224.pb（.pb文件是指tensorflow框架	 下训练的模型，与此类似的有caffemodel文件的caffe模型,还有其他各种框架）<br>

【2】测试数据集：<br>

【3】label标签文件：synset_words.txt<br>
（3）根据build_android文档说明的第7、8步需要将上述所说的数据通过adb传输到手机	上，不过文档当中说的要install adb与adb_driver都是不需要的，因为Android 	Studio软件已经帮我们安装好了这些，具体地址在SDK安装目录下面（这个具体后面	部分会说）：C:\Users\yuyelong\AppData\Local\Android\Sdk\platform-tools<br>


（：这里先说一下，一开始我因为不知道流程所以将上述文件都放在电脑端的project	工程文件夹下面，结果运行的时候出现错误：


（4）那就说说怎么使用adb传输file到手机上吧，实际上可以进行双向传输：教程可以参考：http://adbshell.com/commands/adb-devices<br>

5、在将一些文件通过adb传送至手机上以后，就可以将工程进行下载到手机上了，结果总是好的!
