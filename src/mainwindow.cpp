#include "mainwindow.h"
#include "addproccesswidget.h"
#include "proccesslist.h"

MainWindow::MainWindow()
:   m_schedulerSettingsLayout(nullptr),
    m_fileMenu(nullptr),
    m_addProccessButton(nullptr),
    m_loadAction(nullptr),
    m_schedulingAlgorithmBox(nullptr),
    m_schedulingOptionsLayout(nullptr),
    m_schedulingRadioButtons(nullptr),
    m_quantumValueSelectorBox(nullptr),
    m_quantumValueSelector(nullptr),
    m_proccessList(nullptr),
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
    m_proccessList->AddProccess(m_numberOfProccesses);
}

void MainWindow::ActivateOrDeactivateRunButton(int numberOfProccesses)
{
    // For the run button to be active, there must be
    // at least one proccess on the list.
    // The simulation can't run otherwise.
    if(numberOfProccesses > 0)
    {
        m_runButton->setEnabled(true);
    }
    else
    {
        m_numberOfProccesses = 0;
        m_runButton->setEnabled(false);
    }
}

void MainWindow::Load()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select file");
}

void MainWindow::CreateButtons()
{
    m_addProccessButton = new QPushButton(tr("+\nAdd Process"), this);
    m_addProccessButton->setFixedSize(QSize(100, 50));
    m_addProccessButton->setStatusTip(tr("Add a new process."));

    m_runButton = new QPushButton(tr(">\nRun"), this);
    m_runButton->setFixedSize(QSize(200, 80));
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

    m_schedulingAlgorithmBox->setFixedSize(300, 60);
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
    m_quantumValueSelectorBox->setFixedSize(300, 60);
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
    m_osSettingsGroup = new QGroupBox(tr("Scheduler Settings"), this);
    m_schedulerSettingsLayout = new QBoxLayout(QBoxLayout::LeftToRight, m_osSettingsGroup);
    m_schedulerSettingsLayout->insertWidget(0, m_schedulingAlgorithmBox);
    m_schedulerSettingsLayout->insertWidget(1, m_quantumValueSelectorBox);
    m_osSettingsGroup->setLayout(m_schedulerSettingsLayout);

    m_prccessListGroup = new QWidget(m_osSettingsGroup);
    m_proccessList = new ProccessList(this);
    m_mainGrid = new QVBoxLayout();
    m_mainGrid->addWidget(m_addProccessButton, 0, Qt::AlignCenter | Qt::AlignTop);
    m_mainGrid->addWidget(m_proccessList);
    m_prccessListGroup->setLayout(m_mainGrid);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_osSettingsGroup);
    m_mainLayout->addWidget(m_prccessListGroup);
    m_mainLayout->addWidget(m_runButton, 0, Qt::AlignCenter);

    m_mainWidget = new QWidget(this);
    m_mainWidget->setLayout(m_mainLayout);

    setCentralWidget(m_mainWidget);    
}

void MainWindow::ConnectWidgets()
{
    connect(m_addProccessButton, &QPushButton::clicked, this, &MainWindow::AddProccessBox);
    connect(m_proccessList, &ProccessList::NumberOfProccessesChanged, 
        this, &MainWindow::ActivateOrDeactivateRunButton);
}