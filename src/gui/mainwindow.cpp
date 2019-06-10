#include "mainwindow.h"
#include "proccessbox.h"
#include "proccesslistloader.h"
#include "visualizationwindow.h"
#include <memory>
#include <string>

MainWindow::MainWindow()
:   QMainWindow(),
    m_mainWidget(nullptr),
    m_mainLayout(nullptr),
    m_fileMenu(nullptr),
    m_loadAction(nullptr),
    m_schedulerSettingsGroup(nullptr),
    m_schedulerSettingsLayout(nullptr),
    m_schedulingAlgorithmBox(nullptr),
    m_sheculingAlgorithmBoxLayout(nullptr),
    m_schedulingRadioButtons(nullptr),
    m_quantumValueSelectorBox(nullptr),
    m_quantumValueSelector(nullptr),
    m_overheadValueSelectorBox(nullptr),
    m_overheadValueSelector(nullptr),
    m_proccessListGroup(nullptr),
    m_proccessGroupLayout(nullptr),
    m_proccessList(nullptr),
    m_addProccessButton(nullptr),
    m_runButton(nullptr),
    m_visualizationWindow(nullptr),
    m_numberOfProccesses(0)
{
    setWindowTitle(tr("OS Scheduling Simulator"));

    CreateButtons();
    CreateMenuBar();
    CreateStatusBar();
    CreateSchedulingAlgorithmSelectionBox();
    CreateQuantumValueSelectionBox();
    CreateOverheadValueSelectionBox();
    CreatePRASelectionBox();
    CreateMainLayout();
    ConnectWidgets();

    statusBar()->setSizeGripEnabled(false);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle("OS Scheduling Simulator");
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
        QPalette pal = m_runButton->palette();
        pal.setColor(QPalette::Button, QColor(Qt::green));
        m_runButton->setAutoFillBackground(true);
        m_runButton->setPalette(pal);
        m_runButton->update();
        m_runButton->setEnabled(true);
        m_runButton->setStatusTip(tr("Run simulation using the current confiuration."));
    }
    else
    {
        QPalette pal = m_runButton->palette();
        pal.setColor(QPalette::Button, QColor(Qt::green));
        m_runButton->setAutoFillBackground(true);
        m_runButton->setPalette(pal);
        m_runButton->update();
        m_numberOfProccesses = 0;
        m_runButton->setEnabled(false);
        m_runButton->setStatusTip(tr("Unable to run simulation: At least one proccess is needed."));
    }
}

void MainWindow::Load()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select file"), QString(), tr("Proccess List Files (*.plf)"));
	if(filename == "") return;

	std::string filenameString = filename.toUtf8().constData();
	auto proccessList = ProccessListLoader::LoadProccessList(filenameString);

	if(proccessList.size() > 0)
	{
		m_proccessList->ClearProccesses();
		m_proccessList->AddProccessList(proccessList);
	}
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid proccess list file."), QMessageBox::StandardButton::Ok);
    }
}

void MainWindow::Save()
{
    QString filename = QFileDialog::getSaveFileName(
        this, 
        tr("Save Proccess List"), 
        QString(),
        tr("Proccess List File (*.plf)"));

    // If the users cancelled the action
    if(filename == "") return;

    std::string filenameString = filename.toUtf8().constData();
	std::string extension = ".plf";

	// LINUX might not add the .plf extension to the filename
	// automatically. The verification below will concatenate
	// .plf to the filename if necessary
	if(filenameString.compare(
		filenameString.length() - extension.length(), extension.length(),
		extension) != 0)
	{
		filenameString += extension; 
	}
  	
    std::vector<ProccessTemplate> proccessList = m_proccessList->GetCurrentProccesses();
    ProccessListLoader::SaveProccessList(filenameString, proccessList);
}

void MainWindow::CreateButtons()
{
    m_addProccessButton = new QPushButton(tr("+\nAdd Process"), this);
    m_addProccessButton->setFixedSize(QSize(100, 50));
    m_addProccessButton->setStatusTip(tr("Add a new process."));

    m_runButton = new QPushButton(tr(">\nRun"), this);
    m_runButton->setStatusTip(tr("Unable to run simulation: At least one proccess is needed."));
    m_runButton->setFixedSize(QSize(200, 80));

    QPalette pal = m_runButton->palette();
    pal.setColor(QPalette::Button, QColor(Qt::gray));
    m_runButton->setAutoFillBackground(true);
    m_runButton->setPalette(pal);
    m_runButton->update();


    // Start button as disabled, because
    // at least one proccess is needed to
    // run the simulation.
    m_runButton->setEnabled(false);
}

void MainWindow::CreateSchedulingAlgorithmSelectionBox()
{
    m_schedulingAlgorithmBox = new QGroupBox(tr("Scheduling algorithm"), this);
    m_sheculingAlgorithmBoxLayout = new QHBoxLayout(m_schedulingAlgorithmBox);
    // Radio buttons
    QRadioButton* fifoButton = new QRadioButton(tr("FIFO"), m_schedulingAlgorithmBox);
    QRadioButton* sjfButton = new QRadioButton(tr("SJF"), m_schedulingAlgorithmBox);
    QRadioButton* roundRobinButton = new QRadioButton(tr("Round Robin"), m_schedulingAlgorithmBox);
    QRadioButton* edfButton = new QRadioButton(tr("EDF"), m_schedulingAlgorithmBox);

    m_sheculingAlgorithmBoxLayout->insertWidget(0, fifoButton);
    m_sheculingAlgorithmBoxLayout->insertWidget(1, sjfButton);
    m_sheculingAlgorithmBoxLayout->insertWidget(2, roundRobinButton);
    m_sheculingAlgorithmBoxLayout->insertWidget(3, edfButton);

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
    QLabel* quantumLabel = new QLabel(tr("Duration"), m_quantumValueSelectorBox);
    m_quantumValueSelector = new QSpinBox(m_quantumValueSelectorBox);
    m_quantumValueSelector->setStatusTip(tr("Select the duration of the quantum."));
    m_quantumValueSelector->setMinimum(1);
    quantumSelectorGrid->addWidget(quantumLabel, 0, 0);
    quantumSelectorGrid->addWidget(m_quantumValueSelector, 0, 1);
    
    m_quantumValueSelectorBox->setLayout(quantumSelectorGrid);
    m_quantumValueSelectorBox->setFixedSize(120, 60);
}

void MainWindow::CreateOverheadValueSelectionBox()
{
    m_overheadValueSelectorBox = new QGroupBox(tr("Overhead"), this);

    QGridLayout* overheadSelectorGrid = new QGridLayout(m_overheadValueSelectorBox);
    QLabel* overheadLabel = new QLabel(tr("Value"), m_overheadValueSelectorBox);
    m_overheadValueSelector = new QSpinBox(m_overheadValueSelectorBox);
    m_overheadValueSelector->setStatusTip(tr("Select the duration of the scheduler overhead."));
    m_overheadValueSelector->setMinimum(1);

    overheadSelectorGrid->addWidget(overheadLabel, 0, 0);
    overheadSelectorGrid->addWidget(m_overheadValueSelector, 0, 1);

    m_overheadValueSelectorBox->setLayout(overheadSelectorGrid);
    m_overheadValueSelectorBox->setFixedSize(100, 60);
}

void MainWindow::CreatePRASelectionBox()
{
    m_PRASelectorBox = new QGroupBox(tr("Page Replacement Algorithm"), this);

    QRadioButton* fifoButton = new QRadioButton(tr("FIFO"));
    QRadioButton* lruButton = new QRadioButton(tr("LRU"));

    QHBoxLayout* layout = new QHBoxLayout(m_PRASelectorBox);
    layout->addWidget(fifoButton);
    layout->addWidget(lruButton);

    fifoButton->setChecked(true);

    m_PRARadioButtons = new QButtonGroup(m_PRASelectorBox);
    m_PRARadioButtons->addButton(fifoButton);
    m_PRARadioButtons->addButton(lruButton);

    m_PRASelectorBox->setLayout(layout);
}

void MainWindow::CreateMenuBar()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    
    m_loadAction = new QAction(tr("Load"), this);
    m_loadAction->setShortcut(QKeySequence::Open);
    m_loadAction->setStatusTip(tr("Load proccess configuration from file."));
    connect(m_loadAction, &QAction::triggered, this, &MainWindow::Load);
    m_fileMenu->addAction(m_loadAction);

    m_saveAction = new QAction(tr("Save"), this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setStatusTip(tr("Save proccess configuration to a file."));
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::Save);
    m_fileMenu->addAction(m_saveAction);
}

void MainWindow::CreateStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::CreateMainLayout()
{
    m_schedulerSettingsGroup = new QGroupBox(tr("Scheduler Settings"), this);
    m_schedulerSettingsLayout = new QBoxLayout(QBoxLayout::LeftToRight, m_schedulerSettingsGroup);
    m_schedulerSettingsLayout->insertWidget(0, m_schedulingAlgorithmBox);
    m_schedulerSettingsLayout->insertWidget(1, m_quantumValueSelectorBox);
    m_schedulerSettingsLayout->insertWidget(2, m_overheadValueSelectorBox);
    m_schedulerSettingsGroup->setLayout(m_schedulerSettingsLayout);

    m_proccessListGroup = new QWidget(m_schedulerSettingsGroup);
    m_proccessList = new ProccessList(this);
    m_proccessGroupLayout = new QVBoxLayout(m_proccessListGroup);
    m_proccessGroupLayout->addWidget(m_addProccessButton, 0, Qt::AlignCenter | Qt::AlignTop);
    m_proccessGroupLayout->addSpacing(5);
    m_proccessGroupLayout->addWidget(m_proccessList);
    m_proccessListGroup->setLayout(m_proccessGroupLayout);

    m_mainLayout = new QVBoxLayout(m_mainWidget);
    m_mainLayout->addWidget(m_schedulerSettingsGroup);
    m_mainLayout->addWidget(m_PRASelectorBox);
    m_mainLayout->addSpacing(10);
    m_mainLayout->addWidget(m_proccessListGroup);
    m_mainLayout->addSpacing(30);
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
    connect(m_runButton, &QPushButton::clicked, this, &MainWindow::RunSimulation);
}

void MainWindow::RunSimulation()
{

    std::vector<ProccessTemplate> proccesses = m_proccessList->GetCurrentProccesses();
    
#if FALSE
    OperatingSystem os = OperatingSystem(m_quantumValueSelector->value(),
                                         m_overheadValueSelector->value(),
                                         GetSchedulingAlgorithm());
    for(auto pTemplate = proccesses.begin();
        pTemplate != proccesses.end();
        pTemplate++)
    {
        os.AddProccess(
            pTemplate->ID, 
            pTemplate->arrivalTime, 
            pTemplate->duration, 
            pTemplate->deadline);
    }

    // Run all steps
    while(os.NextStep());

    float averageTurnaround = os.GetAverageTurnaround();
    unsigned executionTime = os.GetExecutionTime();

    QMessageBox::information(this, tr("Simulation Result"),
        tr("Execution time: %1 t.u.\nAverage turnaround: %2 t.u.").arg(
            executionTime).arg(averageTurnaround),
            QMessageBox::StandardButton::Ok);

#endif

    if(m_visualizationWindow != nullptr)
    {
        m_visualizationWindow->hide();
        delete m_visualizationWindow;
    }

    m_visualizationWindow = new VisualizationWindow(
        m_quantumValueSelector->value(),
        m_overheadValueSelector->value(),
        GetSchedulingAlgorithm(),
        GetPageReplacementAlgorithm(),
        proccesses,
        this);
    m_visualizationWindow->show();
}

SchedulingAlgorithm MainWindow::GetSchedulingAlgorithm()
{
    QRadioButton* selectedButton = 
        static_cast<QRadioButton*>(m_schedulingRadioButtons->checkedButton());    

    SchedulingAlgorithm selectedAlgorithm;

    QString buttonText = selectedButton->text();

    if(buttonText == "FIFO") selectedAlgorithm = SchedulingAlgorithm::FIFO;
    else if(buttonText == "SJF") selectedAlgorithm = SchedulingAlgorithm::SJF;
    else if(buttonText == "Round Robin") selectedAlgorithm = SchedulingAlgorithm::ROUND_ROBIN;
    else selectedAlgorithm = SchedulingAlgorithm::EDF;

    return selectedAlgorithm;
}

PageReplacementAlgorithm MainWindow::GetPageReplacementAlgorithm() const
{
    QRadioButton* selectedButton =
        static_cast<QRadioButton*>(m_PRARadioButtons->checkedButton());

    QString buttonText = selectedButton->text();

    if(buttonText == "FIFO") return PageReplacementAlgorithm::FIFO;
    else return PageReplacementAlgorithm::LRU;
}