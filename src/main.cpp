#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include "operatingsystem.h"

int main(int argc, char* args[])
{
    OperatingSystem os = OperatingSystem(2, 1, SchedulingAlgorithm::FIFO);
    os.AddProccess(1, 4, 3, 2);
    os.AddProccess(2, 3, 3, 2);

    QApplication app(argc, args);
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
