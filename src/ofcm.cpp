#include "headers/cuboids.hpp"
#include "headers/haralick.hpp"
#include "headers/opticalflow.hpp"
#include "headers/kmeans.hpp"
#include "headers/coocurrence.hpp"
#include "headers/ofcm.hpp"
#include "headers/plot.hpp"
#include "headers/miscellanius.hpp"

cv::Mat                                                 frame;
cv::Size                                                imageSize;
cv::Size                                                cuboidsSize;

OFCM::OFCM(int re_rows,int re_cols)
{
    this->reRows = re_rows;
    this->reCols = re_cols;
}

std::vector<std::vector<std::vector<std::vector<float>>>> OFCM::get_features(cv::VideoCapture capTemp, std::pair<int,int> sequence, int& cuboidsize)
{
    cv::VideoWriter video("../data/outcpp.avi",CV_FOURCC('M','J','P','G'),30, cv::Size(tWidth*6, tHeight*2));

    std::vector<std::vector<std::vector<int>>>              oMTemp;
    std::vector<std::vector<std::vector<int>>>              mMTemp;
    std::vector<std::vector<std::vector<int>>>              orientationMatrices;
    std::vector<std::vector<std::vector<int>>>              magnitudeMatrices;
    std::vector<cv::Mat>                                    imageBuffer;
    std::vector<std::vector<cv::Mat>>                       cuboids;

    std::vector<std::vector<cv::Mat>>                       coMM(4);
    std::vector<std::vector<cv::Mat>>                       coMO(4);

    std::vector<std::vector<std::vector<std::vector<float>>>>            global_framesFeatures;

    cv::Size imageSize;
    cv::Mat frame;

    char k;

    int FRAMECOUNT = sequence.first;
    capTemp.set(CV_CAP_PROP_POS_FRAMES, sequence.first);

    //std::cout << "-->" << std::endl;

    int proc = 0;

    for (;;)
    {
        //std::cout << "-->" << FRAMECOUNT << std::endl;
        cv::Mat Template = cv::Mat(tHeight*2, tWidth*6, CV_8UC3, cv::Scalar(45));
        capTemp >> frame;
        if(frame.empty())
            break;

        if(FRAMECOUNT >= sequence.second)
            break;

        cv::resize(frame,frame, cv::Size(reCols, reRows));
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
                    opticalFlow(oMTemp,mMTemp,cuboids[icub][i],cuboids[icub][i_next], Levels, windSize);
                }

                for (int io = 0; io < oMTemp.size(); io++){
                    orientationMatrices.push_back(oMTemp[io]);

                    coMO[0].push_back(CoocurrenceFromSingleMatrixAngle(oMTemp[io],  dx,   0, 8, 315, cuboidDim)); // 0  º
                    coMO[1].push_back(CoocurrenceFromSingleMatrixAngle(oMTemp[io],  dx, -dy, 8, 315, cuboidDim)); // 45 º
                    coMO[2].push_back(CoocurrenceFromSingleMatrixAngle(oMTemp[io],   0, -dy, 8, 315, cuboidDim)); // 90 º
                    coMO[3].push_back(CoocurrenceFromSingleMatrixAngle(oMTemp[io], -dy, -dx, 8, 315, cuboidDim)); // 135º
                }

                for (int im = 0; im < mMTemp.size(); im++){
                    magnitudeMatrices.push_back(mMTemp[im]);

                    coMM[0].push_back(CoocurrenceFromSingleMatrixMag(mMTemp[im],  dx,   0, cuboidDim));
                    coMM[1].push_back(CoocurrenceFromSingleMatrixMag(mMTemp[im],  dx, -dy, cuboidDim));
                    coMM[2].push_back(CoocurrenceFromSingleMatrixMag(mMTemp[im],   0, -dy, cuboidDim));
                    coMM[3].push_back(CoocurrenceFromSingleMatrixMag(mMTemp[im], -dy, -dx, cuboidDim));

                }
                oMTemp.clear();
                mMTemp.clear();
            }
            int W = cuboidsSize.width;
            int H = cuboidsSize.height;

            cuboidsize = W*H;
            std::vector<std::vector<float>> framesFeatures;

            //std::cout<< "==> cuboidsSize : " << cuboidsSize << std::endl;
            //getHaralickFeatures(coMM, coMO, cuboidsSize, framesFeatures, T-1);
            std::vector<std::vector<std::vector<float>>> framesFeatures2;
            get_vectorofHF(coMM, coMO, cuboidsSize, framesFeatures2, T-1);
            //std::cout<< "\t==> framesFeatures size : " << framesFeatures.size() << " x "<<  framesFeatures[0].size() << std::endl;
            global_framesFeatures.push_back(framesFeatures2);
            //std::cout<< "\t==> global_framesFeatures size : " << global_framesFeatures.size() << " x "<<  global_framesFeatures[0].size() << std::endl;

            /*
            cv::Mat MagnitudImg    = cv::Mat::zeros(cv::Size((cuboidsSize.width + 1)*cuboidDim/2, (cuboidsSize.height + 1)*cuboidDim/2), CV_8U);
            cv::Mat OrientationImg = cv::Mat::zeros(cv::Size((cuboidsSize.width + 1)*cuboidDim/2, (cuboidsSize.height + 1)*cuboidDim/2), CV_8U);

            plotMO(MagnitudImg, OrientationImg, magnitudeMatrices, orientationMatrices, cuboidsSize, cuboidDim);

            cv::resize(frame,frame, cv::Size(tWidth*2, tHeight*2));
            cv::resize(MagnitudImg,MagnitudImg, cv::Size(tWidth*2, tHeight*2));
            cv::resize(OrientationImg,OrientationImg, cv::Size(tWidth*2, tHeight*2));

            Mat2Mat(frame           , Template, 0, 0);
            Mat2Mat(MagnitudImg     , Template, 0, tWidth*2);
            Mat2Mat(OrientationImg  , Template, 0, tWidth*4);

            cv::imshow("Template",Template);
            */
            cuboids.clear();
            orientationMatrices.clear();
            magnitudeMatrices.clear();

            //video.write(Template);
            for(int qq = 0; qq < 4; qq++){
                coMM[qq].clear();
                coMO[qq].clear();
            }
        }

        k = cv::waitKey(30);
        if ( k == 27)
            break;

        FRAMECOUNT++;
    }

    for(int icub = 0; icub < cuboids.size();icub++)
    {
        cuboids[icub].clear();
    }

    cuboids.clear();

    //video.release();
    return global_framesFeatures;
}
void OFCM::get_features_realTime(cv::VideoCapture capTemp, int& cuboidsize)
{
    cv::VideoWriter video("../data/outcpp.avi",CV_FOURCC('M','J','P','G'),30, cv::Size(tWidth*6, tHeight*2));

    std::vector<std::vector<std::vector<int>>>              oMTemp;
    std::vector<std::vector<std::vector<int>>>              mMTemp;
    std::vector<std::vector<std::vector<int>>>              orientationMatrices;
    std::vector<std::vector<std::vector<int>>>              magnitudeMatrices;
    std::vector<cv::Mat>                                    imageBuffer;
    std::vector<std::vector<cv::Mat>>                       cuboids;

    std::vector<std::vector<cv::Mat>>                       coMM(4);
    std::vector<std::vector<cv::Mat>>                       coMO(4);

    cv::Size imageSize;
    cv::Mat frame;

    char k;

    int proc = 0;
    std::string CENTROIDS_FILE          = "../models/centroids/avrCentroids.txt";
    std::vector<std::vector<std::vector<float>>> cuboidsCenters;
    LoadCentroidsFromFile(CENTROIDS_FILE, cuboidsCenters);

    std::vector<cv::Scalar> colores;
    for(int k = 0;k < 35;k++){
        int valor1 = rand() % 255;
        int valor2 = rand() % 255;
        int valor3 = rand() % 255;
        colores.push_back(cv::Scalar(valor1, valor2, valor3));
    }
    for (;;)
    {

        cv::Mat Template = cv::Mat(tHeight*4, tWidth*6, CV_8UC3, cv::Scalar(45));
        capTemp >> frame;
        if(frame.empty())
            break;

        cv::resize(frame,frame, cv::Size(reCols, reRows));
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
                    opticalFlow(oMTemp,mMTemp,cuboids[icub][i],cuboids[icub][i_next], Levels, windSize);
                }

                for (int io = 0; io < oMTemp.size(); io++){
                    orientationMatrices.push_back(oMTemp[io]);

                    coMO[0].push_back(CoocurrenceFromSingleMatrixAngle(oMTemp[io],  dx,   0, 8, 315, cuboidDim)); // 0  º
                    coMO[1].push_back(CoocurrenceFromSingleMatrixAngle(oMTemp[io],  dx, -dy, 8, 315, cuboidDim)); // 45 º
                    coMO[2].push_back(CoocurrenceFromSingleMatrixAngle(oMTemp[io],   0, -dy, 8, 315, cuboidDim)); // 90 º
                    coMO[3].push_back(CoocurrenceFromSingleMatrixAngle(oMTemp[io], -dy, -dx, 8, 315, cuboidDim)); // 135º
                }

                for (int im = 0; im < mMTemp.size(); im++){
                    magnitudeMatrices.push_back(mMTemp[im]);

                    coMM[0].push_back(CoocurrenceFromSingleMatrixMag(mMTemp[im],  dx,   0, cuboidDim));
                    coMM[1].push_back(CoocurrenceFromSingleMatrixMag(mMTemp[im],  dx, -dy, cuboidDim));
                    coMM[2].push_back(CoocurrenceFromSingleMatrixMag(mMTemp[im],   0, -dy, cuboidDim));
                    coMM[3].push_back(CoocurrenceFromSingleMatrixMag(mMTemp[im], -dy, -dx, cuboidDim));

                }
                oMTemp.clear();
                mMTemp.clear();
            }
            int W = cuboidsSize.width;
            int H = cuboidsSize.height;

            cuboidsize = W*H;

            std::vector<std::vector<std::vector<float>>> framesFeatures;
            get_vectorofHF(coMM, coMO, cuboidsSize, framesFeatures, T-1);
            std::vector<int> vectorHistogram;
            cv::Mat3b imageHis;
            for(int i  = 0; i < framesFeatures.size();i++){
                std::vector<int> histogram;
                kmeans km(cuboidsCenters[i],5);
                km.setFeatures(framesFeatures[i]);
                km.getHistogram(histogram);
                for (auto h : histogram)
                    vectorHistogram.push_back(h);
            }
            drawHistrogram(vectorHistogram,imageHis,5,colores);

            cv::Mat MagnitudImg    = cv::Mat::zeros(cv::Size((cuboidsSize.width + 1)*cuboidDim/2, (cuboidsSize.height + 1)*cuboidDim/2), CV_8U);
            cv::Mat OrientationImg = cv::Mat::zeros(cv::Size((cuboidsSize.width + 1)*cuboidDim/2, (cuboidsSize.height + 1)*cuboidDim/2), CV_8U);

            plotMO(MagnitudImg, OrientationImg, magnitudeMatrices, orientationMatrices, cuboidsSize, cuboidDim);

            cv::resize(frame,frame, cv::Size(tWidth*2, tHeight*2));
            cv::resize(MagnitudImg,MagnitudImg, cv::Size(tWidth*2, tHeight*2));
            cv::resize(OrientationImg,OrientationImg, cv::Size(tWidth*2, tHeight*2));
            cv::resize(imageHis,imageHis, cv::Size(tWidth*4, tHeight*2));

            Mat2Mat(frame           , Template, 0, 0);
            Mat2Mat(MagnitudImg     , Template, 0, tWidth*2);
            Mat2Mat(OrientationImg  , Template, 0, tWidth*4);
            Mat2Mat(imageHis  , Template, (tHeight*2), 0);
            cv::imshow("s",imageHis);
            cv::imshow("Template",Template);
            cuboids.clear();
            orientationMatrices.clear();
            magnitudeMatrices.clear();

            for(int qq = 0; qq < 4; qq++){
                coMM[qq].clear();
                coMO[qq].clear();
            }
        }

        k = cv::waitKey(30);
        if ( k == 27)
            break;

    }

    for(int icub = 0; icub < cuboids.size();icub++)
    {
        cuboids[icub].clear();
    }

    cuboids.clear();


}
