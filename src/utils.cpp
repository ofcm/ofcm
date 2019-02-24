#include "headers/utils.hpp"

void getHaralickFeatures(   std::vector<std::vector<cv::Mat>>   AAM1, 
                            std::vector<std::vector<cv::Mat>>   AAM2, 
                            cv::Size                            cuboidsSize, 
                            std::vector<std::vector<float>>&    res, 
                            int T)
{
    int W = cuboidsSize.width;
    int H = cuboidsSize.height;
    
    int ncuboids = W * H;
    
    std::vector<std::vector<float>> cuboidFeature;

    

    for (int icub = 0; icub < ncuboids; icub++)
    { 
        std::vector<float> f864;
        for (int iangle = 0; iangle < AAM1.size(); iangle++)
        {
            for (int ti = 0; ti < T; ti++)
            {
                cv::Mat Mco             = AAM1[iangle][icub*T+ti];
                std::vector<float> ftmp = haralick(Mco, 12);
                
                for (int fi = 0; fi < 12; fi++)
                    f864.push_back(ftmp[fi]);
            }

            for (int ti = 0; ti < T; ti++)
            {
                cv::Mat Mco             = AAM2[iangle][icub*T+ti];
                std::vector<float> ftmp = haralick(Mco, 12);

                for (int fi = 0; fi < 12; fi++)
                    f864.push_back(ftmp[fi]);
            }
        }
        
        res.push_back(f864);
    }
}
void Mat2Mat(cv::Mat src, cv::Mat& dst, int x0, int y0)
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
    cv::applyColorMap(src1, src1, cv::COLORMAP_JET);
    cv::applyColorMap(src2, src2, cv::COLORMAP_HSV);
}

void PrintLabelInImage(cv::Mat & src, std::map<int, std::string> labels, int predicted){
    std::string action = labels[predicted];
    cv::putText(src, action, cv::Point(10, src.rows - 10), 1, 1.8, cv::Scalar(120, 5, 150), 2, 4);
}


void get_vectorofHF(    std::vector<std::vector<cv::Mat>>   AAM1, 
                        std::vector<std::vector<cv::Mat>>   AAM2, 
                        cv::Size                            cuboidsSize, 
                        std::vector<std::vector<std::vector<float>>>&    res, 
                        int T)
{
    int W = cuboidsSize.width;
    int H = cuboidsSize.height;
    
    int ncuboids = W * H;
    
    std::vector<std::vector<float>> cuboidFeature;

    for (int icub = 0; icub < ncuboids; icub++)
    { 
        std::vector<std::vector<float>> fvectors;
        for (int iangle = 0; iangle < AAM1.size(); iangle++)
        {
            for (int ti = 0; ti < T; ti++)
            {
                cv::Mat Mco             = AAM1[iangle][icub*T+ti];
                std::vector<float> ftmp = haralick(Mco, 12);
                
                fvectors.push_back(ftmp);
            }

            for (int ti = 0; ti < T; ti++)
            {
                cv::Mat Mco             = AAM2[iangle][icub*T+ti];
                std::vector<float> ftmp = haralick(Mco, 12);

                fvectors.push_back(ftmp);
            }
        }
        
        res.push_back(fvectors);
    }

    // res = 35x72x12
}