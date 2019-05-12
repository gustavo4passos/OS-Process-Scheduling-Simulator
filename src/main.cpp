#include <QtWidgets\QApplication>
#include "mainwindow.h"
#include "operatingsystem.h"

#include <iostream>

int main(int argc, char* args[])
{
    QApplication app(argc, args);
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}