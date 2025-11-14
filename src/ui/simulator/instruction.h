#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <vector>

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
        std::string opcode;
        std::string rd, rs, rt;
        int issueCycle = -1;
        int execCycle = -1;
        int writeCycle = -1;
        int commitCycle = -1;

    private:
        // TODO: parsing, formatação e validação    
};

#endif // INSTRUCTION_H