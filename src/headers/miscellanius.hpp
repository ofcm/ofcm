# ifndef __MISCC__
# define __MISCC__

//# include <opencv2/opencv.hpp>
//# include <vector>
# include "utils.hpp"
# include <fstream>
# include <typeinfo>
template <class T>
class SingleFileHandler;

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

template <class T, class L>
class FileHandlerML{

    private:
    SingleFileHandler<T> * fdatahandler;
    SingleFileHandler<L> * flabelhandler;

    public:
    FileHandlerML(std::string tr, std::string lbl){
        fdatahandler = new SingleFileHandler<T>(tr);
        flabelhandler = new SingleFileHandler<L>(lbl);
        //trainfile(tr), labelfile(lbl){};
    }
    void AppendRow(std::vector<T> data, L label){
        fdatahandler->AppendLine(data);
        std::vector<L> lab{label};
        flabelhandler->AppendLine(lab);
    }
    void Release(){
        fdatahandler->Release();
        flabelhandler->Release();
    }
    ~FileHandlerML(){
        delete fdatahandler;
        delete flabelhandler;
    }
    bool LoadFromFile(std::vector<std::vector<T>> & data, std::vector<L> & y){
        if(data.size() == 0){
            std::cout<<"Non data in vector!"<<std::endl;
            return false;
        }
        this->fdatahandler->LoadFromFile(data);
        std::vector<std::vector<L>> lbls;
        this->flabelhandler->LoadFromFile(lbls);
        for(int i = 0; i < lbls.size(); i++)
            y.push_back(lbls[0]);
        return true;
    }

    //std::fstream ftrain;
    //std::fstream flabel;
    //std::string trainfile;
    //std::string labelfile;
/*
    template <class P>
    std::vector<int> ParseLine(std::string line){
        std::vector<int> answer;
        std::string numberst = "";
        for(int i = 0; i < line.size(); i++){
            if(line[i] != ','){
                numberst.append(1, line[i]);
            }
            else{
                try{
                    int num = std::stoi(numberst);
                    answer.push_back(num);
                    numberst = "";
                }
                catch(const std::invalid_argument & ia){
                    std::cout<<"Bad parsing!>> "<<numberst<<std::endl;
                }
                catch(const std::out_of_range & orng){
                    std::cout<<"Out rangeZ! "<<numberst<<std::endl;
                }
            }
        }
        if(numberst.size() > 0){
            try{
                int num = std::stoi(numberst);
                answer.push_back(num);
                numberst = "";
            }
            catch(const std::invalid_argument & ia){
                std::cout<<"Bad parsing!>> "<<numberst<<std::endl;
            }
            catch(const std::out_of_range & orng){
                std::cout<<"Out rangeZ! "<<numberst<<std::endl;
            }
        }
        return answer;
    }*/

    /*void AppendLine(std::vector<T> data, int label){
        if(data.size() == 0){
            std::cout<<"Non data in vector!"<<std::endl;
            return;
        }
        if(!this->ftrain.is_open())
            this->ftrain.open(this->trainfile, std::ios_base::app | std::fstream::out);

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
            flabel.open(this->labelfile, std::ios_base::app | std::fstream::out);

        // Preparing Label
        std::string linelbl = "";
        linelbl.append(std::to_string(label));
        linelbl.append("\n");

        // Writing in files
        ftrain.write(line.c_str(), line.size()*sizeof(char));
        flabel.write(linelbl.c_str(), linelbl.size() * sizeof(char));
    }*/
    /*bool LoadFromFile(std::vector<std::vector<T>> & data, std::vector<int> & y){
        this->Release();
        this->ftrain.open(trainfile, std::fstream::in);
        this->flabel.open(labelfile, std::fstream::in);

        // Reading line training file
        std::string linetr;

        while(std::getline(this->ftrain, linetr)){
            std::vector<int> ans = ParseLine(linetr);
            data.push_back(ans);
            //std::cout<<ans.size()<<std::endl;
        }
        // Reading line of Labels

        // ** Reading labels file
        std::string linelbl;
        while(std::getline(this->flabel, linelbl)){
            std::vector<int> ans  = ParseLine(linelbl);
            if(ans.size() == 1){
                y.push_back(ans[0]);
            }
        }
        //std::cout<<y.size()<<std::endl;
    }*/
    /*void Release(){
        if(this->ftrain.is_open())
            this->ftrain.close();
        if(this->flabel.is_open())
            this->flabel.close();
    }*/
};

template <class T>
class SingleFileHandler{
    private:
    std::fstream fhanler;
    std::string filename;

    public:
    std::vector<T> ParseLine(std::string line){
        std::vector<T> answer;
        std::string numberst = "";
        for(int i = 0; i < line.size(); i++){
            if(line[i] != ','){
                numberst.append(1, line[i]);
            }
            else{
                try{
                    T num;
                    if(typeid(T) == typeid(int))
                        num = std::stoi(numberst);
                    else if (typeid(T) == typeid(float))
                        num = std::stof(numberst);

                    answer.push_back(num);
                    numberst = "";
                }
                catch(const std::invalid_argument & ia){
                    std::cout<<"Bad parsing!>> "<<numberst<<std::endl;
                }
                catch(const std::out_of_range & orng){
                    std::cout<<"Out rangeZ! "<<numberst<<std::endl;
                }
            }
        }
        if(numberst.size() > 0){
            try{
                T num;
                if(typeid(T) == typeid(int))
                    num = std::stoi(numberst);
                else if (typeid(T) == typeid(float))
                    num = std::stof(numberst);

                answer.push_back(num);
                numberst = "";
            }
            catch(const std::invalid_argument & ia){
                std::cout<<"Bad parsing!>> "<<numberst<<std::endl;
            }
            catch(const std::out_of_range & orng){
                std::cout<<"Out rangeZ! "<<numberst<<std::endl;
            }
        }
        return answer;
    }

    SingleFileHandler(std::string fln):filename(fln){};

    bool LoadFromFile(std::vector<std::vector<T>> & data){
        this->Release();
        this->fhanler.open(this->filename, std::fstream::in);
        std::string line;

        while(std::getline(this->fhanler, line)){
            std::vector<T> ans = this->ParseLine(line);
            data.push_back(ans);
            //std::cout<<ans.size()<<std::endl;
        }
        this->Release();
        return true;
    }
    void Release(){
        if(this->fhanler.is_open())
            this->fhanler.close();
    }
    void AppendLine(std::vector<T> data){
        if(data.size() == 0){
            std::cout<<"Non data in vector!"<<std::endl;
            return;
        }
        if(!this->fhanler.is_open())
            this->fhanler.open(this->filename, std::ios_base::app);

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

        // Writing in files
        fhanler.write(line.c_str(), line.size()*sizeof(char));
    }
    void AppendTextLine(std::string newline){
        if(!this->fhanler.is_open())
            this->fhanler.open(this->filename, std::ios_base::app);
        std::string line = newline + "\n";
        this->fhanler.write(line.c_str(), line.size() * sizeof(char));
    }
    void SetFilename(std::string newfn){
        this->filename = newfn;
    }

    void OpenReadingfile(){
        this->Release();
        this->fhanler.open(this->filename, std::fstream::in);
    }
    bool Getline(std::string & line){
        bool val = false;
        if(this->fhanler.is_open()){
            if(std::getline(this->fhanler, line))
                val = true;
        }
        return val;
    }
};
template <class T>
void SaveCentroidsInFile(std::string filename, std::vector<std::vector<std::vector<T>>> cuboidCenters){
    std::cout<<"\n==============================================\n";
    std::cout<<"Saving File of Computed Centroids...\n"<<std::endl;
    SingleFileHandler<T> fhandler(filename);
    for (int icuboid = 0; icuboid < cuboidCenters.size(); icuboid++)
    {
        std::string nameinit = "cuboid > " + std::to_string(icuboid+1);
        fhandler.AppendTextLine(nameinit);
        for(int icluster = 0; icluster < cuboidCenters[0].size(); icluster++){
            fhandler.AppendLine(cuboidCenters[icuboid][icluster]);
        }
    }
    fhandler.Release();
    std::cout<<"Saved Centroids."<<std::endl;
    std::cout<<"\n==============================================\n";
}

template <class T>
void LoadCentroidsFromFile(std::string fname, std::vector<std::vector<std::vector<T>>> & centers){
    std::cout<<"\n==============================================\n";
    std::cout<<"Loading precomputed Centroids...\n"<<std::endl;
    SingleFileHandler<T> fhandler(fname);
    fhandler.OpenReadingfile();
    std::string line;
    bool newcuboid = false;
    int icuboid = 0;
    int icluster = 0;
    std::vector<std::vector<T>> temp;
    while(fhandler.Getline(line)){
        //std::cout<<line<<std::endl;
        if(line.size() > 0){
            if(line[0] != 'c'){
                std::vector<T> ans = fhandler.ParseLine(line);
                temp.push_back(ans);
                newcuboid = false;
            }
            else{
                if(temp.size() > 0){
                    centers.push_back(temp);
                    temp.clear();
                }
                newcuboid = true;
                icuboid++;
                icluster = 0;
            }
        }
    }


    if(temp.size() > 0)
        centers.push_back(temp);
    
    fhandler.Release();
    std::cout<<"["<<centers.size()<<"]";
    if(centers.size() > 0){
        std::cout<<"["<<centers[0].size()<<"]";
        if(centers[0].size() > 0){
           std::cout<<"["<<centers[0][0].size()<<"]"; 
        }
    }
    std::cout<<"\nCenters Loaded!";
    std::cout<<"\n==============================================\n";
}
# endif
