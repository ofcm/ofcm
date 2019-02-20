#include "utils.hpp"

class option{
public:
    int person;
    int action;
    int d;
    std::pair<int,int> sequence1;
    std::pair<int,int> sequence2;
    std::pair<int,int> sequence3;
    std::pair<int,int> sequence4;
    option::option(int,
                int,
                int,
                std::pair<int,int>,
                std::pair<int,int>,
                std::pair<int,int>,
                std::pair<int,int>);
};