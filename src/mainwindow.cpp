#include "mainwindow.h"
#include "addproccesswidget.h"

MainWindow::MainWindow()
:   m_mainWidget(nullptr),
    m_mainLayout(nullptr),
    m_fileMenu(nullptr),
    m_addProccessButon(nullptr),
    m_loadAction(nullptr),
    m_schedulingAlgorithmBox(nullptr),
    m_schedulingOptionsLayout(nullptr),
    m_schedulingRadioButtons(nullptr),
    m_quantumValueSelectorBox(nullptr),
    m_quantumValueSelector(nullptr),
    m_runButton(nullptr),
    m_numberOfProccesses(0)
{
    setWindowTitle(tr("OS Scheduling Simulator"));

    CreateButtons();
    CreateMenuBar();
    CreateStatusBar();
    CreateSchedulingAlgorithmSelectionBox();
    CreateQuantumValueSelectionBox();
    CreateMainLayout();
    ConnectWidgets();

    statusBar()->setSizeGripEnabled(false);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
}

void MainWindow::AddProccessBox()
{
    m_numberOfProccesses++;
    QGroupBox* addProccessWidget = new AddProccessWidget(m_numberOfProccesses, this);
    m_mainLayout->insertWidget(m_numberOfProccesses, addProccessWidget);
}

void MainWindow::Load()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select file");
}

void MainWindow::CreateButtons()
{
    m_addProccessButon = new QPushButton(tr("+\nAdd Process"), this);
    m_addProccessButon->setFixedSize(QSize(70, 40));
    m_addProccessButon->setStatusTip(tr("Add a new process."));

    m_runButton = new QPushButton(tr(">\nRun"), this);
    m_runButton->setFixedSize(QSize(80, 40));
    m_runButton->setStatusTip(tr("Run simulation using the current confiuration."));

    QPalette pal = m_runButton->palette();
    pal.setColor(QPalette::Button, QColor(Qt::green));
    m_runButton->setAutoFillBackground(true);
    m_runButton->setPalette(pal);
    m_runButton->update();

    // Start button as disabled, because
    // at least one proccess is needed to
    // run the simulation.
    m_runButton->setEnabled(false);
}

    #include <iostream>
void MainWindow::CreateSchedulingAlgorithmSelectionBox()
{
    m_schedulingAlgorithmBox = new QGroupBox(tr("Scheduling algorithm"), this);
    m_schedulingOptionsLayout = new QHBoxLayout(m_schedulingAlgorithmBox);
    // Radio buttons
    QRadioButton* fifoButton = new QRadioButton(tr("FIFO"), m_schedulingAlgorithmBox);
    QRadioButton* sjfButton = new QRadioButton(tr("SJF"), m_schedulingAlgorithmBox);
    QRadioButton* roundRobinButton = new QRadioButton(tr("Round Robin"), m_schedulingAlgorithmBox);
    QRadioButton* edfButton = new QRadioButton(tr("EDF"), m_schedulingAlgorithmBox);

    m_schedulingOptionsLayout->insertWidget(0, fifoButton);
    m_schedulingOptionsLayout->insertWidget(1, sjfButton);
    m_schedulingOptionsLayout->insertWidget(2, roundRobinButton);
    m_schedulingOptionsLayout->insertWidget(3, edfButton);

    // Set FIFO option checked by default
    fifoButton->setChecked(true);

    fifoButton->setStatusTip("Use \"First In First Out\" scheduling algorithm");
    sjfButton->setStatusTip("Use \"Shortest Job First\" scheduling algorithm");
    roundRobinButton->setStatusTip("Use \"Round Robin\" scheduling algorithm");
    edfButton->setStatusTip("Use \"Earliest Deadline First\" scheduling algorithm");

    // Add radio buttons to QButtonGroup for easier checking
    m_schedulingRadioButtons = new QButtonGroup(m_schedulingAlgorithmBox);
    m_schedulingRadioButtons->addButton(fifoButton);
    m_schedulingRadioButtons->addButton(sjfButton);
    m_schedulingRadioButtons->addButton(roundRobinButton);
    m_schedulingRadioButtons->addButton(edfButton);
}

void MainWindow::CreateQuantumValueSelectionBox()
{
    m_quantumValueSelectorBox = new QGroupBox(tr("Quantum"), this);

    QGridLayout* quantumSelectorGrid = new QGridLayout(m_quantumValueSelectorBox);
    QLabel* quantumSelector = new QLabel(tr("Duration"), m_quantumValueSelectorBox);
    m_quantumValueSelector = new QSpinBox(m_quantumValueSelectorBox);
    m_quantumValueSelector->setMinimum(1);
    quantumSelectorGrid->addWidget(quantumSelector, 0, 0);
    quantumSelectorGrid->addWidget(m_quantumValueSelector, 0, 1);
    
    m_quantumValueSelectorBox->setLayout(quantumSelectorGrid);
}


void MainWindow::CreateMenuBar()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_loadAction = new QAction(tr("Load"), this);
    connect(m_loadAction, &QAction::triggered, this, &MainWindow::Load);

    m_fileMenu->addAction(m_loadAction);
}

void MainWindow::CreateStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::CreateMainLayout()
{
    m_mainWidget = new QWidget(this);
    m_mainLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    m_mainLayout->insertWidget(0, m_schedulingAlgorithmBox);
    m_mainLayout->insertWidget(1, m_quantumValueSelectorBox);
    m_mainLayout->insertSpacing(2, 10);
    m_mainLayout->insertWidget(3, m_addProccessButon,  0, Qt::AlignCenter | Qt::AlignTop);
    m_mainLayout->insertSpacing(4, 30);
    m_mainLayout->insertWidget(5, m_runButton, 0, Qt::AlignCenter | Qt::AlignTop);
    m_mainLayout->insertSpacing(6, 10);

    // QTableWidget* tw = new QTableWidget(3, 1, this);
    // QTableWidgetItem* pa = new QTableWidgetItem(tr("%1").arg(10));
    // QTableWidgetItem* pd = new QTableWidgetItem(tr("%1").arg(3));
    // QTableWidgetItem* pde = new QTableWidgetItem(tr("%1").arg(20));
    // tw->setItem(0, 0, pa); 
    // tw->setItem(1, 0, pd); 
    // tw->setItem(2, 0, pde); 
    // tw->setHorizontalHeaderItem(0, new QTableWidgetItem("Proccess 1"));
    // tw->setVerticalHeaderItem(0, new QTableWidgetItem("Arrival"));
    // tw->setVerticalHeaderItem(1, new QTableWidgetItem("Duration"));
    // tw->setVerticalHeaderItem(2, new QTableWidgetItem("Deadline"));

    // m_mainLayout->insertWidget(1, tw);
    // for(int i = 0; i < 50; i++)
    // {
    //     for(int j = 0; j < 1; j++)
    //     {
    //         QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(i + j));
    //         item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    //         tw->setItem(i, j, item);
    //     }
    // }
    m_mainWidget->setLayout(m_mainLayout);
    
    // QPropertyAnimation* animation = new QPropertyAnimation(m_addProccessButon, "geometry");
    // animation->setDuration(1000);
    // animation->setStartValue(m_addProccessButon->geometry());
    // QRect rc = m_addProccessButon->geometry();
    // rc.setHeight( rc.height() + 50);
    // animation->setEndValue(rc);
    // animation->setEasingCurve(QEasingCurve::InQuad);
    // animation->start();

    setCentralWidget(m_mainWidget);    
}

void MainWindow::ConnectWidgets()
{
    connect(m_addProccessButon, &QPushButton::clicked, this, &MainWindow::AddProccessBox);
}