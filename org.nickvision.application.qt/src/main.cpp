#include <QApplication>
#include "views/mainwindow.h"

using namespace Nickvision::Application::QT::Views;

int main(int argc, char *argv[]) 
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}