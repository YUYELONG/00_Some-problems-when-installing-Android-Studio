#include <jni.h>
#include <string>
#include "Tengine_Wrapper.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include "opencv2/imgcodecs.hpp"


#define  LOG_TAG    "JNI_PART"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG, __VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, __VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG, __VA_ARGS__)
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG_TAG, __VA_ARGS__)

extern "C" {

TengineWrapper *tengine_wrapper;

JNIEXPORT jstring JNICALL
Java_com_tengine_openailab_mobilenet_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT jint JNICALL
Java_com_tengine_openailab_mobilenet_MainActivity_TengineWrapperInit(
        JNIEnv *env,
        jobject /* this */) {
    tengine_wrapper = new TengineWrapper();
    if(!tengine_wrapper)
        return 10;
    int ret = tengine_wrapper->InitMobilenet();
    if( ret!= 0) {
        return ret;
    }
    return 0;
}

JNIEXPORT jint JNICALL
Java_com_tengine_openailab_mobilenet_MainActivity_TengineWrapperRelease(
        JNIEnv *env,
        jobject /* this */) {
    tengine_wrapper->ReleaseMobilenet();
    delete tengine_wrapper;
    return 0;
}

//JNIEXPORT jint JNICALL
//Java_com_tengine_openailab_mobilenet_MainActivity_RunMobilenet(
//        JNIEnv *env,
//        jobject /* this */, jstring file) {
//    const char* image_file = env->GetStringUTFChars(file,0);
//    env->ReleaseStringUTFChars(file, image_file);
//    if( tengine_wrapper->RunMobilenet(image_file) > 0 )
//        return 1;
//    return 0;
//}


//JNIEXPORT jint JNICALL
//Java_com_tengine_openailab_mobilenet_MainActivity_RunMobilenet(
//        JNIEnv *env,
//        jobject /* this */, jlong mat) {
////    const char* image_file = env->GetStringUTFChars(file,0);
////    env->ReleaseStringUTFChars(file, image_file);
//    cv::Mat javamat = (*(cv::Mat*)mat);
////    if( tengine_wrapper->RunMobilenet(javamat) > 0 )
////        return 1;
//    return tengine_wrapper->RunMobilenet(javamat) ;
//    //return 0;
//}
JNIEXPORT jfloat JNICALL
Java_com_tengine_openailab_mobilenet_MainActivity_RunMobilenet(
        JNIEnv *env,
        jobject /* this */, jlong mat) {
//    const char* image_file = env->GetStringUTFChars(file,0);
//    env->ReleaseStringUTFChars(file, image_file);
    cv::Mat javamat = (*(cv::Mat*)mat);
    if( tengine_wrapper->RunMobilenet(javamat) > 0 )
        return 1;
    //return tengine_wrapper->RunMobilenet(javamat) ;
    return 0;
}

//JNIEXPORT jint JNICALL
//Java_com_tengine_openailab_mobilenet_MainActivity_RunMobilenet(
//        JNIEnv *env,
//        jobject /* this */, jlong mat) {
//    cv::Mat javamat = (*(cv::Mat*)mat);
//    return tengine_wrapper->RunMobilenet(javamat);
//}


//JNIEXPORT jstring JNICALL
//Java_com_tengine_openailab_mobilenet_MainActivity_TengineWrapperGetTop1(
//        JNIEnv *env,
//        jobject /* this */) {
//
//    std::string top1 = tengine_wrapper->GetTop1();
//    return env->NewStringUTF(top1.c_str());
//}


JNIEXPORT jfloatArray JNICALL
Java_com_tengine_openailab_mobilenet_MainActivity_TengineWrapperFaceDetect(
        JNIEnv *env,
        jobject /* this */) {

    //1.新建长度len数组
    jfloatArray jarr = env->NewFloatArray(30);

    //2.获取数组指针
    jfloat *arr = env->GetFloatArrayElements(jarr, NULL);
    //3.赋值
    int i = 0;
    float (*tmp)[30];
    tmp=tengine_wrapper->FaceDetect();
    for(; i < 30; i++){
        //arr[i] = *(tmp+i);
        arr[i] = (*tmp)[i];
        //arr[i] = i+0.1;
    }
    //4.释放资源
    env->ReleaseFloatArrayElements(jarr, arr, 0);
    //5.返回数组
    return jarr;


//    jdouble * tmp = tengine_wrapper->FaceDetect();
//    jdoubleArray my = env->NewDoubleArray(30);
//
//    env->SetDoubleArrayRegion(my,0,30,tmp);
//    env->ReleaseDoubleArrayElements(my,tmp,0);
////    jint len = env->GetArrayLength(tmp);
////    jdoubleArray top1;
////    top1 = env
//    return my;
}

}