#include <iostream>

#include "cfg_function.hpp"
#include "cfg_basic_block.hpp"

void CfgFunction::print() {
    std::cout << this->toString();
}

std::string CfgFunction::toString() {
    std::string ret;
    ret += _name + " (" + _arguments + ")\n{\n";
    for (auto b: _basicBlocks) {
        ret += b->toString();
    }
    ret += "}\n";

    return ret;
}
