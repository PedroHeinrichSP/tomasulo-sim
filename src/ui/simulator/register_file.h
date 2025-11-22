#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <simulator_utils.h>

/*
 * Representa o conjunto de registradores:
 *  - valor atual de cada registrador
 *  - status indicando se está sendo produzido por alguma RS
 */

class RegisterFile {
    private:
        std::unordered_map<std::string, float> values;
        std::unordered_map<std::string, int> status; // reg -> produtor

    public:

        void clear();
        void innit(const std::vector<std::string> &regNames);
        int getProducer(const std::string &regName);
        float getValue(const std::string& regName);
        void setProducer(const std::string& regName, int robIndex);
        void writeValue(const std::string& regName, float value);
        void onCommit(const std::string& regName, int robIndex);
};

#endif // REGISTER_FILE_H