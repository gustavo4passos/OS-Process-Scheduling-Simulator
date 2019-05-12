#ifndef ADDPROCCESSWIDGET_H
#define ADDPROCCESSWIDGET_H

#include <QtWidgets/QtWidgets>

class AddProccessWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit AddProccessWidget(int proccessIndex, QWidget* parent = nullptr);

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