#include "headers/plot.hpp"

void drawHistrogram( std::vector<int> data,cv::Mat &dst,int binSize,std::vector<cv::Scalar> colores)
{
    int max_value = *max_element(data.begin(), data.end());
    int rows = 0;
    int cols = 0;

    rows = max_value + 10 * 20;

    cols = data.size() * binSize;

    dst = cv::Mat3b(rows, cols, cv::Vec3b(0,0,0));


    int k = -1;
    for (int i = 0; i < data.size(); ++i)
    {
        int h = rows - data[i] * 20;
        if(i % 5 == 0){
            k++;
        }            
        cv::rectangle(dst, cv::Point(i*binSize, h), cv::Point((i + 1)*binSize-1, rows), colores[k], CV_FILLED);
    }
}
