#ifndef UTILS_H
#define UTILS_H

#define PI 3.14159265
static  int threshold           = 15;
static  float maxAngle          = 361.0;
static  float orientationBin    = 8.0;
static  float magnitudBin       = 8.0;
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
                                        
void opticalFlow(std::vector<std::vector<std::vector<int>>> &orientationMatrices,
                std::vector<std::vector<std::vector<int>>>  &magnitudeMatrices,
                cv::Mat prevImage,
                cv::Mat nextImage,
                int maxLevel = 0);

void getBetterPoints(std::vector<cv::Point2f> &prevPoints,
                    cv::Mat prevImage,
                    cv::Mat nextImage);

void getMatrixOI(std::vector<cv::Point2f> prevPoints,
                std::vector<cv::Point2f> nextPoints,
                std::vector<std::vector<int>> &orientationMatrix,
                std::vector<std::vector<int>> &magnitudeMatrix);



#endif