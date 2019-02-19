#ifndef HARALICK_HPP
#define HARALICK_HPP

#include <vector>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/video/tracking.hpp>

float angular2ndMoment(cv::Mat Mco);
float contrast(cv::Mat Mco);
float correlation(cv::Mat Mco);
float sumOfSquares(cv::Mat Mco);
float InverseDifferenceMoment(cv::Mat Mco);
float sumAverage(cv::Mat Mco);
float sumEntropy(cv::Mat Mco);
float Entropy(cv::Mat Mco);
float differenceVariance(cv::Mat Mco);
float differenceEntropy(cv::Mat Mco);
float informationCorrelation(cv::Mat Mco);
std::vector<float> haralick(cv::Mat Mco_F, int size);

#endif