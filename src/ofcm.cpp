#include "headers/cuboids.hpp"
#include "headers/haralick.hpp"
#include "headers/opticalflow.hpp"
#include "headers/kmeans.hpp"
#include "headers/coocurrence.hpp"

cv::Mat                                                 frame;
cv::Size                                                imageSize;
cv::Size                                                cuboidsSize;



int cuboidDim       =                                   40; // width and height size
int T               =                                   10; // number of frames
int dx              =                                   1;
int dy              =                                   1;
int tHeight         =                                   160;
int tWidth          =                                   120;

std::vector<std::vector<float>> OFCM(cv::VideoCapture capTemp, std::pair<int,int> sequence)
{
    std::vector<std::vector<std::vector<int>>>              orientationMatricesT;
    std::vector<std::vector<std::vector<int>>>              magnitudeMatricesT;
    std::vector<std::vector<std::vector<int>>>              orientationMatrices;
    std::vector<std::vector<std::vector<int>>>              magnitudeMatrices;
    std::vector<cv::Mat>                                    imageBuffer;
    std::vector<std::vector<cv::Mat>>                       cuboids;

    std::vector<std::vector<cv::Mat>>                       coocurrenceMatricesMagnitud(4);
    std::vector<std::vector<cv::Mat>>                       coocurrenceMatricesOrientation(4);

    std::vector<std::vector<float>>                         AllflattenFeatures;

    
    cv::Size imageSize;
    cv::Mat frame;

    char k;

    int FRAMECOUNT = sequence.first;
    capTemp.set(CV_CAP_PROP_POS_FRAMES, sequence.first);
    
    int proc = 0;
    for (;;)
    {
        capTemp >> frame;
        if(frame.empty())
            break;    

        if(FRAMECOUNT >= sequence.second)
            break;

        //cv::resize(frame,frame, cv::Size(144, 108));
        imageSize  = frame.size();
        cv::Mat fr = frame.clone();

        imageBuffer.push_back(fr);

        if (imageBuffer.size() >= T)
        {
            proc++;
            DenseSampling(imageBuffer, cuboidDim, T, cuboids,cuboidsSize);
            updateBuffer(imageBuffer);
            
            for(int icub = 0; icub < cuboids.size();icub++){
                for(int i = 0; i < cuboids[icub].size() - 1; i++){
                    int i_next = i + 1;
                    opticalFlow(orientationMatricesT,magnitudeMatricesT,cuboids[icub][i],cuboids[icub][i_next],3);
                }
                for (int io = 0; io < orientationMatricesT.size(); io++){
                    orientationMatrices.push_back(orientationMatricesT[io]);                    
                    
                    coocurrenceMatricesOrientation[0].push_back(CoocurrenceFromSingleMatrixAngle(orientationMatricesT[io],  dx,   0, 8, 315, cuboidDim)); // 0  º
                    coocurrenceMatricesOrientation[1].push_back(CoocurrenceFromSingleMatrixAngle(orientationMatricesT[io],  dx, -dy, 8, 315, cuboidDim)); // 45 º
                    coocurrenceMatricesOrientation[2].push_back(CoocurrenceFromSingleMatrixAngle(orientationMatricesT[io],   0, -dy, 8, 315, cuboidDim)); // 90 º
                    coocurrenceMatricesOrientation[3].push_back(CoocurrenceFromSingleMatrixAngle(orientationMatricesT[io], -dy, -dx, 8, 315, cuboidDim)); // 135º
                                    
                }

                for (int im = 0; im < magnitudeMatricesT.size(); im++){
                    magnitudeMatrices.push_back(magnitudeMatricesT[im]);
                    
                    coocurrenceMatricesMagnitud[0].push_back(CoocurrenceFromSingleMatrixMag(magnitudeMatricesT[im],  dx,   0, cuboidDim));
                    coocurrenceMatricesMagnitud[1].push_back(CoocurrenceFromSingleMatrixMag(magnitudeMatricesT[im],  dx, -dy, cuboidDim));
                    coocurrenceMatricesMagnitud[2].push_back(CoocurrenceFromSingleMatrixMag(magnitudeMatricesT[im],   0, -dy, cuboidDim));
                    coocurrenceMatricesMagnitud[3].push_back(CoocurrenceFromSingleMatrixMag(magnitudeMatricesT[im], -dy, -dx, cuboidDim));
                    
                }

                orientationMatricesT.clear();
                magnitudeMatricesT.clear();          
            }
            int W = cuboidsSize.width;
            int H = cuboidsSize.height;

            std::vector<std::vector<float>> flattenFeatures;
            getHaralickFeatures(coocurrenceMatricesMagnitud, coocurrenceMatricesOrientation, cuboidsSize, flattenFeatures, T-1);

            for (int ift = 0; ift < flattenFeatures.size(); ift++)
                AllflattenFeatures.push_back(flattenFeatures[ift]);

            cv::Mat MagnitudImg    = cv::Mat::zeros(cv::Size((cuboidsSize.width + 1)*cuboidDim/2, (cuboidsSize.height + 1)*cuboidDim/2), CV_8U);
            cv::Mat OrientationImg = cv::Mat::zeros(cv::Size((cuboidsSize.width + 1)*cuboidDim/2, (cuboidsSize.height + 1)*cuboidDim/2), CV_8U);
            
            plotMO(MagnitudImg, OrientationImg, magnitudeMatrices, orientationMatrices, cuboidsSize, cuboidDim);

            cv::resize(frame,frame, cv::Size(imageSize.width*2, imageSize.height*2));
            cv::resize(MagnitudImg,MagnitudImg, cv::Size(imageSize.width*2, imageSize.height*2));
            cv::resize(OrientationImg,OrientationImg, cv::Size(imageSize.width*2, imageSize.height*2));

            cv::imshow("Magnitud", MagnitudImg); 
            cv::imshow("OrientationImg", OrientationImg); 
            cv::imshow("frame", frame);

            cv::moveWindow("frame", 100,100);
            cv::moveWindow("Magnitud", 100 + frame.cols,100);
            cv::moveWindow("OrientationImg", 100 + frame.cols *2 ,100);

            cuboids.clear();
            orientationMatrices.clear();
            magnitudeMatrices.clear();

            for(int qq = 0; qq < 4; qq++){
                coocurrenceMatricesMagnitud[qq].clear();
                coocurrenceMatricesOrientation[qq].clear();
            }
        }

        k = cv::waitKey(30);
        if ( k == 27)
            break;

        FRAMECOUNT++;
        //std::cout << "FRAMECOUNT = " << FRAMECOUNT << std::endl;
    }
    
    for(int icub = 0; icub < cuboids.size();icub++)
    {
        cuboids[icub].clear();
    }
    cuboids.clear();

    
    return AllflattenFeatures;
}