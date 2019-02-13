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
            DenseSampling(imageBuffer, N, T, cuboids,cuboidsSize);

            updateBuffer(imageBuffer);
            
            for(int c = 0; c < cuboids.size();c++){
                for(int i = 0; i < cuboids[c].size() - 1; i++){
                    int j = i + 1;
                    if(j < cuboids[c].size()){
                        opticalFlow(orientationMatricesT,magnitudeMatricesT,cuboids[c][i],cuboids[c][j],3);

                    }
                }
                            
                for(auto om : orientationMatricesT)
                    orientationMatrices.push_back(om);

                for(auto mm : magnitudeMatricesT)
                    magnitudeMatrices.push_back(mm);

                orientationMatricesT.clear();
                magnitudeMatricesT.clear();
                
            }
            
            
            /*      
            *
            *  CODE HERE : pitufo
            *  =========
            * */
            //std::cout << magnitudeMatrices.size() << std::endl;
            /********
             * PRINT LAST MATRICES
             */
            std::cout << "Orientation " << std::endl;
            for(int i = 0; i<orientationMatrices[orientationMatrices.size()-1].size(); i++) {
                for(int j = 0; j<orientationMatrices[orientationMatrices.size()-1][i].size(); j++) {
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

            cuboids.clear();
            cv::imshow("frame 10 : ", frame);

            /*
            cv::moveWindow("frame 10 : ", 200,200);
            cv::imshow("Some Cubid (Resize): 9 ", cuboids[80][0]);
            cv::moveWindow("Some Cubid (Resize): ", 100,100);
            cv::imshow("Some Cubid (Resize): 30", cuboids[80][19]);
            break;*/
        }
        //std::cout << "Buffer Size : " << imageBuffer.size() << std::endl;
        k = cv::waitKey(30);
        count +=1;
        if ( k == 27)
            break;
    }
    return 0;
}