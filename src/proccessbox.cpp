#include "proccessbox.h"

ProccessBox::ProccessBox(int proccessIndex, QWidget* parent)
: QGroupBox(tr("Proccess %1").arg(proccessIndex), parent)
{
    m_proccessLayout = new QGridLayout();

    m_priorityLabel = new QLabel(tr("Priority"), this);
    m_priorityInput = new QSpinBox(this);
    m_priorityInput->setMaximum(99999);
    m_priorityInput->setStatusTip(tr("Set the priority for the proccess."));

    m_durationLabel = new QLabel(tr("Duration"), this);
    m_durationInput = new QSpinBox(this);
    m_durationInput->setMinimum(1);
    m_durationInput->setMaximum(99999);
    m_durationInput->setStatusTip(tr("Set the duration of the proccess."));

    m_arrivalTimeLabel = new QLabel(tr("Arrival Time"), this);
    m_arrivalTimeInput = new QSpinBox(this);
    m_arrivalTimeInput->setMaximum(99999);
    m_arrivalTimeInput->setStatusTip(tr("Set the arrival time of the proccess."));

    QPushButton* remove = new QPushButton(tr("Remove"), nullptr);
    connect(remove, &QPushButton::clicked, this, ProccessBox::close);

    m_proccessLayout->addWidget(m_priorityLabel, 0, 0);
    m_proccessLayout->addWidget(m_priorityInput, 0, 1);
    m_proccessLayout->addWidget(m_durationLabel, 1, 0);
    m_proccessLayout->addWidget(m_durationInput, 1, 1);
    m_proccessLayout->addWidget(m_arrivalTimeLabel, 2, 0);
    m_proccessLayout->addWidget(m_arrivalTimeInput, 2, 1);
    m_proccessLayout->addWidget(remove, 3, 0);

    setLayout(m_proccessLayout);
}

int ProccessBox::GetArrivalTime()
{
    return m_arrivalTimeInput->value();
}