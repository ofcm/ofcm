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
   /*
    * @Overview: "CoocurrenceFromSingleMatrixMag"
    * matrix > h^(M) matrix of magnitudes, values must be passed between 0 to 8
    * d_ to compare the intensityof current index to a displacement index
    * dx > Given an offset dx 
    * dy > Given an offset dy
    */ 
    cv::Mat CoocurrenceFromSingleMatrixMag(std::vector<std::vector<int>> matrix, int dx, int dy, int cuboidSize = 18){
    int nvalues = 8;
    // Historigram
    cv::Mat output = cv::Mat::zeros(nvalues, nvalues, CV_32FC1);
    
    for(int i = 0; i < cuboidSize; i++){
        for(int j = 0; j < cuboidSize; j++){
            if(isWithInMatrix(i + dy, j + dx, cuboidSize, cuboidSize)){
                int val1 = matrix[i][j];
                int val2 = matrix[i + dy][j + dx];
                //* since it is normalized from 0 to eight
                //* Increment the historigram at values (v1, v2)
                output.at<float>(val1, val2)++;
            }    
        }
    }
    return output;
    }
    cv::Mat CoocurrenceFromSingleMatrixAngle(const std::vector<std::vector<int>> & matrix, int dx, int dy, int nbins = 4, int maxangle = 135, int cuboidSize = 18){
        // Historigram
        int widthbin = maxangle/(nbins-1);
        cv::Mat output = cv::Mat::zeros(nbins, nbins, CV_32FC1);
        for(int i = 0; i < cuboidSize; i++){
            for(int j = 0; j < cuboidSize; j++){
                if(isWithInMatrix(i + dy, j + dx, cuboidSize, cuboidSize)){
                    int val1 = matrix[i][j]/widthbin;
                    int val2 = matrix[i + dy][j + dx]/widthbin;
                    //* since it is normalized from 0 to eight
                    //* Increment the historigram at values (v1, v2)
                    output.at<float>(val1, val2)++;
                }    
            }
        }
        return output;
    }
}

# endif