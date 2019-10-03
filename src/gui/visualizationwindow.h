#pragma once

#include "../core/operatingsystem.h"
#include "proccesslist.h"
#include <QtWidgets/QtWidgets>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>

class VisualizationWindow : public QMainWindow
{
    Q_OBJECT
public:
    VisualizationWindow(
        unsigned quantum,
        unsigned overhead,
        SchedulingAlgorithm schedulingAlgorithm,
        PageReplacementAlgorithm pra,
        const std::vector<ProccessTemplate>& proccessTemplates,
        QWidget* parent = nullptr
    );
    ~VisualizationWindow();

    void paintEvent(QPaintEvent*);

public slots:
    void RunStep();
    void PlayPause();
    void NextStep();

private:
    void DrawOneStep();
    void CreateTimeline();
    void CreateLeftsideLayout();
    void CreateControls();
    void CreateInfoBox();
    void CreateLegend();
    void CreateMainLayout();
    void ConnectWidgets();

    OperatingSystem* m_os;

    QWidget* m_mainWidget;
    QHBoxLayout* m_mainLayout;

    // QWidget* m_controls;
    QGroupBox* m_controls;
    QHBoxLayout* m_controlsLayout;
    QPushButton* m_playPauseButton;
    QPushButton* m_nextStepButton;
    QGroupBox* m_sliderBox;
    QHBoxLayout* m_sliderBoxLayout;
    QSlider* m_simulationSpeedSlider;
    QPushButton* m_resetButton;

    QWidget* m_leftsideWidget;

    QFrame* m_timelineFrame;
    QVBoxLayout* m_leftsideLayout;
    QTableWidget* m_timeline;

    QTableWidget* m_RAMView;
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    QTimer* m_playTimer;

    QGroupBox* m_infoBox;
    QHBoxLayout* m_infoBoxLayout;
    QLabel* m_shcedulingAlgorithmLabel;
    QLabel* m_PRALabel;
    QLabel* m_currentTimeLabel;
    QLabel* m_schedulingAlgorithmText;
    QLabel* m_averageTurnaroundLabel;

    QWidget* m_legendBox;
    QHBoxLayout* m_legendLayout;

    const static unsigned INITIALCOLUMNNUMBER = 60;
    unsigned m_time;
    bool m_isPlaying;
    const unsigned m_stepDelayMs = 5000;
    const int TIMELINECOLUMNWIDTH = 20;
};
