#include <iostream>
#include <map>
#include <string>
#include <set>

#include "cfg_reader.hpp"
#include "cfg_function.hpp"

void compare(CfgReader &from, CfgReader &to) {
    std::map<std::string, std::shared_ptr<CfgFunction>> fromFunctions;
    std::map<std::string, std::shared_ptr<CfgFunction>> toFunctions;

    for (auto fFrom: from.getFunctions()) {
        fromFunctions[fFrom->getName()] = fFrom;
    }
    for (auto fTo: to.getFunctions()) {
        toFunctions[fTo->getName()] = fTo;
    }

    for (auto fFrom: fromFunctions) {
        if (toFunctions.count(fFrom.first)) {
            auto fTo = toFunctions[fFrom.first];
            std::map<size_t, std::shared_ptr<CfgBasicBlock>> fromBlocks;
            std::map<size_t, std::shared_ptr<CfgBasicBlock>> toBlocks;

            for (auto fBlock: fFrom.second->getBasicBlocks()) {
                fromBlocks[fBlock->getHash()] = fBlock;
            }
            for (auto tBlock: fTo->getBasicBlocks()) {
                toBlocks[tBlock->getHash()] = tBlock;
            }

            for (auto fBlock: fromBlocks) {
                if (!toBlocks.count(fBlock.first)) {
                    std::cout << "Function: \"" << fFrom.first
                        << "\" block deleted:" << std::endl
                        << fBlock.second->toString() << std::endl;
                }
            }
            for (auto tBlock: toBlocks) {
                if (!fromBlocks.count(tBlock.first)) {
                    std::cout << "Function: \"" << fFrom.first
                        << "\" block added:" << std::endl
                        << tBlock.second->toString() << std::endl;
                }
            }
        } else {
            std::cout 
                << "Function: \"" << fFrom.first << "\" deleted." << std::endl;
        }
    }

    for (auto fTo: toFunctions) {
        if (!fromFunctions.count(fTo.first)) {
            std::cout 
                << "Function: \"" << fTo.first << "\" added." << std::endl;
        }
    }
}
