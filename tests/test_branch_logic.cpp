#include "gtest/gtest.h"
#include "tomasulo_core.h"
#include <fstream>

class BranchLogicTest : public ::testing::Test {
protected:
    TomasuloCore core;

    void SetUp() override {
        core.reset();
    }

    void createBranchProgram() {
        std::ofstream outFile("branch_test.txt");
        
        // --- TESTE 1: BEQ NOT TAKEN ---
        // R1 = 10, R2 = 20. BEQ R1, R2, NOWHERE (Não pula)
        outFile << "ADDI R1, R0, 10\n";
        outFile << "ADDI R2, R0, 20\n";
        outFile << "BEQ R1, R2, NOWHERE\n"; 
        outFile << "ADDI R3, R0, 5\n"; // Deve executar (Fallthrough)

        // --- TESTE 2: JAL (Pulo Incondicional) ---
        // Pula para o label TARGET, ignorando o NOP
        outFile << "JAL R31, TARGET\n";
        outFile << "NOWHERE: NOP\n"; // Label NOWHERE
        outFile << "NOP\n";          // Instrução ignorada pelo JAL

        // --- TESTE 3: BEQ TAKEN (Com Flush) ---
        outFile << "TARGET: ADDI R4, R0, 50\n";
        outFile << "ADDI R5, R0, 50\n";
        // BEQ R4, R5, END (50 == 50, Pula!)
        outFile << "BEQ R4, R5, END\n";
        
        // Estas instruções serão emitidas especulativamente e devem sofrer FLUSH
        outFile << "ADDI R6, R0, 999\n"; 
        outFile << "ADDI R7, R0, 999\n"; 
        
        outFile << "END: NOP\n";
        outFile.close();
    }
};

TEST_F(BranchLogicTest, FullControlFlow) {
    createBranchProgram();
    ASSERT_TRUE(core.loadProgram("branch_test.txt"));

    // Executa até terminar (limite seguro)
    int limit = 100;
    while (!core.isFinished() && limit-- > 0) {
        core.step();
    }
    
    EXPECT_TRUE(core.isFinished());

    // VERIFICAÇÕES

    // 1. BEQ Not Taken
    EXPECT_FLOAT_EQ(core.registers.getValue("R1"), 10.0f);
    EXPECT_FLOAT_EQ(core.registers.getValue("R2"), 20.0f);
    // R3 deve ter executado (Fallthrough)
    EXPECT_FLOAT_EQ(core.registers.getValue("R3"), 5.0f);

    // 2. JAL
    // JAL salva PC+1 em R31. JAL está na linha 3 (índice), R31 deve ter 4.
    // (ADDI=0, ADDI=1, BEQ=2, ADDI=3, JAL=4... espera. Vamos contar linhas do arquivo)
    // 0: ADDI
    // 1: ADDI
    // 2: BEQ
    // 3: ADDI
    // 4: JAL -> PC era 4. Retorno = 5.
    // Label NOWHERE aponta para a linha 5 (NOP)
    EXPECT_FLOAT_EQ(core.registers.getValue("R31"), 5.0f);

    // 3. BEQ Taken (Flush)
    EXPECT_FLOAT_EQ(core.registers.getValue("R4"), 50.0f);
    EXPECT_FLOAT_EQ(core.registers.getValue("R5"), 50.0f);
    
    // R6 e R7 foram emitidos especulativamente (999), mas o Flush deve ter matado.
    // Se o Flush funcionou, eles devem ser 0 (valor inicial) ou o valor antigo se existisse.
    EXPECT_FLOAT_EQ(core.registers.getValue("R6"), 0.0f);
    EXPECT_FLOAT_EQ(core.registers.getValue("R7"), 0.0f);
}