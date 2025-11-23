# Tomasulo Simulator

Simulador didático do algoritmo de Tomasulo para instruções MIPS, com interface Qt6
para acompanhar estações de reserva, ROB, registradores e métricas de execução
passo a passo.

## Funcionalidades principais

- Núcleo completo com *reservation stations*, ROB, CDB, especulação simples e
	detecção de *hazards* estruturais e de dados.
- Interface gráfica com abas para Pipeline, Estações de Reserva, ROB, Registradores,
	Logs e Preferências.
- Barra de controle com ações **Executar**, **Pausar**, **Passo** e **Resetar**.
- Caixa de métricas exibindo IPC, ciclos totais, instruções confirmadas, ciclos de
	bolha e estado atual (Parado/Executando/Finalizado).
- Visualização gráfica dos estágios do pipeline e preenchimento das tabelas em tempo
	real a cada ciclo.

## Como compilar

Requisitos: Qt 6 (Widgets), CMake ≥ 3.16 e um compilador C++17.

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/caminho/para/Qt/6.x/gcc_64
cmake --build build
```

Para executar os testes automatizados (GoogleTest):

```bash
cd build
ctest
```

## Como usar a interface

1. Abra o aplicativo (`build/TomasuloSim`).
2. Carregue um arquivo `.asm` ou `.txt` via **Arquivo ▸ Abrir programa...**.
3. Utilize **Executar** para rodar continuamente (um `QTimer` avança os ciclos),
	 **Pausar** para congelar o estado, **Passo** para avançar um ciclo e **Resetar**
	 para limpar o simulador (recarregando o programa atual, se houver).
4. Acompanhe:
	 - **Pipeline**: contagem de instruções nos estágios Issue/Execute/Writeback/Commit.
	 - **Estação de Reserva/ROB/Registradores**: tabelas atualizadas a cada ciclo.
	 - **Logs**: registro de commits e eventos relevantes com o número do ciclo.
	 - **Preferências**: seleção de tema escuro/claro.

Para alterar o estilo, use a aba Preferências ou os botões de tema no painel lateral.
As métricas são atualizadas em tempo real conforme a simulação avança.