#ifndef CFG_BASIC_BLOCK_HPP
#define CFG_BASIC_BLOCK_HPP

#include <string>
#include <list>
#include <memory>
#include <utility>
#include <functional>

#include "cfg_edge.hpp"

class CfgBasicBlock {
private:
    std::string _blockName;
    std::string _labelName;
    std::list<std::string> _code;
    std::list<std::shared_ptr<CfgEdge>> _inEdges;
    std::list<std::shared_ptr<CfgEdge>> _outEdges;

public:
    CfgBasicBlock() {}
    CfgBasicBlock(std::string blockName) : _blockName(blockName) {}
    void setLabel(const std::string label) { _labelName = label; }
    void appendLine(const std::string line) { _code.emplace_back(line); }
    void addInEdge(std::shared_ptr<CfgEdge> inEdge) {
        _inEdges.emplace_back(inEdge);
    }
    void addOutEdge(std::shared_ptr<CfgEdge> outEdge) {
        _outEdges.emplace_back(outEdge);
    }
    void print();
    std::string toString();
    std::list<std::shared_ptr<CfgEdge>> getInEdges() { return _inEdges; }
    std::list<std::shared_ptr<CfgEdge>> getOutEdges() { return _outEdges; }
    std::string getBlockName() { return _blockName; }
    std::string getLabelName() { return _labelName; }
    size_t getHash() { return std::hash<std::string>()(this->toString()); }
};

#endif
