#ifndef SIMULATOR_UTILS_H
#define SIMULATOR_UTILS_H

#include <string>
#include <sstream>
#include <iomanip>

/*
 * Funções utilitárias gerais:
 *  - formatação de strings
 *  - conversão de valores
 *  - debug / logs
 */

namespace SimUtils {
    std::string formatFloat(float value, int precision = 2);
    std::string toUpper(const std::string &s);
    // etc.
}

#endif // SIMULATOR_UTILS_H