#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <gtkmm.h>
#include "../models/update/updater.h"
#include "../controls/headerbar.h"
#include "../controls/infobar.h"

namespace NickvisionApplication::Views
{
    class MainWindow : public Gtk::ApplicationWindow
    {
    public:
        MainWindow();
        ~MainWindow();

    private:
        bool m_opened;
        NickvisionApplication::Models::Update::Updater m_updater;
        //==UI==//
        NickvisionApplication::Controls::HeaderBar m_headerBar;
        Gtk::Box m_mainBox;
        NickvisionApplication::Controls::InfoBar m_infoBar;
        Gtk::Label m_lblName;
        Gtk::Entry m_txtName;
        //==Shortcuts==//
        std::shared_ptr<Gtk::ShortcutController> m_shortcutController;
        //Open
        std::shared_ptr<Gtk::Shortcut> m_shortcutOpenFolder;
        std::shared_ptr<Gtk::KeyvalTrigger> m_shortcutOpenFolderTrigger;
        std::shared_ptr<Gtk::CallbackAction> m_shortcutOpenFolderAction;
        //About
        std::shared_ptr<Gtk::Shortcut> m_shortcutAbout;
        std::shared_ptr<Gtk::KeyvalTrigger> m_shortcutAboutTrigger;
        std::shared_ptr<Gtk::CallbackAction> m_shortcutAboutAction;
        //==Slots==//
        void onShow();
        void openFolder();
        void checkForUpdates(const Glib::VariantBase& args);
        void gitHubRepo(const Glib::VariantBase& args);
        void reportABug(const Glib::VariantBase& args);
        void settings(const Glib::VariantBase& args);
        void shortcuts(const Glib::VariantBase& args);
        void changelog(const Glib::VariantBase& args);
        void about(const Glib::VariantBase& args);
    };
}

#endif // MAINWINDOW_H
