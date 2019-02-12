#include "headers/utils.h"

cv::Mat frame;
cv::Size imageSize;

std::vector<cv::Mat> imageBuffer;

std::string PATH_DATA = "../data/";

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

        imageSize = frame.size();

        imageBuffer.push_back(frame);

        if (imageBuffer.size() > 10)
        {
            updateBuffer(imageBuffer);

            /*
            *
            *  CODE HERE : pitufo
            *  =========
            * 
            */
        }
        std::cout << "Buffer Size : " << imageBuffer.size() << std::endl;
        k = cv::waitKey(30);
        
        if ( k == 27)
            break;
    }
    return 0;
}