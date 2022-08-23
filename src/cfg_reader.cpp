#include <iostream>
#include <string>
#include <memory>
#include <regex>
#include <map>
#include <utility>
#include <list>
#include <set>
#include <queue>

#include "cfg_reader.hpp"
#include "cfg_basic_block.hpp"

typedef struct {
    std::string from;
    std::string to;
    std::string condition;
} cfgEdgeRaw;


CfgReader::CfgReader(std::ifstream& file) {
    if (file) {
        std::string line;
        std::shared_ptr<CfgFunction> function;
        std::shared_ptr<CfgBasicBlock> block;
        std::smatch m;
        std::map<std::string, std::string> tmpVar2realVar;
        std::string retVar;
        std::map<std::string, std::list<std::shared_ptr<cfgEdgeRaw>>> edges;
        bool isInFunction = false;
        bool skipLine = false;
        bool passToNextBlock = true;
        while (std::getline(file, line)) {
            if (skipLine) {
                skipLine = false;
                continue;
            }

            if (isInFunction) {
                // first, replace temp variable to real variable name
                for (auto p: tmpVar2realVar) {
                    line = std::regex_replace(line, std::regex(p.first), p.second);
                }

                // try parse
                if (std::regex_match(line, std::regex("}"))) {
                    isInFunction = false;
                    function->addBasicBlock(block);
                    block.reset();
                    _functions.emplace_back(function);
                    function.reset();
                } else if (std::regex_search(line, m, std::regex("^ +<(.+)>"))) {
                    function->addBasicBlock(block);
                    std::shared_ptr<CfgBasicBlock> nextBlock(
                        new CfgBasicBlock(m[1]));
                    if (passToNextBlock) {
                        std::shared_ptr<cfgEdgeRaw> edge(new cfgEdgeRaw);
                        edge->from = block->getBlockName();
                        edge->to = m[1];
                        edge->condition = "";
                        edges[function->getName()].emplace_back(edge);
                    }
                    block = nextBlock;
                    passToNextBlock = true;
                } else if (std::regex_search(line, m, 
                                             std::regex("^<(.+)>"))) {
                    block->setLabel(m[1]);
                } else if (std::regex_search(line, m, 
                                             std::regex("(_[0-9]+) = (.+);"))) {
                    tmpVar2realVar[m[1]] = m[2];
                } else if (std::regex_search(line, m, 
                           std::regex("if \\(([^!=<>]+) ([!=<>]+) ([^!=<>]+)\\)"))) {
                    std::string lhs = m[1];
                    std::string operand = m[2];
                    std::string rhs = m[3];

                    std::getline(file, line);
                    std::regex_search(line, m, std::regex("goto <(.+)>"));
                    std::shared_ptr<cfgEdgeRaw> edge(new cfgEdgeRaw);
                    edge->from = block->getBlockName();
                    edge->to = m[1];
                    edge->condition = lhs + " " + operand + " " + rhs;
                    edges[function->getName()].emplace_back(edge);

                    std::getline(file, line);
                    std::regex_search(line, m, std::regex("goto <(.+)>"));
                    edge.reset(new cfgEdgeRaw);
                    edge->from = block->getBlockName();
                    edge->to = m[1];
                    edge->condition = "else";
                    edges[function->getName()].emplace_back(edge);

                    passToNextBlock = false;
                } else if (std::regex_search(line, m, 
                           std::regex("switch \\(([^!=<>]+)\\) <(.+)>$"))) {
                    std::list<std::string> casesTokens;
                    std::string targetStr = m[1];
                    std::string casesStr = m[2];
                    int start = 0;
                    while (true) {
                        int pos = casesStr.find(", ", start);
                        if (pos == std::string::npos) {
                            casesTokens.push_back(casesStr.substr(start));
                            break;
                        }
                        casesTokens.push_back(casesStr.substr(start, pos - start));
                        start = pos + 2;
                    }

                    std::string defLabel;
                    for (auto s: casesTokens) {
                        std::smatch mCase;
                        if (std::regex_search(
                                s, mCase, std::regex("case (.+): <(.+)>"))) {
                            std::shared_ptr<cfgEdgeRaw> edge(new cfgEdgeRaw);
                            edge->from = block->getBlockName();
                            edge->to = mCase[2];
                            edge->condition =
                                targetStr + " == " + (std::string)mCase[1];
                            edges[function->getName()].emplace_back(edge);
                        } else if (std::regex_search(
                            s, mCase, std::regex("default: <(.+)>"))) {
                            defLabel = mCase[1];
                        }
                    }
                    if (!defLabel.empty()) {
                        std::shared_ptr<cfgEdgeRaw> edge(new cfgEdgeRaw);
                        edge->from = block->getBlockName();
                        edge->to = defLabel;
                        edge->condition = "else";
                        edges[function->getName()].emplace_back(edge);
                    }

                    passToNextBlock = false;
                } else if (std::regex_search(line, m, std::regex("goto <(.+)>"))) {
                    std::shared_ptr<cfgEdgeRaw> edge(new cfgEdgeRaw);
                    edge->from = block->getBlockName();
                    edge->to = m[1];
                    edge->condition = "";
                    edges[function->getName()].emplace_back(edge);
                    passToNextBlock = false;
                } else if (retVar.empty()
                           && std::regex_search(line, m, std::regex("[a-zA-Z].+ (D.[0-9]+)"))) {
                    retVar = m[1];
                } else if (!retVar.empty()
                           && std::regex_search(line, m, std::regex("return " + retVar))) {
                    // nop
                } else if (!retVar.empty()
                           && std::regex_search(line, m, std::regex(retVar + " = (.*);"))) {
                    block->appendLine("  return " + (std::string)m[1] + ";");
                } else {
                    block->appendLine(line);
                }
            } else {
                if (std::regex_match(line, m, 
                                     std::regex("([a-zA-Z].+) \\((.*)\\)"))) {
                    isInFunction = true;
                    skipLine = true; // next line is '{'
                    passToNextBlock = true;
                    function.reset(new CfgFunction);
                    function->setName(m[1]);
                    function->setArguments(m[2]);
                    block.reset(new CfgBasicBlock);
                    tmpVar2realVar.clear();
                    edges[m[1]] = std::list<std::shared_ptr<cfgEdgeRaw>>();
                    retVar.clear();
                }
            }
        }

        for (auto f: _functions) {
            std::map<std::string, std::shared_ptr<CfgBasicBlock>> label2block;
            for (auto b: f->getBasicBlocks()) {
                std::string blockName = b->getBlockName();
                std::string labelName = b->getLabelName();
                label2block[blockName] = b;
                if (!labelName.empty()) {
                    label2block[labelName] = b;
                }
            }

            for (auto e: edges[f->getName()]) {
                std::shared_ptr<CfgEdge> newEdge(
                    new CfgEdge(label2block[e->from],
                                label2block[e->to],
                                e->condition));
                label2block[e->from]->addOutEdge(newEdge);
                label2block[e->to]->addInEdge(newEdge);
            }
        }
    }
}

void CfgReader::printFunctions() {
    for (auto f: _functions) {
        std::cout << f->toString() << std::endl;
    }
}
