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
void opticalFlow(std::vector<cv::Mat> &orientationMatrices,
                std::vector<cv::Mat> &magnitudeMatrices,
                cv::Mat prevImage,
                cv::Mat nextImage,
                int maxLevel){
    cv::TermCriteria termC(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,20,0.03);
    cv::Mat orientationMatrix;
    cv::Mat magnitudeMatrix;
    std::vector<cv::Point2f> prevPoints;
    std::vector<cv::Point2f> nextPoints;
    std::vector<float> status;
    std::vector<float> errors;
    cv::Size winSize(31,31);
    int rows = prevImage.rows;
    int cols = prevImage.cols;
    getBetterPoints(prevPoints,prevImage,nextImage);
    std::cout << "size points " << prevPoints.size() << std::endl;
    orientationMatrix = cv::Mat(cols,rows,CV_16SC1,-1);
    magnitudeMatrix = cv::Mat(cols,rows,CV_16SC1,-1);
    if (prevPoints.size() > 0) {
        cv::calcOpticalFlowPyrLK(prevImage, nextImage, prevPoints, nextPoints,
                                status, errors, winSize, 3, termC, 0, 0.001);
        getMatrixOI(prevPoints, nextPoints, orientationMatrix,magnitudeMatrix);
        orientationMatrices.push_back(orientationMatrix);
        magnitudeMatrices.push_back(magnitudeMatrix);

    }

}
void getBetterPoints(std::vector<cv::Point2f> &prevPoints,
                    cv::Mat prevImage,
                    cv::Mat nextImage){
    prevPoints.clear();
    cvtColor(prevImage, prevImage, CV_BGR2GRAY);
    cvtColor(nextImage, nextImage, CV_BGR2GRAY);
    std::cout << "prev " << prevImage << std::endl;
    std::cout << "next " << nextImage << std::endl;
    cv::Mat diffImage = cv::abs(nextImage - prevImage);
    std::cout << "mat " << diffImage << std::endl;
    for (int i = 0; i < diffImage.rows; ++i) {
        for (int j = 0; j< diffImage.cols; ++j) {
            if (diffImage.at<uchar>(i, j) > threshold){
                prevPoints.push_back(cv::Point2f(static_cast<float>(j),
                static_cast<float>(i))); 
                //std::cout << "here " << std::endl;
            }
            
        }
    }
}
void getMatrixOI(std::vector<cv::Point2f> prevPoints,
                std::vector<cv::Point2f> nextPoints,
                cv::Mat &orientationMatrix, cv::Mat &magnitudeMatrix){
    float distance, angle, dX,dY;
    int x, y ,distInt,angInt;
    for(int i =0;i<prevPoints.size();i++){
        dX = nextPoints[i].y - prevPoints[i].y;
        dY = nextPoints[i].x - prevPoints[i].x;
        
        distance = sqrt(pow(dX,2.0) - pow(dY,2.0));
        
        angle = atan (dY/dX) * 180 / PI;
        angle = (angle < 0) ? angle += 360.0 : angle = angle;
        
        angInt = (int)(floor(angle / (maxAngle / orientationBin)));
        distInt = (int)(floor(log2(distance)));
        
        if(distInt < 0)
            distInt = 0;

        y = (int) prevPoints[i].y;
        y = (int) prevPoints[i].x;

        orientationMatrix.at<int>(y,x) = angInt;
        magnitudeMatrix.at<int>(y,x) = distInt; 
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