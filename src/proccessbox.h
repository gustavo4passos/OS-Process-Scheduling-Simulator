#ifndef PROCCESSBOX_H
#define PROCCESSBOX_H

#include <QtWidgets/QtWidgets>

class ProccessBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit ProccessBox(int proccessIndex, QWidget* parent = nullptr);

    // Retrieves current value of arrival time
    int GetArrivalTime();

private:
    QGridLayout* m_proccessLayout;
    QLabel* m_priorityLabel;
    QSpinBox* m_priorityInput;
    QLabel* m_durationLabel;
    QSpinBox* m_durationInput;
    QLabel* m_arrivalTimeLabel;
    QSpinBox* m_arrivalTimeInput;
};

#endif