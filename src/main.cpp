// #include <QtWidgets\QApplication>
// #include "mainwindow.h"
#include "operatingsystem.h"

#include <iostream>

int main(int argc, char* args[])
{
    OperatingSystem operatingSystem = OperatingSystem(2, 2, SchedulingAlgorithm::FIFO);
    Proccess* p1 = new Proccess(1, 0, 3, 4, 1);
    Proccess* p2 = new Proccess(2, 1, 2, 4, 2);
    Proccess* p3 = new Proccess(3, 20, 2, 4, 2);

    operatingSystem.AddProccess(p1);
    operatingSystem.AddProccess(p2);
    operatingSystem.AddProccess(p3);
    std::cout << "Hi" << std::endl;
    while(operatingSystem.NextStep());
    std::cout << "Done." << std::endl;
    std::cout << "Turnaround: " << operatingSystem.GetAverageTurnaround() << std::endl;

    const std::vector<Proccess*>& fp = operatingSystem.GetFinishedProccesses();
    for(auto p = fp.begin(); p != fp.end(); p++)
    {
        std::cout << "Proccess " << (*p)->GetID() << ". Started at " <<
        (*p)->GetArrivalTime() << " and finished at " << (*p)->GetFinishTime() <<
        ". TA is " << (*p)->GetTurnAround() << std::endl;
    }
    std::cin.get();

    // std::cin.get();
    // QApplication app(argc, args);
    // MainWindow mainWindow;
    // mainWindow.show();

    // return app.exec();
}