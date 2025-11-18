#include "reservation_station.h"
#include <string>


ReservationStation::ReservationStation(int size, const std::string& name, int cicles) : poolName(name), cicles(cicles){
    entries.resize(size);
    clear();
}

void ReservationStation::clear() {

    int size = entries.size();
    entries.clear();
    entries.resize(size);

    for(int c = 0; c < size; c++){
        entries[c].name = poolName + std::to_string(c);
    }

    listeners.clear();
}

RSEntry* ReservationStation::findFreeSlot() {

    for (RSEntry& entry : entries) {
        if (!entry.busy) {
            return &entry;
        }
    }
    return nullptr;
}

void ReservationStation::issue(RSEntry* slot, OpCode opcode, int rob_index,
                               int qj_tag, float vj_val,
                               int qk_tag, float vk_val) {

    slot->busy = true;
    slot->op = opcode;
    slot->robIndex = rob_index;


    slot->Qj = qj_tag;
    if(qj_tag == NO_PRODUCER) slot->Vj = vj_val;

    slot->Qk = qk_tag;
    if (qk_tag == NO_PRODUCER) slot->Vk = vk_val;
    
    slot->cyclesLeft = cicles;
}

void ReservationStation::registerListener(RSEntry* entry, int tagToWaitFor) {

    if (tagToWaitFor != NO_PRODUCER) {
        listeners[tagToWaitFor].push_back(entry);
    }
}

void ReservationStation::snoopPool(int tag, float value) {

    auto it = listeners.find(tag);

    if (it == listeners.end()) {
        return;
    }

    std::list<RSEntry*>& waitingEntries = it->second;
    
    for (RSEntry* entry : waitingEntries) {
        
        if (entry->Qj == tag) {
            entry->Vj = value;
            entry->Qj = NO_PRODUCER;
        }else{
            entry->Vk = value;
            entry->Qk = NO_PRODUCER;
        }
        
    }

    listeners.erase(it);
}

std::vector<RSEntry>& ReservationStation::getEntries() {
    return entries;
}