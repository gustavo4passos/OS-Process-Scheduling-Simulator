#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QtWidgets>
#include "operatingsystem.h"

class ProccessList;

class MainWindow : public QMainWindow
{
        Q_OBJECT
public:
    MainWindow();

private slots:
    void AddProccessBox();
    void ActivateOrDeactivateRunButton(int numberOfProccesses);
    void Load();

private:
    void CreateButtons();
    void CreateSchedulingAlgorithmSelectionBox();
    void CreateQuantumValueSelectionBox();
    void CreateOverheadValueSelectionBox();
    void CreateMenuBar();
    void CreateStatusBar();
    void CreateMainLayout();
    void ConnectWidgets();

    void RunSimulation();
    SchedulingAlgorithm GetSchedulingAlgorithm();


    QWidget* m_mainWidget;
    QVBoxLayout* m_mainLayout;

    QMenu* m_fileMenu;
    QAction* m_loadAction;

    QGroupBox* m_schedulerSettingsGroup;
    QBoxLayout* m_schedulerSettingsLayout;
    QGroupBox* m_schedulingAlgorithmBox;
    QHBoxLayout* m_sheculingAlgorithmBoxLayout;
    QButtonGroup* m_schedulingRadioButtons;
    QGroupBox* m_quantumValueSelectorBox;
    QSpinBox* m_quantumValueSelector;
    QGroupBox* m_overheadValueSelectorBox;
    QSpinBox* m_overheadValueSelector;

    QWidget* m_proccessListGroup;
    QVBoxLayout* m_proccessGroupLayout;
    ProccessList* m_proccessList;
    QPushButton* m_addProccessButton;

    QPushButton* m_runButton;
    int m_numberOfProccesses;
};

#endif
