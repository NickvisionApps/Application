#include "infobar.h"

namespace NickvisionApplication::Controls
{
    InfoBar::InfoBar()
    {
        //==Signals==//
        signal_response().connect([&](int response)
        {
           hide();
        });
        //==Layout==//
        Gtk::Box mainBox(Gtk::Orientation::HORIZONTAL);
        m_lblMessage.set_margin_start(6);
        mainBox.append(m_lblTitle);
        mainBox.append(m_lblMessage);
        add_child(mainBox);
        hide();
    }

    void InfoBar::showMessage(const std::string& title, const std::string& message, bool closeable, Gtk::MessageType messageType)
    {
        m_lblTitle.set_markup("<b>" + title + "</b>");
        m_lblMessage.set_text(message);
        set_show_close_button(closeable);
        set_message_type(messageType);
        show();
        Glib::MainContext::get_default()->iteration(true);
    }
}
