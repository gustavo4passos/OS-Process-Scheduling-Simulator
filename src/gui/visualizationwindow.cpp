#include "visualizationwindow.h"
#include "Rect.hpp"

#include <QtWidgets/QtWidgets>

VisualizationWindow::VisualizationWindow(
    unsigned quantum,
    unsigned overhead,
    SchedulingAlgorithm schedulingAlgorithm,
    PageReplacementAlgorithm pra,
    const std::vector<ProccessTemplate>& proccessTemplates,
    QWidget* parent)
:   QMainWindow(parent),
    m_mainWidget(nullptr),
    m_mainLayout(nullptr),
    m_timeline(nullptr),
    m_RAMView(nullptr),
    m_scene(nullptr),
    m_time(0)
{
    resize(960, 544);

    m_os = new OperatingSystem(quantum, overhead, schedulingAlgorithm, pra);
    
    for(auto& p : proccessTemplates)
    {
        m_os->AddProccess(p.ID,
                          p.arrivalTime,
                          p.duration,
                          p.deadline,
                          p.numberOfPages);
    }

    CreateTimeline();
    CreateLeftsideLayout();
    CreateMainLayout();

    setCentralWidget(m_mainWidget);

    m_playTimer = new QTimer(this);
    
    ConnectWidgets();

    m_playTimer->start(m_stepDelayMs / m_simulationSpeedSlider->value());
    m_isPlaying = true;

    setWindowTitle(tr("Simulation"));
}

VisualizationWindow::~VisualizationWindow()
{
    delete m_os;
}

void VisualizationWindow::paintEvent(QPaintEvent* pe)
{
    QMainWindow::paintEvent(pe);

    QPainter painter(this);
    painter.drawText(
        QPointF(m_timeline->rect().right() / 2.f, 15.f), 
        tr("Timeline"));
}

void VisualizationWindow::RunStep()
{
    if(m_os->NextStep())
    {
        // Column limit reached. Extend it.
        if(m_time + 1 > (unsigned)m_timeline->columnCount())
        {
            int columnCount = m_timeline->columnCount();
            for(int i = 0; i < columnCount; i++)
            {
                m_timeline->insertColumn(m_timeline->columnCount());
                m_timeline->setColumnWidth(columnCount + i, TIMELINECOLUMNWIDTH);
            }
        }

        std::vector<Proccess*> proccesses = m_os->GetActiveProccesses();
        std::vector<Proccess*> blockedProccesses = m_os->GetBlockedProcesses();
        for(unsigned i = 0; i < proccesses.size(); i++)
        {
            if(proccesses[i]->GetState() == ProccessState::RUNNING ||
               proccesses[i]->GetState() == ProccessState::DONE)
            {
                QTableWidgetItem* newItem = new QTableWidgetItem();
                if(m_os->GetSchedulingAlgorithm() == SchedulingAlgorithm::EDF &&
                   proccesses[i]->GetDeadline() <= m_time)
                {
                    newItem->setBackgroundColor(Qt::darkBlue);
                }
                else
                {
                    newItem->setBackgroundColor(Qt::green);
                }
                m_timeline->setItem(proccesses[i]->GetID() - 1, m_time, newItem);
                m_timeline->scrollToItem(newItem);
            }
            else if(proccesses[i]->GetState() == ProccessState::OVERHEAD)
            {
                QTableWidgetItem* newItem = new QTableWidgetItem();
                newItem->setBackgroundColor(Qt::red);
                m_timeline->setItem(proccesses[i]->GetID() - 1, m_time, newItem);
                m_timeline->scrollToItem(newItem);
            }
            else if(proccesses[i]->GetState() == ProccessState::IO)
            {
                QTableWidgetItem* newItem = new QTableWidgetItem();
                newItem->setBackgroundColor(Qt::blue);
                m_timeline->setItem(proccesses[i]->GetID() - 1, m_time, newItem);
                m_timeline->scrollToItem(newItem);
            }

        }

        for(auto& proccess : blockedProccesses)
        {
            if(proccess->GetState() == ProccessState::IO)
            {
                QTableWidgetItem* newItem = new QTableWidgetItem();
                newItem->setBackgroundColor(Qt::blue);
                m_timeline->setItem(proccess->GetID() - 1, m_time, newItem);
                m_timeline->scrollToItem(newItem);
            }
        }

        for(int i = 0; i < 50; i++)
        {
            QString cellText;
            int ramPage = m_os->GetMemoryManager()->GetRAM()[i];
            if(ramPage == -1) cellText = tr("free");
            else cellText = tr("%1 (P%2)").arg(
                ramPage).arg(m_os->WhichProccessOwnsThisPage(ramPage));

            QTableWidgetItem* newItem = new QTableWidgetItem(cellText);
            m_RAMView->setItem(i, 0, newItem);
        }
        m_time++;
        m_currentTimeLabel->setText(tr("Time: %1").arg(m_time));

        if(m_isPlaying)
        {
            m_playTimer->start(m_stepDelayMs / m_simulationSpeedSlider->value());
        }
    }
    else
    {
        m_averageTurnaroundLabel->setText(
            tr("Average Turnaround: %1").arg(m_os->GetAverageTurnaround()));
    }
}

void VisualizationWindow::PlayPause()
{
    if(m_isPlaying)
    {
        m_playTimer->stop();
        m_isPlaying = false;
        m_playPauseButton->setText(tr("Play"));
        m_playPauseButton->setIcon(
            this->style()->standardIcon(QStyle::SP_MediaPlay));
    }
    else
    {
        m_playTimer->start(m_stepDelayMs / m_simulationSpeedSlider->value());
        m_isPlaying = true;
        m_playPauseButton->setText(tr("Pause"));
        m_playPauseButton->setIcon(
            this->style()->standardIcon(QStyle::SP_MediaPause));
    }
    
}

void VisualizationWindow::NextStep()
{
    if(!m_isPlaying) RunStep();
}
void VisualizationWindow::CreateTimeline()
{
    if(m_timeline != nullptr) delete m_timeline;

    m_timeline = new QTableWidget(m_os->GetNumberOfProccesses(),
        INITIALCOLUMNNUMBER, this);

    // Make table non-selectable and non-editable
    m_timeline->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_timeline->setFocusPolicy(Qt::NoFocus);
    m_timeline->setSelectionMode(QAbstractItemView::NoSelection);

    for(unsigned i = 1; i <= m_os->GetNumberOfProccesses(); i++)
    {
        QTableWidgetItem* header = new QTableWidgetItem(tr("P%1").arg(i));
        m_timeline->setVerticalHeaderItem(i - 1, header);
    }

    m_RAMView = new QTableWidget(
        m_os->GetMemoryManager()->GetRAM().size(), 1, this);
    // Make table non-selectable and non-editable
    m_RAMView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_RAMView->setFocusPolicy(Qt::NoFocus);
    m_RAMView->setSelectionMode(QAbstractItemView::NoSelection);
    m_RAMView->setMaximumWidth(150);
    m_RAMView->setHorizontalHeaderLabels(QStringList() << "RAM/ Owner");

    for(int i = 0; i < m_timeline->columnCount(); i++)
    {
        m_timeline->setColumnWidth(i, TIMELINECOLUMNWIDTH);
    }

   
}

void VisualizationWindow::CreateLeftsideLayout()
{
    m_leftsideWidget = new QWidget(this);
    m_leftsideLayout = new QVBoxLayout(m_leftsideWidget);
    m_leftsideLayout->addWidget(m_timeline);
    m_leftsideWidget->setLayout(m_leftsideLayout);

    CreateLegend();
    m_leftsideLayout->addWidget(m_legendBox);

    CreateControls();
    m_leftsideLayout->addWidget(m_controls);

    CreateInfoBox();
    m_leftsideLayout->addWidget(m_infoBox);
}

void VisualizationWindow::CreateControls()
{
    m_controls = new QGroupBox(tr("Controls"), m_leftsideWidget);

    m_playPauseButton = new QPushButton(tr("Pause"), m_controls);
    m_playPauseButton->setIcon(
        this->style()->standardIcon(QStyle::SP_MediaPause));
    m_resetButton = new QPushButton(tr("Reset"), m_controls);

    m_nextStepButton = new QPushButton(tr("Next Step"));
    m_nextStepButton->setIcon(
        this->style()->standardIcon(QStyle::SP_MediaSeekForward));

    m_simulationSpeedSlider = new QSlider(Qt::Horizontal, m_controls);
    m_simulationSpeedSlider->setMaximum(40);
    m_simulationSpeedSlider->setMinimum(5);
    m_simulationSpeedSlider->setValue(10);
    m_simulationSpeedSlider->setTickInterval(1);
    m_sliderBox = new QGroupBox(tr("Simulation Speed"));
    m_sliderBoxLayout = new QHBoxLayout(m_sliderBox);
    m_sliderBoxLayout->addWidget(m_simulationSpeedSlider);
    m_sliderBox->setLayout(m_sliderBoxLayout);

    m_controlsLayout = new QHBoxLayout(m_controls);
    m_controlsLayout->addWidget(m_playPauseButton);
    m_controlsLayout->addWidget(m_resetButton);
    m_controlsLayout->addWidget(m_nextStepButton);
    m_controlsLayout->addWidget(m_sliderBox);
    m_controls->setLayout(m_controlsLayout);
    m_controls->setMaximumHeight(100);
}

void VisualizationWindow::CreateInfoBox()
{
    m_infoBox = new QGroupBox(tr("Info"));
    m_infoBoxLayout = new QHBoxLayout(m_infoBox);

    QString schedulingAlgorithm;

    switch(m_os->GetSchedulingAlgorithm())
    {
        case SchedulingAlgorithm::FIFO:
            schedulingAlgorithm = tr("FIFO");
            break;
        
        case SchedulingAlgorithm::SJF:
            schedulingAlgorithm = tr("SJF");
            break;

        case SchedulingAlgorithm::ROUND_ROBIN:
            schedulingAlgorithm = tr("Round Robin");
            break;

        case SchedulingAlgorithm::EDF:
            schedulingAlgorithm = tr("EDF");
            break;
        
        default:
            break;
    }
    
    m_shcedulingAlgorithmLabel = new QLabel(tr("Scheduling Algorithm: %1").arg(
        schedulingAlgorithm));

    QString pra;

    switch(m_os->GetMemoryManager()->GetPageReplacementAlgorithm())
    {
        case PageReplacementAlgorithm::FIFO:
            pra = tr("FIFO");
            break;
        
        case PageReplacementAlgorithm::LRU:
            pra = tr("LRU");
            break;

        default:
            pra = tr("Error");
            break;
    }

    m_PRALabel = new QLabel(tr("Page Replacement Algorithm: %1").arg(pra));

    m_currentTimeLabel = new QLabel(tr("Time: 0"));

    m_averageTurnaroundLabel = new QLabel(tr("Average Turnaround: --"));

    m_infoBoxLayout->addWidget(m_shcedulingAlgorithmLabel);
    m_infoBoxLayout->addWidget(m_PRALabel);
    m_infoBoxLayout->addWidget(m_currentTimeLabel);
    m_infoBoxLayout->addWidget(m_averageTurnaroundLabel);

    m_infoBox->setLayout(m_infoBoxLayout);
    m_infoBox->setMaximumHeight(100);
}

void VisualizationWindow::CreateLegend()
{
    m_legendBox = new QWidget(this);
    m_legendLayout = new QHBoxLayout(m_legendBox);

    QLabel* running = new QLabel(tr("Running"));
    Rect* runningLegend = new Rect(this, Qt::green, 15);
    QLabel* overhead = new QLabel(tr("Scheduler Overhead"));
    Rect* overheadLegend = new Rect(this, Qt::red, 15);
    QLabel* IO = new QLabel(tr("Loading Pages"));
    Rect* IOLegend = new Rect(this, Qt::blue, 15);
    QLabel* postDeadlineExecution = new QLabel(tr("Post Deadline Execution"));
    Rect* postDeadlineExecutionLegend = new Rect(this, Qt::darkBlue, 15);

    m_legendLayout->addWidget(running, 0, Qt::AlignCenter);
    m_legendLayout->addWidget(runningLegend);
    m_legendLayout->addWidget(overhead, 0, Qt::AlignCenter);
    m_legendLayout->addWidget(overheadLegend);
    m_legendLayout->addWidget(IO, 0, Qt::AlignCenter);
    m_legendLayout->addWidget(IOLegend);
    m_legendLayout->addWidget(postDeadlineExecution, 0, Qt::AlignCenter);
    m_legendLayout->addWidget(postDeadlineExecutionLegend);

    m_legendBox->setLayout(m_legendLayout);
    m_legendBox->setMaximumHeight(100);
}


void VisualizationWindow::CreateMainLayout()
{
    m_mainWidget = new QWidget(this);
    m_mainLayout = new QHBoxLayout(m_mainWidget);
    m_mainLayout->addWidget(m_leftsideWidget);
    m_mainLayout->addWidget(m_RAMView);
    m_mainWidget->setLayout(m_mainLayout);
}

void VisualizationWindow::ConnectWidgets()
{
    connect(m_playTimer, &QTimer::timeout, this, &VisualizationWindow::RunStep);
    connect(m_playPauseButton, &QPushButton::clicked, this, &VisualizationWindow::PlayPause);
    connect(m_nextStepButton, &QPushButton::clicked, this, &VisualizationWindow::NextStep);
}