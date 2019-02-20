#include "headers/kmeans.hpp"


kmeans::kmeans(std::vector<std::vector<float>> features,std::vector<std::vector<float>> centers,
                            int numberClass){
    this->features    = features;
    this->numberClass = numberClass;
    this->centers     = centers;
    cluster = new std::vector<std::vector<float>>[numberClass];
}

void kmeans::runKmeans(std::vector<int>& result){
    //startingCenters();
    oldCenters = centers;
    while (flag == false){
        for(auto feature : features){
            int goodClass = getGoodCluster(feature);
            cluster[goodClass].push_back(feature);
        }
        for(int k = 0;k < numberClass;k++){
            for(int i = 0;i < cluster[k].size();i++){
                for(int j = 0;j < cluster[k].size();j++){
                    if(i != j){
                        int sum = 0;
                        for(int val = 0;val < cluster[k][i].size();val++){
                            centers[k][val] += val/cluster[k].size();
                        }
                    } 
                }
            }
        }
        for(int k = 0;k < numberClass;k++){
            for(int i = 0;i < centers[numberClass].size();i++){
                float sum = 0.0;
                for(auto cValue : centers[numberClass]){
                    for(auto vOldCenter : oldCenters[k]){
                        sum += pow(vOldCenter - cValue,2);
                    }
                }
                sum = sqrt(sum);
                if(sum <= limitError)
                    flag *= true;
                else{
                    flag *= false;
                }
            }
        }
    }
    for(int k = 0;k < numberClass;k++){
        result.push_back(cluster[k].size());
    }
}

void kmeans::startingCenters(){
    for(int k = 0;k < numberClass;k++){
        int random = rand() % (features.size() - 1);
        centers.push_back(features[random]);
    }  
}   

int kmeans::getGoodCluster(std::vector<float> feat){
    int gc;
    
    for(int k = 0;k < numberClass;k++){
        float sum = 0.0;
        for(auto value : feat){
            for(auto center : centers[k]){
                sum += pow(center - value,2);
            }
        }
        sum = sqrt(sum);
        if(sum < minError){
            minError = sum;
            gc = k;
        }
    }
    return gc;
}