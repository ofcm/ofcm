#include "headers/utils.hpp"
#include "headers/haralick.hpp"
#include "headers/coocurrence.hpp"
#include "headers/cuboids.hpp"
#include "headers/miscellanius.hpp"

cv::Mat                                                 frame;
cv::Size                                                imageSize;
std::vector<cv::Mat>                                    imageBuffer;
std::vector<std::vector<cv::Mat>>                       cuboids;
cv::Size                                                cuboidsSize;
std::string PATH_DATA =                                 "../data/";
std::vector<std::vector<std::vector<int>>>              orientationMatricesT;
std::vector<std::vector<std::vector<int>>>              magnitudeMatricesT;
std::vector<std::vector<std::vector<int>>>              orientationMatrices;
std::vector<std::vector<std::vector<int>>>              magnitudeMatrices;

// 0: 0º, 1 > 45º, 2 > 90º, 3 > 135º

std::vector<std::vector<cv::Mat>>                       coocurrenceMatricesMagnitud(4);
std::vector<std::vector<cv::Mat>>                       coocurrenceMatricesOrientation(4);
cv::Mat                                                 temp;

int cuboidDim       =                                   36; // width and height size
int T               =                                   10; // number of frames
int dx              =                                   1;
int dy              =                                   1;
int tHeight         =                                   160;
int tWidth          =                                   120;


int main(int argc, char** argv){
    temp = cv::Mat(tHeight*2, tWidth*2, CV_8UC3, cv::Scalar(45,45,45));

    if (argc < 1)
    {
        std::cout << "Input file video ..." << std::endl;
        return -1;
    }

    std::string FILENAME = argv[1];

    cv::VideoCapture cap(PATH_DATA + FILENAME);

    if (!cap.isOpened())
    {
        std::cout << "Failed to open camera." << std::endl;
        return -1;
    }
    
    char k;
    int count = 0;
    for (;;)
    {
        cap >> frame;

        if(frame.empty())
            break;    

        cv::resize(frame,frame, cv::Size(144, 108));
        imageSize  = frame.size();
        cv::Mat fr = frame.clone();

        imageBuffer.push_back(fr);

        if (imageBuffer.size() >= T)
        {
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

                //std::cout << "Sz cooc "<<coocurrenceMatricesOrientation[0].size()<<", "<<coocurrenceMatricesOrientation[0][0].size()<<std::endl;
                for (int im = 0; im < magnitudeMatricesT.size(); im++){
                    magnitudeMatrices.push_back(magnitudeMatricesT[im]);
                    
                    coocurrenceMatricesMagnitud[0].push_back(CoocurrenceFromSingleMatrixMag(magnitudeMatricesT[im],  dx,   0, cuboidDim));
                    coocurrenceMatricesMagnitud[1].push_back(CoocurrenceFromSingleMatrixMag(magnitudeMatricesT[im],  dx, -dy, cuboidDim));
                    coocurrenceMatricesMagnitud[2].push_back(CoocurrenceFromSingleMatrixMag(magnitudeMatricesT[im],   0, -dy, cuboidDim));
                    coocurrenceMatricesMagnitud[3].push_back(CoocurrenceFromSingleMatrixMag(magnitudeMatricesT[im], -dy, -dx, cuboidDim));
                    
                }
                //std::cout << "Sz cooc "<<coocurrenceMatricesOrientation[0].size()<<", "<<coocurrenceMatricesOrientation[0][0].size()<<std::endl;
                orientationMatricesT.clear();
                magnitudeMatricesT.clear();          
            }
            //std::cout << "Sz cooc "<<coocurrenceMatricesMagnitud[0].size()<<", "<<coocurrenceMatricesMagnitud[0][0].size()<<std::endl;
            //std::cout << "size = [ " << coocurrenceMatricesMagnitud.size()    << ", " << 
            //                            coocurrenceMatricesMagnitud[0].size() << ", " << std::endl;
            int W = cuboidsSize.width;
            int H = cuboidsSize.height;

            //std::cout << "cuboidsSize = " << cuboidsSize << std::endl;
            std::vector<std::vector<std::vector<std::vector<float>>>> resM = 
            std::vector<std::vector<std::vector<std::vector<float>>>> ( H, 
                                                                        std::vector<std::vector<std::vector<float>>>(W, 
                                                                        std::vector<std::vector<float>>(12, std::vector<float>(T-1, 0))));

            std::vector<std::vector<std::vector<std::vector<float>>>> resO = 
            std::vector<std::vector<std::vector<std::vector<float>>>> ( H, 
                                                                        std::vector<std::vector<std::vector<float>>>(W, 
                                                                        std::vector<std::vector<float>>(12, std::vector<float>(T-1, 0))));

            getFeatures(coocurrenceMatricesMagnitud[0], cuboidsSize, resM, T - 1);
            getFeatures(coocurrenceMatricesOrientation[0], cuboidsSize, resO, T - 1);
            
            //std::cout << "Final size matrix 1: " << orientationMatrices.size() << std::endl;
            //std::cout << "Final size matrix 2: " << magnitudeMatrices.size()   << std::endl;
            
            //std::cout << "cuboidsSize = " << cuboidsSize << std::endl;
            
            cv::Mat MagnitudImg    = cv::Mat::zeros(cv::Size((cuboidsSize.width + 1)*cuboidDim/2, (cuboidsSize.height + 1)*cuboidDim/2), CV_8U);
            cv::Mat OrientationImg = cv::Mat::zeros(cv::Size((cuboidsSize.width + 1)*cuboidDim/2, (cuboidsSize.height + 1)*cuboidDim/2), CV_8U);
            
            plotMO(MagnitudImg, OrientationImg, magnitudeMatrices, orientationMatrices, cuboidsSize, cuboidDim);

            cv::resize(frame,frame, cv::Size(imageSize.width*2, imageSize.height*2));
            cv::resize(MagnitudImg,MagnitudImg, cv::Size(imageSize.width*2, imageSize.height*2));
            cv::resize(OrientationImg,OrientationImg, cv::Size(imageSize.width*2, imageSize.height*2));

            cv::imshow("Magnitud", MagnitudImg); 
            cv::imshow("OrientationImg", OrientationImg); 
            cv::imshow("frame", frame);


            //cv::imshow("Some Cubid (Resize): 9 ", cuboids[0][9]);
            //cv::moveWindow("Some Cubid (Resize): ", 100,100);
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
        count +=1;
        if ( k == 27)
            break;
    }
    
    for(int icub = 0; icub < cuboids.size();icub++)
    {
        cuboids[icub].clear();
    }
    cuboids.clear();

    return 0;
}