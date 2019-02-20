#include "headers/utils.hpp"
// For a dimension of HxMxalphaxfx(t -1)
/*
void getHaralickFeatures(std::vector<cv::Mat> Mco_Array, cv::Size cuboidsSize, std::vector<std::vector<std::vector<std::vector<float>>>>& res, int T)
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
*/
void getHaralickFeatures(std::vector<std::vector<cv::Mat>> AAM1, std::vector<std::vector<cv::Mat>> AAM2, cv::Size cuboidsSize, std::vector<std::vector<float>>& res, int T)
{
    int W = cuboidsSize.width;
    int H = cuboidsSize.height;

    for (int iangle = 0; iangle < AAM1.size(); iangle++)
    {
        for (int i = 0; i < AAM1[iangle].size(); i+=9)
        {
            std::vector<float> f(12, 0.0);
            for (int ti = 0; ti < T; ti++)
            {
                cv::Mat Mco             = AAM1[iangle][i+ti];
                std::vector<float> ftmp = haralick(Mco, 12);

                for (int fi = 0; fi < 12; fi++)
                {
                    f[fi] += ftmp[fi]/((float)T);
                }

            }

            float sum = 0.0;
            for (int fi = 0; fi < 12; fi++)
            {
                sum += abs(f[fi]);
            }

            if (sum == 0.0)
                continue;

            res.push_back(f);
        }
    }

    for (int iangle = 0; iangle < AAM2.size(); iangle++)
    {
        for (int i = 0; i < AAM2[iangle].size(); i+=9)
        {
            std::vector<float> f(12, 0.0);
            for (int ti = 0; ti < T; ti++)
            {
                cv::Mat Mco             = AAM2[iangle][i+ti];
                std::vector<float> ftmp = haralick(Mco, 12);

                for (int fi = 0; fi < 12; fi++)
                {
                    f[fi] += ftmp[fi]/((float)T);
                }
            }

            float sum = 0.0;
            for (int fi = 0; fi < 12; fi++)
            {
                sum += abs(f[fi]);
            }

            if (sum == 0.0)
                continue;
                
            res.push_back(f);
        }
    }

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


void plotMO(cv::Mat& src1, 
            cv::Mat& src2, 
            std::vector<std::vector<std::vector<int>>>  magnitudeMatrices, 
            std::vector<std::vector<std::vector<int>>> orientationMatrices,
            cv::Size cuboidsSize,
            int cuboidDim)
{
    int magVal;
    int oriVal;

    for(int icub = 8; icub < magnitudeMatrices.size(); icub+=9 )
    {
        int icub_x = (icub/9) % cuboidsSize.width;
        int icub_y = (icub/9) / cuboidsSize.width;
        for(int i = 0; i<magnitudeMatrices[icub].size()/2; i++) {
            for(int j = 0; j<magnitudeMatrices[icub][i].size()/2; j++) {
                magVal = (int)pow(2.0,(double)magnitudeMatrices[icub][i][j] + 1) - 1;

                if (orientationMatrices[icub][i][j] < 0)
                    oriVal = 170;
                else
                    oriVal = 32.0 * (double)orientationMatrices[icub][i][j];

                if (magVal > 255)
                    magVal = 255;
                else if (magVal < 0)
                    magVal = 0;

                src1.at<unsigned char>(icub_y*cuboidDim/2 + j, icub_x*cuboidDim/2+ i) = magVal;   
                src2.at<unsigned char>(icub_y*cuboidDim/2 + j, icub_x*cuboidDim/2+ i) = oriVal;
            }
        }          

        if (icub_x == (cuboidsSize.width - 1))
        {
            for(int i = magnitudeMatrices[icub].size()/2; i<magnitudeMatrices[icub].size(); i++) {
                for(int j = 0; j<magnitudeMatrices[icub][i].size()/2; j++) {
                    magVal = (int)pow(2.0,(double)magnitudeMatrices[icub][i][j] + 1) - 1;

                    if (orientationMatrices[icub][i][j] < 0)
                        oriVal = 170;
                    else
                        oriVal = 32.0 * (double)orientationMatrices[icub][i][j];

                    if (magVal > 255)
                        magVal = 255;
                    else if (magVal < 0)
                        magVal = 0;

                    src1.at<unsigned char>(icub_y*cuboidDim/2 + j, icub_x*cuboidDim/2+ i)    = magVal;   
                    src2.at<unsigned char>(icub_y*cuboidDim/2 + j, icub_x*cuboidDim/2+ i) = oriVal;
                }
            }                        
        }

        if (icub_y == (cuboidsSize.height - 1))
        {
            for(int i = 0; i<magnitudeMatrices[icub].size()/2; i++) {
                for(int j = magnitudeMatrices[icub][i].size()/2; j<magnitudeMatrices[icub][i].size(); j++) {
                    magVal = (int)pow(2.0,(double)magnitudeMatrices[icub][i][j] + 1) - 1;

                    if (orientationMatrices[icub][i][j] < 0)
                        oriVal = 170;
                    else
                        oriVal = 32.0 * (double)orientationMatrices[icub][i][j];

                    if (magVal > 255)
                        magVal = 255;
                    else if (magVal < 0)
                        magVal = 0;

                    src1.at<unsigned char>(icub_y*cuboidDim/2 + j, icub_x*cuboidDim/2+ i)    = magVal;   
                    src2.at<unsigned char>(icub_y*cuboidDim/2 + j, icub_x*cuboidDim/2+ i) = oriVal;
                }
            }                        
        }

        if (icub_x == (cuboidsSize.width - 1) && icub_y == (cuboidsSize.height - 1))
        {
            for(int i = magnitudeMatrices[icub].size()/2; i<magnitudeMatrices[icub].size(); i++) {
                for(int j = magnitudeMatrices[icub][i].size()/2; j<magnitudeMatrices[icub][i].size(); j++) {
                    magVal = (int)pow(2.0,(double)magnitudeMatrices[icub][i][j] + 1) - 1;

                    if (orientationMatrices[icub][i][j] < 0)
                        oriVal = 170;
                    else
                        oriVal = 32.0 * (double)orientationMatrices[icub][i][j];

                    if (magVal > 255)
                        magVal = 255;
                    else if (magVal < 0)
                        magVal = 0;

                    src1.at<unsigned char>(icub_y*cuboidDim/2 + j, icub_x*cuboidDim/2+ i)    = magVal;   
                    src2.at<unsigned char>(icub_y*cuboidDim/2 + j, icub_x*cuboidDim/2+ i) = oriVal;
                }
            }                     
        }
    }
    cv::applyColorMap(src1, src1, cv::COLORMAP_HOT);
    cv::applyColorMap(src2, src2, cv::COLORMAP_HSV);
}