# ifndef __SVMHANDLER__
# define __SVMHANDLER__

# include "libsvm.hpp"
# include <vector>
# include <iostream>
/*
 *  @Brief
 *  predicted:  The predicted value of labels
 *  labels:     The Set of labels
 *  features:   Features of problem
 */ 
class SVMhandler{
    public:
    SVMhandler(){
        this->parameters.C = 0.1;
        this->parameters.kernel_type = LINEAR;
        this->parameters.svm_type = C_SVC;
        this->parameters.cache_size = 10;
        this->parameters.eps = 0.001;
        this->parameters.nr_weight = 0;
        this->parameters.gamma = 0;
        this->parameters.coef0 = 0;
        this->parameters.degree = 0;
        this->parameters.shrinking = 0;
        this->parameters.probability = 0;
    }
    // Data type
    private:
    svm_parameter parameters;
    svm_model * sModel = nullptr;
    template <class T>
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
    template <class T>
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
    template <class T>
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
        return true;
    }

    template <class T>
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


};
# endif