#pragma once

#include <string>
#include <adwaita.h>

namespace NickvisionApplication::UI
{
    class Widget
    {
    public:
        Widget(const std::string& resourcePath);
        virtual ~Widget() = default;
        GtkBuilder* getBuilder();
        virtual GtkWidget* gobj() = 0;
        virtual void show();

    protected:
        GtkBuilder* m_builder;
    };
}
