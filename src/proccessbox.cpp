#include "proccessbox.h"

ProccessBox::ProccessBox(unsigned proccessIndex, QWidget* parent)
: QGroupBox(tr("Proccess %1").arg(proccessIndex), parent),
    m_ID(proccessIndex)
{
    CreateSpinBoxes();
    CreateRemoveButton();
    CreateMainLayout();
}

void ProccessBox::CreateSpinBoxes()
{
    m_arrivalTimeLabel = new QLabel(tr("Arrival Time"), this);
    m_arrivalTimeInput = new QSpinBox(this);
    m_arrivalTimeInput->setMaximum(99999);
    m_arrivalTimeInput->setStatusTip(tr("Set the arrival time of the proccess."));

    m_durationLabel = new QLabel(tr("Duration"), this);
    m_durationInput = new QSpinBox(this);
    m_durationInput->setMinimum(1);
    m_durationInput->setMaximum(99999);
    m_durationInput->setStatusTip(tr("Set the duration of the proccess."));

    m_deadlineLabel = new QLabel(tr("Deadline"), this);
    m_deadlineInput = new QSpinBox(this);
    m_deadlineInput->setMaximum(99999);
    m_deadlineInput->setStatusTip(tr("Set the priority for the proccess."));
}

void ProccessBox::CreateRemoveButton()
{
    m_removeButton = new QPushButton(tr("Remove"), this);
    connect(m_removeButton, &QPushButton::clicked, 
        this, &ProccessBox::RemoveButtonClicked);
}

void ProccessBox::CreateMainLayout()
{
    m_proccessLayout = new QGridLayout(this);
    m_proccessLayout->addWidget(m_arrivalTimeLabel, 0, 0);
    m_proccessLayout->addWidget(m_arrivalTimeInput, 0, 1);
    m_proccessLayout->addWidget(m_durationLabel, 1, 0);
    m_proccessLayout->addWidget(m_durationInput, 1, 1);
    m_proccessLayout->addWidget(m_deadlineLabel, 2, 0);
    m_proccessLayout->addWidget(m_deadlineInput, 2, 1);
    m_proccessLayout->addWidget(m_removeButton, 3, 1, Qt::AlignRight);

    setFixedSize(150, 150);
    setLayout(m_proccessLayout);
}

unsigned ProccessBox::GetArrivalTime()
{
    return (unsigned)m_arrivalTimeInput->value();
}

unsigned ProccessBox::GetDuration()
{
    return (unsigned)m_durationInput->value();
}

unsigned ProccessBox::GetDeadline()
{
    return (unsigned)m_deadlineInput->value();
}

void ProccessBox::RemoveButtonClicked()
{
    emit RemoveProccessRequested(this);
}