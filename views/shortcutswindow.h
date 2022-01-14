#ifndef SHORTCUTSWINDOW_H
#define SHORTCUTSWINDOW_H

#include <gtkmm.h>

namespace NickvisionApplication::Views
{
    class ShortcutsWindow : public Gtk::ShortcutsWindow
    {
    public:
        ShortcutsWindow(Gtk::Window& parent);

    private:
        Gtk::ShortcutsSection m_section;
        Gtk::ShortcutsGroup m_grpFolder;
        Gtk::ShortcutsShortcut m_shortOpenFolder;
        Gtk::ShortcutsGroup m_grpApplication;
        Gtk::ShortcutsShortcut m_shortAbout;
    };
}

#endif // SHORTCUTSWINDOW_H
