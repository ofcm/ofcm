#ifndef UTILS_H
#define UTILS_H

#define PI 3.14159265
static  int threshold = 0;
static  float maxAngle = 361.0;
static  int orientationBin = 8;
static  int maxDistance = 15;


#include <iostream>
#include <string>
#include <vector>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/video/tracking.hpp>

void updateBuffer(std::vector<cv::Mat>& imageBuffer);
void DenseSampling(std::vector<cv::Mat> imageBuffer, 
                                        int N, 
                                        int T, 
                                        std::vector<std::vector<cv::Mat>>& cuboids,
                                        cv::Size &cuboidsSize);
                                        
void opticalFlow(std::vector<cv::Mat> &orientationMatrices,
                std::vector<cv::Mat> &magnitudeMatrices,
                cv::Mat prevImage,
                cv::Mat nextImage,
                int maxLevel = 0);

void getBetterPoints(std::vector<cv::Point2f> &prevPoints,
                    cv::Mat prevImage,
                    cv::Mat nextImage);

void getMatrixOI(std::vector<cv::Point2f> prevPoints,
                std::vector<cv::Point2f> nextPoints,
                cv::Mat &orientationMatrix, cv::Mat &magnitudeMatrix);



#endif