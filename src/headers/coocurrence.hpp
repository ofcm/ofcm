# ifndef __COOCURRENCE__
# define __COOCURRENCE__

# include <math.h>
# include <opencv2/opencv.hpp>
# include <vector>
/*
 * Define the offset  
 */
/*
struct Offset_
{
    int dx, dy;
    Offset_(float angle, int d){
        dx = int(d * cos(angle) + 0.5f);
        dy = int(d * sin(angle) + 0.5f);
    }
};

class Coocurrence{
    private:
        Offset_ offset_o;
        std::vector<cv::Mat> CoocurrTheta;
        std::vector<cv::Mat> CoocurrMag;
    public:
        // Recive angle & the distance d
        Coocurrence(float, int);
        std::vector<cv::Mat> ComputeMatrixesFromCuboid(const std::vector<cv::Mat> & cuboid);
};*/
bool isWithInMatrix(int h, int w, int maxheight, int maxwidth){
    if(h >= maxheight || h < 0 || w >= maxwidth || w < 0)
        return false;
    else
        return true;
}

namespace coocurrence{
    cv::Mat CoocurrenceFromSingleMatrixMag(const std::vector<std::vector<int>> & matrix, int dx, int dy, int cuboidSize = 18);
    cv::Mat CoocurrenceFromSingleMatrixAngle(const std::vector<std::vector<int>> & matrix, int dx, int dy, int cuboidSize = 18, int nbins = 4, int maxangle = 135);
    
}

# endif