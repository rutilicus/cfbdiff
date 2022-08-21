#include <string>
#include <memory>

#include "cfg_basic_block.hpp"
#include "cfg_edge.hpp"

std::string CfgEdge::toString() {
    std::string ret;
    std::shared_ptr<CfgBasicBlock> fromPtr = _from.lock();
    std::shared_ptr<CfgBasicBlock> toPtr = _to.lock();
    if (fromPtr && toPtr) {
        ret = fromPtr->toString() + " -> " + toPtr->toString();
        if (!_condition.empty()) {
            ret += " (" + _condition + ")";
        }
    }

    return ret;
}
