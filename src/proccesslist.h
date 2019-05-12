#ifndef PROCCESLIST_H
#define PROCCESSLIST_H

#include <QtWidgets/QtWidgets>

class ProccessList : public QScrollArea
{
    Q_OBJECT

public:
    ProccessList(QWidget* parent = nullptr);
    void AddProccess(int index);

signals:
    void NumberOfProccessesChanged(int value);

private slots:
    void RemoveProccess(QWidget* widget);

private:
    QHBoxLayout* m_mainLayout;;
    QWidget* m_centralWidget;
    
    int m_numberOfProccesses;

    void CreateMainLayout();
};

#endif