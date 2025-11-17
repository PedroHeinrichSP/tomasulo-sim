#include "gtest/gtest.h"
#include "common_data_bus.h" // A classe que estamos testando

// Teste 1: Estado Inicial
TEST(CommonDataBusTest, InitialState) {
    CommonDataBus cdb;
    // O barramento deve começar vazio
    EXPECT_TRUE(cdb.messages.empty());
}

// Teste 2: Broadcast (anúncio de resultados)
TEST(CommonDataBusTest, Broadcast) {
    CommonDataBus cdb;
    
    // Anuncia o primeiro resultado
    cdb.broadcast(3, 10.5f);
    
    EXPECT_EQ(cdb.messages.size(), 1);
    EXPECT_EQ(cdb.messages[0].tag, 3);
    EXPECT_EQ(cdb.messages[0].value, 10.5f);

    // Anuncia um segundo resultado (múltiplos broadcasts por ciclo)
    cdb.broadcast(5, -20.0f);
    EXPECT_EQ(cdb.messages.size(), 2);
    EXPECT_EQ(cdb.messages[1].tag, 5);
    EXPECT_EQ(cdb.messages[1].value, -20.0f);
}

// Teste 3: Limpeza do Barramento (fim do ciclo)
TEST(CommonDataBusTest, Clear) {
    CommonDataBus cdb;
    
    // Anuncia um resultado
    cdb.broadcast(1, 1.0f);
    EXPECT_FALSE(cdb.messages.empty());
    
    // Limpa o barramento
    cdb.clear();
    EXPECT_TRUE(cdb.messages.empty());
}