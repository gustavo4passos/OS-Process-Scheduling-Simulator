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
    unsigned GetNumberOfPages();

	void SetArrivalTime(unsigned value);
	void SetDuration(unsigned value);
	void SetDeadline(unsigned deadline);
    void SetNumberOfPages(unsigned nPages);

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
    QLabel*      m_arrivalTimeLabel;
    QSpinBox*    m_arrivalTimeInput;
    QLabel*      m_durationLabel;
    QSpinBox*    m_durationInput;
    QLabel*      m_deadlineLabel;
    QSpinBox*    m_deadlineInput;
    QLabel*      m_nPagesLabel;
    QSpinBox*    m_nPagesInput;
    QPushButton* m_removeButton;

    const unsigned MAX_NUMBER_OF_PAGES = 10;
    const unsigned MIN_NUMBER_OF_PAGES = 1;
};

#endif
