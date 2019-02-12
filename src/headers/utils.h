#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>



void updateBuffer(std::vector<cv::Mat>& imageBuffer);
void DenseSampling(std::vector<cv::Mat> imageBuffer, int N, int T, std::vector<std::vector<cv::Mat>>& Cuboids);

#endif