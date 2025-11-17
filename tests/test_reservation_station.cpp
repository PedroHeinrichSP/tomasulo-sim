#include "gtest/gtest.h"
#include "reservation_station.h" // A *nova* classe "Gerente"

// Testa o *Gerente* (o Pool)
class ReservationStationPoolTest : public ::testing::Test {
protected:
    // Cria um pool com 2 estações ("Add0", "Add1") e 2 CICLOS
    ReservationStation addPool{2, "Add", 2};

    void SetUp() override {
        addPool.clear();
    }
};

// Testa se a alocação de slots (o 'findFreeSlot') funciona
TEST_F(ReservationStationPoolTest, FindAndFillSlots) {
    // 1. Pega o primeiro slot livre
    RSEntry* slot1 = addPool.findFreeSlot();
    ASSERT_NE(slot1, nullptr);
    EXPECT_EQ(slot1->name, "Add0");
    
    // 2. Ocupa o slot1 (simulando um 'issue' de ADD)
    //    Não passamos mais '2' ciclos, a 'issue' usa o 'cicles' do pool
    addPool.issue(slot1, OpCode::ADD, 1, NO_PRODUCER, 10.f, NO_PRODUCER, 20.f);
    EXPECT_TRUE(slot1->busy);
    EXPECT_EQ(slot1->cyclesLeft, 2); // Testa o 'cicles' do pool

    // 3. Pega o segundo slot livre
    RSEntry* slot2 = addPool.findFreeSlot();
    ASSERT_NE(slot2, nullptr);
    EXPECT_EQ(slot2->name, "Add1");

    // 4. Ocupa o slot2 (simulando um 'issue' de SUB)
    addPool.issue(slot2, OpCode::SUB, 2, 3, 0.f, 4, 0.f);
    EXPECT_TRUE(slot2->busy);
    EXPECT_EQ(slot2->Qj, 3);
    EXPECT_EQ(slot2->cyclesLeft, 2); // Testa o 'cicles' do pool

    // 5. Tenta pegar outro slot
    RSEntry* slot3 = addPool.findFreeSlot();
    EXPECT_EQ(slot3, nullptr); // Pool está cheio, retorna null
}

// Testa o "mapa de snoop" (a sua otimização)
TEST_F(ReservationStationPoolTest, OptimizedSnoopPool) {
    // 1. Ocupa os dois slots
    RSEntry* slot1 = addPool.findFreeSlot(); // Add0
    RSEntry* slot2 = addPool.findFreeSlot(); // Add1
    
    // slot1 (Add0) espera pela tag 5
    addPool.issue(slot1, OpCode::ADD, 1, 5, 0.0f, NO_PRODUCER, 10.0f);
    
    // slot2 (Add1) também espera pela tag 5
    addPool.issue(slot2, OpCode::SUB, 2, 5, 0.0f, NO_PRODUCER, 20.0f);

    // 2. Registra os "ouvintes" (o Core faria isso)
    addPool.registerListener(slot1, 5);
    addPool.registerListener(slot2, 5);

    // Verifica o estado "isReady" manualmente
    auto isReady = [](RSEntry* e) {
        return e->busy && e->Qj == NO_PRODUCER && e->Qk == NO_PRODUCER;
    };
    
    EXPECT_FALSE(isReady(slot1));
    EXPECT_FALSE(isReady(slot2));

    // 3. O CDB faz broadcast de uma tag irrelevante (99)
    addPool.snoopPool(99, 123.0f);
    EXPECT_EQ(slot1->Qj, 5); // Não mudou
    EXPECT_EQ(slot2->Qj, 5); // Não mudou

    // 4. O CDB faz broadcast da tag 5
    addPool.snoopPool(5, 99.0f);

    // 5. AMBOS os slots (que estavam na lista) devem ter recebido o valor
    EXPECT_EQ(slot1->Vj, 99.0f);
    EXPECT_EQ(slot2->Vj, 99.0f);
    
    // Verifica se "isReady" agora é verdadeiro
    EXPECT_TRUE(isReady(slot1));
    EXPECT_TRUE(isReady(slot2));
    
    // 6. O mapa de 'listeners' para a tag 5 deve estar vazio
    addPool.snoopPool(5, 99.0f); // Chamar de novo não deve fazer nada
}

// Testa o 'clear' do pool
TEST_F(ReservationStationPoolTest, ClearPool) {
    RSEntry* slot1 = addPool.findFreeSlot();
    addPool.issue(slot1, OpCode::ADD, 1, 3, 0.0f, NO_PRODUCER, 20.0f);
    addPool.registerListener(slot1, 3);
    EXPECT_TRUE(slot1->busy);
    EXPECT_EQ(slot1->cyclesLeft, 2);

    addPool.clear();
    
    // O slot1 (agora pego de novo) deve estar limpo
    RSEntry* slot1_again = addPool.findFreeSlot();
    EXPECT_FALSE(slot1_again->busy);
    EXPECT_EQ(slot1_again->name, "Add0");
    
    // O mapa de listeners também deve estar limpo
    addPool.snoopPool(3, 10.0f); // Não deve ter efeito
    EXPECT_EQ(slot1_again->Qj, NO_PRODUCER);
    EXPECT_EQ(slot1_again->cyclesLeft, 0); // Verifica se o 'clear' zerou os ciclos
}