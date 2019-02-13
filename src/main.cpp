#include "headers/utils.h"

cv::Mat                             frame;
cv::Size                            imageSize;
std::vector<cv::Mat>                imageBuffer;
std::vector<std::vector<cv::Mat>>   cuboids;
cv::Size                            cuboidsSize;
std::string PATH_DATA = "../data/";
std::vector<std::vector<std::vector<int>>>               orientationMatricesT;
std::vector<std::vector<std::vector<int>>>                magnitudeMatricesT;
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
        
        if (imageBuffer.size() > T)
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
            
            for(int c = 0; c < cuboids.size();c++){
                //std::cout << " cuboids " << c << std::endl;
                //std::cout << "here 1" << std::endl;
                for(int i = 0; i < cuboids[c].size() - 1; i++){
                    int j = i + 1;
                    if(j < cuboids[c].size()){
                        
                        opticalFlow(orientationMatricesT,magnitudeMatricesT,cuboids[c][i],cuboids[c][j],3);
                        
                    }
                }
                std::cout << "Orientation " << std::endl;
                for(int i = 0; i<orientationMatricesT[0].size(); i++) {
                    for(int j = 0; j<orientationMatricesT[0][i].size(); j++) {
                        std::cout << '(' << orientationMatricesT[0][i][j] << ")";
                    }
                    std::cout << "\n";
                }

                std::cout << "Magnitude " << std::endl;
                for(int i = 0; i<magnitudeMatricesT[0].size(); i++) {
                    for(int j = 0; j<magnitudeMatricesT[0][i].size(); j++) {
                        std::cout << '(' << magnitudeMatricesT[0][i][j] << ")";
                    }
                    std::cout << "\n";
                }

                /*for(auto om : orientationMatrices10)
                    orientationMatrices.push_back(om);
                                std::cout << "here 3" << orientationMatrices.size() << std::endl;

                for(auto mm : magnitudeMatrices10)
                    magnitudeMatrices.push_back(mm);
                std::cout << "here 4"  << magnitudeMatrices.size()  << std::endl;
             */
                //std::cout << "here 4"  << orientationMatricesT.size()  << std::endl;
                //std::cout << "here 4"  << magnitudeMatricesT.size()  << std::endl;
                orientationMatricesT.clear();
                magnitudeMatricesT.clear();
                
                
            }
            

            std::cout << "Size" << orientationMatrices.size() << std::endl;

            cuboids.clear();
            cv::imshow("frame 10 : ", frame);
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