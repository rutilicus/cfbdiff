#ifndef CFG_READER_HPP
#define CFG_READER_HPP

#include <fstream>
#include <list>
#include <memory>

#include "cfg_function.hpp"

class CfgReader {
private:
    CfgReader() {}
    std::list<std::shared_ptr<CfgFunction>> _functions;

public:
    CfgReader(std::ifstream& file);
    void printFunctions();
    std::list<std::shared_ptr<CfgFunction>> getFunctions() {
        return _functions;
    }
};

#endif
