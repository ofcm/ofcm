#include "headers/utils.hpp"
#include "headers/haralick.hpp"
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
// 0: 0º, 1 > 45º, 2 > 90º, 3 > 135º
std::vector<std::vector<cv::Mat>>                       coocurrenceMatricesMagnitud(4);
std::vector<std::vector<cv::Mat>>                       coocurrenceMatricesOrientation(4);
cv::Mat                                                 temp;

int CuboidWidth     =                                   36; // width and height size
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
    unsigned char a = 10;
    unsigned char b = 20;

    std::cout << a - b << std::endl;
    //return 0;
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
            DenseSampling(imageBuffer, CuboidWidth, T, cuboids,cuboidsSize);
            updateBuffer(imageBuffer);
            
            for(int icub = 0; icub < cuboids.size();icub++){
                for(int i = 0; i < cuboids[icub].size() - 1; i++){
                    opticalFlow(orientationMatricesT,magnitudeMatricesT,cuboids[icub][i],cuboids[icub][i+1],3);
                }
                for (int io = 0; io < orientationMatricesT.size(); io++){
                    orientationMatrices.push_back(orientationMatricesT[io]);                    
                    cv::Mat ang= CoocurrenceFromSingleMatrixAngle(orientationMatricesT[io], dx, 0, 8, 315, CuboidWidth);
                    
                    // 0  º
                    coocurrenceMatricesOrientation[0].push_back(ang);
                    // 45 º
                    coocurrenceMatricesOrientation[1].push_back(CoocurrenceFromSingleMatrixAngle(orientationMatricesT[io], dx, -dy, 8, 315, CuboidWidth));
                    // 90 º
                    coocurrenceMatricesOrientation[2].push_back(CoocurrenceFromSingleMatrixAngle(orientationMatricesT[io], 0, -dy, 8, 315 ,CuboidWidth));
                    // 135º
                    coocurrenceMatricesOrientation[3].push_back(CoocurrenceFromSingleMatrixAngle(orientationMatricesT[io], -dy, -dx, 8, 315, CuboidWidth));
                }
                //std::cout << "Sz cooc "<<coocurrenceMatricesOrientation[0].size()<<", "<<coocurrenceMatricesOrientation[0][0].size()<<std::endl;
                for (int im = 0; im < magnitudeMatricesT.size(); im++){
                    magnitudeMatrices.push_back(magnitudeMatricesT[im]);

                    cv::Mat mg = CoocurrenceFromSingleMatrixMag(magnitudeMatricesT[im], dx, 0, CuboidWidth);
                    coocurrenceMatricesMagnitud[0].push_back(mg);
                    coocurrenceMatricesMagnitud[1].push_back(CoocurrenceFromSingleMatrixMag(magnitudeMatricesT[im],   dx, - dy, CuboidWidth));
                    coocurrenceMatricesMagnitud[2].push_back(CoocurrenceFromSingleMatrixMag(magnitudeMatricesT[im],    0, - dy, CuboidWidth));
                    coocurrenceMatricesMagnitud[3].push_back(CoocurrenceFromSingleMatrixMag(magnitudeMatricesT[im], - dy, - dx, CuboidWidth));
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

            std::cout << "cuboidsSize = " << cuboidsSize << std::endl;
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
            
            std::cout << "Final size matrix 1: " << orientationMatrices.size() << std::endl;
            std::cout << "Final size matrix 2: " << magnitudeMatrices.size()   << std::endl;
            
            //std::cout << "cuboidsSize = " << cuboidsSize << std::endl;
            
            cv::Mat CUBOID_IMG = cv::Mat::zeros(cv::Size(cuboidsSize.width*CuboidWidth, cuboidsSize.height*CuboidWidth), CV_8U);;
            
            for(int icub = 8; icub < magnitudeMatrices.size(); icub+=9 )
            {
                for(int i = 0; i<magnitudeMatrices[icub].size(); i++) {
                    for(int j = 0; j<magnitudeMatrices[icub][i].size(); j++) {
                        //std::cout << "i,j = " << i << ", " << j << std::endl;
                        //std::cout << "==>   " << i  << "+ "<<  int(CuboidWidth/2)*((icub/9)/cuboidsSize.width) << " , " << j <<" + " <<  int(CuboidWidth/2)*((icub/9)%cuboidsSize.width) << std::endl;
                        int val = (int)pow(2.0,(double)magnitudeMatrices[icub][i][j] + 1) - 1;
                        //if (val > 1)
                        //    std::cout << "val = " << val << std::endl;
                        CUBOID_IMG.at<unsigned char>(i + CuboidWidth*((icub/9)/(cuboidsSize.width)), j + CuboidWidth*((icub/9)%(cuboidsSize.width))) = val;     
                    }
                }          
            }

            cv::Mat orientationImg = cv::Mat::zeros(cv::Size(cuboidsSize.width*CuboidWidth, cuboidsSize.height*CuboidWidth), CV_8U);;
            int valOrientation;
            for(int icub = 8; icub < orientationMatrices.size(); icub+=9 )
            {
                for(int i = 0; i<orientationMatrices[icub].size(); i++) {
                    for(int j = 0; j<orientationMatrices[icub][i].size(); j++) {
                        if((int)pow(2.0,(double)magnitudeMatrices[icub][i][j]) > 1){
                            valOrientation = (int)(orientationMatrices[icub][i][j] * 45);
                            //std::cout << " val " << valOrientation << std::endl;
                        }
                        else{
                            valOrientation = 180;
                        }
                        orientationImg.at<unsigned char>(i + CuboidWidth*((icub/9)/cuboidsSize.width), j + CuboidWidth*((icub/9)%cuboidsSize.width)) = valOrientation;
                        
                    }
                }          
            }
            
            //Mat2Mat(frame        , temp, 0              ,               0);
            cv::resize(CUBOID_IMG,CUBOID_IMG, cv::Size(imageSize.width, imageSize.height));
            cv::resize(orientationImg,orientationImg, cv::Size(imageSize.width, imageSize.height));
            //Mat2Mat(CUBOID_IMG        , temp, 120              ,               0);
            cv::applyColorMap(orientationImg, orientationImg, cv::COLORMAP_HSV);
            cv::imshow("cuboid", CUBOID_IMG); 
            cv::imshow("Map color", orientationImg); 
            /*std::cout << " ORIENTATION " << std::endl;
            for(int i = 0; i<orientationMatrices[0].size(); i++) {
                for(int j = 0; j<orientationMatrices[0][i].size(); j++) {
                    std::cout << '(' << orientationMatrices[orientationMatrices.size()-1][i][j] * 35<< ")";
                }
                std::cout << "\n";
            }*/
              /*
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
            for(int qq = 0; qq < 4; qq++){
                coocurrenceMatricesMagnitud[qq].clear();
                coocurrenceMatricesOrientation[qq].clear();
            }
        }
        //std::cout << "Buffer Size : " << imageBuffer.size() << std::endl;
        k = cv::waitKey(30);
        count +=1;
        if ( k == 27)
            break;
    }
    return 0;
}