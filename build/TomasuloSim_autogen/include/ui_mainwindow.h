/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbrir;
    QAction *actionSair;
    QAction *actionSobre;
    QWidget *centralwidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *controlBar;
    QPushButton *btnRun;
    QPushButton *btnPause;
    QPushButton *btnStep;
    QPushButton *btnReset;
    QSpacerItem *spacer1;
    QLabel *lblCycle;
    QWidget *metricsContainer;
    QVBoxLayout *metricsContainerLayout;
    QLabel *lblMetricsTitle;
    QGroupBox *metricsBox;
    QGridLayout *gridLayout;
    QLabel *lblIPC;
    QLabel *valIPC;
    QLabel *lblCiclos;
    QLabel *valCiclos;
    QLabel *lblCommits;
    QLabel *valCommits;
    QLabel *lblBubbles;
    QLabel *valBubbles;
    QLabel *lblStatus;
    QLabel *valStatus;
    QSplitter *splitter;
    QTabWidget *tabWidget;
    QWidget *tabPipeline;
    QVBoxLayout *vboxLayout;
    QGraphicsView *graphicsPipeline;
    QWidget *tabRS;
    QVBoxLayout *vboxLayout1;
    QTableWidget *tableRS;
    QWidget *tabROB;
    QVBoxLayout *vboxLayout2;
    QTableWidget *tableROB;
    QWidget *tabRegs;
    QVBoxLayout *vboxLayout3;
    QTableWidget *tableRegs;
    QWidget *tabLogs;
    QVBoxLayout *vboxLayout4;
    QPlainTextEdit *txtLog;
    QWidget *tabSettings;
    QVBoxLayout *layoutSettings;
    QGroupBox *groupTheme;
    QHBoxLayout *hboxLayout;
    QRadioButton *radioDark;
    QRadioButton *radioLight;
    QSpacerItem *spacerFuture;
    QMenuBar *menubar;
    QMenu *menuArquivo;
    QMenu *menuExibir;
    QMenu *menuAjuda;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/cpu_icon.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MainWindow->setWindowIcon(icon);
        actionAbrir = new QAction(MainWindow);
        actionAbrir->setObjectName("actionAbrir");
        actionSair = new QAction(MainWindow);
        actionSair->setObjectName("actionSair");
        actionSobre = new QAction(MainWindow);
        actionSobre->setObjectName("actionSobre");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QVBoxLayout(centralwidget);
        mainLayout->setObjectName("mainLayout");
        controlBar = new QHBoxLayout();
        controlBar->setObjectName("controlBar");
        btnRun = new QPushButton(centralwidget);
        btnRun->setObjectName("btnRun");

        controlBar->addWidget(btnRun);

        btnPause = new QPushButton(centralwidget);
        btnPause->setObjectName("btnPause");

        controlBar->addWidget(btnPause);

        btnStep = new QPushButton(centralwidget);
        btnStep->setObjectName("btnStep");

        controlBar->addWidget(btnStep);

        btnReset = new QPushButton(centralwidget);
        btnReset->setObjectName("btnReset");

        controlBar->addWidget(btnReset);

        spacer1 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        controlBar->addItem(spacer1);

        lblCycle = new QLabel(centralwidget);
        lblCycle->setObjectName("lblCycle");

        controlBar->addWidget(lblCycle);


        mainLayout->addLayout(controlBar);

        metricsContainer = new QWidget(centralwidget);
        metricsContainer->setObjectName("metricsContainer");
        metricsContainerLayout = new QVBoxLayout(metricsContainer);
        metricsContainerLayout->setObjectName("metricsContainerLayout");
        metricsContainerLayout->setContentsMargins(0, 0, 0, 0);
        lblMetricsTitle = new QLabel(metricsContainer);
        lblMetricsTitle->setObjectName("lblMetricsTitle");
        lblMetricsTitle->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        lblMetricsTitle->setFont(font);

        metricsContainerLayout->addWidget(lblMetricsTitle);

        metricsBox = new QGroupBox(metricsContainer);
        metricsBox->setObjectName("metricsBox");
        metricsBox->setMinimumSize(QSize(320, 0));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(metricsBox->sizePolicy().hasHeightForWidth());
        metricsBox->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(metricsBox);
        gridLayout->setObjectName("gridLayout");
        lblIPC = new QLabel(metricsBox);
        lblIPC->setObjectName("lblIPC");

        gridLayout->addWidget(lblIPC, 0, 0, 1, 1);

        valIPC = new QLabel(metricsBox);
        valIPC->setObjectName("valIPC");
        QFont font1;
        font1.setBold(true);
        valIPC->setFont(font1);

        gridLayout->addWidget(valIPC, 0, 1, 1, 1);

        lblCiclos = new QLabel(metricsBox);
        lblCiclos->setObjectName("lblCiclos");

        gridLayout->addWidget(lblCiclos, 1, 0, 1, 1);

        valCiclos = new QLabel(metricsBox);
        valCiclos->setObjectName("valCiclos");
        valCiclos->setFont(font1);

        gridLayout->addWidget(valCiclos, 1, 1, 1, 1);

        lblCommits = new QLabel(metricsBox);
        lblCommits->setObjectName("lblCommits");

        gridLayout->addWidget(lblCommits, 2, 0, 1, 1);

        valCommits = new QLabel(metricsBox);
        valCommits->setObjectName("valCommits");
        valCommits->setFont(font1);

        gridLayout->addWidget(valCommits, 2, 1, 1, 1);

        lblBubbles = new QLabel(metricsBox);
        lblBubbles->setObjectName("lblBubbles");

        gridLayout->addWidget(lblBubbles, 3, 0, 1, 1);

        valBubbles = new QLabel(metricsBox);
        valBubbles->setObjectName("valBubbles");
        valBubbles->setFont(font1);

        gridLayout->addWidget(valBubbles, 3, 1, 1, 1);

        lblStatus = new QLabel(metricsBox);
        lblStatus->setObjectName("lblStatus");

        gridLayout->addWidget(lblStatus, 4, 0, 1, 1);

        valStatus = new QLabel(metricsBox);
        valStatus->setObjectName("valStatus");
        valStatus->setFont(font1);

        gridLayout->addWidget(valStatus, 4, 1, 1, 1);


        metricsContainerLayout->addWidget(metricsBox);


        mainLayout->addWidget(metricsContainer);

        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Horizontal);
        tabWidget = new QTabWidget(splitter);
        tabWidget->setObjectName("tabWidget");
        tabPipeline = new QWidget();
        tabPipeline->setObjectName("tabPipeline");
        vboxLayout = new QVBoxLayout(tabPipeline);
        vboxLayout->setObjectName("vboxLayout");
        graphicsPipeline = new QGraphicsView(tabPipeline);
        graphicsPipeline->setObjectName("graphicsPipeline");

        vboxLayout->addWidget(graphicsPipeline);

        tabWidget->addTab(tabPipeline, QString());
        tabRS = new QWidget();
        tabRS->setObjectName("tabRS");
        vboxLayout1 = new QVBoxLayout(tabRS);
        vboxLayout1->setObjectName("vboxLayout1");
        tableRS = new QTableWidget(tabRS);
        tableRS->setObjectName("tableRS");

        vboxLayout1->addWidget(tableRS);

        tabWidget->addTab(tabRS, QString());
        tabROB = new QWidget();
        tabROB->setObjectName("tabROB");
        vboxLayout2 = new QVBoxLayout(tabROB);
        vboxLayout2->setObjectName("vboxLayout2");
        tableROB = new QTableWidget(tabROB);
        tableROB->setObjectName("tableROB");

        vboxLayout2->addWidget(tableROB);

        tabWidget->addTab(tabROB, QString());
        tabRegs = new QWidget();
        tabRegs->setObjectName("tabRegs");
        vboxLayout3 = new QVBoxLayout(tabRegs);
        vboxLayout3->setObjectName("vboxLayout3");
        tableRegs = new QTableWidget(tabRegs);
        tableRegs->setObjectName("tableRegs");

        vboxLayout3->addWidget(tableRegs);

        tabWidget->addTab(tabRegs, QString());
        tabLogs = new QWidget();
        tabLogs->setObjectName("tabLogs");
        vboxLayout4 = new QVBoxLayout(tabLogs);
        vboxLayout4->setObjectName("vboxLayout4");
        txtLog = new QPlainTextEdit(tabLogs);
        txtLog->setObjectName("txtLog");
        txtLog->setReadOnly(true);

        vboxLayout4->addWidget(txtLog);

        tabWidget->addTab(tabLogs, QString());
        tabSettings = new QWidget();
        tabSettings->setObjectName("tabSettings");
        layoutSettings = new QVBoxLayout(tabSettings);
        layoutSettings->setObjectName("layoutSettings");
        groupTheme = new QGroupBox(tabSettings);
        groupTheme->setObjectName("groupTheme");
        hboxLayout = new QHBoxLayout(groupTheme);
        hboxLayout->setObjectName("hboxLayout");
        radioDark = new QRadioButton(groupTheme);
        radioDark->setObjectName("radioDark");
        radioDark->setChecked(true);

        hboxLayout->addWidget(radioDark);

        radioLight = new QRadioButton(groupTheme);
        radioLight->setObjectName("radioLight");

        hboxLayout->addWidget(radioLight);


        layoutSettings->addWidget(groupTheme);

        spacerFuture = new QSpacerItem(20, 300, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        layoutSettings->addItem(spacerFuture);

        tabWidget->addTab(tabSettings, QString());
        splitter->addWidget(tabWidget);

        mainLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menuArquivo = new QMenu(menubar);
        menuArquivo->setObjectName("menuArquivo");
        menuExibir = new QMenu(menubar);
        menuExibir->setObjectName("menuExibir");
        menuAjuda = new QMenu(menubar);
        menuAjuda->setObjectName("menuAjuda");
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuArquivo->menuAction());
        menubar->addAction(menuExibir->menuAction());
        menubar->addAction(menuAjuda->menuAction());
        menuArquivo->addAction(actionAbrir);
        menuArquivo->addAction(actionSair);
        menuAjuda->addAction(actionSobre);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Tomasulo Simulator", nullptr));
        actionAbrir->setText(QCoreApplication::translate("MainWindow", "Abrir programa...", nullptr));
        actionSair->setText(QCoreApplication::translate("MainWindow", "Sair", nullptr));
        actionSobre->setText(QCoreApplication::translate("MainWindow", "Sobre o simulador", nullptr));
        btnRun->setText(QCoreApplication::translate("MainWindow", "Executar", nullptr));
        btnPause->setText(QCoreApplication::translate("MainWindow", "Pausar", nullptr));
        btnStep->setText(QCoreApplication::translate("MainWindow", "Passo", nullptr));
        btnReset->setText(QCoreApplication::translate("MainWindow", "Resetar", nullptr));
        lblCycle->setText(QCoreApplication::translate("MainWindow", "Ciclo: 0", nullptr));
        lblMetricsTitle->setText(QCoreApplication::translate("MainWindow", "M\303\251tricas de desempenho", nullptr));
        metricsBox->setTitle(QString());
        lblIPC->setText(QCoreApplication::translate("MainWindow", "IPC m\303\251dio:", nullptr));
        valIPC->setText(QCoreApplication::translate("MainWindow", "0.00", nullptr));
        lblCiclos->setText(QCoreApplication::translate("MainWindow", "Ciclos:", nullptr));
        valCiclos->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblCommits->setText(QCoreApplication::translate("MainWindow", "Instru\303\247\303\265es confirmadas:", nullptr));
        valCommits->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblBubbles->setText(QCoreApplication::translate("MainWindow", "Ciclos de bolha:", nullptr));
        valBubbles->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblStatus->setText(QCoreApplication::translate("MainWindow", "Estado:", nullptr));
        valStatus->setText(QCoreApplication::translate("MainWindow", "Parado", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabPipeline), QCoreApplication::translate("MainWindow", "Pipeline", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabRS), QCoreApplication::translate("MainWindow", "Esta\303\247\303\265es de Reserva", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabROB), QCoreApplication::translate("MainWindow", "Reorder Buffer", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabRegs), QCoreApplication::translate("MainWindow", "Registradores", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabLogs), QCoreApplication::translate("MainWindow", "Logs", nullptr));
        groupTheme->setTitle(QCoreApplication::translate("MainWindow", "Tema", nullptr));
        radioDark->setText(QCoreApplication::translate("MainWindow", "Escuro", nullptr));
        radioLight->setText(QCoreApplication::translate("MainWindow", "Claro", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabSettings), QCoreApplication::translate("MainWindow", "Prefer\303\252ncias", nullptr));
        menuArquivo->setTitle(QCoreApplication::translate("MainWindow", "Arquivo", nullptr));
        menuExibir->setTitle(QCoreApplication::translate("MainWindow", "Exibir", nullptr));
        menuAjuda->setTitle(QCoreApplication::translate("MainWindow", "Ajuda", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
