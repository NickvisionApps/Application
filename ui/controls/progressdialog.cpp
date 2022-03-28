#include "progressdialog.h"

using namespace NickvisionApplication::UI;
using namespace NickvisionApplication::UI::Controls;

ProgressDialog::ProgressDialog(GtkWidget* parent, const std::string& description, const std::function<void()>& work) : Widget("/ui/controls/progressdialog.xml"), m_work(work), m_isFinished(false)
{
    //==Dialog==//
    gtk_window_set_transient_for(GTK_WINDOW(ProgressDialog::gobj()), GTK_WINDOW(parent));
    g_timeout_add(50, [](void* data) -> int { return reinterpret_cast<ProgressDialog*>(data)->timeout(); }, this);
    //==Description==//
    gtk_label_set_markup(GTK_LABEL(gtk_builder_get_object(m_builder, "gtk_lblDescription")), std::string("<b>" + description + "</b>").c_str());
    //==Thread==//
    m_thread = std::jthread([&]()
    {
        m_work();
        m_isFinished = true;
    });
}

ProgressDialog::~ProgressDialog()
{
    gtk_window_destroy(GTK_WINDOW(ProgressDialog::gobj()));
}

GtkWidget* ProgressDialog::gobj()
{
    return GTK_WIDGET(gtk_builder_get_object(m_builder, "gtk_progDialog"));
}

void ProgressDialog::show()
{
    if(!m_isFinished)
    {
        gtk_widget_show(gobj());
    }
}

bool ProgressDialog::timeout()
{
    gtk_progress_bar_pulse(GTK_PROGRESS_BAR(gtk_builder_get_object(m_builder, "gtk_progBar")));
    if(m_isFinished)
    {
        gtk_widget_hide(gobj());
        return false;
    }
    return true;
}
