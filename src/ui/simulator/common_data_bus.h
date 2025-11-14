#ifndef COMMON_DATA_BUS_H
#define COMMON_DATA_BUS_H

#include <string>
#include <vector>

/*
 * Simula o barramento de dados comum (CDB):
 *  - responsável por transmitir resultados das execuções
 *  - notifica estações de reserva e o ROB
 */

struct CDBMessage {
    std::string tag;
    float value;
};

class CommonDataBus {
    public:
        std::vector<CDBMessage> messages;

    private:
        // TODO:
        // - broadcast()
        // - clearCycle()
};

#endif // COMMON_DATA_BUS_H