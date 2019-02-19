#include "headers/utils.hpp"

void getFeatures(std::vector<cv::Mat> Mco_Array, cv::Size cuboidsSize, std::vector<std::vector<std::vector<std::vector<float>>>>& res, int T)
{
    int W = cuboidsSize.width;
    int H = cuboidsSize.height;

    //std::vector<std::vector<std::vector<float>>> res(W, H, T);


    for (int i = 0; i < Mco_Array.size(); i++)
    {
        cv::Mat Mco          = Mco_Array[i];
        //std::cout<<"....sss"<<std::endl;
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
            //std::cout << "i,j = " << i << ", " << j << std::endl;    
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

void Mat2Mat(cv::Mat& src, cv::Mat& dst, int x0, int y0)
{
    for(int i = x0; i < x0 + src.rows; i++)
    {
        for(int j = y0; j < y0 + src.cols; j++)
        {
            dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(i-x0, j-y0);
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