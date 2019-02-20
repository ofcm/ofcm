#include "headers/kmeans.hpp"


kmeans::kmeans(std::vector<std::vector<float>> features,std::vector<std::vector<float>> centers,
                            int numberClass){
    this->features    = features;
    this->numberClass = numberClass;
    this->centers     = centers;
    
}

void kmeans::runKmeans(std::vector<int>& result){
    std::vector<std::vector<std::vector<float>>> cluster(numberClass);
    oldCenters = centers;

    std::cout << "numberClass = " << numberClass << std::endl;
    int itr = 0;
    while (flag == false){
        flag = true;
        std::cout << "itr = " << itr << std::endl;
        for(auto feature : features){
            int goodClass = getGoodCluster(feature);
            //std::cout << "goodClass = " << goodClass << std::endl;
            cluster[goodClass].push_back(feature);
            //std::cout << "cluster " << goodClass << " = " <<cluster[goodClass].size() << std::endl;
        }
        for(int k = 0;k < numberClass;k++){
            std::cout << "size = " << k << " -> "<< cluster[k].size() << std::endl;
        }
        std::vector<std::vector<float>> newCenters(numberClass, std::vector<float>(12,0.0));
        for(int k = 0;k < numberClass;k++){
            for(int i = 0;i < cluster[k].size();i++){
                for(int val = 0;val < cluster[k][i].size();val++){
//                    std::cout << "k = "<< k << ", val = "<< val << " (i) = "<< i <<std::endl;
                    newCenters[k][val] += cluster[k][i][val]/cluster[k].size();
                }
            }
        }

        
        for(int k = 0;k < numberClass;k++){

            float sum = 0.0;

            //std::cout << "----->" << oldCenters[0].size() << std::endl;

            for(int i =0; i< newCenters[k].size();i++)
            {
                sum += ((oldCenters[k][i] - newCenters[k][i])*(oldCenters[k][i] - newCenters[k][i]));
            }
                
            sum = sqrt(sum);
            std::cout << "sum = " << sum << std::endl;
            if(sum <= limitError)
            {
                flag *= true;
            }
            else{
                flag *= false;
            }
            
        }
        itr++;
        oldCenters = newCenters;
    }
    for(int k = 0;k < numberClass;k++){
        std::cout << "size = " << k << " -> "<< cluster[k].size() << std::endl;
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