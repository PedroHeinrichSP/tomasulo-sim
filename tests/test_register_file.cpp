#include "gtest/gtest.h"
#include "register_file.h" // Classe que estamos testando

// Uma "Fixture" de Teste para evitar repetir o setup
class RegisterFileTest : public ::testing::Test {
protected:
    RegisterFile regFile;
    std::vector<std::string> regNames = {"R1", "R2", "R3"};

    // Roda antes de CADA teste
    void SetUp() override {
        // CORREÇÃO: Usamos init() para criar a estrutura inicial
        regFile.innit(regNames);
    }
};

// Teste 1: Estado Inicial (após o init())
TEST_F(RegisterFileTest, InitialState) {
    // Verifica se os valores iniciais são 0.0
    EXPECT_EQ(regFile.getValue("R1"), 0.0f);
    EXPECT_EQ(regFile.getValue("R2"), 0.0f);
    
    // Verifica se todos estão "livres" (produtor = -1)
    EXPECT_EQ(regFile.getProducer("R1"), NO_PRODUCER);
    EXPECT_EQ(regFile.getProducer("R2"), NO_PRODUCER);
}

// Teste 2: Renomeação (Set Producer)
TEST_F(RegisterFileTest, SetProducer) {
    regFile.setProducer("R1", 3); // Renomeia R1 para ser produzido por ROB 3
    EXPECT_EQ(regFile.getProducer("R1"), 3);

    // Outro registrador não deve ser afetado
    EXPECT_EQ(regFile.getProducer("R2"), NO_PRODUCER);
}

// Teste 3: Escrita (Commit)
TEST_F(RegisterFileTest, WriteValue) {
    regFile.writeValue("R2", 12.5f);
    EXPECT_EQ(regFile.getValue("R2"), 12.5f);
}

// Teste 4: Limpeza da RAT no Commit (Caso Simples)
TEST_F(RegisterFileTest, OnCommitClearsRAT) {
    // 1. Renomeia R1 para ser produzido por ROB 3
    regFile.setProducer("R1", 3);
    EXPECT_EQ(regFile.getProducer("R1"), 3);
    
    // 2. ROB 3 comita
    regFile.onCommit("R1", 3);

    // 3. A RAT deve agora estar limpa para R1
    EXPECT_EQ(regFile.getProducer("R1"), NO_PRODUCER);
}

// Teste 5: Lógica de Risco WAW (Write-After-Write) - O Teste Mais Importante!
TEST_F(RegisterFileTest, WAW_HazardOnCommit) {
    // 1. ADD R1 (ROB3) é emitida
    regFile.setProducer("R1", 3);
    EXPECT_EQ(regFile.getProducer("R1"), 3);

    // 2. SUB R1 (ROB4) é emitida (depois)
    regFile.setProducer("R1", 4);
    EXPECT_EQ(regFile.getProducer("R1"), 4); // RAT agora aponta para ROB4

    // 3. O ROB 3 (ADD) comita (antes, pois é mais rápido ou foi emitido antes)
    regFile.onCommit("R1", 3);

    // 4. A RAT NÃO PODE ser limpa, pois ela espera por ROB 4!
    EXPECT_EQ(regFile.getProducer("R1"), 4);

    // 5. O ROB 4 (SUB) comita
    regFile.onCommit("R1", 4);

    // 6. AGORA SIM a RAT deve ser limpa
    EXPECT_EQ(regFile.getProducer("R1"), NO_PRODUCER);
}