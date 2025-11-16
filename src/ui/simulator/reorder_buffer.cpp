#include "reorder_buffer.h"

ReorderBuffer::ReorderBuffer(int size) {
    entries.resize(size);
    clear();
}

void ReorderBuffer::clear() {

    for (size_t i = 0; i < entries.size(); ++i) {
        entries[i].busy = false;
        entries[i].op = OpCode::INVALID;
        entries[i].destReg = "";
        entries[i].value = 0.0f;
        entries[i].state = ROBState::ISSUE;
    }

    head = 0;
    tail = 0;
    count = 0;
}

bool ReorderBuffer::isFull() {
    return count == entries.size();
}

int ReorderBuffer::allocate(OpCode op, const std::string& destReg) {

    if (isFull()) {
        return NO_PRODUCER;
    }

    int tag = tail;

    entries[tag].busy = true;
    entries[tag].op = op;
    entries[tag].destReg = destReg;
    entries[tag].value = 0.0f;
    entries[tag].state = ROBState::ISSUE;

    tail = (tail + 1) % entries.size();
    count++;

    return tag;
}

void ReorderBuffer::snoopCDB(int tag, float value) {

    if (entries[tag].busy) {

        entries[tag].value = value;
        entries[tag].state = ROBState::WRITE_BACK;
    }
}

ROBEntry* ReorderBuffer::getNextToCommit() {

    if (count == 0) {
        return nullptr;
    }
    
    ROBEntry* entry = &entries[head];

    if (entry->state == ROBState::WRITE_BACK) {
        return entry;
    }

    return nullptr;
}

void ReorderBuffer::commitHead() {

    if (count == 0) {
        return; 
    }

    entries[head].busy = false;

    head = (head + 1) % entries.size();
    count--;
}