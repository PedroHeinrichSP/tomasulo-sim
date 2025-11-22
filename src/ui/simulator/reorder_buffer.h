#ifndef REORDER_BUFFER_H
#define REORDER_BUFFER_H

#include <string>
#include <vector>
#include <simulator_utils.h>

/*
 * Implementa o Buffer de Reordenação (ROB):
 *  - controla commit em ordem
 *  - mantém estado e valor dos resultados
 *  - suporta especulação e flush em caso de desvio incorreto
 */

struct ROBEntry {
    bool busy = false;
    OpCode op = OpCode::INVALID;
    std::string destReg;
    float value = 0.0f;
    float storeData = 0.0f;
    ROBState state = ROBState::ISSUE;
};

class ReorderBuffer {
    private:
        std::vector<ROBEntry> entries;
        int head, tail, count;
    public:

    ReorderBuffer(int size = 16);

    void clear();
    int allocate(OpCode op, const std::string& destReg);
    void snoopCDB(int tag, float value);
    void setStoreData(int tag, float data); // NOVO: Para SW salvar o dado
    ROBEntry* getNextToCommit();
    void commitHead();
    bool isReady(int tag);
    float getValue(int tag);
    int getHeadIndex();
    int getTailIndex(); // Útil para debug ou flush
    bool isFull();
    bool isEmpty();
    int getSize() const { return entries.size(); }
    void removeTail(); // Remove a instrução mais recente (undo allocate)
    ROBEntry& getEntry(int tag); // Acesso direto
};

#endif // REORDER_BUFFER_H