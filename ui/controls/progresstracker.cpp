#include "progresstracker.h"

using namespace NickvisionApplication::UI;
using namespace NickvisionApplication::UI::Controls;

ProgressTracker::ProgressTracker(const std::string& description, const std::function<void()>& work, const std::function<void()>& then) : Widget("/ui/controls/progresstracker.xml"), m_work(work), m_then(then), m_isFinished(false)
{
    //==Signals==//
    g_timeout_add(50, [](void* data) -> int 
    { 
        ProgressTracker* tracker = reinterpret_cast<ProgressTracker*>(data);
        bool result = tracker->timeout();
        if(!result)
        {
            delete tracker;
        }
        return result;
    }, this);
    //==Description==//
    gtk_label_set_markup(GTK_LABEL(gtk_builder_get_object(m_builder, "gtk_lblDescription")), std::string("<b>" + description + "</b>").c_str());
    //==Thread==//
    m_thread = std::jthread([&]()
    {
        m_work();
        std::lock_guard<std::mutex> lock(m_mutex);
        m_isFinished = true;
    });
}

GtkWidget* ProgressTracker::gobj()
{
    return GTK_WIDGET(gtk_builder_get_object(m_builder, "gtk_btnProgTracker"));
}

void ProgressTracker::show()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if(!m_isFinished)
    {
        gtk_widget_show(gobj());
        gtk_popover_popup(GTK_POPOVER(gtk_builder_get_object(m_builder, "gtk_popover")));
    }
}

bool ProgressTracker::timeout()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    gtk_progress_bar_pulse(GTK_PROGRESS_BAR(gtk_builder_get_object(m_builder, "gtk_progBar")));
    if(m_isFinished)
    {
        m_then();
        gtk_popover_popdown(GTK_POPOVER(gtk_builder_get_object(m_builder, "gtk_popover")));
        gtk_widget_hide(gobj());
        return false;
    }
    return true;
}
