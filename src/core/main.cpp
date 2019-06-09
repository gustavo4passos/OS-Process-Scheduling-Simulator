#include <QtWidgets/QApplication>
#include "../gui/mainwindow.h"
#include "operatingsystem.h"

int main(int argc, char* args[])
{
    QApplication app(argc, args);
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
