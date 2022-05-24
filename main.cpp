#include "ui/application.h"

int main(int argc, char* argv[])
{
    NickvisionApplication::UI::Application app{"org.nickvision.application"};
    return app.run(argc, argv);
}
