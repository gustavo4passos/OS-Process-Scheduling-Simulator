#ifndef ADDPROCCESSWIDGET_H
#define ADDPROCCESSWIDGET_H

#include <QtWidgets/QtWidgets>

class ProccessBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit ProccessBox(unsigned proccessIndex, QWidget* parent = nullptr);

    // Retrieves current value of arrival time
    unsigned GetID() const { return m_ID; }
    unsigned GetArrivalTime();
    unsigned GetDuration();
    unsigned GetDeadline();


private slots:
    void RemoveButtonClicked();

signals:
    void RemoveProccessRequested(QWidget* proccess);

private:
    void CreateSpinBoxes();
    void CreateRemoveButton();
    void CreateMainLayout();
    unsigned m_ID;

    QGridLayout* m_proccessLayout;
    QLabel* m_arrivalTimeLabel;
    QSpinBox* m_arrivalTimeInput;
    QLabel* m_durationLabel;
    QSpinBox* m_durationInput;
    QLabel* m_deadlineLabel;
    QSpinBox* m_deadlineInput;
    QPushButton* m_removeButton;
};

#endif