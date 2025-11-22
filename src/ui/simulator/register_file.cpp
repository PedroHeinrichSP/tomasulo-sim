#include "register_file.h"
#include <stdexcept>

void RegisterFile::innit(const std::vector<std::string>& regNames) {

    values.clear();
    status.clear();

    for (const std::string& reg : regNames) {
        values[reg] = 0.0f;
        status[reg] = NO_PRODUCER;
    }
}


void RegisterFile::clear() {
    values.clear();
    status.clear();

}


int RegisterFile::getProducer(const std::string& regName) {

    auto it = status.find(regName);
    if (it != status.end()) {
        return it->second;
    }

    return NO_PRODUCER; 
}

float RegisterFile::getValue(const std::string& regName) {

    auto it = values.find(regName);
    if (it != values.end()) {
        return it->second;
    }

    return 0.0f;
}

void RegisterFile::setProducer(const std::string& regName, int robIndex) {
    status[regName] = robIndex;
}

void RegisterFile::writeValue(const std::string& regName, float value) {
    values[regName] = value;
}

void RegisterFile::onCommit(const std::string& regName, int robIndex) {

    /*
    only clen the rob table if the commit is the same rob as the register is mapped as
    exemple: add r1 r2 r3 as rob1 and sub r1 r4 r5 as rob2
    if we commit that rob1 is done then we should not change since the r1 is currently mapped for r2
    */
    
    auto it = status.find(regName);
    if (it != status.end() && it->second == robIndex) {
        it->second = NO_PRODUCER;
    }
}