#ifndef OPTICALFLOW_HPP
#define OPTICALFLOW_HPP

#include "utils.hpp"

void opticalFlow(std::vector<std::vector<std::vector<int>>> &orientationMatrices,
               std::vector<std::vector<std::vector<int>>> &magnitudeMatrices,
                cv::Mat prevImage,
                cv::Mat nextImage,
                int maxLevel);

void getBetterPoints(std::vector<cv::Point2f> &prevPoints,
                    cv::Mat prevImage,
                    cv::Mat nextImage);

void getMatrixOI(std::vector<cv::Point2f> prevPoints,
                std::vector<cv::Point2f> nextPoints,
                std::vector<std::vector<int>> &orientationMatrix, 
                std::vector<std::vector<int>> &magnitudeMatrix);

#endif