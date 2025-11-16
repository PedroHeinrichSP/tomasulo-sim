#include "instruction.h"
#include <string>
#include <sstream>
#include <vector>
#include <algorithm> 
#include <regex>

const std::map<std::string, OpCode> Instruction::opcodeMap = {
    {"NOP", OpCode::NOP},
    {"ADD", OpCode::ADD},
    {"SUB", OpCode::SUB},
    {"MUL", OpCode::MUL},
    {"DIV", OpCode::DIV},
    {"ADD.D", OpCode::ADD_D},
    {"SUB.D", OpCode::SUB_D},
    {"MUL.D", OpCode::MUL_D},
    {"DIV.D", OpCode::DIV_D},
    {"ADDI", OpCode::ADDI},
    {"SUBI", OpCode::SUBI},
    {"LW", OpCode::LW},
    {"SW", OpCode::SW},
    {"L.D", OpCode::L_D},
    {"S.D", OpCode::S_D},
    {"BEQ", OpCode::BEQ},
    {"BNE", OpCode::BNE},
    {"JAL", OpCode::JAL}
};


std::vector<std::string> Instruction::tokenize(const std::string& raw_line) {

    std::string line = raw_line;
    std::replace(line.begin(), line.end(), ',', ' ');

    std::transform(line.begin(), line.end(), line.begin(), ::toupper);

    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

Instruction::Instruction(const std::string& raw_line) {
    parse(raw_line);
}

bool Instruction::isValid() {
    return this->valid;
}

void Instruction::parse(const std::string& raw_line) {

    opcode = OpCode::INVALID;
    rd = rs = rt = immediate = "";
    this->valid = false;

    std::vector<std::string> tokens = tokenize(raw_line);

    if (tokens.empty()) {
        return;
    }

    auto it = opcodeMap.find(tokens[0]);
    if (it == opcodeMap.end()) {
        return;
    }
    
    this->opcode = it->second;


    switch (this->opcode) {

        case OpCode::ADD:
        case OpCode::SUB:
        case OpCode::MUL:
        case OpCode::DIV:
        case OpCode::ADD_D:
        case OpCode::SUB_D:
        case OpCode::MUL_D:
        case OpCode::DIV_D:

            if (tokens.size() == 4) {
                rd = tokens[1];
                rs = tokens[2];
                rt = tokens[3];
                this->valid = true;
            }
            break;

        case OpCode::ADDI:
        case OpCode::SUBI:

            if (tokens.size() == 4) {
                rd = tokens[1];
                rs = tokens[2];
                immediate = tokens[3];
                this->valid = true;
            }
            break;

        case OpCode::LW:
        case OpCode::L_D:
        case OpCode::SW:
        case OpCode::S_D:
        
            if (tokens.size() == 3) {
                
                // tokens[2] é a string "0(R2)"
                const std::regex ls_regex(R"(^([A-Z0-9\-]+)\(([A-Z0-9]+)\)$)");
                std::smatch ls_match;
                
                if (std::regex_match(tokens[2], ls_match, ls_regex) && ls_match.size() == 3) {
                    immediate = ls_match[1].str(); // "0"
                    rs = ls_match[2].str(); // "R2" (Base)
                    
                    if (this->opcode == OpCode::SW || this->opcode == OpCode::S_D) {
                        rt = tokens[1]; // Fonte
                    } else {
                        rd = tokens[1]; // Destino
                    }
                    this->valid = true;
                }
            }
            break;

        case OpCode::BEQ:
        case OpCode::BNE:

            if (tokens.size() == 4) {
                rs = tokens[1];
                rt = tokens[2];
                immediate = tokens[3]; 
                this->valid = true;
            }
            break;

        case OpCode::JAL:

            if (tokens.size() == 3) {
                rd = tokens[1];
                immediate = tokens[2];
                this->valid = true;
            }
            break;
        
        case OpCode::NOP:

            if (tokens.size() == 1) {
                this->valid = true;
            }
            break;

        default:
            this->valid = false;
            break;
    }
}