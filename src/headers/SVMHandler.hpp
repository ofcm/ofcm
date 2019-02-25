# ifndef __SVMHANDLER__
# define __SVMHANDLER__

# include "libsvm.hpp"
# include <vector>
# include <iostream>
# include <stdio.h>
/*
 *  @Brief
 *  predicted:  The predicted value of labels
 *  labels:     The Set of labels
 *  features:   Features of problem
 */ 
template <class T>
class SVMhandler{
    public:
    SVMhandler(){
        this->parameters.C = 3.0;
        this->parameters.kernel_type = RBF;
        this->parameters.svm_type = C_SVC;
        this->parameters.cache_size = 2000;
        this->parameters.eps = 0.1;
        this->parameters.nr_weight = 0;
        this->parameters.gamma = 0.001;
        this->parameters.coef0 = 0;
        this->parameters.degree = 0;
        this->parameters.shrinking = 0;
        this->parameters.probability = 0;
    }
    // Data type
    private:
    svm_parameter parameters;
    svm_model * sModel = nullptr;

    svm_problem * CreateProblem(std::vector<int> predicted, std::vector<int> labels, std::vector<std::vector<T>> data){
    
        if(predicted.size() != data.size()){
            std::cout<<"# predicted  != # data\n";
            return nullptr;
        }
            
        // Number of Samples to train
        int n = predicted.size();
        // Init the svm problem
        // Warning: Possible problem creating array of 'svm_problem'
        svm_problem * problem = new svm_problem[n];
        // init the predicted pointer
        double * prd = new double[n];
        // Fill predicted to problem
        for(int i = 0; i < n; i++){
            prd[i] = double(predicted[i]);
        }
        // Fill The set of features
        svm_node ** d = CreateFeatureNodes(data);
        problem->l = n;
        problem->y = prd;
        problem->x = d;

        return problem;
    }

    svm_node ** CreateFeatureNodes(const std::vector<std::vector<T>> & data){
        // Fill The set of features
        int n = data.size();
        svm_node ** d = new svm_node * [n];
        int nFeatures = data[0].size();
        for(int i = 0; i < n; i++){
            std::vector<svm_node> nodes;
            for(int j = 0; j < nFeatures; j++){
                double valueF = data[i][j];
                //
                nodes.push_back(svm_node{j, valueF});
            }
            nodes.push_back(svm_node{-1, 0.0});
            int length = int(nodes.size());
            d[i] = new svm_node[length];
            for(int k = 0; k < length; k++)
                d[i][k] = nodes[k];
        }
        return d;
    }
    public:

    bool fit(std::vector<int> predicted, std::vector<int> labels, std::vector<std::vector<T>> data, svm_parameter * p = nullptr){
        if(p != nullptr)
            this->parameters = *p;
         
        svm_problem * problem = CreateProblem(predicted, labels, data);
        const char * logError = svm_check_parameter(problem, &this->parameters);
        if(logError != nullptr){
            std::cout<<"Error in paramerters"<<std::endl;
            return -1;
        }
        this->sModel = svm_train(problem, &this->parameters);
        
        delete problem;
        return true;
    }

    std::vector<int> predict(std::vector<std::vector<T>> data){
        int n = data.size();
        std::vector<int> predicted(n);
        if(this->sModel == nullptr){
            std::cout<<"Model is not Fitted yet!"<<std::endl;
            return predicted;
        }
        // Fill The set of features
        svm_node ** d = CreateFeatureNodes(data);
        for(int i = 0; i < n; i++){
            int lbl = svm_predict(this->sModel, d[i]);
            predicted[i] = lbl;
        }
        return predicted;
    }

    /*
     * Cumpute The accuracy of certain data
     */
    float validate(const std::vector<std::vector<T>> & data, std::vector<int> y){
        if(this->sModel==nullptr){
            std::cout<<"Model is not fitted yet!"<<std::endl;
            return 0.0;
        }
        std::vector<int> y_hat = this->predict(data);
        int n = data.size();
        float accuracy = 0.0f;
        for(int i = 0; i < n; i++){
            if(y_hat[i] == y[i]){
                accuracy += 1.0f;
            }
        }
        accuracy /= float(n);
        return accuracy;
    }
    void setKernell(int kernel_type){
        if(kernel_type != LINEAR && kernel_type !=  POLY && kernel_type != RBF && kernel_type != SIGMOID && kernel_type != PRECOMPUTED){
            std::cout<<"Kernell not available!!"<<std::endl;
            return;
        }
        this->parameters.kernel_type = kernel_type;
    }
    int SaveModel(std::string modelname){
        if(this->sModel == nullptr){
            std::cout<<"There is not model fitted yet!!"<<std::endl;
            return -1;
        }
        FILE * pFile;
        pFile = fopen(modelname.c_str(), "w");
        int retval = svm_save_model(pFile, this->sModel);
        fclose(pFile);
        return retval;
    }

    bool LoadModelFromFile(std::string modelname){
        FILE * pFile;
        if(this->sModel != nullptr)
            delete this->sModel;
        pFile = fopen(modelname.c_str(), "r");

        this->sModel = svm_load_model(pFile);
        fclose(pFile);

        return 0;
    }

        bool shuffle_randomly_data(const std::vector<std::vector<T>> data, const std::vector<int> y, std::vector<std::vector<T>> & data_shuff, std::vector<int> & y_shuff){
        std::srand ( unsigned ( time(0) ) );
        std::vector<unsigned long> indexes(data.size());
        for(unsigned long i = 0; i < indexes.size(); i++){
            indexes[i] = i;
        }
        unsigned long inisum = 0;
        for(unsigned long i = 0; i < indexes.size(); i++){
            inisum += indexes[i];
        }
        //std::cout<<inisum<<std::endl;
        std::random_shuffle ( indexes.begin(), indexes.end() );
        
        data_shuff = data;
        y_shuff = y;
        for(int i = 0; i < indexes.size(); i++){
            data_shuff[i] = data[indexes[i]];
            y_shuff[i] = y[indexes[i]];
        }

        //unsigned long sum = 0;
        //for (std::vector<int>::iterator it=indexes.begin(); it!=indexes.end(); ++it){
        //    sum += *it;
            //std::cout  << *it<<std::endl;
        //}
        //for (unsigned long i = 0; i < indexes.size(); i++){
        //    sum += indexes[i];
            //std::cout  << indexes[i]<<std::endl;
        //}
        
        //std::cout<<"sum> "<<sum<<std::endl;
        //std::cout<<"Size> "<<indexes.size()<<std::endl;
        return true;
    }

    bool split_data_train(const std::vector<std::vector<T>> data, const std::vector<int> y, std::vector<std::vector<T>> & data_train, std::vector<int> & y_train, std::vector<std::vector<T>> & data_test, std::vector<int> & y_test, float test_percent = 0.3){
        std::vector<std::vector<T>> data_shuff;
        std::vector<int> y_shuff;
        shuffle_randomly_data(data, y, data_shuff, y_shuff);
        unsigned long n_size = data.size();
        unsigned long n_train_size = n_size -  test_percent * n_size;
        
        int nFeatures = data[0].size();
        //std::vector<std::vector<T>> data_train;
        for(unsigned long i = 0; i < n_size; i++){
            if(i < n_train_size){
                data_train.push_back(data_shuff[i]);
                y_train.push_back(y_shuff[i]);
            }
            else
            {
                data_test.push_back(data_shuff[i]);
                y_test.push_back(y_shuff[i]);
            }
        }
        std::cout<<"SIZE data train> \t["<<data_train.size()<<", "<<data_train[0].size()<<"]"<<std::endl;
        std::cout<<"SIZE y_train> \t\t["<<y_train.size()<<"]"<<std::endl;

        std::cout<<"SIZE data test> \t["<<data_test.size()<<", "<<data_test[0].size()<<"]"<<std::endl;
        std::cout<<"SIZE y_test> \t\t["<<y_test.size()<<"]"<<std::endl;
        
    }


};
# endif