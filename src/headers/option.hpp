#include "utils.hpp"

class option{
public:
    std::string person;
    std::string action;
    std::string d;
    std::pair<int,int> sequence1;
    std::pair<int,int> sequence2;
    std::pair<int,int> sequence3;
    std::pair<int,int> sequence4;
    option( std::string,
            std::string,
            std::string,
            std::pair<int,int>,
            std::pair<int,int>,
            std::pair<int,int>,
            std::pair<int,int>);
};