// # include "headers/coocurrence.hpp"
/*
Coocurrence::Coocurrence(float a, int d):offset_o(Offset_(a, d)){};


std::vector<cv::Mat> Coocurrence::ComputeMatrixesFromCuboid(const std::vector<cv::Mat> & cuboid){
    cv::Mat a1 = cuboid[0];
    cv::Size sz = cuboid[0].size();
    for(int h = 0; h < sz.height; h++){
        for(int w = 0; w < sz.width; w++){

        }
    }
}
bool isWithInMatrix(int h, int w, int maxheight, int maxwidth){
    if(h >= maxheight || h < 0 || w >= maxwidth || w < 0)
        return false;
    else
        return true;
}*/

/*
    * @Overview: "CoocurrenceFromSingleMatrixMag"
    * matrix > h^(M) matrix of magnitudes, values must be passed between 0 to 8
    * d_ to compare the intensityof current index to a displacement index
    * dx > Given an offset dx 
    * dy > Given an offset dy
    */ 
/*
namespace coocurrence{
cv::Mat CoocurrenceFromSingleMatrixMag(std::vector<std::vector<int>> matrix, int dx, int dy, int cuboidSize){
    int nvalues = 8;
    // Historigram
    cv::Mat output = cv::Mat::zeros(nvalues, nvalues, CV_32FC1);
    
    for(int i = 0; i < cuboidSize; i++){
        for(int j = 0; j < cuboidSize; j++){
            if(isWithInMatrix(i + dy, j + dx, cuboidSize, cuboidSize)){
                int val1 = matrix[i][j];
                int val2 = matrix[i + dy][j + dx];
                //* since it is normalized from 0 to eight
                //* Increment the historigram at values (v1, v2)
                output.at<float>(val1, val2)++;
            }    
        }
    }
    return output;
}
cv::Mat CoocurrenceFromSingleMatrixAngle(const std::vector<std::vector<int>> & matrix, int dx, int dy, int cuboidSize, int nbins, int maxangle){
    // Historigram
    int widthbin = maxangle/(nbins-1);
    cv::Mat output = cv::Mat::zeros(nbins, nbins, CV_32FC1);
    for(int i = 0; i < cuboidSize; i++){
        for(int j = 0; j < cuboidSize; j++){
            if(isWithInMatrix(i + dy, j + dx, cuboidSize, cuboidSize)){
                int val1 = matrix[i][j]/widthbin;
                int val2 = matrix[i + dy][j + dx]/widthbin;
                //* since it is normalized from 0 to eight
                //* Increment the historigram at values (v1, v2)
                output.at<float>(val1, val2)++;
            }    
        }
    }
    return output;
}
}*/

