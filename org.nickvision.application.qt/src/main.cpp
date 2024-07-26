#include "application.h"

using namespace Nickvision::Application::QT;

int main(int argc, char *argv[]) 
{
    Application app{ argc, argv };
    return app.exec();
}