#ifndef OFCM_HPP
#define OFCM_HPP

#include <vector>
#include "utils.hpp"

std::vector<std::vector<float>> OFCM(cv::VideoCapture capTemp, std::pair<int,int> sequence);

#endif