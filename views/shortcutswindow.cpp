#include "shortcutswindow.h"

namespace NickvisionApplication::Views
{
    ShortcutsWindow::ShortcutsWindow(Gtk::Window& parent)
    {
        //==Settings==//
        set_title("Shortcuts");
        set_transient_for(parent);
        set_modal(true);
        set_hide_on_close(true);
        //==Folder==//
        m_grpFolder.property_title().set_value("Folder");
        m_shortOpenFolder.property_accelerator().set_value("<Ctrl>O");
        m_shortOpenFolder.property_title().set_value("Open Folder");
        m_grpFolder.append(m_shortOpenFolder);
        m_section.append(m_grpFolder);
        //==Application==//
        m_grpApplication.property_title().set_value("Application");
        m_shortAbout.property_accelerator().set_value("F1");
        m_shortAbout.property_title().set_value("About");
        m_grpApplication.append(m_shortAbout);
        m_section.append(m_grpApplication);
        //==Layout==//
        set_child(m_section);
    }
}
