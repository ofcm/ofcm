#ifndef OPTICALFLOW_HPP
#define OPTICALFLOW_HPP

#include "utils.hpp"

#define PI 3.14159265

static  int     threshold         = 50;
static  float   maxAngle          = 361.0;
static  float   orientationBin    = 8.0;
static  float   magnitudBin       = 8.0;
static  int     maxDistance       = 15;

void opticalFlow(std::vector<std::vector<std::vector<int>>> &orientationMatrices,
               std::vector<std::vector<std::vector<int>>> &magnitudeMatrices,
                cv::Mat prevImage,
                cv::Mat nextImage,
                int maxLevel,
                int windSize);

void getBetterPoints(std::vector<cv::Point2f> &prevPoints,
                    cv::Mat prevImage,
                    cv::Mat nextImage);

void getMatrixOI(std::vector<cv::Point2f> prevPoints,
                std::vector<cv::Point2f> nextPoints,
                std::vector<std::vector<int>> &orientationMatrix, 
                std::vector<std::vector<int>> &magnitudeMatrix);

#endif