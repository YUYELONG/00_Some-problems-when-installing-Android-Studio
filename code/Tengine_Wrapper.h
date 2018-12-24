//
// Created by user on 10/26/2018.
//

#ifndef MOBILENET_TENGINE_WRAPPER_H
#define MOBILENET_TENGINE_WRAPPER_H

#include "tengine_c_api.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

class TengineWrapper{
private:
    graph_t g_mobilenet;
    float* g_mobilenet_input;

    float get_input_data(cv::Mat sample, float* data, int img_h, int img_w);

public:
    TengineWrapper(){};
    ~TengineWrapper(){};
    int InitMobilenet();
    //int RunMobilenet(const char* image);
    float RunMobilenet(cv::Mat image);
    //double RunMobilenet(cv::Mat image);
    int ReleaseMobilenet();
    std::string GetTop1();
    //double * FaceDetect();
    float (*FaceDetect())[30];
};

#endif //MOBILENET_TENGINE_WRAPPER_H
