#ifndef CUBOIDS_HPP
#define CUBOIDS_HPP

#include "utils.hpp"

void DenseSampling(std::vector<cv::Mat> imageBuffer, 
                                        int N, int T, 
                                        std::vector<std::vector<cv::Mat>>& cuboids,
                                        cv::Size &cuboidsSize );

#endif