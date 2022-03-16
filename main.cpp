#include "application.h"

int main(int argc, char* argv[])
{
    NickvisionApplication::Application app("org.nickvision.application");
    return app.run(argc, argv);
}
