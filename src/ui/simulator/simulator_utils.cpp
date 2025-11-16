#ifndef SIMULATOR_UTILS_H
#define SIMULATOR_UTILS_H

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>

#define NO_PRODUCER -1


// --- Enums Globais ---

/**
 * @enum OpCode
 * @brief Todos os Opcodes suportados pelo simulador.
 */
enum class OpCode {
    INVALID,
    NOP,
    // R-Type
    ADD, SUB, MUL, DIV,
    ADD_D, SUB_D, MUL_D, DIV_D,
    // I-Type
    ADDI, SUBI,
    // Load/Store
    LW, SW, L_D, S_D,
    // Branch/Jump
    BEQ, BNE, JAL
};

/**
 * @enum ROBState
 * @brief Os estados de ciclo de vida de uma entrada no Reorder Buffer.
 */
enum class ROBState {
    ISSUE,      // A instrução foi emitida e está esperando na RS
    EXECUTE,    // A instrução está sendo executada (enviada para a UF)
    WRITE_BACK, // A execução terminou, o resultado está no CDB e foi escrito no ROB
    COMMIT      // O resultado foi escrito no RegisterFile e a entrada está livre
};


// --- Funções Utilitárias ---
namespace SimUtils {
    std::string formatFloat(float value, int precision = 2);
    std::string toUpper(const std::string &s);
}

#endif // SIMULATOR_UTILS_H