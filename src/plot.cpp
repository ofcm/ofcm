#include "headers/plot.hpp"

void drawHist(std::vector<float>& data, cv::Mat3b& dst, int binSize, int height)
{
    int max_value = *max_element(data.begin(), data.end());
    int rows = 0;
    int cols = 0;
    if (height == 0) {
        rows = (max_value*100) + 10;
    } else {
        rows = std::max(max_value + 10, height);
    }

    cols = data.size() * binSize;

    dst = cv::Mat3b(rows, cols, cv::Vec3b(0,0,0));

    for (int i = 0; i < data.size(); ++i)
    {
        int h = rows - data[i]*100;
        rectangle(dst, cv::Point(i*binSize, h), cv::Point((i + 10)*binSize-1, rows), (i%2) ? cv::Scalar(0, 100, 255) : cv::Scalar(0, 0, 255), CV_FILLED);
    }
}

void plotGhrap(std::vector<std::vector<float>> vectorData){
    int c = 0;
    for(auto data : vectorData){

        cv::Mat3b image;
        drawHist(data, image,3,0);
        std::string title = "Histogram " + std::to_string(c+1);
        imshow(title, image);
        c++;
        cv::waitKey();

    }
}
