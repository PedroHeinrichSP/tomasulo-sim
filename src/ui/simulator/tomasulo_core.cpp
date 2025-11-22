#include "tomasulo_core.h"
#include <fstream>
#include <iostream>

TomasuloCore::TomasuloCore() {
    
    // Add/Sub: 3 unidades, 2 ciclos
    stationMap["Add"] = new ReservationStation(3, "Add", 2);
    
    // Mul/Div: 2 unidades, 10 ciclos
    stationMap["Mul"] = new ReservationStation(2, "Mul", 10);
    
    // Load/Store: 3 unidades, 2 ciclos
    stationMap["Load"] = new ReservationStation(3, "Load", 2);

    innitRegisters();
}

TomasuloCore::~TomasuloCore() {

    for (auto& pair : stationMap) {
        delete pair.second;
    }

    stationMap.clear();
}

void TomasuloCore::reset() {

    cycle = 0;
    instructionsCommitted = 0;
    executionFinished = false;
    
    instructionMemory.clear();
    labelMap.clear();
    dataMemory.clear();
    pc = 0;

    rob.clear();
    innitRegisters();
    cdb.clear();
    
    for (auto& pair : stationMap) {
        pair.second->clear();
    }
}

std::string trim_str(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

bool TomasuloCore::loadProgram(const std::string& filepath) {

    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    int currentLineIndex = 0; // Conta qual será o índice no instructionMemory

    while (std::getline(file, line)) {

        // 1. Remove comentários
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        commentPos = line.find(';');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        line = trim_str(line);
        if (line.empty()) continue;

        // 2. Detecta Labels (ex: "LOOP: ADDI R1...")
        size_t labelPos = line.find(':');
        if (labelPos != std::string::npos) {
            // Extrai o nome do label (ex: "LOOP")
            std::string labelName = trim_str(line.substr(0, labelPos));
            
            // Mapeia o nome para o índice atual (onde a próxima instrução será inserida)
            labelMap[labelName] = currentLineIndex; 
            
            // Remove o label da linha, deixando só a instrução (ex: "ADDI R1...")
            line = trim_str(line.substr(labelPos + 1));
        }

        // 3. Se sobrou instrução na linha, adiciona
        if (!line.empty()) {
            addInstruction(line);
            currentLineIndex++; // Só incrementa se realmente adicionou uma instrução
        }
    }
    return true;
}

void TomasuloCore::addInstruction(const std::string& raw_line) {

    if (raw_line.empty()) return;

    Instruction inst(raw_line);
    if (inst.isValid()) {
        instructionMemory.push_back(inst);
    }
}

bool TomasuloCore::isFinished() {
    return (pc >= (int)instructionMemory.size()) && rob.isEmpty();
}


void TomasuloCore::step() {

    if (isFinished()) {
        executionFinished = true;
        return; 
    }

    cycle++;

    commit();
    writeBack();
    execute();
    issue();
    
    cdb.clear();
}

void TomasuloCore::commit() {
    ROBEntry* entry = rob.getNextToCommit();
    
    if (entry != nullptr) {
        int tag = rob.getHeadIndex(); 

        if (entry->op == OpCode::BEQ || entry->op == OpCode::BNE) {
            
            if (entry->value != 0.0f) {

                flushSpeculativeInstructions(tag);

                if (labelMap.count(entry->destReg)) { // Usamos destReg para guardar o label temporariamente
                    pc = labelMap[entry->destReg];
                }
            }
        }else if (entry->op == OpCode::SW || entry->op == OpCode::S_D) {
            int address = (int)entry->value;
            float data = entry->storeData;
            dataMemory[address] = data;

        } else if (!entry->destReg.empty()) {
            registers.writeValue(entry->destReg, entry->value);
            registers.onCommit(entry->destReg, tag);
        }

        rob.commitHead();
        instructionsCommitted++;
    }
}


void TomasuloCore::writeBack() {

    for (auto& pair : stationMap) {
        
        for (auto& entry : pair.second->getEntries()) {

            if (entry.busy && entry.cyclesLeft == 0 && entry.op != OpCode::INVALID) {
                
                float result = 0.0f;
                int address = 0;

                switch (entry.op) {
                    case OpCode::ADD: 
                    case OpCode::ADD_D: 
                    case OpCode::ADDI:
                        result = entry.Vj + entry.Vk; 
                        break;
                    case OpCode::SUB: 
                    case OpCode::SUB_D: 
                    case OpCode::SUBI:
                        result = entry.Vj - entry.Vk; 
                        break;
                    case OpCode::MUL: 
                    case OpCode::MUL_D:
                        result = entry.Vj * entry.Vk; 
                        break;
                    case OpCode::DIV: 
                    case OpCode::DIV_D:
                        if (entry.Vk != 0) result = entry.Vj / entry.Vk; 
                        break;
                    case OpCode::LW: 
                    case OpCode::L_D: 
                        address = (int)(entry.Vj + entry.Vk); 
                        if (dataMemory.count(address)) result = dataMemory[address];
                        else result = 0.0f;
                        break;
                    case OpCode::SW: 
                    case OpCode::S_D:
                        result = entry.Vj + entry.imm;
                        rob.setStoreData(entry.robIndex, entry.Vk);
                        break;
                    case OpCode::BEQ:
                        result = (entry.Vj == entry.Vk) ? 1.0f : 0.0f; break;
                    case OpCode::BNE:
                        result = (entry.Vj != entry.Vk) ? 1.0f : 0.0f; break;

                    default: 
                        result = entry.Vj; 
                }
                
                cdb.broadcast(entry.robIndex, result);
                entry.clear();
            }
        }
    }

    for (const auto& msg : cdb.messages) {

        rob.snoopCDB(msg.tag, msg.value);

        for (auto& pair : stationMap) {
            pair.second->snoopPool(msg.tag, msg.value);
        }
    }
}

void TomasuloCore::execute() {

    for (auto& pair : stationMap) {
        
        for (auto& entry : pair.second->getEntries()) {

            if (entry.busy && 
                entry.Qj == NO_PRODUCER && 
                entry.Qk == NO_PRODUCER && 
                entry.cyclesLeft > 0) 
                    entry.cyclesLeft--;
            
        }
    }
}

void TomasuloCore::issue() {
        
    if (pc >= (int)instructionMemory.size()) return;
    if (instructionMemory.empty()) return;
    if (rob.isFull()) return;

    Instruction inst = instructionMemory[pc]; 

    if (inst.opcode == OpCode::NOP) {
        pc++;
        int robTag = rob.allocate(inst.opcode, "");
        rob.snoopCDB(robTag, 0.0f);
        return;
    }

    if (inst.opcode == OpCode::JAL) {
        float retAddr = (float)(pc + 1);
        int robTag = rob.allocate(inst.opcode, inst.rd);
        rob.snoopCDB(robTag, retAddr);
        
        if (labelMap.count(inst.immediate)) pc = labelMap[inst.immediate];
        else pc++;
        return;
    }

    ReservationStation* pool = getPoolForOpcode(inst.opcode);
    
    if (!pool) {
        pc++;
        return;
    }

    RSEntry* rsSlot = pool->findFreeSlot();
    if (rsSlot == nullptr) {
        return; 
    }

    std::string destRegOrLabel = inst.rd;
    if (inst.opcode == OpCode::BEQ || inst.opcode == OpCode::BNE) {
        destRegOrLabel = inst.immediate; 
    }
    
    pc++;

    int robTag = rob.allocate(inst.opcode, inst.rd);
    int qj = NO_PRODUCER, qk = NO_PRODUCER;
    float vj = 0.0f, vk = 0.0f;
    float imm = 0.0f;

    auto resolveOperand = [&](const std::string& regName, int& tagOut, float& valOut) {
        int producer = registers.getProducer(regName);
        
        if (producer != NO_PRODUCER) {
            if (rob.isReady(producer)) {
                valOut = rob.getValue(producer);
                tagOut = NO_PRODUCER; 
            } else {
                tagOut = producer;
            }
        } else {
            valOut = registers.getValue(regName);
            tagOut = NO_PRODUCER;
        }
    };

    // Fonte 1
    if (!inst.rs.empty()) {
        resolveOperand(inst.rs, qj, vj);
    }
    
    // Fonte 2
    if (!inst.rt.empty() && inst.opcode != OpCode::LW && inst.opcode != OpCode::L_D) {
        resolveOperand(inst.rt, qk, vk);
    }

    // Imediatos
    if (!inst.immediate.empty()) {
        try {
            imm = std::stof(inst.immediate);
            
            if (inst.opcode == OpCode::ADDI || inst.opcode == OpCode::SUBI || 
                inst.opcode == OpCode::LW || inst.opcode == OpCode::L_D) {
                vk = imm;
                qk = NO_PRODUCER;
            }
        } catch (...) {}
    }

    pool->issue(rsSlot, inst.opcode, robTag, qj, vj, qk, vk, imm);

    if (qj != NO_PRODUCER) {
        pool->registerListener(rsSlot, qj);
    }

    if (qk != NO_PRODUCER) {
        pool->registerListener(rsSlot, qk);
    }

    // Renomeia
    bool writesToRegister = false;
    if (!inst.rd.empty()) {
        if (inst.opcode == OpCode::ADD   || inst.opcode == OpCode::SUB   || 
            inst.opcode == OpCode::MUL   || inst.opcode == OpCode::DIV   ||
            inst.opcode == OpCode::ADD_D || inst.opcode == OpCode::SUB_D || 
            inst.opcode == OpCode::MUL_D || inst.opcode == OpCode::DIV_D ||
            inst.opcode == OpCode::ADDI  || inst.opcode == OpCode::SUBI  ||
            inst.opcode == OpCode::LW    || inst.opcode == OpCode::L_D   ||
            inst.opcode == OpCode::JAL) {
            writesToRegister = true;
        }
    }

    if (writesToRegister) {
        registers.setProducer(inst.rd, robTag);
    }
}

inline
ReservationStation* TomasuloCore::getPoolForOpcode(OpCode op) {

    switch (op) {
        case OpCode::ADD: case OpCode::SUB: 
        case OpCode::ADDI: case OpCode::SUBI:
        case OpCode::BEQ: case OpCode::BNE:
            return stationMap["Add"];
            
        case OpCode::MUL: case OpCode::DIV:
        case OpCode::ADD_D: case OpCode::SUB_D: 
        case OpCode::MUL_D: case OpCode::DIV_D:
            return stationMap["Mul"];
            
        case OpCode::LW: case OpCode::SW:
        case OpCode::L_D: case OpCode::S_D:
            return stationMap["Load"];
            
        default: return nullptr;
    }
}

void TomasuloCore::innitRegisters() {

    std::vector<std::string> regNames;

    for (int i = 0; i < 32; i++) {
        
        regNames.push_back("R" + std::to_string(i));
        regNames.push_back("F" + std::to_string(i));
    }

    registers.innit(regNames);
}

void TomasuloCore::flushSpeculativeInstructions(int branchRobTag) {
    
    // 1. Remove do ROB (do fim até chegar no branch)
    while (true) {
        int tail = (rob.getTailIndex() - 1 + rob.getSize()) % rob.getSize();
        if (tail == branchRobTag) break; // Chegamos no branch, paramos
        if (rob.isEmpty()) break;

        ROBEntry& entry = rob.getEntry(tail);
        
        // Se instrução especulativa escreveu no RAT, limpa o mapeamento
        if (!entry.destReg.empty()) {
            int currentProducer = registers.getProducer(entry.destReg);
            if (currentProducer == tail) {
                registers.setProducer(entry.destReg, NO_PRODUCER);
            }
        }
        rob.removeTail();
    }

    // 2. Limpa Estações de Reserva que dependem de instruções futuras (agora inválidas)
    // Simplificação: Limpa qualquer instrução na RS que tenha robIndex "maior" que o branch
    // (Considerando a circularidade do buffer)
    for (auto& pair : stationMap) {
        for (auto& entry : pair.second->getEntries()) {
            if (!entry.busy) continue;
            
            // Cálculo de distância circular
            int dist = (entry.robIndex - branchRobTag + rob.getSize()) % rob.getSize();
            if (dist > 0) { // É mais jovem que o branch
                entry.clear();
            }
        }
    }
}
