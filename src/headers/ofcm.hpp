#ifndef OFCM_HPP
#define OFCM_HPP

#include <vector>
#include "utils.hpp"

/*
* cuboidsSize           : [7 x 5]
* framesFeatures size   : 35 x 864
*/

class OFCM
{
    public:
        // reRows = 108, reCols = 144 (cuboidDim = 36)
        int reRows;
        int reCols;

        OFCM(int re_rows,int re_cols);
        ~OFCM();
        std::vector<std::vector<std::vector<float>>> get_features(cv::VideoCapture capTemp, std::pair<int,int> sequence);

    private:
        int windSize = 31;
        int Levels   = 3;

        int cuboidDim       =                                   36; // width and height size
        int T               =                                   10; // number of frames
        int dx              =                                   1;
        int dy              =                                   1;
        int tWidth          =                                   160;
        int tHeight         =                                   120;       
};

#endif