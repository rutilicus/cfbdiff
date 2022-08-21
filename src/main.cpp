#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>

#include "cfg_reader.hpp"
#include "comparator.hpp"

int main(int argc, char *argv[]) {
    // argument check
    if (argc < 3) {
        std::cout <<
            "usage: acfg [compiler options] c_from_filename c_to_filename" <<
            std::endl;
        return 1;
    }

    // create cfg by calling g++.
    char tmpFromFilePath[] = "/tmp/cfbdiff_XXXXXX";
    char tmpToFilePath[] = "/tmp/cfbdiff_XXXXXX";
    int fdFrom = mkstemp(tmpFromFilePath);
    int fdTo = mkstemp(tmpToFilePath);
    if (fdFrom == -1 || fdTo == -1) {
        std::cerr << "make tmp file name error." << std::endl;
        return 1;
    }
    std::string commandBase = "g++ -fdump-tree-cfg=";
    std::string compileOptions;
    for (int i = 1; i < argc - 2; i++) {
        compileOptions += " " + std::string(argv[i]);
    }
    std::string command =
        commandBase + std::string(tmpFromFilePath) +
        compileOptions + " " + std::string(argv[argc - 2]);
    int commandResult = system(command.c_str());
    if (commandResult) {
        std::cout << "command: " << command << " error("
            << commandResult << ")" << std::endl;
        return 1;
    }
    command = commandBase + std::string(tmpToFilePath) +
        compileOptions + " " + std::string(argv[argc - 1]);
    commandResult = system(command.c_str());
    if (commandResult) {
        std::cout << "command: " << command << " error("
            << commandResult << ")" << std::endl;
        return 1;
    }

    // read cfg file and compare.
    std::ifstream stream = std::ifstream(std::string(tmpFromFilePath).c_str());
    CfgReader fromReader(stream);
    stream.close();
    stream = std::ifstream(std::string(tmpToFilePath).c_str());
    CfgReader toReader(stream);
    stream.close();

    compare(fromReader, toReader);

    // delete temporary file.
    remove(tmpFromFilePath);
    remove(tmpToFilePath);

    return 0;
}
