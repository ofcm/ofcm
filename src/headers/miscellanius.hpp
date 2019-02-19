# ifndef __MISCC__
# define __MISCC__

# include <opencv2/opencv.hpp>
# include <vector>

template <class T>
void printMatInConsole(const cv::Mat & matrix){
    for(int tt = 0; tt< matrix.rows; tt++) {
        for(int hh = 0; hh< matrix.cols; hh++) {
            std::cout << '(' << matrix.at<T>(tt, hh) << ")";
        }
        std::cout<<std::endl;
    }
}
template <class T>
void print2DstdVector(const std::vector<std::vector<T>> matrix){
    for(int tt = 0; tt< matrix.size(); tt++) {
        for(int hh = 0; hh< matrix[0].size(); hh++) {
            std::cout << '(' << matrix[tt][hh] << ")";
        }
        std::cout<<std::endl;
    }
}
# endif