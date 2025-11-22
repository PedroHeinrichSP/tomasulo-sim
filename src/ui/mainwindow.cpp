#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      pipelineScene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    loadStyleSheet(":/styles/style_dark.qss");

    ui->graphicsPipeline->setScene(pipelineScene);
    setupTables();

    // Conexões
    connect(ui->btnRun, &QPushButton::clicked, this, &MainWindow::onRunClicked);
    connect(ui->btnPause, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
    connect(ui->btnStep, &QPushButton::clicked, this, &MainWindow::onStepClicked);
    connect(ui->btnReset, &QPushButton::clicked, this, &MainWindow::onResetClicked);

    connect(ui->actionAbrir, &QAction::triggered, this, &MainWindow::onActionAbrirTriggered);
    connect(ui->actionSair, &QAction::triggered, this, &MainWindow::onActionSairTriggered);
    connect(ui->actionSobre, &QAction::triggered, this, &MainWindow::onActionSobreTriggered);

    connect(ui->radioDark, &QRadioButton::toggled, this, &MainWindow::onThemeChanged);
    connect(ui->radioLight, &QRadioButton::toggled, this, &MainWindow::onThemeChanged);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupTables() {
    ui->tableRS->setColumnCount(8);
    ui->tableRS->setHorizontalHeaderLabels({"Estação", "Busy", "Op", "Vj", "Vk", "Qj", "Qk", "Dest"});
    ui->tableRS->setRowCount(4);

    // placeholder
    auto *item = new QTableWidgetItem("ADD.D F2, F4, F6");
    applyStageColor(item, "issue");
    ui->tableRS->setItem(0, 2, item);
}

void MainWindow::applyStageColor(QTableWidgetItem *item, const QString &stage) {
    QColor color;
    if (stage == "issue") color = QColor("#4FC3F7");
    else if (stage == "execute") color = QColor("#FFD54F");
    else if (stage == "writeback") color = QColor("#81C784");
    else if (stage == "commit") color = QColor("#BA68C8");
    else color = QColor("#B0BEC5");
    item->setBackground(color);
}

void MainWindow::loadStyleSheet(const QString &path) {
    QFile f(path);
    if (f.open(QFile::ReadOnly))
        qApp->setStyleSheet(QString::fromUtf8(f.readAll()));
}

// === MENU ACTIONS ===

void MainWindow::onActionAbrirTriggered() {
    QFileDialog::getOpenFileName(this, "Abrir programa", QString(), "Assembly (*.asm *.txt)");
}

void MainWindow::onActionSairTriggered() {
    close();
}

void MainWindow::onActionSobreTriggered() {
    QMessageBox::information(this, "Sobre",
        "Simulador de Tomasulo - v0.1\n"
        "Desenvolvido para fins didáticos.\n"
        "Suporta especulação e reorder buffer.");
}

void MainWindow::onThemeChanged() {
    if (ui->radioDark->isChecked()) loadStyleSheet(":/styles/style_dark.qss");
    else loadStyleSheet(":/styles/style_light.qss");
}

// === SIMULAÇÃO ===
void MainWindow::onRunClicked() {}
void MainWindow::onPauseClicked() {}
void MainWindow::onStepClicked() {}
void MainWindow::onResetClicked() {}