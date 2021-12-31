#include "headerbar.h"

namespace NickvisionApplication::Controls
{
    HeaderBar::HeaderBar()
    {
        //==Open Folder==//
        m_btnOpenFolder.set_icon_name("folder-open");
        m_btnOpenFolder.set_tooltip_text("Open Folder");
        //==Help==//
        m_actionHelp = Gio::SimpleActionGroup::create();
        m_actionCheckForUpdates = m_actionHelp->add_action("checkForUpdates");
        m_actionGitHubRepo = m_actionHelp->add_action("gitHubRepo");
        m_actionReportABug = m_actionHelp->add_action("reportABug");
        m_actionChangelog = m_actionHelp->add_action("changelog");
        m_actionAbout = m_actionHelp->add_action("about");
        insert_action_group("help", m_actionHelp);
        m_menuHelp = Gio::Menu::create();
        std::shared_ptr<Gio::Menu> menuHelpUpdate = Gio::Menu::create();
        menuHelpUpdate->append("Check for Updates", "help.checkForUpdates");
        std::shared_ptr<Gio::Menu> menuHelpLinks = Gio::Menu::create();
        menuHelpLinks->append("GitHub Repo", "help.gitHubRepo");
        menuHelpLinks->append("Report a Bug", "help.reportABug");
        std::shared_ptr<Gio::Menu> menuHelpActions = Gio::Menu::create();
        menuHelpActions->append("Changelog", "help.changelog");
        menuHelpActions->append("About", "help.about");
        m_menuHelp->append_section(menuHelpUpdate);
        m_menuHelp->append_section(menuHelpLinks);
        m_menuHelp->append_section(menuHelpActions);
        m_btnMenuHelp.set_direction(Gtk::ArrowType::NONE);
        m_btnMenuHelp.set_menu_model(m_menuHelp);
        //==Settings==//
        m_btnSettings.set_icon_name("settings");
        m_btnSettings.set_tooltip_text("Settings");

        //==Layout==//
        pack_start(m_btnOpenFolder);
        pack_end(m_btnMenuHelp);
        pack_end(m_btnSettings);
    }

    Glib::SignalProxy<void()> HeaderBar::getSignalClickedOpenFolder()
    {
        return m_btnOpenFolder.signal_clicked();
    }

    Glib::SignalProxy<void()> HeaderBar::getSignalClickedSettings()
    {
        return m_btnSettings.signal_clicked();
    }

    Glib::SignalProxy<void(const Glib::VariantBase&)> HeaderBar::getSignalActivateCheckForUpdates()
    {
        return m_actionCheckForUpdates->signal_activate();
    }

    Glib::SignalProxy<void(const Glib::VariantBase&)> HeaderBar::getSignalActivateGitHubRepo()
    {
        return m_actionGitHubRepo->signal_activate();
    }

    Glib::SignalProxy<void(const Glib::VariantBase&)> HeaderBar::getSignalActivateReportABug()
    {
        return m_actionReportABug->signal_activate();
    }

    Glib::SignalProxy<void(const Glib::VariantBase&)> HeaderBar::getSignalActivateChangelog()
    {
        return m_actionChangelog->signal_activate();
    }

    Glib::SignalProxy<void(const Glib::VariantBase&)> HeaderBar::getSignalActivateAbout()
    {
        return m_actionAbout->signal_activate();
    }
}
