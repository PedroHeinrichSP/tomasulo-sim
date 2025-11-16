#ifndef REORDER_BUFFER_H
#define REORDER_BUFFER_H

#include <string>
#include <vector>
#include <simulator_utils.h>

/*
 * Implementa o Buffer de Reordenação (ROB):
 *  - controla commit em ordem
 *  - mantém estado e valor dos resultados
 *  - suporta especulação e flush em caso de desvio incorreto
 */

struct ROBEntry {
    bool busy = false;
    OpCode op = OpCode::INVALID;
    std::string destReg;
    float value = 0.0f;
    ROBState state = ROBState::ISSUE;
};

class ReorderBuffer {
    private:
        std::vector<ROBEntry> entries;
        int head, tail, count;
    public:
    /**
     * @brief Construtor. Cria o buffer com um tamanho fixo.
     * @param size O número de entradas no ROB (ex: 16).
     */
    ReorderBuffer(int size = 16);

    /**
     * @brief Limpa (reseta) o ROB para um estado inicial.
     */
    void clear();

    /**
     * @brief Tenta alocar uma nova entrada para uma instrução.
     * Mapeia o 'allocateEntry()' do TODO.
     * @param op O opcode da instrução.
     * @param destReg O registrador de destino.
     * @return O índice (tag) da entrada alocada (ex: 3) ou -1 se o ROB estiver CHEIO.
     */
    int allocate(OpCode op, const std::string& destReg);

    /**
     * @brief "Ouve" (snoops) o CDB.
     * Se uma entrada do ROB corresponder à tag, ela armazena o 'value'
     * e se marca como 'WRITE_BACK'.
     * @param tag A tag (índice do ROB) transmitida no CDB.
     * @param value O valor (resultado) transmitido.
     */
    void snoopCDB(int tag, float value);

    /**
     * @brief Verifica a instrução na 'head' e a retorna se estiver pronta.
     * Mapeia o 'commitEntry()' do TODO (Parte 1: Verificação).
     * @return Um ponteiro para a entrada na 'head' SE o estado for WRITE_BACK,
     * caso contrário, nullptr.
     */
    ROBEntry* getNextToCommit();

    /**
     * @brief Avança o ponteiro 'head', liberando a entrada.
     * Mapeia o 'commitEntry()' do TODO (Parte 2: Avanço).
     * Deve ser chamado DEPOIS que o Core processou o commit.
     */
    void commitHead();

    /**
     * @brief Verifica se o ROB está cheio.
     * @return true se (count == size).
     */
    bool isFull();
    
    /**
     * @brief Limpa o ROB a partir de uma tag (predição de desvio errada).
     * Mapeia o 'flushOnBranchMiss()' do TODO.
     * (Esta é uma função avançada que implementaremos depois)
     * @param robIndex A tag da instrução de desvio.
     */
    void flush(int robIndex);
};

#endif // REORDER_BUFFER_H