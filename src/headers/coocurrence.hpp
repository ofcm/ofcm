#ifndef COOCURRENCE_HPP
#define COOCURRENCE_HPP

//# include <math.h>
//# include <opencv2/opencv.hpp>
//# include <vector>
# include "utils.hpp"

bool    isWithInMatrix(int h, int w, int maxheight, int maxwidth);
cv::Mat CoocurrenceFromSingleMatrixMag(std::vector<std::vector<int>> matrix, int dx, int dy, int cuboidSize);
cv::Mat CoocurrenceFromSingleMatrixAngle(const std::vector<std::vector<int>> & matrix, int dx, int dy, int nbins, int maxangle, int cuboidSize);

#endif