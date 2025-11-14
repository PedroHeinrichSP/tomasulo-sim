#ifndef REORDER_BUFFER_H
#define REORDER_BUFFER_H

#include <string>
#include <vector>

/*
 * Implementa o Buffer de Reordenação (ROB):
 *  - controla commit em ordem
 *  - mantém estado e valor dos resultados
 *  - suporta especulação e flush em caso de desvio incorreto
 */

struct ROBEntry {
    bool busy = false;
    std::string instruction;
    std::string destination;
    float value = 0.0f;
    std::string state; // Issue, Execute, WriteBack, Commit
};

class ReorderBuffer {
    public:
        std::vector<ROBEntry> entries;

    private:
        // TODO:
        // - allocateEntry()
        // - commitEntry()
        // - flushOnBranchMiss()
};

#endif // REORDER_BUFFER_H