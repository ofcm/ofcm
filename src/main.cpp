#include "headers/utils.h"

cv::Mat                             frame;
cv::Size                            imageSize;
std::vector<cv::Mat>                imageBuffer;
std::vector<std::vector<cv::Mat>>   cuboids;
cv::Size                            cuboidsSize;
std::string PATH_DATA = "../data/";
std::vector<cv::Mat>                orientationMatrices10;
std::vector<cv::Mat>                magnitudeMatrices10;
std::vector<cv::Mat>                orientationMatrices;
std::vector<cv::Mat>                magnitudeMatrices;

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
        
        if (imageBuffer.size() > 20)
        {
            /*
            for(int qq = 0; qq < imageBuffer.size(); qq++){
                std::string st ="imb";
                st.append(std::to_string(qq));
                st.append(".png");
                cv::imwrite(st, imageBuffer[qq]);
            }
            */
            

            DenseSampling(imageBuffer, N, T, cuboids,cuboidsSize);

            updateBuffer(imageBuffer);
            //std::cout << "1" << std::endl;
            
            /*      
            *
            *  CODE HERE : pitufo
            *  =========
            * */
            
            for(auto c : cuboids){
                for(int i = 0; i < c.size(); i++){
                    int j = i + 1;
                    if(j < c.size()){
                        opticalFlow(orientationMatrices10,magnitudeMatrices10,c[i],c[j],0);
                    }
                }
                for(auto om : orientationMatrices10)
                    orientationMatrices.push_back(om);
                        
                for(auto mm : magnitudeMatrices10)
                    magnitudeMatrices.push_back(mm);

                orientationMatrices10.clear();
                magnitudeMatrices10.clear();
            }


            std::cout << orientationMatrices.size() << std::endl;
            std::cout << "Orientation matrix 10: " << orientationMatrices[9]  << std::endl;
            std::cout << "Magnitude matrix 10: " << magnitudeMatrices[9]  << std::endl;
            cuboids.clear();
            /*
            cv::imshow("frame 10 : ", frame);
            cv::moveWindow("frame 10 : ", 200,200);
            cv::imshow("Some Cubid (Resize): 9 ", cuboids[80][0]);
            cv::moveWindow("Some Cubid (Resize): ", 100,100);
            cv::imshow("Some Cubid (Resize): 30", cuboids[80][19]);
            break;*/
        }
        //std::cout << "Buffer Size : " << imageBuffer.size() << std::endl;
        k = cv::waitKey(30);
        
        if ( k == 27)
            break;
    }
    return 0;
}