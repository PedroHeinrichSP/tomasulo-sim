#ifndef RESERVATION_STATION_H
#define RESERVATION_STATION_H

#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include "simulator_utils.h" // Para OpCode e NO_PRODUCER

struct RSEntry {
    std::string name; // Nome (ex: "Add1") para UI/debug
    bool busy = false;  // Esta estação está em uso?

    OpCode op = OpCode::INVALID; // Operação (ex: ADD)
    int robIndex = -1; // Para qual entrada do ROB esta RS está trabalhando

    // Operandos (Vj, Vk) ou Produtores (Qj, Qk)
    float Vj = 0, Vk = 0, imm = 0;   // Valores (se prontos)
    int Qj = NO_PRODUCER;   // Tag do produtor (se esperando por Vj)
    int Qk = NO_PRODUCER;   // Tag do produtor (se esperando por Vk)

    // Estado da execução (para o TomasuloCore)
    int cyclesLeft = 0; // Quantos ciclos faltam para executar

    void clear();
};



class ReservationStation {
private:

    std::string poolName; // ex: "AddSubPool"
    std::vector<RSEntry> entries;
    std::unordered_map<int, std::list<RSEntry*>> listeners;
    int cicles;
public:

    ReservationStation(int size, const std::string& name, int cicles);
    void clear();
    RSEntry* findFreeSlot();
    void issue(RSEntry* slot, OpCode opcode, int rob_index,
               int qj_tag, float vj_val,
               int qk_tag, float vk_val,
               float immediate = 0.0f);
    void registerListener(RSEntry* entry, int tagToWaitFor);
    void snoopPool(int tag, float value);
    std::vector<RSEntry>& getEntries();
};

#endif // RESERVATION_STATION_H