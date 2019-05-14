#include "proccesslist.h"
#include "proccessbox.h"

ProccessList::ProccessList(QWidget* parent)
:   QScrollArea(parent),
    m_mainLayout(nullptr),
    m_centralWidget(nullptr),
    m_numberOfProccesses(0)
{
    setWidgetResizable(true);
    setFixedHeight(190);
    CreateMainLayout();
}

void ProccessList::AddProccess(int index)
{
    ProccessBox* proccessBox = new ProccessBox(index, this);
    connect(proccessBox, &ProccessBox::RemoveProccessRequested,
        this, &ProccessList::RemoveProccess);
    m_mainLayout->insertWidget(m_numberOfProccesses, proccessBox, 
        0, Qt::AlignLeft | Qt::AlignTop);

    m_numberOfProccesses++;

    emit NumberOfProccessesChanged(m_numberOfProccesses);
}

std::vector<ProccessTemplate> ProccessList::GetCurrentProccesses()
{
    std::vector<ProccessTemplate> proccessTemplates;
    for(int i = 0; i < m_mainLayout->count(); ++i)
    {
        ProccessBox* pWidget = static_cast<ProccessBox*>(m_mainLayout->itemAt(i)->widget());
        proccessTemplates.push_back({
            pWidget->GetID(),
            pWidget->GetDuration(),
            pWidget->GetArrivalTime(),
            pWidget->GetDeadline()
        });
    }

    return proccessTemplates;
}


void ProccessList::RemoveProccess(QWidget* widget)
{
    m_numberOfProccesses--;
    m_mainLayout->removeWidget(widget);
    delete widget;
    widget = nullptr;

    emit NumberOfProccessesChanged(m_numberOfProccesses);
}

void ProccessList::CreateMainLayout()
{
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setAlignment(Qt::AlignLeft);
    m_centralWidget = new QWidget(this);
    this->setWidget(m_centralWidget);

    m_centralWidget->setLayout(m_mainLayout);
}