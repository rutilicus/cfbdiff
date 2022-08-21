#ifndef CFG_EDGE_HPP
#define CFG_EDGE_HPP

#include <memory>
#include <string>

#include "cfg_basic_block.hpp"

class CfgBasicBlock;

class CfgEdge {
private:
    std::weak_ptr<CfgBasicBlock> _from;
    std::weak_ptr<CfgBasicBlock> _to;
    std::string _condition;
    CfgEdge() {}
public:
    CfgEdge(std::shared_ptr<CfgBasicBlock> from,
            std::shared_ptr<CfgBasicBlock> to): _from(from), _to(to) {}
    CfgEdge(std::shared_ptr<CfgBasicBlock> from,
            std::shared_ptr<CfgBasicBlock> to,
            std::string condition):
            _from(from), _to(to), _condition(condition) {}
    std::weak_ptr<CfgBasicBlock> getFrom() { return _from; }
    std::weak_ptr<CfgBasicBlock> getFTo() { return _to; }
    std::string getCondition() { return _condition; }
    std::string toString();
};

#endif
