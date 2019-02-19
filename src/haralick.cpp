#include "headers/haralick.hpp"

float EPSILON = 0.0001;

float angular2ndMoment(cv::Mat Mco)
{
    float sum = 0.0;
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            //std::cout << "pixel =  " << Mco.at<float>(i,j) << std::endl;
            sum += (Mco.at<float>(i,j)*Mco.at<float>(i,j));
        }
    }
    return sum;
}

float contrast(cv::Mat Mco)
{
    int Ng = Mco.cols;
    
    float sum = 0.0;

    for (int n = 0; n < Ng; n++)
    {
        for (int i = 0; i < Ng; i++)
        {
            for (int j = 0; j < Ng; j++)
            {
                if (abs(i-j) == n)
                    sum += n * n * Mco.at<float>(i,j);
            }
        }
    }

    return sum;
}

float correlation(cv::Mat Mco)
{
    std::vector<float> px;

    for (int i = 0; i < Mco.rows; i++)
    {
        float sumPxi = 0.0;
        for (int j = 0; j < Mco.cols; j++)
        {
            sumPxi += Mco.at<float>(i,j);
        }
        px.push_back(sumPxi);
    } 

    std::vector<float> py;

    for (int j = 0; j < Mco.cols; j++)
    {
        float sumPyj = 0.0;
        for (int i = 0; i < Mco.rows; i++)
        {
            sumPyj += Mco.at<float>(i,j);
        }
        py.push_back(sumPyj);
    } 

    float muX = 0.0;

    for (int ix = 0; ix < px.size(); ix++)
        muX += px[ix];

    muX /= ((float)px.size());

    float muY = 0.0;

    for (int iy = 0; iy < py.size(); iy++)
        muY += py[iy];

    muY /= ((float)py.size());

    float sigmaX = 0.0;

    for (int ix = 0; ix < px.size(); ix++)
        sigmaX += ((px[ix]-muX) * (px[ix]-muX));

    sigmaX /= ((float)px.size());
    sigmaX  = sqrt(sigmaX);

    float sigmaY = 0.0;

    for (int iy = 0; iy < py.size(); iy++)
        sigmaY += ((py[iy]-muY) * (py[iy]-muY));

    sigmaY /= ((float)py.size());
    sigmaY  = sqrt(sigmaY);


    float sumCR = 0.0;
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            sumCR += (((float)i*(float)j)* (Mco.at<float>(i,j)) - muX * muY);
        }
    }
    sumCR /= (sigmaX * sigmaY);

    return sumCR;
}

float sumOfSquares(cv::Mat Mco)
{
    float sumVar = 0.0;
    float mean   = 0.0;

    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            mean += Mco.at<float>(i,j);
        }
    }   

    mean /= ((float)Mco.rows * (float)Mco.cols);

    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            sumVar += ((float)i - mean ) * ((float)i - mean) * Mco.at<float>(i,j);
        }
    }   

    return sumVar;
}

float InverseDifferenceMoment(cv::Mat Mco)
{
    float sumIDM = 0.0;
    
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            sumIDM += (1.0 / (1 + (float)((i - j)*(i - j)))) * Mco.at<float>(i,j);
        }
    }   

    return sumIDM;   
}

float sumAverage(cv::Mat Mco)
{
    std::vector<float> pX_plus_Y(Mco.rows + Mco.cols - 1, 0);
    
    int k;
    //std::cout << "size = " << pX_plus_Y.size() << std::endl;
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            k = i + j;
            pX_plus_Y[k] += Mco.at<float>(i,j);
        }
    }   
    
    float sumAvr = 0.0;

    for (int i = 0; i < pX_plus_Y.size(); i++)
    {
        sumAvr += ((float)i * pX_plus_Y[i]);
    }

    return sumAvr;
}

float sumEntropy(cv::Mat Mco)
{
    std::vector<float> pX_plus_Y(Mco.rows + Mco.cols - 1, 0);
    
    int k;
    //std::cout << "size = " << pX_plus_Y.size() << std::endl;
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            k = i + j;
            pX_plus_Y[k] += Mco.at<float>(i,j);
        }
    }   

    float sumEtp = 0.0;

    for (int i = 0; i < pX_plus_Y.size(); i++)
    {
        sumEtp += (pX_plus_Y[i]) * log10(pX_plus_Y[i] + EPSILON);
    }

    return -sumEtp;

}

float Entropy(cv::Mat Mco)
{
    float etp = 0.0;
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            etp += Mco.at<float>(i,j) * log10(Mco.at<float>(i,j) + EPSILON);
        }
    } 

    return -etp;
}

float differenceVariance(cv::Mat Mco)
{
    std::vector<float> pX_minus_Y(Mco.rows - 1, 0);
    
    int k;
    //std::cout << "size = " << pX_minus_Y.size() << std::endl;
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            k = abs(i - j);
            pX_minus_Y[k] += Mco.at<float>(i,j);
        }
    }  

    float meanP = 0.0;

    for (int ip = 0; ip < pX_minus_Y.size(); ip++)
    {
        meanP += pX_minus_Y[ip]; 
    }
    meanP /= ((float)pX_minus_Y.size());

    float var = 0.0;

    for (int ip = 0; ip < pX_minus_Y.size(); ip++)
    {
        var += (pX_minus_Y[ip] - meanP) * (pX_minus_Y[ip] - meanP); 
    }

    var /= ((float)pX_minus_Y.size());

    return var;
}

float differenceEntropy(cv::Mat Mco)
{
    std::vector<float> pX_minus_Y(Mco.rows - 1, 0);
    
    int k;
    //std::cout << "size = " << pX_minus_Y.size() << std::endl;
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            k = abs(i - j);
            pX_minus_Y[k] += Mco.at<float>(i,j);
        }
    }  
    
    float diffEtp = 0.0;
    
    for (int ip = 0; ip < pX_minus_Y.size(); ip++)
    {
        diffEtp += pX_minus_Y[ip] * log10(pX_minus_Y[ip] + EPSILON);
    }
    
    return diffEtp;
}

float informationCorrelation(cv::Mat Mco)
{
    std::vector<float> px(Mco.rows,0.0);
    std::vector<float> py(Mco.cols,0.0);
    
    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            px[i] = Mco.at<float>(i,j);
            py[j] = Mco.at<float>(i,j);
        }
    } 

    float HX = 0.0;
    float HY = 0.0;
    
    float HXY  = 0.0;
    float HXY1 = 0.0;

    for (int i = 0; i < Mco.rows; i++)
    {
        for (int j = 0; j < Mco.cols; j++)
        {
            HXY  -= Mco.at<float>(i, j) * log10(Mco.at<float>(i, j) + EPSILON);
            HXY1 -= Mco.at<float>(i, j) * log10(px[i] * py[j] + EPSILON); 
        }
    }

    for (auto i = 0; i < px.size(); ++i) {
        HX -= px[i] * log10(px[i] + EPSILON);
        HY -= py[i] * log10(py[i] + EPSILON);

    float res = (HXY - HXY1) / std::max(HX, HY);

    return res;
}

}
std::vector<float> haralick(cv::Mat Mco_F, int size)
{
    //cv::Mat Mco_F;
    //cv::resize(Mco,Mco, cv::Size(18,18));
    //assert (Mco.cols == Mco.rows);

    //Mco.convertTo(Mco_F, CV_32FC1);

    std::vector<float> haralickVector(size);
    
    for (int i = 0; i < Mco_F.rows; i++)
    {
        for (int j = 0; j < Mco_F.cols; j++)
        {
            Mco_F.at<float>(i,j) /= (2.0 * (float)Mco_F.rows * (float)Mco_F.cols);
        }
    }
    
    haralickVector[0]  = angular2ndMoment(Mco_F);
    haralickVector[1]  = contrast(Mco_F);
    haralickVector[2]  = correlation(Mco_F);
    haralickVector[3]  = sumOfSquares(Mco_F);
    haralickVector[4]  = InverseDifferenceMoment(Mco_F);
    haralickVector[5]  = sumAverage(Mco_F);
    haralickVector[6]  = sumOfSquares(Mco_F);
    haralickVector[7]  = sumEntropy(Mco_F);
    haralickVector[8]  = Entropy(Mco_F);
    haralickVector[9]  = differenceVariance(Mco_F);
    haralickVector[10] = differenceEntropy(Mco_F);
    haralickVector[11] = informationCorrelation(Mco_F);

    return haralickVector;
}