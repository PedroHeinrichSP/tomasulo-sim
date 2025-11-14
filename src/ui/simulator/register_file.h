#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include <string>
#include <unordered_map>

/*
 * Representa o conjunto de registradores:
 *  - valor atual de cada registrador
 *  - status indicando se está sendo produzido por alguma RS
 */

class RegisterFile {
    public:
        std::unordered_map<std::string, float> values;
        std::unordered_map<std::string, std::string> status; // reg -> produtor

    private:
        // TODO:
        // - read()
        // - write()
        // - clear()
};

#endif // REGISTER_FILE_H