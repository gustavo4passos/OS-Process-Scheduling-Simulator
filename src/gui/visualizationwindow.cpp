#include "visualizationwindow.h"

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
    m_scene(nullptr)
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


    QGraphicsRectItem* r = new QGraphicsRectItem(0, 0, 0, 50);
    QGraphicsRectItem* n = new QGraphicsRectItem(50, 50, 50, 50);

    r->setBrush(QBrush(Qt::black));
    n->setBrush(QBrush(Qt::green));

    QTimeLine* timeline = new QTimeLine(5000, this);
    timeline->setFrameRange(0, 100);

    QGraphicsItemAnimation* animation = new QGraphicsItemAnimation;
    QGraphicsItemAnimation* a = new QGraphicsItemAnimation;

    animation->setItem(r);
    a->setItem(n);

    animation->setTimeLine(timeline);
    a->setTimeLine(timeline);

    // animation->setPosAt(0.2f, QPointF(10, 10));
    // animation->setPosAt(1.f, QPointF(960, 544));
    animation->setScaleAt(.1f, 100.f, 1.f);
    // a->setPosAt(.2f, QPointF(50.f, 0.f));
    a->setScaleAt(.0f, 0.f, 0.f);
    a->setScaleAt(.1f, 1.f, 1.f);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 960, 544);

#if TRUE
    for(int i = 0; i < 1920; i += 50)
    {
        m_scene->addLine(i, 0, i, 1080, QPen(Qt::red));
    }
    for(int i = 0; i < 1080; i+= 50)
    {
        m_scene->addLine(0, i, 1920, i);
    }
#endif

#ifdef FALSE
    QBrush brush = QBrush();
    brush.setColor(Qt::blue);
    m_scene->setBackgroundBrush(brush);
#endif

    // m_scene->addItem(r);
    m_scene->addItem(n);

    m_view = new QGraphicsView(m_scene);
    m_view->show();

    setCentralWidget(m_mainWidget);

    timeline->start();
}
VisualizationWindow::~VisualizationWindow()
{
    delete m_os;
}

void VisualizationWindow::CreateTimeline()
{
    m_timeline = new QTableWidget(m_os->GetNumberOfProccesses(),
        INITIALCOLUMNNUMBER, this);

    m_RAMView = new QTableWidget(
        m_os->GetMemoryManager()->GetRAM().size(), 1, this);

    unsigned time = 0;
    while(m_os->NextStep())
    {
        std::vector<Proccess*> proccesses = m_os->GetActiveProccesses();
        std::vector<Proccess*> blockedProccesses = m_os->GetBlockedProcesses();
        for(unsigned i = 0; i < proccesses.size(); i++)
        {
            if(proccesses[i]->GetState() == ProccessState::RUNNING ||
               proccesses[i]->GetState() == ProccessState::DONE)
            {
                QTableWidgetItem* newItem = new QTableWidgetItem();
                if(m_os->GetSchedulingAlgorithm() == SchedulingAlgorithm::EDF &&
                   proccesses[i]->GetDeadline() <= time)
                {
                    newItem->setBackgroundColor(Qt::darkBlue);
                }
                else
                {
                    newItem->setBackgroundColor(Qt::green);
                }
                m_timeline->setItem(proccesses[i]->GetID() - 1, time, newItem);
            }
            else if(proccesses[i]->GetState() == ProccessState::OVERHEAD)
            {
                QTableWidgetItem* newItem = new QTableWidgetItem();
                newItem->setBackgroundColor(Qt::red);
                m_timeline->setItem(proccesses[i]->GetID() - 1, time, newItem);
            }
            else if(proccesses[i]->GetState() == ProccessState::BLOCKED)
            {
                QTableWidgetItem* newItem = new QTableWidgetItem();
                newItem->setBackgroundColor(Qt::black);
                m_timeline->setItem(proccesses[i]->GetID() - 1, time, newItem);
            }
            else if(proccesses[i]->GetState() == ProccessState::IO)
            {
                QTableWidgetItem* newItem = new QTableWidgetItem();
                newItem->setBackgroundColor(Qt::blue);
                m_timeline->setItem(proccesses[i]->GetID() - 1, time, newItem);
            }
        }

        for(auto& proccess : blockedProccesses)
        {
            QTableWidgetItem* newItem = new QTableWidgetItem();
            if(proccess->GetState() == ProccessState::IO)
            {
                newItem->setBackgroundColor(Qt::blue);
            }
            else newItem->setBackgroundColor(Qt::black);
            m_timeline->setItem(proccess->GetID() - 1, time, newItem);
        }

        for(int i = 0; i < 50; i++)
        {
            QTableWidgetItem* newItem = new QTableWidgetItem(tr("%1").arg(
                m_os->GetMemoryManager()->GetRAM()[i]
            ));
            m_RAMView->setItem(i, time, newItem);
        }
        m_RAMView->insertColumn(m_RAMView->columnCount());
        time++;
    }

    

    

    for(int i = 0; i < m_timeline->columnCount(); i++)
    {
        m_timeline->setColumnWidth(i, 2);
    }

    m_mainWidget = new QWidget(this);
    m_mainLayout = new QVBoxLayout(m_mainWidget);
    m_mainLayout->addWidget(m_timeline);
    m_mainLayout->addWidget(m_RAMView);
    m_mainWidget->setLayout(m_mainLayout);
}