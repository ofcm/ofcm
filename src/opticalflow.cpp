#include "headers/opticalflow.hpp"

void opticalFlow(std::vector<std::vector<std::vector<int>>> &orientationMatrices,
               std::vector<std::vector<std::vector<int>>> &magnitudeMatrices,
                cv::Mat prevImage,
                cv::Mat nextImage,
                int maxLevel){
    cv::TermCriteria termC(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,20,0.03);
    
    std::vector<cv::Point2f> prevPoints;
    std::vector<cv::Point2f> nextPoints;
    std::vector<uchar> status;
    std::vector<float> errors;

    cv::Size winSize(31,31);  
    int rows = prevImage.rows;
    int cols = prevImage.cols;

    //std::cout << "row = " << rows << ", cols = " << cols << std::endl;
    std::vector<std::vector<int> >  orientationMatrix(
                                    rows,
                                    std::vector<int>(cols, -1));
    std::vector<std::vector<int> >  magnitudeMatrix(
                                    rows,
                                    std::vector<int>(cols, -1));

    getBetterPoints(prevPoints,prevImage,nextImage);
    //orientationMatrix = cv::Mat(cols,rows,CV_16SC1,-1);
    //magnitudeMatrix = cv::Mat(cols,rows,CV_16SC1,-1);
    cv::cvtColor(prevImage, prevImage, cv::COLOR_RGB2GRAY);
    cv::cvtColor(nextImage, nextImage, cv::COLOR_RGB2GRAY);

    if (prevPoints.size() > 0) {
       
        cv::calcOpticalFlowPyrLK(prevImage, nextImage, prevPoints, nextPoints,
                                status, errors, winSize, maxLevel, termC, 0, 0.001);
        getMatrixOI(prevPoints, nextPoints, orientationMatrix,magnitudeMatrix);
    }

    orientationMatrices.push_back(orientationMatrix);
    magnitudeMatrices.push_back(magnitudeMatrix);
}

void getBetterPoints(std::vector<cv::Point2f> &prevPoints,
                    cv::Mat prevImage,
                    cv::Mat nextImage){
    prevPoints.clear();
    cvtColor(prevImage, prevImage, CV_BGR2GRAY);
    cvtColor(nextImage, nextImage, CV_BGR2GRAY);

    cv::Mat prevImage_F;
    cv::Mat nextImage_F;

    prevImage.convertTo(prevImage_F, CV_32FC1);
    nextImage.convertTo(nextImage_F, CV_32FC1);

    //std::cout << nextImage_F - prevImage_F << std::endl;
    cv::Mat diffImage = cv::abs(nextImage_F - prevImage_F);
    diffImage.convertTo(diffImage, CV_8U);
    //std::cout << diffImage << std::endl;
    for (int i = 0; i < diffImage.rows; ++i) {
        for (int j = 0; j< diffImage.cols; ++j) {
            //std::cout << "diff = " << (int)diffImage.at<uchar>(i, j) << std::endl;
            if (diffImage.at<uchar>(i, j) > threshold){
                prevPoints.push_back(cv::Point2f(static_cast<float>(j), static_cast<float>(i))); 
            }        
        }
    }
    //cv::imshow("diffImage", diffImage);
    //cv::waitKey(30);
}

void getMatrixOI(std::vector<cv::Point2f> prevPoints,
                std::vector<cv::Point2f> nextPoints,
                std::vector<std::vector<int>> &orientationMatrix, 
                std::vector<std::vector<int>> &magnitudeMatrix){
    float distance, angle, dX,dY;
    int x, y ,distInt,angInt;
    float div_;

    for(int i =0;i<prevPoints.size();i++){
        //std::cout << "next = " << (int)nextPoints[i].y << ", prev = " << prevPoints[i].y << std::endl;
        //std::cout << "next = " << (int)nextPoints[i].x << ", prev = " << prevPoints[i].x << std::endl;
        dY = nextPoints[i].y - prevPoints[i].y;
        dX = nextPoints[i].x - prevPoints[i].x;

        distance = sqrt((dX * dX) + (dY*dY));

        if(dY == 0)
            angle = 0.0;

        else
        {
            angle = atan2(dY,dX);
            angle = angle * (angle >= 0) + (angle + PI * 2.0) * (angle < 0);
            angle = angle * 180.0 / PI;
        }

        angInt  = static_cast<float>(floor(orientationBin * angle / (maxAngle)));
        //distInt = static_cast<int>(floor(log2(distance)));
        distInt = static_cast<int>(floor(magnitudBin * distance / 3.0));
        
        
        //std::cout << "distInt = " << distInt << std::endl; 
        //std::cout << "angInt : " << angInt << std::endl;

        if(distInt < 0)
            distInt = 0;
        else if (distInt >= magnitudBin)
            distInt = magnitudBin - 1;

        
        y = (int) prevPoints[i].y;
        x = (int) prevPoints[i].x;
        //std::cout << "prev orient " << magnitudeMatrix<< std::endl;
        orientationMatrix[x][y] = angInt;
        magnitudeMatrix[x][y]   = distInt;
        //std::cout << "next orient " << distInt << std::endl; 
    }
     
}