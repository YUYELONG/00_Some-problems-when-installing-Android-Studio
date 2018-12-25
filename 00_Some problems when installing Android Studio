00 Some problems when installing Android Studio
===

Problem1：no emulator installed
---
这个问题可能是因为没有把电脑上的杀毒软件关掉，导致运行模拟器的时候被移除了。衷心希望大家不要遇到我这个问题!
```
SDK folder location：
C:\Users\yuyelong\AppData\Local\Android\Sdk\extras\intel\Hardware_Accelerated_Execution_Manager
```
```
“emulator” and ”system-images” folder is located at:
C:\Users\yuyelong\AppData\Local\Android\Sdk
```
https://stackoverflow.com/questions/40181450/android-studio-emulator-error-intel-haxm-is-required-no-emulator-installed

Problem2：enable VT ( Intel virtualization technology)-x in your bios security
---

进入BIOS将Intel virtualization technology这一项使能即可。
https://www.cnblogs.com/sunyl/p/5915144.html

  

***针对oppo r11手机Android Studio的一次开发经历（调用摄像头20181128）***


1、使用butterknife
https://blog.csdn.net/qq_31852701/article/details/80312719
https://blog.csdn.net/qq_33792946/article/details/53816552
https://blog.csdn.net/u010937230/article/details/60762472
https://blog.csdn.net/androidwubo/article/details/78835706
https://blog.csdn.net/qq_31852701/article/details/80312719

项目下面的build.gradle

app下面的build.gradle

2、activity code:
https://blog.csdn.net/qq_33792946/article/details/54094307
package com.example.yuyelong.helloword;

import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.Button;
import android.widget.ImageView;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class HelloWorldActivity extends AppCompatActivity {
    @BindView(R.id.button_1)
    Button button_1;
    @BindView(R.id.imageIV)
    ImageView imageIV;

    private final int CAMERA_REQUEST = 8888;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_hello_world);
        ButterKnife.bind(this);
    }

    @OnClick(R.id.button_1)
    public void onClick() {
        Intent cameraIntent = new Intent(android.provider.MediaStore.ACTION_IMAGE_CAPTURE);
        startActivityForResult(cameraIntent, CAMERA_REQUEST);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == CAMERA_REQUEST && resultCode == RESULT_OK) {
            Bitmap photo = (Bitmap) data.getExtras().get("data");
            imageIV.setImageBitmap(photo);
        }
    }
}
3、针对oppo r11手机安装apk问题：
在gradle.properties上加一句：这样以后安装没问题，但是还是会出现闪退的现象。
android.injected.testOnly=false

oppo手机安装apk后闪退的处理：
https://blog.csdn.net/zhao8856234/article/details/79958867
在settings里面去掉下面这个功能：


生成签名文件请参考《第一行代码》15.1
其中key store path的路径除了文件夹再加上自己的新命名jks文件即可 

4、打开oppo手机的开发者选项参考：版本号点五下，还有这种骚操作真是
https://jingyan.baidu.com/article/a501d80c37cc83ec620f5e5d.html
https://blog.csdn.net/yeyuewushang/article/details/80179803

5、结果
  
