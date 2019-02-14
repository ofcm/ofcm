#include "headers/utils.h"

float EPSILON = 0.0001;
float angular2ndMoment(cv::Mat Mco)
{
    float sum = 0.0;
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            //std::cout << "pixel =  " << Mco.at<float>(i,j) << std::endl;
            sum += (Mco.at<float>(i,j)*Mco.at<float>(i,j));
        }
    }
    return sum;
}

float contrast(cv::Mat Mco)
{
    int Ng = Mco.cols;
    
    float sum = 0.0;

    for (int n = 0; n < Ng; n++)
    {
        for (int i = 0; i < Ng; i++)
        {
            for (int j = 0; j < Ng; j++)
            {
                if (abs(i-j) == n)
                    sum += n * n * Mco.at<float>(i,j);
            }
        }
    }

    return sum;
}

float correlation(cv::Mat Mco)
{
    std::vector<float> px;

    for (int i = 0; i < Mco.rows; i++)
    {
        float sumPxi = 0.0;
        for (int j = 0; j < Mco.cols; j++)
        {
            sumPxi += Mco.at<float>(i,j);
        }
        px.push_back(sumPxi);
    } 

    std::vector<float> py;

    for (int j = 0; j < Mco.cols; j++)
    {
        float sumPyj = 0.0;
        for (int i = 0; i < Mco.rows; i++)
        {
            sumPyj += Mco.at<float>(i,j);
        }
        py.push_back(sumPyj);
    } 

    float muX = 0.0;

    for (int ix = 0; ix < px.size(); ix++)
        muX += px[ix];

    muX /= ((float)px.size());

    float muY = 0.0;

    for (int iy = 0; iy < py.size(); iy++)
        muY += py[iy];

    muY /= ((float)py.size());

    float sigmaX = 0.0;

    for (int ix = 0; ix < px.size(); ix++)
        sigmaX += ((px[ix]-muX) * (px[ix]-muX));

    sigmaX /= ((float)px.size());
    sigmaX  = sqrt(sigmaX);

    float sigmaY = 0.0;

    for (int iy = 0; iy < py.size(); iy++)
        sigmaY += ((py[iy]-muY) * (py[iy]-muY));

    sigmaY /= ((float)py.size());
    sigmaY  = sqrt(sigmaY);


    float sumCR = 0.0;
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            sumCR += (((float)i*(float)j)* (Mco.at<float>(i,j)) - muX * muY);
        }
    }
    sumCR /= (sigmaX * sigmaY);

    return sumCR;
}

float sumOfSquares(cv::Mat Mco)
{
    float sumVar = 0.0;
    float mean   = 0.0;

    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            mean += Mco.at<float>(i,j);
        }
    }   

    mean /= ((float)Mco.rows * (float)Mco.cols);

    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            sumVar += ((float)i - mean ) * ((float)i - mean) * Mco.at<float>(i,j);
        }
    }   

    return sumVar;
}

float InverseDifferenceMoment(cv::Mat Mco)
{
    float sumIDM = 0.0;
    
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            sumIDM += (1.0 / (1 + (float)((i - j)*(i - j)))) * Mco.at<float>(i,j);
        }
    }   

    return sumIDM;   
}

float sumAverage(cv::Mat Mco)
{
    std::vector<float> pX_plus_Y(Mco.rows + Mco.cols - 1, 0);
    
    int k;
    //std::cout << "size = " << pX_plus_Y.size() << std::endl;
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            k = i + j;
            pX_plus_Y[k] += Mco.at<float>(i,j);
        }
    }   
    
    float sumAvr = 0.0;

    for (int i = 0; i < pX_plus_Y.size(); i++)
    {
        sumAvr += ((float)i * pX_plus_Y[i]);
    }

    return sumAvr;
}

float sumEntropy(cv::Mat Mco)
{
    std::vector<float> pX_plus_Y(Mco.rows + Mco.cols - 1, 0);
    
    int k;
    //std::cout << "size = " << pX_plus_Y.size() << std::endl;
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            k = i + j;
            pX_plus_Y[k] += Mco.at<float>(i,j);
        }
    }   

    float sumEtp = 0.0;

    for (int i = 0; i < pX_plus_Y.size(); i++)
    {
        sumEtp += (pX_plus_Y[i]) * log10(pX_plus_Y[i] + EPSILON);
    }

    return -sumEtp;

}

float Entropy(cv::Mat Mco)
{
    float etp = 0.0;
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            etp += Mco.at<float>(i,j) * log10(Mco.at<float>(i,j) + EPSILON);
        }
    } 

    return -etp;
}

float differenceVariance(cv::Mat Mco)
{
    std::vector<float> pX_minus_Y(Mco.rows - 1, 0);
    
    int k;
    //std::cout << "size = " << pX_minus_Y.size() << std::endl;
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            k = abs(i - j);
            pX_minus_Y[k] += Mco.at<float>(i,j);
        }
    }  

    float meanP = 0.0;

    for (int ip = 0; ip < pX_minus_Y.size(); ip++)
    {
        meanP += pX_minus_Y[ip]; 
    }
    meanP /= ((float)pX_minus_Y.size());

    float var = 0.0;

    for (int ip = 0; ip < pX_minus_Y.size(); ip++)
    {
        var += (pX_minus_Y[ip] - meanP) * (pX_minus_Y[ip] - meanP); 
    }

    var /= ((float)pX_minus_Y.size());

    return var;
}

float differenceEntropy(cv::Mat Mco)
{
    std::vector<float> pX_minus_Y(Mco.rows - 1, 0);
    
    int k;
    //std::cout << "size = " << pX_minus_Y.size() << std::endl;
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            k = abs(i - j);
            pX_minus_Y[k] += Mco.at<float>(i,j);
        }
    }  
    
    float diffEtp = 0.0;
    
    for (int ip = 0; ip < pX_minus_Y.size(); ip++)
    {
        diffEtp += pX_minus_Y[ip] * log10(pX_minus_Y[ip] + EPSILON);
    }
    
    return diffEtp;
}

float informationCorrelation(cv::Mat Mco)
{
    std::vector<float> px(Mco.rows,0.0);
    std::vector<float> py(Mco.cols,0.0);
    
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            px[i] = Mco.at<float>(i,j);
            py[j] = Mco.at<float>(i,j);
        }
    } 

    float HX = 0.0;
    float HY = 0.0;
    
    float HXY  = 0.0;
    float HXY1 = 0.0;

    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            HXY  -= Mco.at<float>(i, j) * log10(Mco.at<float>(i, j) + EPSILON);
            HXY1 -= Mco.at<float>(i, j) * log10(px[i] * py[j] + EPSILON); 
        }
    }

    for (auto i = 0; i < px.size(); ++i) {
        HX -= px[i] * log10(px[i] + EPSILON);
        HY -= py[i] * log10(py[i] + EPSILON);

    float res = (HXY - HXY1) / std::max(HX, HY);

    return res;
}

}
std::vector<float> haralick(cv::Mat Mco_F, int size)
{
    //cv::Mat Mco_F;
    //cv::resize(Mco,Mco, cv::Size(18,18));
    //assert (Mco.cols == Mco.rows);

    //Mco.convertTo(Mco_F, CV_32FC1);

    std::vector<float> haralickVector(size);
    
    for (int i = 0; i < Mco_F.rows; i++)
    {
        for (int j = 0; j < Mco_F.cols; j++)
        {
            Mco_F.at<float>(i,j) /= (2.0 * (float)Mco_F.rows * (float)Mco_F.cols);
        }
    }
    
    haralickVector[0]  = angular2ndMoment(Mco_F);
    haralickVector[1]  = contrast(Mco_F);
    haralickVector[2]  = correlation(Mco_F);
    haralickVector[3]  = sumOfSquares(Mco_F);
    haralickVector[4]  = InverseDifferenceMoment(Mco_F);
    haralickVector[5]  = sumAverage(Mco_F);
    haralickVector[6]  = sumOfSquares(Mco_F);
    haralickVector[7]  = sumEntropy(Mco_F);
    haralickVector[8]  = Entropy(Mco_F);
    haralickVector[9]  = differenceVariance(Mco_F);
    haralickVector[10] = differenceEntropy(Mco_F);
    haralickVector[11] = informationCorrelation(Mco_F);

    return haralickVector;
}

void getFeatures(std::vector<cv::Mat> Mco_Array, cv::Size cuboidsSize, std::vector<std::vector<std::vector<std::vector<float>>>>& res, int T)
{
    int W = cuboidsSize.width;
    int H = cuboidsSize.height;

    //std::vector<std::vector<std::vector<float>>> res(W, H, T);


    for (int i = 0; i < Mco_Array.size(); i++)
    {
        cv::Mat Mco          = Mco_Array[i];
        std::vector<float> f = haralick(Mco, 12);

        
        for (int fi = 0; fi < f.size(); fi++)
        {
            //std::cout << "-> " << (i/9) / W << ", " <<(i/9) % W << ", " << fi << ", "<< i%9 <<std::endl;
            res[(i/9) / W][(i/9) % W][fi][i%9] = f[fi];
        }
    }
}
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
    //cv::Mat diffImage = cv::abs(nextImage_F - prevImage_F);

    //cv::Mat diff;


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
            std::cout << "i,j = " << i << ", " << j << std::endl;    
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

    cv::Size winSize(31,31);  
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