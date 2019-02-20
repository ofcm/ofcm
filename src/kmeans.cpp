#include "headers/kmeans.hpp"


kmeans::kmeans(std::vector<std::vector<float>> centers,
                            int numberClass){
    this->numberClass = numberClass;
    this->centers     = centers;    
}

void kmeans::setFeatures(std::vector<std::vector<float>> features){
    this->features = features;
}

void kmeans::runKmeans(std::vector<int>& result){
    std::vector<std::vector<std::vector<float>>> cluster(numberClass);
    oldCenters = centers;

    int itr = 0;

    flag = false;

    while (flag == false){
        flag = true;
        /*
        for (int k = 0; k < numberClass; k++)
        {
            for (int icen = 0; icen < centers[k].size(); icen++)
            {
                std::cout << oldCenters[k][icen] << " ";
            }
            std::cout << std::endl;
        }
        */
        for (int ic = 0; ic < cluster.size(); ic++)
            cluster[ic].clear();
        for(auto feature : features){
            int goodClass = getGoodCluster(feature);
            cluster[goodClass].push_back(feature);
        }

        //std::vector<std::vector<float>> newCenters(numberClass, std::vector<float>(12,0.0));

        for (int k = 0; k < numberClass; k++)
        {
            for (int icen = 0; icen < centers[k].size(); icen++)
            {
                centers[k][icen] = 0.0;
            }
        }

        for(int k = 0;k < numberClass;k++){
            for(int i = 0;i < cluster[k].size();i++){
                for(int val = 0;val < cluster[k][i].size();val++){
//                    std::cout << "k = "<< k << ", val = "<< val << " (i) = "<< i <<std::endl;
                    centers[k][val] += cluster[k][i][val]/cluster[k].size();
                }
            }
        }

        
        for(int k = 0;k < numberClass;k++){

            float sum = 0.0;

            //std::cout << "----->" << oldCenters[0].size() << std::endl;

            for(int i =0; i< centers[k].size();i++)
            {
                sum += ((oldCenters[k][i] - centers[k][i])*(oldCenters[k][i] - centers[k][i]));
            }
                
            sum = sqrt(sum);
            //std::cout << "sum = " << sum << std::endl;
            if(sum <= limitError)
            {
                flag *= true;
            }
            else{
                flag *= false;
            }
            
        }
        itr++;
        //std::cout << "itr = " << itr << std::endl;
        oldCenters = centers;
    }
    for(int k = 0;k < numberClass;k++){
        //std::cout << "size = " << k << " -> "<< cluster[k].size() << std::endl;
        result.push_back(cluster[k].size());
    }
}

void kmeans::startingCenters(){
    for(int k = 0;k < numberClass;k++){
        int random = rand() % (features.size() - 1);
        centers.push_back(features[random]);
    }  
    /*
    for (int k = 0; k < numberClass; k++)
    {
        for (int icen = 0; icen < centers.size(); icen++)
        {
            std::cout << centers[k][icen] << " ";
        }
        std::cout << std::endl;
    }
    */

}   

int kmeans::getGoodCluster(std::vector<float> feature){
    int gc;
    float minError = std::numeric_limits<double>::infinity();

    for(int k = 0;k < numberClass;k++){
        float sum = 0.0;

        for(int i = 0; i < feature.size(); i++)
        {
            sum += (centers[k][i] - feature[i]) * (centers[k][i] - feature[i]);
        }
        sum = sqrt(sum);
        //std::cout << "sum (" << k << ") = " << sum <<std::endl;
        if(sum < minError){
            minError = sum;
            gc = k;
        }
    }
    //if (gc == 2)
    //    std::cout << "***" << std::endl;
    return gc;
}