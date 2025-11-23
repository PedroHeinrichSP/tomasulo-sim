#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTableWidgetItem>
#include "TomasuloCore.h"

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

private:
    Ui::MainWindow *ui;
    QGraphicsScene *pipelineScene;
    void setupTables();
    void applyStageColor(QTableWidgetItem *item, const QString &stage);
    void loadStyleSheet(const QString &path);
};

#endif // MAINWINDOW_H