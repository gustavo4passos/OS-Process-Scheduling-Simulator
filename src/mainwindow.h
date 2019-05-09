#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets\QtWidgets>

class MainWindow : public QMainWindow
{
        Q_OBJECT
public:
    MainWindow();

private slots:
    void AddProccessBox();
    void Load();

private:
    void CreateButtons();
    void CreateMenuBar();
    void CreateStatusBar();
    void CreateMainLayout();
    void ConnectWidgets();

    QWidget* m_mainWidget;
    QBoxLayout* m_mainLayout;
    QMenu* m_fileMenu;
    QPushButton* m_addProccessButon;
    QAction* m_loadAction;

    int m_numberOfProccesses;
};

#endif