#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets\QtWidgets>

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
    void CreateMenuBar();
    void CreateStatusBar();
    void CreateMainLayout();
    void ConnectWidgets();

    QWidget* m_mainWidget;

    QVBoxLayout* m_mainGrid;
    QVBoxLayout* m_mainLayout;

    QGroupBox* m_osSettingsGroup;
    QMenu* m_fileMenu;
    QAction* m_loadAction;

    QGroupBox* m_schedulingAlgorithmBox;
    QBoxLayout* m_schedulerSettingsLayout;
    QHBoxLayout* m_schedulingOptionsLayout;
    QButtonGroup* m_schedulingRadioButtons;
    QGroupBox* m_quantumValueSelectorBox;
    QSpinBox* m_quantumValueSelector;

    QWidget* m_prccessListGroup;
    ProccessList* m_proccessList;
    QPushButton* m_addProccessButton;

    QPushButton* m_runButton;
    int m_numberOfProccesses;
};

#endif