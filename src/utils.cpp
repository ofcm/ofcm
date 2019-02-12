#include "headers/utils.h"

void DenseSampling(std::vector<cv::Mat> imageBuffer, int N, int T, std::vector<std::vector<cv::Mat>>& Cuboids)
{
    std::vector<cv::Mat>              Cuboid;

    cv::Rect window;
    cv::Mat  crop;
    for (int i = 0; i < imageBuffer[0].rows - N + 1; i+= int(N/2))
    {
        for (int j = 0; j < imageBuffer[0].cols - N + 1; j+= int(N/2))
        {
            Cuboid.clear();
            for (int t = imageBuffer.size() - 1; t > 0; t--)
            {
                //std::cout << "i : " << i << ", j : " << j << ", t : " << t << ", size : " << imageBuffer[t].size() <<std::endl;
                window = cv::Rect(j,i, N, N);
                crop   = cv::Mat(imageBuffer[t], window);
                Cuboid.push_back(crop);
            }
            Cuboids.push_back(Cuboid);
            //std::cout << "size : " << Cuboid.size() << std::endl;
        }
    }
}
void updateBuffer(std::vector<cv::Mat>& imageBuffer)
{
    for (int i = 1; i < imageBuffer.size(); i++)
    {
        imageBuffer[i - 1] = imageBuffer[i];
    }
    
    imageBuffer.pop_back();
}