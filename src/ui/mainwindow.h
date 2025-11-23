#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTableWidgetItem>
#include <QTimer>
#include <QStringList>
#include "simulator/tomasulo_core.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Interface principal do simulador de Tomasulo.
 *
 * Responsável por controlar o timer de execução, coordenar interações do usuário
 * (botões/menu), preencher as tabelas do pipeline/RS/ROB/registradores e exibir
 * métricas e logs em tempo real sem alterar o núcleo em `src/ui/simulator`.
 */

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /** Botões da barra de controle */
    void onRunClicked();
    void onPauseClicked();
    void onStepClicked();
    void onResetClicked();

    /** Ações de menu */
    void onActionAbrirTriggered();
    void onActionSairTriggered();
    void onActionSobreTriggered();

    /** Preferências e laço de simulação */
    void onThemeChanged();
    void performSimulationStep();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *pipelineScene;
    QTimer *simulationTimer;
    TomasuloCore core;
    QString loadedProgramPath;
    QStringList registerNames;
    QStringList stationOrder;
    int bubbleCycles = 0;
    bool isRunning = false;

    // === Configuração inicial ===
    void setupTables();
    void applyStageColor(QTableWidgetItem *item, const QString &stage);
    void loadStyleSheet(const QString &path);
    void initializeRegisterNames();

    // === Atualização das visualizações ===
    void refreshUI();
    void updateReservationStationsTable();
    void updateROBTable();
    void updateRegistersTable();
    void updatePipelineScene();
    void updateMetrics();

    // === Utilitários ===
    QString opcodeToString(OpCode op) const;
    QString robStateToString(ROBState state) const;
    bool loadProgramFromFile(const QString &filePath);
    void appendLog(const QString &message);
    void stopSimulation();
    void resetState(bool reloadProgram = true);
    void updateStatusLabel(const QString &statusText);
};

#endif // MAINWINDOW_H