Android+OpenCV(20181216)
===

一、安装OpenCV过程：
---
安装教程网上很多，我一开始参考的是以下这篇博文：
https://blog.csdn.net/gao_chun/article/details/49359535
但是这里有两个问题：<br>
（1）他的示例代码中没有给全，实际上要能够运行的话需要在前面添加许多import库，比如博客里面的代码需要添加：（可以根据底下的错误提示来选择）<br>
```
package com.tengine.openailab.mobilenet;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.graphics.BitmapFactory;
import android.view.View;
import android.graphics.Bitmap;
import android.widget.Button;
import android.widget.ImageView;
import android.util.Log;

import butterknife.BindView;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.imgproc.Imgproc;
```
（2）第二个没有解决的问题是OpenCV manager安装的问题，上面博客里面提到了这一点，但是没有说如何解决，在实际运行当中如果没有安装这个会在手机上运行的时候出现下面的错误提示：<br>
```
Package not found

OpenCV is not ready
```
而且这个OpenCV manager已经显示下架了，想安装也装不上（oppo手机如此），于是需要另寻他解，有许多博客说不用安装manager也可以实现，但是最后各种方法试下来，下面这种方法是我最终成功的：
https://www.cnblogs.com/tail/p/4618790.html
意思是OpenCV在AS3.0版本里所给出的示例程序就说可以免OpenCV安装，它在初始代码里面的onResume函数当中是这样写的：<br>
```
public void onResume()
    {
        super.onResume();
        if (!OpenCVLoader.initDebug()) {
            Log.d(TAG, "Internal OpenCV library not found. Using OpenCV Manager for initialization");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_0_0, this, mLoaderCallback);
        } else {
            Log.d(TAG, "OpenCV library found inside package. Using it!");
            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
    }
```
直接替换亲测有效。

二、OpenCV实现人脸检测
---
&emsp;&emsp;这部分讲述一下使用opencv自带的人脸检测模型来检测人脸实现方式，其实很简单，但是中间过程有许多问题，现在也有很多问题还没有解决。该部分实例参考TryOpenCV_SinglePictureagain目录下面的工程。<br>
【1】准备运动：这里就是之前1中所说的将opencv当中的sdk装载到AS工程下面，然后要将opencv当中自带的人脸检测模型拷贝到app下面的raw目录：app/src/main/res/raw，这个自带的模型在opencv安装包当中的目录.<br>
【2】然后我们需要修改三个地方的代码，这些代码都可以参考opencv安装包里面的samples例程，<br>
***第一个***是activity.java文件：需要修改这几个地方：<br>
（1）package的名字修改成自己的：<br>
```
package com.tengine.openailab.mobilenet;
```
（2）activity的名字修改成自己的：<br>
```
public class MainActivity extends Activity implements CvCameraViewListener2 
```
（3）因为这部分代码既可以使用JAVA_DETECTOR，也可以使用NATIVE_DETECTOR，但是NATIVE_DETECTOR在这里还不能用，因为sample代码是有另外一个.java文件来描述这个NATIVE_DETECTOR检测器的，于是我们需要将原来代码里面的与NATIVE_DETECTOR相关的都注释掉：
```
// System.loadLibrary("detection_based_tracker");

//mNativeDetector = new DetectionBasedTracker(mCascadeFile.getAbsolutePath(), 0);

//mNativeDetector.setMinFaceSize(mAbsoluteFaceSize);

//if (mNativeDetector != null)
    //mNativeDetector.detect(mGray, faces);

if (type == NATIVE_DETECTOR) {
    Log.i(TAG, "Detection Based Tracker enabled");
    //mNativeDetector.start();
} else {
    Log.i(TAG, "Cascade detector enabled");
    //mNativeDetector.stop();
}
```
***第二个文件***是布局文件activity_main.xml，这里我是直接复制过来的，就建立了一个JavaCameraView用作摄像头采集界面：（不晓得这个linearlayout对界面有没有影响？）
```
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent" >

        <org.opencv.android.JavaCameraView
            android:layout_width="fill_parent"
            android:layout_height="fill_parent"
            android:id="@+id/fd_activity_surface_view" />
</LinearLayout>
```

***第三个文件***是权限文件AndroidManifest.xml，主要需要开通几个权限：<br>
（1）摄像头权限（但是其实在这里开通之后在手机上还是需要在允许调用摄像头操作）
```
<uses-permission android:name="android.permission.CAMERA" />

<uses-feature 
android:name="android.hardware.camera" 
android:required="false" />
<uses-feature 
android:name="android.hardware.camera.autofocus"
    android:required="false" />
<uses-feature 
android:name="android.hardware.camera.front"   
android:required="false" />
<uses-feature
    android:name="android.hardware.camera.front.autofocus"
    android:required="false" />
```
（2）设置手机横屏，不设置的话手机是倒的屏幕
```
<activity android:name=".MainActivity"
    android:screenOrientation="landscape">  <!--landscape:设置手机横屏-->
```
【3】这样以后下载到手机上就可以进行人脸框图了。
