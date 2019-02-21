#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/video/tracking.hpp>

#define PI 3.14159265

static  int threshold           = 30;
static  float maxAngle          = 361.0;
static  float orientationBin    = 8.0;
static  float magnitudBin       = 8.0;
static  int maxDistance = 15;

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


//void getHaralickFeatures(std::vector<cv::Mat> Mco_Array, cv::Size cuboidsSize, std::vector<std::vector<std::vector<std::vector<float>>>>& res, int T);
void getHaralickFeatures(std::vector<std::vector<cv::Mat>> AAM1, std::vector<std::vector<cv::Mat>> AAM2, cv::Size cuboidsSize, std::vector<std::vector<float>>& res, int T);
void Mat2Mat(cv::Mat& src, cv::Mat& dst, int x0, int y0);

std::vector<float> haralick(cv::Mat Mco, int size);

void plotMO(cv::Mat& src1, 
            cv::Mat& src2, 
            std::vector<std::vector<std::vector<int>>>  magnitudeMatrices, 
            std::vector<std::vector<std::vector<int>>> orientationMatrices,
            cv::Size cuboidsSize,
            int cuboidDim);

void PrintLabelInImage(cv::Mat & src, std::map<int, std::string> labels, int predicted);
#endif