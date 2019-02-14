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

    cv::Mat prevImage;
    cv::Mat nextImage;

    cv::Mat prevImage_F;
    cv::Mat nextImage_F;
   
    //for (int i = 1; )
    //prevImage.convertTo(prevImage_F, CV_32FC1);
    //nextImage.convertTo(nextImage_F, CV_32FC1);

    //std::cout << nextImage_F - prevImage_F << std::endl;
    cv::Mat diffImage = cv::abs(nextImage_F - prevImage_F);

    cv::Mat diff;


    for (int i = 0; i < imageBuffer[0].rows - N + 1; i+= int(N/2))
    {
        w =0;
        for (int j = 0; j < imageBuffer[0].cols - N + 1; j+= int(N/2))
        {
            cuboid.clear();
            for (int t = 0; t < imageBuffer.size(); t++)
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

    /*
    * Cuboids Difference
    * ==================
    * 
    *  Uncomment
    */

    /*
    for (int k = 0; k < cuboids.size(); k++)
    {
        prevImage = cuboids[0][0];
        nextImage = cuboids[0][1];

        prevImage.convertTo(prevImage_F, CV_32FC1);
        nextImage.convertTo(nextImage_F, CV_32FC1); 


        diff = cv::abs(nextImage_F - prevImage_F);
        std::cout << "diff \n====\n" << diff << std::endl;
    }
    */
}
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
    cv::Size winSize(3,3);  
    int rows = prevImage.rows;
    int cols = prevImage.cols;

    //std::cout << "row = " << rows << ", cols = " << cols << std::endl;
    std::vector<std::vector<int> >  orientationMatrix(
                                    cols,
                                    std::vector<int>(rows, 0));
    std::vector<std::vector<int> >  magnitudeMatrix(
                                    cols,
                                    std::vector<int>(rows, 0));

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

    //std::cout << diffImage << std::endl;
    for (int i = 0; i < diffImage.rows; ++i) {
        for (int j = 0; j< diffImage.cols; ++j) {
            //std::cout << "diff = " << (int)diffImage.at<uchar>(i, j) << std::endl;
            if (diffImage.at<uchar>(i, j) > threshold){
                prevPoints.push_back(cv::Point2f(static_cast<float>(j), static_cast<float>(i))); 
            }        
        }
    }
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
        dY = (int)nextPoints[i].y - prevPoints[i].y;
        dX = (int)nextPoints[i].x - prevPoints[i].x;

        distance = sqrt((dX * dX) + (dY*dY));
        if(dY == 0)
        {
            angle = 0.0;
        }    //div_ = 0;

        else
        {
            angle = atan2(dY,dX);
            
            angle = angle * (angle >= 0) + (angle + PI * 2.0) * (angle < 0);
            angle = angle * 180 / PI;
        }

        //std::cout << "angle = " << angle << ", mod = "<< distance << std::endl;
            //div_ = (dY/dX);
        /*
        angle = atan2(div_);
        a = a .* (a >= 0) + (a + 2 * pi) .* (a < 0);
         * 180.0 / PI;

        if(angle < 0) 
            angle += 360.0 ;
        */
       
        angInt  = static_cast<float>(floor(angle / (maxAngle / orientationBin)));
        distInt = static_cast<int>(floor(log2(distance)));
       
        if(distInt < 0)
            distInt = 0;
        else if (distInt >= magnitudBin)
            distInt = magnitudBin - 1;

        y = (int) prevPoints[i].y;
        x = (int) prevPoints[i].x;
        //std::cout << "prev orient " << magnitudeMatrix<< std::endl;
        orientationMatrix[y][x] = angInt;
        magnitudeMatrix[y][x]   = distInt;
        //std::cout << "next orient " << distInt << std::endl; 
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