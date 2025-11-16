#include "register_file.h"
#include <stdexcept>

void RegisterFile::clear(const std::vector<std::string>& regNames) {
    
    values.clear();
    status.clear();
    for (const std::string& reg : regNames) {
        values[reg] = 0.0f;
        status[reg] = NO_PRODUCER; // -1
    }
}

int RegisterFile::getProducer(const std::string& regName) {
    auto it = status.find(regName);
    if (it != status.end()) {
        return it->second;
    }
    // Lida com o caso de um registrador inválido ser pedido
    return NO_PRODUCER; 
}

float RegisterFile::getValue(const std::string& regName) {
    auto it = values.find(regName);
    if (it != values.end()) {
        return it->second;
    }
    // Lida com o caso de um registrador inválido ser pedido
    return 0.0f;
}

void RegisterFile::setProducer(const std::string& regName, int robIndex) {
    // Assume que regName é válido (foi inicializado em clear())
    status[regName] = robIndex;
}

void RegisterFile::writeValue(const std::string& regName, float value) {
    values[regName] = value;
}

void RegisterFile::onCommit(const std::string& regName, int robIndex) {
    // Esta verificação é CRUCIAL para perigos WAW (Write-After-Write)
    // Ex: ADD R1 (ROB3) e SUB R1 (ROB4)
    // Se ROB3 comitar, a RAT para R1 ainda deve ser ROB4, não -1.
    // Só limpamos se a tag que está comitando for a que a RAT espera.
    
    auto it = status.find(regName);
    if (it != status.end() && it->second == robIndex) {
        it->second = NO_PRODUCER;
    }
}