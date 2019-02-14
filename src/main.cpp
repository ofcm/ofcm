#include "headers/utils.h"

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

int N = 18; // width and height size
int T = 10; // number of frames

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

        imageSize = frame.size();
        cv::Mat fr = frame.clone();

        imageBuffer.push_back(fr);
        // Obtain initial set of features
        
        if (imageBuffer.size() >= T)
        {
            DenseSampling(imageBuffer, N, T, cuboids,cuboidsSize);
            updateBuffer(imageBuffer);
            
            for(int icub = 0; icub < cuboids.size();icub++){
                for(int i = 0; i < cuboids[icub].size() - 1; i++){
                    opticalFlow(orientationMatricesT,magnitudeMatricesT,cuboids[icub][i],cuboids[icub][i+1],1);
                }
                for (int io = 0; io < orientationMatricesT.size(); io++)
                    orientationMatrices.push_back(orientationMatricesT[io]);

                for (int im = 0; im < magnitudeMatricesT.size(); im++)
                    magnitudeMatrices.push_back(magnitudeMatricesT[im]);

                orientationMatricesT.clear();
                magnitudeMatricesT.clear();          
            }
            //std::cout << "Final size matrix 1: " << orientationMatrices.size() << std::endl;
            //std::cout << "Final size matrix 2: " << magnitudeMatrices.size()   << std::endl;
            
            //std::cout << "Orientation " << std::endl;
            /*
            for(int i = 0; i<orientationMatrices[0].size(); i++) {
                for(int j = 0; j<orientationMatrices[0][i].size(); j++) {
                    std::cout << '(' << orientationMatrices[orientationMatrices.size()-1][i][j] << ")";
                }
                std::cout << "\n";
            }
            */

            cv::Mat CUBOID_IMG = cv::Mat::zeros(cv::Size(16*18,12*18), CV_8U);;
            //std::cout << "Magnitude " << std::endl;

            for(int icub = 0; icub < magnitudeMatrices.size(); icub+=9 )
            {
                for(int i = 0; i<magnitudeMatrices[icub].size(); i++) {
                    for(int j = 0; j<magnitudeMatrices[icub][i].size(); j++) {
                        //std::cout << '(' << (int)pow(2.0,(double)magnitudeMatrices[0][i][j]) << ")";
                        CUBOID_IMG.at<unsigned char>(i + 18*((icub/9)/16), j + 18*((icub/9)%16)) = (int)pow(2.0,(double)magnitudeMatrices[icub][i][j]);
                    }
                    //std::cout << "\n";
                }          
            }
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