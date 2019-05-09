#include "mainwindow.h"
#include "proccessbox.h"

MainWindow::MainWindow()
:   m_mainWidget(nullptr),
    m_mainLayout(nullptr),
    m_fileMenu(nullptr),
    m_addProccessButon(nullptr),
    m_loadAction(nullptr),
    m_numberOfProccesses(0)
{
    CreateButtons();
    CreateMenuBar();
    CreateStatusBar();
    CreateMainLayout();
    ConnectWidgets();
}

void MainWindow::AddProccessBox()
{
    m_numberOfProccesses++;
    m_mainLayout->insertWidget(m_numberOfProccesses, new ProccessBox(m_numberOfProccesses, this));
}

void MainWindow::Load()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select file");
}

void MainWindow::CreateButtons()
{
    m_addProccessButon = new QPushButton(tr("+\nAdd Process"), this);
    m_addProccessButon->setStatusTip(tr("Add a new process"));

}

void MainWindow::CreateMenuBar()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_loadAction = new QAction(tr("Load"), this);
    connect(m_loadAction, &QAction::triggered, this, &MainWindow::Load);

    m_fileMenu->addAction(m_loadAction);
}

void MainWindow::CreateStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::CreateMainLayout()
{
    m_mainWidget =
     new QWidget(this);
    m_mainLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    m_mainLayout->insertWidget(0, m_addProccessButon);

    QTableWidget* tw = new QTableWidget(50, 1, this);
    m_mainLayout->insertWidget(1, tw);
    for(int i = 0; i < 50; i++)
    {
        for(int j = 0; j < 1; j++)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(i + j));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            tw->setItem(i, j, item);
        }
    }
    m_mainWidget->setLayout(m_mainLayout);
    
    QPropertyAnimation* animation = new QPropertyAnimation(m_addProccessButon, "geometry");
    animation->setDuration(1000);
    animation->setStartValue(m_addProccessButon->geometry());
    QRect rc = m_addProccessButon->geometry();
    rc.setHeight( rc.height() + 50);
    animation->setEndValue(rc);
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->start();

    setCentralWidget(m_mainWidget);    
}

void MainWindow::ConnectWidgets()
{
    connect(m_addProccessButon, &QPushButton::clicked, this, &MainWindow::AddProccessBox);
}