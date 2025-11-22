#include "gtest/gtest.h"
#include "tomasulo_core.h"
#include <fstream>

class StepByStepTest : public ::testing::Test {
protected:
    TomasuloCore core;

    void SetUp() override {
        core.reset();
    }

    void createTestProgram() {
        std::ofstream outFile("robust_test.txt");
        outFile << "ADDI R1, R0, 10\n"; 
        outFile << "ADDI R2, R0, 20\n"; 
        outFile << "ADD R3, R1, R2\n";  
        outFile.close();
    }
};

TEST_F(StepByStepTest, DetailedExecutionFlow) {
    createTestProgram();
    ASSERT_TRUE(core.loadProgram("robust_test.txt"));

    // --- ESTADO INICIAL ---
    EXPECT_EQ(core.cycle, 0);
    EXPECT_EQ(core.pc, 0); // PC começa em 0

    // --- CICLO 1: Issue ADDI R1 ---
    core.step(); 
    EXPECT_EQ(core.cycle, 1);
    EXPECT_EQ(core.pc, 1); // PC avançou
    EXPECT_FALSE(core.rob.isEmpty());
    EXPECT_EQ(core.registers.getProducer("R1"), 0); 

    // --- CICLO 2: Issue ADDI R2 ---
    core.step();
    EXPECT_EQ(core.cycle, 2);
    EXPECT_EQ(core.pc, 2);
    EXPECT_EQ(core.registers.getProducer("R2"), 1);

    // --- CICLO 3: Issue ADD R3 ---
    core.step();
    EXPECT_EQ(core.pc, 3); // Todas emitidas

    // Roda até terminar
    int limit = 50;
    while (!core.isFinished() && limit-- > 0) {
        core.step();
    }

    // --- VALIDAÇÃO FINAL ---
    EXPECT_TRUE(core.isFinished());
    EXPECT_FLOAT_EQ(core.registers.getValue("R3"), 30.0f); 
}

TEST_F(StepByStepTest, InstructionQueueConsumption) {
    core.addInstruction("NOP");
    core.addInstruction("NOP");
    core.addInstruction("NOP");
    
    EXPECT_EQ(core.instructionMemory.size(), 3);
    EXPECT_EQ(core.pc, 0);
    
    core.step(); // Issue NOP 1
    core.step(); // Issue NOP 2
    core.step(); // Issue NOP 3
    
    // Agora PC deve estar no fim
    EXPECT_EQ(core.pc, 3);
    
    // Como NOPs comitam rápido, pode ser que já tenha acabado ou falte um ciclo de commit
    // O importante é que o PC andou
}