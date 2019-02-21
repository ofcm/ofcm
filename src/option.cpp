#include "headers/option.hpp"

option::option(std::string person1,
            std::string action1,
            std::string d1,
            std::vector<int> sequence1){
    this->person = person1;
    this->action = action1;
    this->d = d1;
    this->sequence = sequence1;
}
