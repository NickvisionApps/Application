#ifndef WIDGET_H
#define WIDGET_H

#include <string>
#include <adwaita.h>

namespace NickvisionApplication::UI
{
    using Callback_GioAction = void (*)(GSimpleAction*, GVariant*, gpointer*);
    using Callback_GtkWidget = void (*)(GtkWidget*, gpointer*);
    using Callback_GtkDialog_Response = void (*)(GtkDialog*, gint, gpointer*);
    using Callback_GtkButton = void (*)(GtkButton*, gpointer*);
    using Callback_GtkListBox_Selection = void (*)(GtkListBox*, GtkListBoxRow*, gpointer*);
    using Callback_AdwActionRow = void (*)(AdwActionRow*, gpointer*);

    class Widget
    {
    public:
        Widget(const std::string& resourcePath);
        virtual ~Widget() = default;
        GtkBuilder* getBuilder();
        virtual GtkWidget* gobj() = 0;
        virtual void show();

    protected:
        GtkBuilder* m_builder = nullptr;
    };
}

#endif // WIDGET_H
