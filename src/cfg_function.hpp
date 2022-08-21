#ifndef CFG_FUNCTION_HPP
#define CFG_FUNCTION_HPP

#include <string>
#include <list>
#include <memory>

#include "cfg_basic_block.hpp"

class CfgFunction {
private:
    std::string _name;
    std::string _arguments;
    std::list<std::shared_ptr<CfgBasicBlock>> _basicBlocks;

public:
    CfgFunction() {}
    void addBasicBlock(std::shared_ptr<CfgBasicBlock> block) { 
        _basicBlocks.emplace_back(block);
    }
    void print();
    std::string toString();
    void setName(std::string name) { _name = name; }
    void setArguments(std::string arguments) { _arguments = arguments; }
    std::list<std::shared_ptr<CfgBasicBlock>> getBasicBlocks() { 
        return _basicBlocks;
    }
    std::shared_ptr<CfgBasicBlock> getEntryBlock() {
        return _basicBlocks.front();
    }
    std::string getName() { return _name; }
};

#endif
