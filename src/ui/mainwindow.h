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

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRunClicked();
    void onPauseClicked();
    void onStepClicked();
    void onResetClicked();

    void onActionAbrirTriggered();
    void onActionSairTriggered();
    void onActionSobreTriggered();

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
    void setupTables();
    void applyStageColor(QTableWidgetItem *item, const QString &stage);
    void loadStyleSheet(const QString &path);
    void initializeRegisterNames();
    void refreshUI();
    void updateReservationStationsTable();
    void updateROBTable();
    void updateRegistersTable();
    void updatePipelineScene();
    void updateMetrics();
    QString opcodeToString(OpCode op) const;
    QString robStateToString(ROBState state) const;
    bool loadProgramFromFile(const QString &filePath);
    void appendLog(const QString &message);
    void stopSimulation();
    void resetState(bool reloadProgram = true);
    void updateStatusLabel(const QString &statusText);
};

#endif // MAINWINDOW_H