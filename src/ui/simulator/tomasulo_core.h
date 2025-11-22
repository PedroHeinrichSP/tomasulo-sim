#ifndef TOMASULO_CORE_H
#define TOMASULO_CORE_H

#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include <map> 
#include <list>

#include "simulator_utils.h"
#include "instruction.h"
#include "reorder_buffer.h"
#include "register_file.h"
#include "common_data_bus.h"
#include "reservation_station.h" 

class TomasuloCore {
public:
    int cycle = 0;
    int instructionsCommitted = 0;
    bool executionFinished = false;

    std::vector<Instruction> instructionMemory; 
    int pc = 0; 
    
    std::map<std::string, int> labelMap;
    std::map<int, float> dataMemory;

    ReorderBuffer rob;
    RegisterFile registers;
    CommonDataBus cdb;
    std::map<std::string, ReservationStation*> stationMap;

    TomasuloCore();
    ~TomasuloCore();

    bool loadProgram(const std::string& filepath);
    void addInstruction(const std::string& raw_line);
    void step();
    void reset();
    bool isFinished();

private:
    void commit();
    void writeBack();
    void execute();
    void issue();
    
    void flushSpeculativeInstructions(int branchRobTag);

    ReservationStation* getPoolForOpcode(OpCode op);
    void innitRegisters();
};

#endif // TOMASULO_CORE_H