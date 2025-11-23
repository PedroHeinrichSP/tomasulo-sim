#include "gtest/gtest.h"
#include "instruction.h"

// Teste: entrada com letras minúsculas e espaços irregulares deve ser normalizada
TEST(InstructionParseExtra, LowercaseAndSpacing) {
    Instruction inst("  add   r1 , r2,   r3  ");

    EXPECT_TRUE(inst.isValid());
    EXPECT_EQ(inst.opcode, OpCode::ADD);
    EXPECT_EQ(inst.rd, "R1");
    EXPECT_EQ(inst.rs, "R2");
    EXPECT_EQ(inst.rt, "R3");
}

// Teste: load com offset negativo e registrador de índice de dois dígitos
TEST(InstructionParseExtra, LoadNegativeOffsetAndLargeRegister) {
    Instruction inst("LW F2, -8(R10)");

    EXPECT_TRUE(inst.isValid());
    EXPECT_EQ(inst.opcode, OpCode::LW);
    EXPECT_EQ(inst.rd, "F2");
    EXPECT_EQ(inst.rs, "R10");
    EXPECT_EQ(inst.immediate, "-8");
}

// Teste: formato de load malformado (sem parênteses) deve ser inválido
TEST(InstructionParseExtra, LoadMalformedMissingParenthesis) {
    Instruction inst("LW F1, 32R4"); // falta '(' e ')'

    EXPECT_FALSE(inst.isValid());
    // opcode is recognized (LW) but parsing of the address failed -> invalid instruction
    EXPECT_EQ(inst.opcode, OpCode::LW);
}

// Teste: instrução de ponto flutuante R-Type
TEST(InstructionParseExtra, FloatingPointRType) {
    Instruction inst("ADD.D F1, F2, F3");

    EXPECT_TRUE(inst.isValid());
    EXPECT_EQ(inst.opcode, OpCode::ADD_D);
    EXPECT_EQ(inst.rd, "F1");
    EXPECT_EQ(inst.rs, "F2");
    EXPECT_EQ(inst.rt, "F3");
}

// Teste: linha vazia ou apenas espaços deve ser inválida
TEST(InstructionParseExtra, EmptyOrWhitespace) {
    Instruction inst1("");
    Instruction inst2("   ");

    EXPECT_FALSE(inst1.isValid());
    EXPECT_EQ(inst1.opcode, OpCode::INVALID);

    EXPECT_FALSE(inst2.isValid());
    EXPECT_EQ(inst2.opcode, OpCode::INVALID);
}
