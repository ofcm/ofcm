#include "headers/utils.h"
#include "headers/coocurrence.hpp"
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
std::vector<cv::Mat>                                    coocurrenceMatricesMagnitud;
std::vector<cv::Mat>                                    coocurrenceMatricesOrientation;

int N = 36; // width and height size
int T = 10; // number of frames
int dx = 1;
int dy = 1;

int main(int argc, char** argv){
    
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
    unsigned char a = 10;
    unsigned char b = 20;


    //std::cout << a - b << std::endl;
    //return 0;
    char k;
    int count = 0;
    for (;;)
    {
        cap >> frame;

        if(frame.empty())
            break;    

        imageSize  = frame.size();
        cv::Mat fr = frame.clone();

        imageBuffer.push_back(fr);
        // Obtain initial set of features
        
        std::vector<float> res = haralick(fr, 12);

        std::cout << "haralick 0  = " << res[0]  << std::endl;
        std::cout << "haralick 1  = " << res[1]  << std::endl;
        std::cout << "haralick 2  = " << res[2]  << std::endl;
        std::cout << "haralick 3  = " << res[3]  << std::endl;
        std::cout << "haralick 4  = " << res[4]  << std::endl;
        std::cout << "haralick 5  = " << res[5]  << std::endl;
        std::cout << "haralick 6  = " << res[6]  << std::endl;
        std::cout << "haralick 7  = " << res[7]  << std::endl;
        std::cout << "haralick 8  = " << res[8]  << std::endl;
        std::cout << "haralick 9  = " << res[9]  << std::endl;
        std::cout << "haralick 10 = " << res[10] << std::endl;
        std::cout << "haralick 11 = " << res[11] << std::endl;

        if (imageBuffer.size() >= T)
        {
            DenseSampling(imageBuffer, N, T, cuboids,cuboidsSize);
            updateBuffer(imageBuffer);
            
            for(int icub = 0; icub < cuboids.size();icub++){
                for(int i = 0; i < cuboids[icub].size() - 1; i++){
                    opticalFlow(orientationMatricesT,magnitudeMatricesT,cuboids[icub][i],cuboids[icub][i+1],3);
                }
                for (int io = 0; io < orientationMatricesT.size(); io++){
                    orientationMatrices.push_back(orientationMatricesT[io]);
                    cv::Mat ang= coocurrence::CoocurrenceFromSingleMatrixAngle(orientationMatricesT[io], dx, dy, 8, 315);
                    //print2DstdVector<int>(orientationMatricesT[io]);
                    //printMatInConsole<float>(ang);
                    //std::cout << "\n\n-------------------------------------\n\n";
                    coocurrenceMatricesMagnitud.push_back(ang);
                }
                for (int im = 0; im < magnitudeMatricesT.size(); im++){
                    magnitudeMatrices.push_back(magnitudeMatricesT[im]);

                    cv::Mat mg = coocurrence::CoocurrenceFromSingleMatrixMag(magnitudeMatricesT[im], dx, dy, N);
                    //printMatInConsole<float>(mg);
                    //print2DstdVector(mag)
                    //std::cout << "\n\n-------------------------------------\n\n";
                    coocurrenceMatricesOrientation.push_back(mg);
                }
                orientationMatricesT.clear();
                magnitudeMatricesT.clear();          
            }
            //
            
            //std::cout << "Final size matrix 1: " << orientationMatrices.size() << std::endl;
            //std::cout << "Final size matrix 2: " << magnitudeMatrices.size()   << std::endl;
            
            //std::cout << "cuboidsSize = " << cuboidsSize << std::endl;
            cv::Mat CUBOID_IMG = cv::Mat::zeros(cv::Size(cuboidsSize.width*N, cuboidsSize.height*N), CV_8U);;

            for(int icub = 8; icub < magnitudeMatrices.size(); icub+=9 )
            {
                for(int i = 0; i<magnitudeMatrices[icub].size(); i++) {
                    for(int j = 0; j<magnitudeMatrices[icub][i].size(); j++) {
                        CUBOID_IMG.at<unsigned char>(i + N*((icub/9)/cuboidsSize.width), j + N*((icub/9)%cuboidsSize.width)) = 
                        (int)pow(2.0,(double)magnitudeMatrices[icub][i][j]);
                    }
                    //std::cout << "\n";
                }          
            }
            cv::resize(CUBOID_IMG,CUBOID_IMG, cv::Size(160,120));
            cv::imshow("cuboid", CUBOID_IMG); 

            /*
            for(int i = 0; i<orientationMatrices[0].size(); i++) {
                for(int j = 0; j<orientationMatrices[0][i].size(); j++) {
                    std::cout << '(' << orientationMatrices[orientationMatrices.size()-1][i][j] << ")";
                }
                std::cout << "\n";
            }

            std::cout << "Magnitude " << std::endl;
            for(int i = 0; i<magnitudeMatrices[magnitudeMatrices.size() - 1].size(); i++) {
                for(int j = 0; j<magnitudeMatrices[magnitudeMatrices.size() - 1][i].size(); j++) {
                    std::cout << '(' << magnitudeMatrices[magnitudeMatrices.size() - 1][i][j] << ")";
                }
                std::cout << "\n";
            }
            */
            
            
            cv::imshow("frame 10 : ", frame);

            /*
            cv::moveWindow("frame 10 : ", 100,100);
            cv::imshow("Some Cubid (Resize): 9 ", cuboids[0][0]);
            cv::moveWindow("Some Cubid (Resize): ", 100,100);
            cv::imshow("Some Cubid (Resize): 30", cuboids[0][19]);
            */
            cuboids.clear();
            orientationMatrices.clear();
            magnitudeMatrices.clear();
        }
        //std::cout << "Buffer Size : " << imageBuffer.size() << std::endl;
        k = cv::waitKey(30);
        count +=1;
        if ( k == 27)
            break;
    }
    return 0;
}