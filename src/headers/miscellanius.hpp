# ifndef __MISCC__
# define __MISCC__

//# include <opencv2/opencv.hpp>
//# include <vector>
# include "utils.hpp"
# include <fstream>

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
            std::cout << matrix[tt][hh] << std::endl;;
        }
        std::cout<<std::endl;
    }
}

template <class T>
class FileHandler{

    private:
    std::fstream ftrain;
    std::fstream flabel;
    std::string trainfile;
    std::string labelfile;
    public:
    FileHandler(std::string tr, std::string lbl):trainfile(tr), labelfile(lbl){};

    void AppendLine(std::vector<T> data, int label){
        if(data.size() == 0){
            std::cout<<"Non data in vector!"<<std::endl;
            return;
        }
        if(!this->ftrain.is_open())
            this->ftrain.open(this->trainfile, std::ios_base::app);
       
        // Row data feature
        int nFeatures = data.size();
        
        std::string line = "";
        for(int j = 0; j < nFeatures; j++){
            line.append(std::to_string(data[j]));
            if(j!=nFeatures - 1)
                line.append(",");
            else
                line.append("\n");
        }
        if(!this->flabel.is_open())
            flabel.open(this->labelfile, std::ios_base::app);
        
        // Preparing Label
        std::string linelbl = "";
        linelbl.append(std::to_string(label));
        linelbl.append("\n");

        // Writing in files
        ftrain.write(line.c_str(), line.size()*sizeof(char));
        flabel.write(linelbl.c_str(), linelbl.size() * sizeof(char));
    }
    bool LoadFromFile(std::vector<std::vector<T>> & data, std::vector<int> & y){
        this->Release();
        this->ftrain.open(trainfile, std::fstream::in);
        this->flabel.open(labelfile, std::fstream::in);
        
        // ** Reading training file

    }
    void Release(){
        if(this->ftrain.is_open())
            this->ftrain.close();
        if(this->flabel.is_open())
            this->flabel.close();
    }
};

# endif