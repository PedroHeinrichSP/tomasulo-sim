#include "gtest/gtest.h"
#include "reorder_buffer.h"

// Teste com um ROB pequeno (4 entradas) para facilitar
class ReorderBufferTest : public ::testing::Test {
protected:
    ReorderBuffer rob{4}; // Um ROB com 4 entradas (0, 1, 2, 3)

    void SetUp() override {
        rob.clear(); // Garante que o ROB esteja limpo
    }
};

TEST_F(ReorderBufferTest, InitialState) {
    EXPECT_FALSE(rob.isFull());
    EXPECT_EQ(rob.getNextToCommit(), nullptr);
}

TEST_F(ReorderBufferTest, Allocation) {
    // Aloca a primeira entrada (tag 0)
    int tag0 = rob.allocate(OpCode::ADD, "R1");
    EXPECT_EQ(tag0, 0);

    // Aloca a segunda entrada (tag 1)
    int tag1 = rob.allocate(OpCode::SUB, "R2");
    EXPECT_EQ(tag1, 1);
}

TEST_F(ReorderBufferTest, IsFull) {
    rob.allocate(OpCode::ADD, "R1"); // tag 0
    rob.allocate(OpCode::ADD, "R2"); // tag 1
    rob.allocate(OpCode::ADD, "R3"); // tag 2
    rob.allocate(OpCode::ADD, "R4"); // tag 3
    
    EXPECT_TRUE(rob.isFull());

    // Tenta alocar mais um
    int tagFail = rob.allocate(OpCode::ADD, "R5");
    EXPECT_EQ(tagFail, NO_PRODUCER); // Deve falhar (-1)
}

TEST_F(ReorderBufferTest, CommitInOrder) {
    // 1. Aloca duas instruções
    int tag0 = rob.allocate(OpCode::ADD, "R1"); // head = 0
    int tag1 = rob.allocate(OpCode::SUB, "R2"); // tail = 2

    // 2. Resultados chegam FORA DE ORDEM (Snoop do CDB)
    rob.snoopCDB(tag1, 20.0f); // ROB1 (SUB) termina
    rob.snoopCDB(tag0, 10.0f); // ROB0 (ADD) termina
    
    // 3. Tenta comitar
    ROBEntry* entry = rob.getNextToCommit();
    ASSERT_NE(entry, nullptr); // Deve pegar a entrada da 'head' (tag 0)
    EXPECT_EQ(entry->op, OpCode::ADD);
    EXPECT_EQ(entry->destReg, "R1");
    EXPECT_EQ(entry->value, 10.0f);

    // 4. Confirma o commit
    rob.commitHead(); // Avança a 'head' para 1

    // 5. Tenta comitar o próximo
    entry = rob.getNextToCommit();
    ASSERT_NE(entry, nullptr); // Deve pegar a entrada da 'head' (tag 1)
    EXPECT_EQ(entry->op, OpCode::SUB);
    EXPECT_EQ(entry->destReg, "R2");
    EXPECT_EQ(entry->value, 20.0f);

    // 6. Confirma o commit
    rob.commitHead(); // Avança a 'head' para 2
    
    // 7. ROB agora deve estar vazio
    EXPECT_EQ(rob.getNextToCommit(), nullptr);
}

TEST_F(ReorderBufferTest, CommitWaitsForWriteBack) {
    // 1. Aloca uma instrução
    int tag0 = rob.allocate(OpCode::MUL_D, "R1"); // head = 0

    // 2. Tenta comitar ANTES do resultado chegar
    ROBEntry* entry = rob.getNextToCommit();
    EXPECT_EQ(entry, nullptr); // Deve falhar (estado != WRITE_BACK)
    
    // 3. Resultado chega (Snoop do CDB)
    rob.snoopCDB(tag0, 99.0f);
    
    // 4. Tenta comitar DE NOVO
    entry = rob.getNextToCommit();
    ASSERT_NE(entry, nullptr); // Agora deve funcionar
    EXPECT_EQ(entry->value, 99.0f);
}

TEST_F(ReorderBufferTest, CircularWrapAround) {
    rob.allocate(OpCode::ADD, "R1"); // tag 0
    rob.allocate(OpCode::ADD, "R2"); // tag 1
    rob.allocate(OpCode::ADD, "R3"); // tag 2
    rob.allocate(OpCode::ADD, "R4"); // tag 3
    EXPECT_TRUE(rob.isFull());

    // Comita os dois primeiros (libera 0 e 1)
    rob.snoopCDB(0, 1.0f);
    rob.snoopCDB(1, 2.0f);
    rob.getNextToCommit();
    rob.commitHead(); // head = 1
    rob.getNextToCommit();
    rob.commitHead(); // head = 2
    
    EXPECT_FALSE(rob.isFull()); // Agora temos 2 espaços

    // Aloca de novo (deve usar os espaços 0 e 1)
    int tag0_new = rob.allocate(OpCode::SUB, "R5"); // Aloca no 'tail' (que está em 0)
    EXPECT_EQ(tag0_new, 0);

    int tag1_new = rob.allocate(OpCode::SUB, "R6"); // Aloca no 'tail' (que está em 1)
    EXPECT_EQ(tag1_new, 1);
    
    EXPECT_TRUE(rob.isFull()); // Cheio de novo
}