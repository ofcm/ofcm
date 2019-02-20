#include "option.hpp"

option::option(int person,
            int action,
            int d,
            std::pair<int,int> sequence1,
            std::pair<int,int> sequence2,
            std::pair<int,int> sequence3,
            std::pair<int,int> sequence4){
    this->person = person;
    this->action = action;
    this->d = d;
    this->sequence1 = sequence1;
    this->sequence2 = sequence1;
    this->sequence3 = sequence1;
    this->sequence4 = sequence1;
}
