#include "headers/utils.hpp"
#include "headers/haralick.hpp"
#include "headers/coocurrence.hpp"
#include "headers/cuboids.hpp"
#include "headers/miscellanius.hpp"
#include "headers/kmeans.hpp"
#include "getOptions.cpp"

cv::Mat                                                 frame;
cv::Size                                                imageSize;
std::vector<cv::Mat>                                    imageBuffer;
std::vector<std::vector<cv::Mat>>                       cuboids;
cv::Size                                                cuboidsSize;
std::string PATH_DATA =                                 "../data/";
std::vector<std::vector<std::vector<int>>>              orientationMatricesT;
std::vector<std::vector<std::vector<int>>>              magnitudeMatricesT;
std::vector<std::vector<std::vector<int>>>              orientationMatrices;
std::vector<std::vector<std::vector<int>>>              magnitudeMatrices;

// 0: 0º, 1 > 45º, 2 > 90º, 3 > 135º

std::vector<std::vector<cv::Mat>>                       coocurrenceMatricesMagnitud(4);
std::vector<std::vector<cv::Mat>>                       coocurrenceMatricesOrientation(4);
cv::Mat                                                 temp;

int cuboidDim       =                                   36; // width and height size
int T               =                                   10; // number of frames
int dx              =                                   1;
int dy              =                                   1;
int tHeight         =                                   160;
int tWidth          =                                   120;

std::vector<option> trainingOptions,    
                    validationOptions,
                    testOptions;

int main(int argc, char** argv){
    getOptions(trainingOptions,validationOptions,testOptions,PATH_DATA+"textOption.txt");
    return 0;
}