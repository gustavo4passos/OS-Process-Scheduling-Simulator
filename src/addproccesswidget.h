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

private slots:
    void RemoveButtonClicked();

signals:
    void RemoveProccessRequested(QWidget* proccess);

private:
    QGridLayout* m_proccessLayout;
    QLabel* m_deadlineLabel;
    QSpinBox* m_deadlineInput;
    QLabel* m_durationLabel;
    QSpinBox* m_durationInput;
    QLabel* m_arrivalTimeLabel;
    QSpinBox* m_arrivalTimeInput;
};

#endif