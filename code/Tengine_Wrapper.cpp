//
// Created by user on 10/26/2018.
//
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include "Tengine_Wrapper.h"
#include "opencv2/imgcodecs.hpp"

using namespace std;
tensor_t output_tensor;
tensor_t input_tensor;
//const char filename[] = "/data/local/tmp/yuyelong.txt";
//std::ofstream fout(filename);

int TengineWrapper::InitMobilenet()
{
    init_tengine_library();
    if (request_tengine_version("0.1") < 0)
        return -1;

    //const char* mobilenet_tf_model = "/data/frozen_mobilenet_v1_224.pb";
    const char* mobilenet_tf_model = "/data/local/tmp/model3model.pb";
    //const char* mobilenet_tf_model = "/data/local/tmp/model2model.pb";
    //const char* mobilenet_caffe_proto = "/sdcard/openailab/models/mobilenet_deploy.prototxt";
    //const char* mobilenet_caffe_model = "/sdcard/openailab/models/mobilenet.caffemodel";
    const char* format = "tensorflow";

    if (load_model("mobilenet", format, mobilenet_tf_model) < 0)
    //if (load_model("mobilenet", format, mobilenet_caffe_proto, mobilenet_caffe_model) < 0)
        return 4;


    g_mobilenet = create_runtime_graph("graph0","mobilenet",NULL);
    if (!check_graph_valid(g_mobilenet))
        return 5;

    const int img_h = 96;
    const int img_w = 96;

    //int image_size = img_h * img_w * 3;
    int image_size = img_h * img_w;
    g_mobilenet_input = (float *) malloc(sizeof(float) * image_size);

    //int dims[4] = {1, 3, img_h, img_w};
    int dims[4] = {1, 1, img_h, img_w};

    //tensor_t input_tensor = get_graph_input_tensor(g_mobilenet, 0, 0);
    input_tensor = get_graph_input_tensor(g_mobilenet, 0, 0);
    if(!check_tensor_valid(input_tensor))
        return 6;

    set_tensor_shape(input_tensor, dims, 4);
    set_tensor_buffer(input_tensor, g_mobilenet_input, image_size * 4);

    if( prerun_graph(g_mobilenet)!=0 )
        return 1;

    return 0;
}

//int TengineWrapper::InitMobilenet()
//{
//    init_tengine_library();
//    if (request_tengine_version("0.1") < 0)
//        return -1;
//
//    //const char* mobilenet_tf_model = "/data/frozen_mobilenet_v1_224.pb";
//    //const char* mobilenet_tf_model = "/data/local/tmp/frozen_mobilenet_v1_224.pb";
//    const char* mobilenet_tf_model = "/data/local/tmp/model3model.pb";
//    //const char* mobilenet_caffe_proto = "/sdcard/openailab/models/mobilenet_deploy.prototxt";
//    //const char* mobilenet_caffe_model = "/sdcard/openailab/models/mobilenet.caffemodel";
//    const char* format = "tensorflow";
//
//    if (load_model("mobilenet", format, mobilenet_tf_model) < 0)
//        //if (load_model("mobilenet", format, mobilenet_caffe_proto, mobilenet_caffe_model) < 0)
//        return 4;
//
//
//    g_mobilenet = create_runtime_graph("graph0","mobilenet",NULL);
//    if (!check_graph_valid(g_mobilenet))
//        return 5;
//
//    const int img_h = 224;
//    const int img_w = 224;
//
//    int image_size = img_h * img_w * 3;
//    g_mobilenet_input = (float*) malloc(sizeof(float) * image_size);
//
//    int dims[4] = {1, 3, img_h, img_w};
//
//    tensor_t input_tensor = get_graph_input_tensor(g_mobilenet, 0, 0);
//    if(!check_tensor_valid(input_tensor))
//        return 6;
//
//    set_tensor_shape(input_tensor, dims, 4);
//    set_tensor_buffer(input_tensor, g_mobilenet_input, image_size * 4);
//
//    if( prerun_graph(g_mobilenet)!=0 )
//        return 1;
//
//    return 0;
//}


int TengineWrapper::ReleaseMobilenet()
{
    sleep(1);
    /*
    tensor_t input_tensor = get_graph_input_tensor(g_mobilenet, 0, 0);
    put_graph_tensor(input_tensor);
    free(g_mobilenet_input);
    postrun_graph(g_mobilenet);
    destroy_runtime_graph(g_mobilenet);
    remove_model("mobilenet");
    */
    return 0;
}

float TengineWrapper::get_input_data(cv::Mat sample, float * data, int img_h, int img_w)
{
    //cv::Mat sample = cv::imread(image);
    if (sample.empty())
        return 1;
    cv::Mat img;
//    if (sample.channels() == 4)
//    {
//        cv::cvtColor(sample, img, cv::COLOR_BGRA2BGR);
//    }
//    else if (sample.channels() == 1)
//    {
//        cv::cvtColor(sample, img, cv::COLOR_GRAY2BGR);
//    }
//    else
//    {
//        img=sample;
//    }
    img = sample;
    cv::resize(img, img, cv::Size(img_h, img_w));
    //img.convertTo(img, CV_32FC3);
    img.convertTo(img, CV_32FC1);
    float *img_data = (float *)img.data;
    for (int h = 0; h < img_h; h++)
    {
        for (int w = 0; w < img_w; w++)
        {
            //data[h * img_w + w] = img.at<uchar>(h,w)*0.0039215686;
            data[h * img_w + w] = (*img_data)*0.0039215686;
            img_data++;
//            if((h * img_w + w+1)%96==0)
//                fout << data[h * img_w + w] << ";";
//            else
//                fout << data[h * img_w + w] << ",";
            //fout << data[h * img_w + w] << ((h * img_w + w+1)%96==0) ?";":",";
            //img_data++;
        }
        //fout << endl;
    }

//    int hw = img_h * img_w;
//    //float mean[3]={104.f,117.f,123.f};
//    //float mean[3]={127.5,127.5,127.5};
//    for (int h = 0; h < img_h; h++)
//    {
//        for (int w = 0; w < img_w; w++)
//        {
//            for (int c = 0; c < 3; c++)
//            {
//                data[c * hw + h * img_w + w] = (*img_data - mean[c])*0.017;
//                img_data++;
//            }
//
//        }
//    }

/**/

    return 0;
    //return data[9215];
    //return img.at<uchar>(0,0);
    //return sample.channels();
}


//int TengineWrapper::get_input_data(cv::Mat sample, float* data, int img_h, int img_w)
//{
//    //cv::Mat sample = cv::imread(image);
//    if (sample.empty())
//        return 1;
//    cv::Mat img;
//    if (sample.channels() == 4)
//    {
//        cv::cvtColor(sample, img, cv::COLOR_BGRA2BGR);
//    }
//    else if (sample.channels() == 1)
//    {
//        cv::cvtColor(sample, img, cv::COLOR_GRAY2BGR);
//    }
//    else
//    {
//        img=sample;
//    }
//    cv::resize(img, img, cv::Size(img_h, img_w));
//    img.convertTo(img, CV_32FC3);
//    float *img_data = (float *)img.data;
//    int hw = img_h * img_w;
//    float mean[3]={104.f,117.f,123.f};
//    //float mean[3]={127.5,127.5,127.5};
//    for (int h = 0; h < img_h; h++)
//    {
//        for (int w = 0; w < img_w; w++)
//        {
//            for (int c = 0; c < 3; c++)
//            {
//                data[c * hw + h * img_w + w] = (*img_data - mean[c])*0.017;
//                img_data++;
//            }
//        }
//    }
//
///**/
//    return 0;
//}



//int TengineWrapper::RunMobilenet(const char* image)
//{
//
//    if( get_input_data(image, g_mobilenet_input, 96, 96) )
//        return 7;
//
//    if( !run_graph(g_mobilenet,1))
//        return 2;
//
//    return 0;
//}


//int TengineWrapper::RunMobilenet(cv::Mat image)
//{
//
//    if( get_input_data(image, g_mobilenet_input, 96, 96) )
//        return 7;
//
//    if( !run_graph(g_mobilenet,1))
//        return 2;
//
//    output_tensor = get_graph_output_tensor(g_mobilenet, 0, 0);
//    //return image.at<uchar>(0,0);
//    return 0;
//}


float TengineWrapper::RunMobilenet(cv::Mat image)
{

    //double myaa=get_input_data(image, g_mobilenet_input, 96, 96);
    if( get_input_data(image, g_mobilenet_input, 96, 96) )
        return 7;

    //set_tensor_buffer(input_tensor, g_mobilenet_input, 96 * 96 * 8);

    if( !run_graph(g_mobilenet,1))
        return 2;

    output_tensor = get_graph_output_tensor(g_mobilenet, 0, 0);
    float *data = (float *)get_tensor_buffer(output_tensor);

    //return image.at<uchar>(0,0);
    //return get_input_data(image, g_mobilenet_input, 96, 96);
    // return g_mobilenet_input[9215];
    //return data[0];
    //return output_tensor[1];
    return 0;
}


std::string TengineWrapper::GetTop1()
{
    //const char* label_file = "/data/synset_words.txt";
    const char* label_file = "/data/local/tmp/synset_words.txt";
    std::vector<std::string> result;
    std::ifstream labels(label_file);

    std::string line;
    while (std::getline(labels, line))
        result.push_back(line);

    int true_id = 0;
    float true_score=0.f;

    tensor_t output_tensor = get_graph_output_tensor(g_mobilenet, 0, 0);
    float *data = (float *)get_tensor_buffer(output_tensor);

    for(int i=0; i<1000; i++)
    {
        if(data[i]>true_score)
        {
            true_id = i;
            true_score = data[i];
        }
    }

    return result[true_id - 1];
}



float (*TengineWrapper::FaceDetect())[30]
{

    //tensor_t output_tensor = get_graph_output_tensor(g_mobilenet, 0, 0);
    float *data = (float *)get_tensor_buffer(output_tensor);

    float (*datatmp)[30]=(float (*)[30])new float[30];
    for(int i=0;i<30;i++)
    {
        (*datatmp)[i]=data[i];
        //(*datatmp)[i]=i+0.1;
    }
    delete datatmp;
    return datatmp;
}