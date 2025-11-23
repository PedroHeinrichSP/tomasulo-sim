#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QAbstractItemView>
#include <QFileInfo>
#include <QHeaderView>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QPalette>
#include <QGraphicsTextItem>
#include <QVector>
#include <QPair>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
            ui(new Ui::MainWindow),
            pipelineScene(new QGraphicsScene(this)),
            simulationTimer(new QTimer(this))
{
    ui->setupUi(this);
    loadStyleSheet(":/styles/style_dark.qss");

    stationOrder = {QStringLiteral("Add"), QStringLiteral("Mul"), QStringLiteral("Load")};
    initializeRegisterNames();

    ui->graphicsPipeline->setScene(pipelineScene);
    setupTables();
    refreshUI();
    updateStatusLabel(tr("Parado"));

    simulationTimer->setInterval(250);
    connect(simulationTimer, &QTimer::timeout, this, &MainWindow::performSimulationStep);

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
    ui->tableRS->clear();
    ui->tableRS->setColumnCount(9);
    ui->tableRS->setHorizontalHeaderLabels({"Estação", "Busy", "Op", "Vj", "Vk", "Qj", "Qk", "Dest", "Ciclos"});
    ui->tableRS->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableRS->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableRS->verticalHeader()->setVisible(false);
    ui->tableRS->horizontalHeader()->setStretchLastSection(true);
    ui->tableRS->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    int totalEntries = 0;
    for (const auto &name : stationOrder) {
        auto it = core.stationMap.find(name.toStdString());
        if (it != core.stationMap.end()) {
            totalEntries += static_cast<int>(it->second->getEntries().size());
        }
    }
    ui->tableRS->setRowCount(totalEntries);

    ui->tableROB->clear();
    ui->tableROB->setColumnCount(6);
    ui->tableROB->setHorizontalHeaderLabels({"Idx", "Busy", "Op", "Dest", "Valor", "Estado"});
    ui->tableROB->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableROB->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableROB->verticalHeader()->setVisible(false);
    ui->tableROB->horizontalHeader()->setStretchLastSection(true);
    ui->tableROB->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableROB->setRowCount(core.rob.getSize());

    ui->tableRegs->clear();
    ui->tableRegs->setColumnCount(3);
    ui->tableRegs->setHorizontalHeaderLabels({"Registrador", "Valor", "Produtor"});
    ui->tableRegs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableRegs->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableRegs->verticalHeader()->setVisible(false);
    ui->tableRegs->horizontalHeader()->setStretchLastSection(true);
    ui->tableRegs->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableRegs->setRowCount(registerNames.size());
}

void MainWindow::initializeRegisterNames() {
    registerNames.clear();
    for (int i = 0; i < 32; ++i) {
        registerNames << QStringLiteral("R%1").arg(i);
    }
    for (int i = 0; i < 32; ++i) {
        registerNames << QStringLiteral("F%1").arg(i);
    }
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

void MainWindow::refreshUI() {
    ui->lblCycle->setText(tr("Ciclo: %1").arg(core.cycle));
    updateReservationStationsTable();
    updateROBTable();
    updateRegistersTable();
    updatePipelineScene();
    updateMetrics();
}

void MainWindow::updateReservationStationsTable() {
    int totalEntries = 0;
    for (const auto &name : stationOrder) {
        auto it = core.stationMap.find(name.toStdString());
        if (it != core.stationMap.end()) {
            totalEntries += static_cast<int>(it->second->getEntries().size());
        }
    }

    if (ui->tableRS->rowCount() != totalEntries) {
        ui->tableRS->setRowCount(totalEntries);
    }

    ui->tableRS->clearContents();

    auto createItem = [](const QString &text) {
        auto *item = new QTableWidgetItem(text);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        return item;
    };

    int row = 0;
    for (const auto &name : stationOrder) {
        auto it = core.stationMap.find(name.toStdString());
        if (it == core.stationMap.end()) {
            continue;
        }

        const auto &entries = it->second->getEntries();
        for (const auto &entry : entries) {
            if (row >= ui->tableRS->rowCount()) {
                ui->tableRS->insertRow(row);
            }

            QString stage = "idle";
            if (entry.busy) {
                if (entry.Qj != NO_PRODUCER || entry.Qk != NO_PRODUCER) stage = "issue";
                else if (entry.cyclesLeft > 0) stage = "execute";
                else stage = "writeback";
            }

            const auto formatValue = [](float value) {
                return QString::number(value, 'f', 2);
            };

            ui->tableRS->setItem(row, 0, createItem(QString::fromStdString(entry.name)));
            ui->tableRS->setItem(row, 1, createItem(entry.busy ? tr("Sim") : tr("Não")));

            auto *opItem = createItem(opcodeToString(entry.op));
            applyStageColor(opItem, stage);
            ui->tableRS->setItem(row, 2, opItem);

            ui->tableRS->setItem(row, 3, createItem(entry.busy ? formatValue(entry.Vj) : "-"));
            ui->tableRS->setItem(row, 4, createItem(entry.busy ? formatValue(entry.Vk) : "-"));
            ui->tableRS->setItem(row, 5, createItem(entry.Qj == NO_PRODUCER ? "-" : QString::number(entry.Qj)));
            ui->tableRS->setItem(row, 6, createItem(entry.Qk == NO_PRODUCER ? "-" : QString::number(entry.Qk)));
            ui->tableRS->setItem(row, 7, createItem(entry.robIndex >= 0 ? tr("ROB%1").arg(entry.robIndex) : "-"));
            ui->tableRS->setItem(row, 8, createItem(entry.busy ? QString::number(entry.cyclesLeft) : "-"));

            row++;
        }
    }
}

void MainWindow::updateROBTable() {
    const int robSize = core.rob.getSize();
    if (ui->tableROB->rowCount() != robSize) {
        ui->tableROB->setRowCount(robSize);
    }

    ui->tableROB->clearContents();

    auto createItem = [](const QString &text) {
        auto *item = new QTableWidgetItem(text);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        return item;
    };

    for (int i = 0; i < robSize; ++i) {
        const ROBEntry &entry = core.rob.getEntry(i);
        ui->tableROB->setItem(i, 0, createItem(QString::number(i)));
        ui->tableROB->setItem(i, 1, createItem(entry.busy ? tr("Sim") : tr("Não")));
        ui->tableROB->setItem(i, 2, createItem(opcodeToString(entry.op)));
        ui->tableROB->setItem(i, 3, createItem(QString::fromStdString(entry.destReg)));
        ui->tableROB->setItem(i, 4, createItem(entry.busy ? QString::number(entry.value, 'f', 2) : "-"));
        ui->tableROB->setItem(i, 5, createItem(robStateToString(entry.state)));
    }
}

void MainWindow::updateRegistersTable() {
    if (ui->tableRegs->rowCount() != registerNames.size()) {
        ui->tableRegs->setRowCount(registerNames.size());
    }

    ui->tableRegs->clearContents();

    auto createItem = [](const QString &text) {
        auto *item = new QTableWidgetItem(text);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        return item;
    };

    for (int row = 0; row < registerNames.size(); ++row) {
        const QString &regName = registerNames[row];
        const std::string regStd = regName.toStdString();
        float value = core.registers.getValue(regStd);
        int producer = core.registers.getProducer(regStd);

        ui->tableRegs->setItem(row, 0, createItem(regName));
        ui->tableRegs->setItem(row, 1, createItem(QString::number(value, 'f', 2)));
        ui->tableRegs->setItem(row, 2, createItem(producer == NO_PRODUCER ? "-" : QString::number(producer)));
    }
}

void MainWindow::updatePipelineScene() {
    pipelineScene->clear();

    int issueCount = 0;
    int writeBackCount = 0;
    int executingCount = 0;

    const int robSize = core.rob.getSize();
    for (int i = 0; i < robSize; ++i) {
        const ROBEntry &entry = core.rob.getEntry(i);
        if (!entry.busy) continue;
        if (entry.state == ROBState::ISSUE) issueCount++;
        else if (entry.state == ROBState::WRITE_BACK) writeBackCount++;
    }

    for (const auto &name : stationOrder) {
        auto it = core.stationMap.find(name.toStdString());
        if (it == core.stationMap.end()) continue;
        for (const auto &entry : it->second->getEntries()) {
            if (entry.busy && entry.Qj == NO_PRODUCER && entry.Qk == NO_PRODUCER && entry.cyclesLeft > 0) {
                executingCount++;
            }
        }
    }

    const QVector<QPair<QString, int>> stages = {
        {tr("Issue"), issueCount},
        {tr("Execute"), executingCount},
        {tr("Writeback"), writeBackCount},
        {tr("Commit"), core.instructionsCommitted}
    };

    const int rectWidth = 110;
    const int rectHeight = 60;
    const int spacing = 15;
    int x = 0;

    const QColor textColor = palette().color(QPalette::WindowText);

    for (const auto &stage : stages) {
        QRectF rect(x, 0, rectWidth, rectHeight);
        pipelineScene->addRect(rect, QPen(QColor("#90CAF9")), QBrush(QColor(0, 0, 0, 30)));
        auto *text = pipelineScene->addText(QString("%1\n%2").arg(stage.first).arg(stage.second));
        text->setDefaultTextColor(textColor);
        text->setPos(x + 10, 10);
        x += rectWidth + spacing;
    }
}

void MainWindow::updateMetrics() {
    const int cycles = core.cycle;
    const int commits = core.instructionsCommitted;
    const double ipc = (cycles > 0) ? static_cast<double>(commits) / static_cast<double>(cycles) : 0.0;

    ui->valIPC->setText(QString::number(ipc, 'f', 2));
    ui->valCiclos->setText(QString::number(cycles));
    if (ui->valCommits) ui->valCommits->setText(QString::number(commits));
    if (ui->valBubbles) ui->valBubbles->setText(QString::number(bubbleCycles));
}

QString MainWindow::opcodeToString(OpCode op) const {
    switch (op) {
        case OpCode::ADD: return "ADD";
        case OpCode::SUB: return "SUB";
        case OpCode::MUL: return "MUL";
        case OpCode::DIV: return "DIV";
        case OpCode::ADD_D: return "ADD.D";
        case OpCode::SUB_D: return "SUB.D";
        case OpCode::MUL_D: return "MUL.D";
        case OpCode::DIV_D: return "DIV.D";
        case OpCode::ADDI: return "ADDI";
        case OpCode::SUBI: return "SUBI";
        case OpCode::LW: return "LW";
        case OpCode::SW: return "SW";
        case OpCode::L_D: return "L.D";
        case OpCode::S_D: return "S.D";
        case OpCode::BEQ: return "BEQ";
        case OpCode::BNE: return "BNE";
        case OpCode::JAL: return "JAL";
        case OpCode::NOP: return "NOP";
        default: return "-";
    }
}

QString MainWindow::robStateToString(ROBState state) const {
    switch (state) {
        case ROBState::ISSUE: return tr("Issue");
        case ROBState::EXECUTE: return tr("Execute");
        case ROBState::WRITE_BACK: return tr("Writeback");
        case ROBState::COMMIT: return tr("Commit");
        default: return "-";
    }
}

bool MainWindow::loadProgramFromFile(const QString &filePath) {
    if (filePath.isEmpty()) return false;

    const QString absolutePath = QFileInfo(filePath).absoluteFilePath();
    const QString previousPath = loadedProgramPath;
    stopSimulation();
    core.reset();
    bubbleCycles = 0;

    if (!core.loadProgram(absolutePath.toStdString())) {
        QMessageBox::warning(this, tr("Erro"), tr("Não foi possível abrir %1").arg(filePath));
        if (!previousPath.isEmpty()) {
            core.reset();
            core.loadProgram(previousPath.toStdString());
            loadedProgramPath = previousPath;
        }
        refreshUI();
        return false;
    }

    loadedProgramPath = absolutePath;
    ui->txtLog->clear();
    appendLog(tr("Programa carregado (%1 instruções)").arg(core.instructionMemory.size()));
    refreshUI();
    updateStatusLabel(tr("Pronto"));
    return true;
}

void MainWindow::appendLog(const QString &message) {
    const QString line = QStringLiteral("[C%1] %2")
        .arg(core.cycle, 4, 10, QLatin1Char('0'))
        .arg(message);
    ui->txtLog->appendPlainText(line);
}

void MainWindow::stopSimulation() {
    if (simulationTimer->isActive()) {
        simulationTimer->stop();
    }
    isRunning = false;
}

void MainWindow::resetState(bool reloadProgram) {
    stopSimulation();
    bubbleCycles = 0;
    const QString path = (reloadProgram && !loadedProgramPath.isEmpty()) ? loadedProgramPath : QString();
    core.reset();
    if (!path.isEmpty()) {
        core.loadProgram(path.toStdString());
    }
    refreshUI();
}

void MainWindow::updateStatusLabel(const QString &statusText) {
    if (ui->valStatus) {
        ui->valStatus->setText(statusText);
    }
}

void MainWindow::performSimulationStep() {
    if (core.instructionMemory.empty()) {
        stopSimulation();
        QMessageBox::information(this, tr("Sem programa"), tr("Carregue um programa antes de executar."));
        return;
    }

    if (core.isFinished()) {
        stopSimulation();
        updateStatusLabel(tr("Finalizado"));
        return;
    }

    const int previousCycle = core.cycle;
    const int previousCommits = core.instructionsCommitted;
    const int headIndex = core.rob.getHeadIndex();
    const ROBEntry headSnapshot = core.rob.getEntry(headIndex);
    const bool headReady = headSnapshot.busy && headSnapshot.state == ROBState::WRITE_BACK;

    core.step();

    if (core.cycle > previousCycle && core.instructionsCommitted == previousCommits) {
        bubbleCycles++;
    }

    if (headReady && core.instructionsCommitted > previousCommits) {
        const QString target = QString::fromStdString(headSnapshot.destReg);
        const QString valueText = QString::number(headSnapshot.value, 'f', 2);
        if (!target.isEmpty()) {
            appendLog(tr("Commit #%1: %2 = %3")
                          .arg(core.instructionsCommitted)
                          .arg(target)
                          .arg(valueText));
        } else {
            appendLog(tr("Commit #%1 concluído").arg(core.instructionsCommitted));
        }
    }

    refreshUI();

    if (core.isFinished()) {
        stopSimulation();
        updateStatusLabel(tr("Finalizado"));
        appendLog(tr("Execução concluída."));
    }
}

// === MENU ACTIONS ===

void MainWindow::onActionAbrirTriggered() {
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Abrir programa"),
        QFileInfo(loadedProgramPath).absolutePath(),
        tr("Assembly (*.asm *.txt)"));

    if (!filePath.isEmpty()) {
        loadProgramFromFile(filePath);
    }
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
    refreshUI();
}

// === SIMULAÇÃO ===
void MainWindow::onRunClicked() {
    if (core.instructionMemory.empty()) {
        QMessageBox::information(this, tr("Sem programa"), tr("Carregue um programa antes de executar."));
        return;
    }

    if (isRunning) {
        return;
    }

    if (!simulationTimer->isActive()) {
        simulationTimer->start();
    }
    isRunning = true;
    updateStatusLabel(tr("Executando"));
}

void MainWindow::onPauseClicked() {
    stopSimulation();
    updateStatusLabel(tr("Pausado"));
}

void MainWindow::onStepClicked() {
    if (core.instructionMemory.empty()) {
        QMessageBox::information(this, tr("Sem programa"), tr("Carregue um programa antes de executar."));
        return;
    }

    performSimulationStep();
}

void MainWindow::onResetClicked() {
    resetState(!loadedProgramPath.isEmpty());
    updateStatusLabel(tr("Pronto"));
    appendLog(tr("Estado resetado."));
}