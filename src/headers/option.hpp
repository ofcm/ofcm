#include "utils.hpp"

class option{
public:
    std::string person;
    std::string action;
    std::string d;
    std::vector<int> sequence;
    option();
    option(std::string person,
            std::string action,
            std::string d,
            std::vector<int> sequence);
};