#include "headers/kmeans.hpp"


kmeans::kmeans(std::vector<std::vector<float>> centers,
                            int numberClass){
    this->numberClass = numberClass;
    this->centers     = centers;   
}

void kmeans::setFeatures(std::vector<std::vector<float>> features){
    this->features = features;
}

std::vector<std::vector<float>> kmeans::getCentroids(){
    return centers;
}

void kmeans::getHistogram(std::vector<int>& result)
{
    std::vector<std::vector<std::vector<float>>> cluster(numberClass);

    for(auto feature : features)
    {
        int goodClass = getGoodCluster(feature);
        cluster[goodClass].push_back(feature);
    }

    for(int k = 0;k < numberClass;k++){
        result.push_back(cluster[k].size());
    }
}

void kmeans::runKmeans(){
    std::vector<std::vector<std::vector<float>>> cluster(numberClass);
    oldCenters = centers;

    int itr = 0;

    flag = false;

    while (true){
        flag = true;
        /*
        std::cout << "\nCentroids conv (it = "<< itr << ") :\n" << std::endl;
        for (int k = 0; k < numberClass; k++)
        {
            for (int icen = 0; icen < centers[k].size(); icen++)
            {
                std::cout << centers[k][icen] << " ";
                if ((icen+1) % 24 == 0)
                    std::cout << std::endl;
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

        for (int k = 0; k < numberClass; k++)
        {
            if (cluster[k].size() != 0)
            {
                for (int icen = 0; icen < centers[k].size(); icen++)
                {
                    centers[k][icen] = 0.0;
                }
            }
            /*
            else{
                std::cout << "*" << k <<" " << numberClass <<std::endl;
            }
            */
        }
        
        for(int k = 0;k < numberClass;k++){
            for(int i = 0;i < cluster[k].size();i++){
                for(int val = 0;val < cluster[k][i].size();val++){
                    centers[k][val] += cluster[k][i][val]/cluster[k].size();
                }
            }
        }
        
        float loss = 0.0;
        for(int k = 0;k < numberClass;k++){
            float sum = 0.0;
            for(int i =0; i< centers[k].size();i++)
            {
                sum += ((oldCenters[k][i] - centers[k][i])*(oldCenters[k][i] - centers[k][i]));
            }
            //std::cout << "\tsum = " << sum << std::endl;
            sum = sqrt(sum);
            //std::cout << "\tsum sqrt = " << sum << std::endl;
            loss += sum;
            if(sum <= limitError)
                flag *= true;
            else
                flag *= false;            
        }

        std::cout << "\t\tKmeans loss itr "<< itr + 1 << " : \t"<< loss/((float)numberClass) << std::endl;
        
        if (flag == true)
            break;

        itr++;

        if (itr > 99)
            break;

        oldCenters = centers;
    }
}

void kmeans::startingCenters(){
    for(int k = 0;k < numberClass;k++){
        int random = rand() % (features.size() - 1);
        centers.push_back(features[random]);
    }  
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
        if(sum < minError){
            minError = sum;
            gc = k;
        }
    }

    return gc;
}