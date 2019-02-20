#include "headers/option.hpp"
#include <fstream>
#include <string>

void getOptions(std::vector<option> trainingOption,
            std::vector<option> validationOption,
            std::vector<option> testOption,
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
    if (myfile.is_open())
    {
        int countLine = 0;
        while (std::getline(myfile,line))
        {
            if(countLine == 0){
                for(int l = 0;l < line.size();l+=4){
                    //char 
                    //std::string value = ("" + line[l]) + ("" + line[l+1]);
                    //std::cout << line[l] << " - " << line[l+1] << std::endl;
                    char * value = new char[2];
                    value[0] = line[l];
                    value[1] = line[l+1];
                    int numberPerson = atoi(value);
                    trainingIndex.push_back(numberPerson);
                }
            }
            else if(countLine == 1){
                for(int l = 0;l < line.size();l+=4){
                    //char 
                    //std::string value = ("" + line[l]) + ("" + line[l+1]);
                    //std::cout << line[l] << " - " << line[l+1] << std::endl;
                    char * value = new char[2];
                    value[0] = line[l];
                    value[1] = line[l+1];
                    int numberPerson = atoi(value);
                    validationIndex.push_back(numberPerson);
                }
            }
            else if(countLine == 2){
                for(int l = 0;l < line.size();l+=4){
                    //char 
                    //std::string value = ("" + line[l]) + ("" + line[l+1]);
                    //std::cout << line[l] << " - " << line[l+1] << std::endl;
                    char * value = new char[2];
                    value[0] = line[l];
                    value[1] = line[l+1];
                    int numberPerson = atoi(value);
                    testIndex.push_back(numberPerson);
                }
            }
            else if(countLine > 3){
                
                if(linePerson == 0 || linePerson % 25 != 0){
                    
                    for(int l = 0;l < line.size();l+=1){
                        
                        if(line[l] != '_'){
                            if(l <= 32)
                                text += line[l];
                            else{
                                if(line[l] != '-'){
                                    textNumber += line[l];
                                }
                                else if (line[l] == '-'){
                                    textNumber = "";
                                    number.push_back(atoi(textNumber.c_str()));
                                    std::cout << "number " << atoi(textNumber.c_str()) << std::endl;
                                }
                                else if (line[l] == ','){
                                    line += 2;
                                }
                            }
                            //std::cout << " here " << line[l] << std::endl;
                        }
                        else{
                            texts.push_back(text);
                            std::cout <<  text << countBar << std::endl;
                            text = "";
                            countBar += 1;
                        }

                        if(countBar == 3 )
                        {
                            countBar = 0;
                            //std::cout << "here " << std::endl;
                            l = line.size();
                        }
                            
                    }
                }   
                linePerson++;
            }
            countLine ++;
            //std::cout << "\n";
        }
        myfile.close();
    }
    else std::cout << "Unable to open file" << std::endl; 
}