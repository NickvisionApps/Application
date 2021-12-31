#ifndef HEADERBAR_H
#define HEADERBAR_H

#include <memory>
#include <gtkmm.h>

namespace NickvisionApplication::Controls
{
    class HeaderBar : public Gtk::HeaderBar
    {
    public:
        HeaderBar();
        Glib::SignalProxy<void()> getSignalClickedOpenFolder();
        Glib::SignalProxy<void()> getSignalClickedSettings();
        Glib::SignalProxy<void(const Glib::VariantBase&)> getSignalActivateCheckForUpdates();
        Glib::SignalProxy<void(const Glib::VariantBase&)> getSignalActivateGitHubRepo();
        Glib::SignalProxy<void(const Glib::VariantBase&)> getSignalActivateReportABug();
        Glib::SignalProxy<void(const Glib::VariantBase&)> getSignalActivateChangelog();
        Glib::SignalProxy<void(const Glib::VariantBase&)> getSignalActivateAbout();

    private:
        Gtk::Button m_btnOpenFolder;
        Gtk::Button m_btnSettings;
        std::shared_ptr<Gio::SimpleActionGroup> m_actionHelp;
        std::shared_ptr<Gio::SimpleAction> m_actionCheckForUpdates;
        std::shared_ptr<Gio::SimpleAction> m_actionGitHubRepo;
        std::shared_ptr<Gio::SimpleAction> m_actionReportABug;
        std::shared_ptr<Gio::SimpleAction> m_actionChangelog;
        std::shared_ptr<Gio::SimpleAction> m_actionAbout;
        std::shared_ptr<Gio::Menu> m_menuHelp;
        std::shared_ptr<Gio::Menu> m_menuHelpUpdate;
        std::shared_ptr<Gio::Menu> m_menuHelpLinks;
        std::shared_ptr<Gio::Menu> m_menuHelpActions;
        Gtk::MenuButton m_btnHelp;
    };
}

#endif // HEADERBAR_H
