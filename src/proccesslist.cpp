#include "proccesslist.h"
#include "addproccesswidget.h"

ProccessList::ProccessList(QWidget* parent)
:   QScrollArea(parent),
    m_mainLayout(nullptr),
    m_centralWidget(nullptr),
    m_numberOfProccesses(0)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setWidgetResizable(true);
    setFixedHeight(190);
    setFixedWidth(600);
    CreateMainLayout();
}

void ProccessList::AddProccess(int index)
{
    AddProccessWidget* proccessBox = new AddProccessWidget(index, this);
    connect(proccessBox, &AddProccessWidget::RemoveProccessRequested,
        this, &ProccessList::RemoveProccess);
    m_mainLayout->insertWidget(m_numberOfProccesses, proccessBox, 
        0, Qt::AlignLeft | Qt::AlignTop);

    m_numberOfProccesses++;

    emit NumberOfProccessesChanged(m_numberOfProccesses);
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