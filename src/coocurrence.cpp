#include "headers/coocurrence.hpp"

bool isWithInMatrix(int h, int w, int maxheight, int maxwidth){
    if(h >= maxheight || h < 0 || w >= maxwidth || w < 0)
        return false;
    else
        return true;
}

cv::Mat CoocurrenceFromSingleMatrixMag(std::vector<std::vector<int>> matrix, int dx, int dy, int cuboidSize)
{
    int nvalues = 8;
    // Historigram
    cv::Mat output = cv::Mat::zeros(nvalues, nvalues, CV_32FC1);
    
    for(int i = 0; i < cuboidSize; i++){
        for(int j = 0; j < cuboidSize; j++){
            if(isWithInMatrix(i + dy, j + dx, cuboidSize, cuboidSize)){
                int val1 = matrix[i][j];
                int val2 = matrix[i + dy][j + dx];

                if (val1 < 0 || val2 < 0)
                    continue;
                //* since it is normalized from 0 to eight
                //* Increment the historigram at values (v1, v2)
                output.at<float>(val1, val2)++;
            }    
        }
    }
    return output;
}
    
cv::Mat CoocurrenceFromSingleMatrixAngle(const std::vector<std::vector<int>> & matrix, int dx, int dy, int nbins, int maxangle, int cuboidSize)
{
    // Historigram
    int widthbin = maxangle/(nbins-1);
    cv::Mat output = cv::Mat::zeros(nbins, nbins, CV_32FC1);
    for(int i = 0; i < cuboidSize; i++){
        for(int j = 0; j < cuboidSize; j++){
            if(isWithInMatrix(i + dy, j + dx, cuboidSize, cuboidSize)){
                int val1 = matrix[i][j];///widthbin;
                int val2 = matrix[i + dy][j + dx];///widthbin;

                if (val1 < 0 || val2 < 0)
                    continue;
                //* since it is normalized from 0 to eight
                //* Increment the historigram at values (v1, v2)
                output.at<float>(val1, val2)++;
            }    
        }
    }
    return output;
}