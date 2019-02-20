#include "headers/utils.hpp"
#include "headers/cuboids.hpp"

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
