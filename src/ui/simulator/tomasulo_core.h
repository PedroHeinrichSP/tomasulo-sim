#ifndef TOMASULO_CORE_H
#define TOMASULO_CORE_H

#include "instruction.h"
#include "reservation_station.h"
#include "reorder_buffer.h"
#include "register_file.h"
#include "common_data_bus.h"

/*
 * Núcleo do simulador:
 *  - controla o fluxo de instruções (issue → execute → write → commit)
 *  - gerencia as dependências e atualiza estruturas
 *  - responsável pelo avanço de ciclos
 */

class TomasuloCore {
    public:
        std::vector<Instruction> instructionQueue;
        std::vector<ReservationStation> stations;
        ReorderBuffer rob;
        RegisterFile registers;
        CommonDataBus cdb;

        int cycle = 0;
        int committed = 0;
        int bubbles = 0;

    private:
        // TODO:
        // - step()
        // - run()
        // - reset()
        // - calculateIPC()
};

#endif // TOMASULO_CORE_H