# include "SVMHandler.hpp"
# include <vector>

int n = 10;
int main(){
    std::cout<<"hello"<<std::endl;
    std::vector<std::vector<float>> data{{0.8f , 0.8f}, {0.7f , 0.7f},
                                        {0.9f , 0.8f}, {-0.8f , -0.9f},
                                        {-0.8f , -0.7f}, {-0.7f , -0.7f}};
    
    std::vector<int> y{1, 1, 1, -1, -1 ,-1};
    std::vector<int> labels{-1, 1};

    // Create SVM Handler
    SVMhandler svmhandler;
    // Train the model
    bool val = svmhandler.fit(y, labels, data, nullptr);
    std::cout<<"Training successfully>> " << val<<std::endl;

    std::vector<std::vector<float>> query{{0.7, 0.95},{-0.1, - 0.6}, {-1.0, -0.98}, {0.34, 0.77}};
    // Predict
    std::vector<int> answer = svmhandler.predict(query);

    for(int i = 0; i < answer.size(); i++){
        std::cout<<answer[i]<<std::endl;
    }
}