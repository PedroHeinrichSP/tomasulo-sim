#ifndef RESERVATION_STATION_H
#define RESERVATION_STATION_H

#include <string>
#include <vector>

/*
 * Estrutura responsável por manter instruções esperando operandos.
 * Campos típicos:
 *  - nome da estação
 *  - operação
 *  - Vj, Vk (valores prontos)
 *  - Qj, Qk (dependências)
 *  - destino no ROB
 *  - flag Busy
 */

class ReservationStation {
    public:
        std::string name;
        std::string op;
        bool busy = false;
        std::string Qj, Qk;
        float Vj = 0, Vk = 0;
        int robIndex = -1;

    private:
        // TODO:
        // - issueInstruction()
        // - updateValues()
        // - clear()
};

#endif // RESERVATION_STATION_H