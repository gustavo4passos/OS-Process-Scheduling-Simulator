#include "addproccesswidget.h"

AddProccessWidget::AddProccessWidget(int proccessIndex, QWidget* parent)
: QGroupBox(tr("Proccess %1").arg(proccessIndex), parent)
{
    m_proccessLayout = new QGridLayout();

    m_deadlineLabel = new QLabel(tr("Deadline"), this);
    m_deadlineInput = new QSpinBox(this);
    m_deadlineInput->setMaximum(99999);
    m_deadlineInput->setStatusTip(tr("Set the priority for the proccess."));

    m_durationLabel = new QLabel(tr("Duration"), this);
    m_durationInput = new QSpinBox(this);
    m_durationInput->setMinimum(1);
    m_durationInput->setMaximum(99999);
    m_durationInput->setStatusTip(tr("Set the duration of the proccess."));

    m_arrivalTimeLabel = new QLabel(tr("Arrival Time"), this);
    m_arrivalTimeInput = new QSpinBox(this);
    m_arrivalTimeInput->setMaximum(99999);
    m_arrivalTimeInput->setStatusTip(tr("Set the arrival time of the proccess."));

    QPushButton* removeButton = new QPushButton(tr("Remove"), nullptr);
    connect(removeButton, &QPushButton::clicked, 
        this, &AddProccessWidget::RemoveButtonClicked);

    m_proccessLayout->addWidget(m_deadlineLabel, 0, 0);
    m_proccessLayout->addWidget(m_deadlineInput, 0, 1);
    m_proccessLayout->addWidget(m_durationLabel, 1, 0);
    m_proccessLayout->addWidget(m_durationInput, 1, 1);
    m_proccessLayout->addWidget(m_arrivalTimeLabel, 2, 0);
    m_proccessLayout->addWidget(m_arrivalTimeInput, 2, 1);
    m_proccessLayout->addWidget(removeButton, 3, 1, Qt::AlignRight);

    setFixedSize(150, 150);
    setLayout(m_proccessLayout);
}

int AddProccessWidget::GetArrivalTime()
{
    return m_arrivalTimeInput->value();
}

void AddProccessWidget::RemoveButtonClicked()
{
    emit RemoveProccessRequested(this);
}