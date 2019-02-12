#include "headers/utils.h"

cv::Mat                             frame;
cv::Size                            imageSize;
std::vector<cv::Mat>                imageBuffer;
std::vector<std::vector<cv::Mat>>   Cuboids;

std::string PATH_DATA = "../data/";

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

    for (;;)
    {
        cap >> frame;

        if(frame.empty())
            break;    

        imageSize = frame.size();

        imageBuffer.push_back(frame);

        if (imageBuffer.size() > 10)
        {

            DenseSampling(imageBuffer, N, T, Cuboids);
            updateBuffer(imageBuffer);

            /*
            *
            *  CODE HERE : pitufo
            *  =========
            * 
            */
            cv::imshow("frame 10 : ", frame);
            cv::moveWindow("frame 10 : ", 100,100);
            cv::imshow("Some Cubid (Resize): ", Cuboids[0][9]);
            cv::moveWindow("Some Cubid (Resize): ", 100,100);
        }
        //std::cout << "Buffer Size : " << imageBuffer.size() << std::endl;
        k = cv::waitKey(30);
        
        if ( k == 27)
            break;
    }
    return 0;
}