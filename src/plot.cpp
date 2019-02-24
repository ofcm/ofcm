#include "headers/plot.hpp"

void drawMatrix( std::vector<int> data,cv::Mat &dst,int blockSize, int row,int cols,int m_cluster)
{
    std::vector<unsigned int> color;
    for(auto d : data){
        //std::cout << " color " << (d * 255)/m_cluster << std::endl;
        color.push_back((d * 255)/m_cluster);
    }
    int count=0;
    for(int i=0;i<cols;i=i+blockSize){
        for(int j=0;j<row;j=j+blockSize){
            cv::Mat ROI=dst(cv::Rect(i,j,blockSize,blockSize));
            ROI.setTo(cv::Scalar::all(color[count]));
            count++;
        }
    }
    
}
