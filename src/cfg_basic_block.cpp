#include <iostream>
#include <string>

#include "cfg_basic_block.hpp"

void CfgBasicBlock::print() {
    std::cout << this->toString();
}

std::string CfgBasicBlock::toString() {
    std::string ret;

    for (std::string s: _code) {
        ret += s + "\n";
    }

    return ret;
}
