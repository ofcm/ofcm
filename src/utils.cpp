#include "headers/utils.h"

void DenseSampling(std::vector<cv::Mat> imageBuffer, 
                                        int N, int T, 
                                        std::vector<std::vector<cv::Mat>>& cuboids,
                                        cv::Size &cuboidsSize )
{
    std::vector<cv::Mat>    cuboid;
    cv::Rect                window;
    cv::Mat                 crop;
    int                     w = 0; 
    int                     h = 0;
    for (int i = 0; i < imageBuffer[0].rows - N + 1; i+= int(N/2))
    {
        w =0;
        for (int j = 0; j < imageBuffer[0].cols - N + 1; j+= int(N/2))
        {
            cuboid.clear();
            for (int t = imageBuffer.size() - 1; t > 0; t--)
            {
                //std::cout << "i : " << i << ", j : " << j << ", t : " << t << ", size : " << imageBuffer[t].size() <<std::endl;
                window = cv::Rect(j,i, N, N);
                crop   = cv::Mat(imageBuffer[t], window);
                cuboid.push_back(crop);
            }
            cuboids.push_back(cuboid);
            //std::cout << "size : " << Cuboid.size() << std::endl;
            w++;    
        }
        h++;
    }
    cuboidsSize = cv::Size(w,h);
}
void opticalFlow(cv::Mat prevImage,
                cv::Mat nextImage,
                std::vector<cv::Point2f> prevPoints,
                std::vector<cv::Point2f> &nextPoints,
                std::vector<float> &status,
                std::vector<float> &errors,
                int maxLevel = 0){
    cv::Size winSize(31,31);
    cv::TermCriteria termcrit(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,20,0.03);
    int rows = prevImage.rows;
    int cols = prevImage.cols;
    getBetterPoints(prevPoints,prevImage,nextImage);


}
void getBetterPoints(std::vector<cv::Point2f> &prevPoints,
                    cv::Mat prevImage,
                    cv::Mat nextImage){
    prevPoints.clear();
    cvtColor(prevImage, prevImage, CV_BGR2GRAY);
    cvtColor(nextImage, nextImage, CV_BGR2GRAY);
    cv::Mat diffImage = cv::abs(nextImage - prevImage);
    for (int i = 0; i < diffImage.rows; ++i) {
        for (int j = 0; j< diffImage.cols; ++j) {
        if (diffImage.at<uchar>(i, j) > threshold)
            prevPoints.push_back(cv::Point2f(static_cast<float>(j),
            static_cast<float>(i)));
        }
    }
}
void updateBuffer(std::vector<cv::Mat>& imageBuffer)
{
    for (int i = 1; i < imageBuffer.size(); i++)
    {
        imageBuffer[i - 1] = imageBuffer[i];
    }
    imageBuffer.pop_back();
}