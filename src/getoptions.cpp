#include "headers/option.hpp"
#include <fstream>
#include <string>
#include <ctype.h>

void getOptions(std::vector<option>& trainingOption,
            std::vector<option>&     validationOption,
            std::vector<option>&     testOption,
            std::string optionFile){
    std::ifstream myfile(optionFile);
    std::string line;
    std::vector<int> trainingIndex, validationIndex, testIndex;
    int linePerson = 0;

    std::vector<std::string> texts;
    int countBar = 0;
    std::string text;
    std::string textNumber;
    std::vector<int> number;

    std::vector<option> aux;
    std::cout << optionFile << std::endl;

    int countaux = 0;
    if (myfile.is_open())
    {
        int countLine = 0;
        while (std::getline(myfile,line))
        {
            if(countLine == 0){
                for(int l = 0;l < line.size();l+=4){
                    char * value = new char[2];
                    value[0] = line[l];
                    value[1] = line[l+1];
                    int numberPerson = atoi(value);
                    trainingIndex.push_back(numberPerson);
                }
            }
            else if(countLine == 1){
                for(int l = 0;l < line.size();l+=4){
                    char * value = new char[2];
                    value[0] = line[l];
                    value[1] = line[l+1];
                    int numberPerson = atoi(value);
                    validationIndex.push_back(numberPerson);
                }
            }
            else if(countLine == 2){
                for(int l = 0;l < line.size();l+=4){
                    char * value = new char[2];
                    value[0] = line[l];
                    value[1] = line[l+1];
                    int numberPerson = atoi(value);
                    testIndex.push_back(numberPerson);
                }
            }
            else if(countLine > 3){

                if(line.size() > 0){
                    bool firstPart=true;
                    number.push_back(1);
                    int countBar = 0;
                    int countNumber = 0;
                    firstPart == true;
                    for(int l = 0;l < line.size();l++){
                        if(firstPart == true){
                            if(line[l] != '_'){
                                text += line[l];
                            }
                            else{
                                texts.push_back(text);
                                //std::cout <<  text << std::endl;
                                text = "";
                                countBar += 1;

                            }
                            if(countBar == 2 )
                            {
                                countBar = 0;
                                for(int k=1;k<3;k++){
                                    text += line[l+k];
                                }
                                firstPart = false;

                                //l = line.size();
                            }

                        }
                        else{
                            texts.push_back(text);
                            text = "";

                            if(line[l] != '-' && line[l] != ','){
                                textNumber += line[l];
                                //std::cout << "number " << textNumber << std::endl;
                            }
                            if (line[l] == '-'){

                                    number.push_back(atoi(textNumber.c_str()));
                                    //std::cout << "here 1 " << atoi(textNumber.c_str()) << std::endl;
                                    textNumber = "";


                            }

                            if (line[l] == ','){
                                //std::cout << "here 2 " << atoi(textNumber.c_str()) << std::endl;
                                //line += 2;
                                number.push_back(atoi(textNumber.c_str()));
                                textNumber = "";
                                countNumber++;
                                //std::cout << " count " << countNumber << std::endl;
                            }
                            if(countNumber == 3){
                                countNumber = 0;
                                for(int k=1;k<15;k++){
                                    if(line[l+k] != '-')
                                        textNumber += line[l+k];

                                    else if(line[l+k] != '\0'){
                                        //std::cout << "here 3 " << atoi(textNumber.c_str()) << std::endl;
                                        number.push_back(atoi(textNumber.c_str()));
                                        textNumber = "";
                                    }
                                    /*
                                    else if(line[l+k] == '\0')
                                        break;


                                    else {
                                        number.push_back(atoi(textNumber.c_str()));
                                        textNumber = "";
                                    }
                                    */
                                }
                                number.push_back(atoi(textNumber.c_str()));

                                textNumber = "";
                                l = line.size();

                            }

                        }

                    }
                    //std::cout << " person " << linePerson / 24 << std::endl;
                    if(number.size() > 7)
                        number.erase (number.begin()+1);


                    //std::cout << number.size() << std::endl;
                    option opt(texts[0],texts[1],texts[2],number);
                    std::string txtNumber = "";
                    for (auto t : texts[0]){
                        if(isdigit(t)){
                            textNumber += t;
                        }
                    }
                    int numberIndex = atoi(textNumber.c_str());
                    //std::cout << " numero index " << numberIndex << std::endl;
                    /*
                    if(numberIndex == 18){
                        for(auto n : number){
                            std::cout << " numberr " << n << std::endl;
                        }
                        std::cout << " -- " << std::endl;
                    }
                    */
                    for(auto indext : trainingIndex){
                        if(indext == numberIndex){
                            //std::cout << " person " << indext << "training d " << opt.d << " number " << opt.sequence[2] << " - " << opt.sequence[3] <<  std::endl;
                            trainingOption.push_back(opt);
                            break;
                        }
                    }

                    for(auto indexv : validationIndex){
                        if(indexv == numberIndex){
                            //std::cout << " person " << indexv << "val " << std::endl;
                            validationOption.push_back(opt);
                            break;
                        }
                    }
                    for(auto indextest : testIndex){
                        if(indextest == numberIndex){
                            //std::cout << " person " << indextest << "test " << std::endl;
                            testOption.push_back(opt);
                            break;
                        }
                    }

                    number.clear();
                    texts.clear();
                    countaux++;
                }

                linePerson++;
            }

            countLine ++;

        }
        //std::cout << " training " << trainingOption[trainingOption.size()-1].d << " final number " << trainingOption[trainingOption.size()-1].sequence[2] << std::endl;
        //std::cout << "number r " << countaux<<std::endl;
        myfile.close();
        //std::cout << "size training " << trainingOption.size() << std::endl;
    }
    else std::cout << "Unable to open file" << std::endl;
}
