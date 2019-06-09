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

private:
    void CreateTimeline();

    OperatingSystem* m_os;

    QWidget* m_mainWidget;
    QVBoxLayout* m_mainLayout;
    QTableWidget* m_timeline;
    QTableWidget* m_RAMView;
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;

    const static unsigned INITIALCOLUMNNUMBER = 60;
};
