#include "gtest/gtest.h"
#include "instruction.h" // Inclui a classe que estamos testando

// Teste para instruções R-Type
TEST(InstructionParse, R_Type) {
    Instruction inst("ADD R1, R2, R3");
    
    EXPECT_TRUE(inst.isValid());
    EXPECT_EQ(inst.opcode, OpCode::ADD);
    EXPECT_EQ(inst.rd, "R1");
    EXPECT_EQ(inst.rs, "R2");
    EXPECT_EQ(inst.rt, "R3");
    EXPECT_EQ(inst.immediate, ""); // Imediato deve estar vazio
}

// Teste para instruções I-Type (Load)
// Teste com sintaxe original: LW F1, 32(R4)
TEST(InstructionParse, Load_Type) {
    Instruction inst("LW F1, 32(R4)");

    EXPECT_TRUE(inst.isValid());
    EXPECT_EQ(inst.opcode, OpCode::LW);
    EXPECT_EQ(inst.rd, "F1");     // Destino
    EXPECT_EQ(inst.rs, "R4");     // Base
    EXPECT_EQ(inst.immediate, "32");    // Imediato
    EXPECT_EQ(inst.rt, "");       // rt deve estar vazio
}

// Teste para instruções S-Type (Store)
// Teste com sintaxe original: SW F2, 0(R1)
TEST(InstructionParse, Store_Type) {
    Instruction inst("SW F2, 0(R1)");

    EXPECT_TRUE(inst.isValid());
    EXPECT_EQ(inst.opcode, OpCode::SW);
    EXPECT_EQ(inst.rt, "F2");     // Fonte (Store usa 'rt')
    EXPECT_EQ(inst.rs, "R1");     // Base
    EXPECT_EQ(inst.immediate, "0");     // Imediato
    EXPECT_EQ(inst.rd, "");       // rd deve estar vazio
}

// Teste para instruções I-Type (Imediato)
TEST(InstructionParse, Immediate_Type) {
    Instruction inst("ADDI R5, R6, -50"); // O tokenizer vai separar "-50"

    EXPECT_TRUE(inst.isValid());
    EXPECT_EQ(inst.opcode, OpCode::ADDI);
    EXPECT_EQ(inst.rd, "R5");
    EXPECT_EQ(inst.rs, "R6");
    EXPECT_EQ(inst.immediate, "-50");   // Imediato (negativo)
    EXPECT_EQ(inst.rt, "");
}

// Teste para sintaxe inválida (opcode não existe)
TEST(InstructionParse, Invalid_Opcode) {
    Instruction inst("FOO BAR BAZ");
    EXPECT_FALSE(inst.isValid());
    EXPECT_EQ(inst.opcode, OpCode::INVALID);
}

// Teste para sintaxe inválida (formato errado)
TEST(InstructionParse, Invalid_Format_RType) {
    Instruction inst("ADD R1, R2(R3)"); // Tenta usar formato LW
    EXPECT_FALSE(inst.isValid()); // Falha no check tokens.size() == 4
}

// Teste para sintaxe inválida (tokens a mais)
TEST(InstructionParse, Invalid_Format_TooManyTokens) {
    Instruction inst("JAL R1, LABEL, EXTRA");
    EXPECT_FALSE(inst.isValid()); // JAL espera 3 tokens, recebeu 4
}

// Teste para instruções de Branch (BEQ)
TEST(InstructionParse, Branch_Type) {
    Instruction inst("BEQ R1, R2, MY_LABEL");
    EXPECT_TRUE(inst.isValid());
    EXPECT_EQ(inst.opcode, OpCode::BEQ);
    EXPECT_EQ(inst.rs, "R1");
    EXPECT_EQ(inst.rt, "R2");
    EXPECT_EQ(inst.immediate, "MY_LABEL");
    EXPECT_EQ(inst.rd, "");
}

// Teste para instruções de Jump (JAL)
TEST(InstructionParse, Jump_Type_JAL) {
    Instruction inst("JAL R5, DO_STUFF");
    EXPECT_TRUE(inst.isValid());
    EXPECT_EQ(inst.opcode, OpCode::JAL);
    EXPECT_EQ(inst.rd, "R5");
    EXPECT_EQ(inst.immediate, "DO_STUFF");
    EXPECT_EQ(inst.rs, "");
    EXPECT_EQ(inst.rt, "");
}

// Teste para NOP
TEST(InstructionParse, NOP_Type) {
    Instruction inst("NOP");
    EXPECT_TRUE(inst.isValid());
    EXPECT_EQ(inst.opcode, OpCode::NOP);
}