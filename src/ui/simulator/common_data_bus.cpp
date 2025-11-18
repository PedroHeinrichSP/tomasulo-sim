#include "common_data_bus.h"

/*
 * TODO:
 * - Implementar broadcast() para enviar valores a todas as RS e ao ROB
 * - Implementar clearCycle() para limpar mensagens no fim de cada ciclo
 * - Adicionar logs para visualização no painel de “Logs” da interface
 */

void CommonDataBus::broadcast(int tag, float value) {
    messages.push_back({tag, value});
}

void CommonDataBus::clear(){
    messages.clear();
}

