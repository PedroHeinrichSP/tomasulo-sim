#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <vector>
#include <map>
#include <simulator_utils.h>

/*
 * Representa uma instrução MIPS simulada no algoritmo de Tomasulo.
 * Deve conter campos como:
 *  - opcode
 *  - registradores destino e origem
 *  - status atual (Issue, Execute, WriteBack, Commit)
 *  - contadores de ciclo (??maybe??)
 */

class Instruction {
    public:

        OpCode opcode = OpCode::INVALID;
        std::string rd, rs, rt, immediate;
        int issueCycle = -1;
        int execCycle = -1;
        int writeCycle = -1;
        int commitCycle = -1;
        bool valid = false;

        Instruction(const std::string& raw_line);
        bool isValid();

    private:
        static const std::map<std::string, OpCode> opcodeMap;
        std::vector<std::string> tokenize(const std::string& raw_line);
        void parse(const std::string& raw_line);  
};

#endif // INSTRUCTION_H