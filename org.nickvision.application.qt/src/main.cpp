#include "application.h"

using namespace Nickvision::Application::Qt;

int main(int argc, char *argv[]) 
{
    Application app{ argc, argv };
    return app.exec();
}