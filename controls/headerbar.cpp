#include "headerbar.h"

namespace NickvisionApplication::Controls
{
    HeaderBar::HeaderBar()
    {
        //==Title==//
        m_boxTitle.set_orientation(Gtk::Orientation::VERTICAL);
        m_boxTitle.set_halign(Gtk::Align::CENTER);
        m_boxTitle.set_valign(Gtk::Align::CENTER);
        m_lblTitle.get_style_context()->add_class("title");
        m_lblSubtitle.get_style_context()->add_class("subtitle");
        m_boxTitle.append(m_lblTitle);
        //==Open Folder==//
        m_btnOpenFolder.set_icon_name("folder-open");
        m_btnOpenFolder.set_tooltip_text("Open Folder");
        //==Help==//
        m_actionHelp = Gio::SimpleActionGroup::create();
        m_actionCheckForUpdates = m_actionHelp->add_action("checkForUpdates");
        m_actionGitHubRepo = m_actionHelp->add_action("gitHubRepo");
        m_actionReportABug = m_actionHelp->add_action("reportABug");
        m_actionSettings = m_actionHelp->add_action("settings");
        m_actionShortcuts = m_actionHelp->add_action("shortcuts");
        m_actionChangelog = m_actionHelp->add_action("changelog");
        m_actionAbout = m_actionHelp->add_action("about");
        insert_action_group("help", m_actionHelp);
        m_menuHelp = Gio::Menu::create();
        m_menuHelpUpdate = Gio::Menu::create();
        m_menuHelpUpdate->append("Check for Updates", "help.checkForUpdates");
        m_menuHelpLinks = Gio::Menu::create();
        m_menuHelpLinks->append("GitHub Repo", "help.gitHubRepo");
        m_menuHelpLinks->append("Report a Bug", "help.reportABug");
        m_menuHelpActions = Gio::Menu::create();
        m_menuHelpActions->append("Settings", "help.settings");
        m_menuHelpActions->append("Keyboard Shortcuts", "help.shortcuts");
        m_menuHelpActions->append("Changelog", "help.changelog");
        m_menuHelpActions->append("About Application", "help.about");
        m_menuHelp->append_section(m_menuHelpUpdate);
        m_menuHelp->append_section(m_menuHelpLinks);
        m_menuHelp->append_section(m_menuHelpActions);
        m_btnHelp.set_direction(Gtk::ArrowType::NONE);
        m_btnHelp.set_menu_model(m_menuHelp);
        m_btnHelp.set_tooltip_text("Help");
        //==Layout==//
        set_title_widget(m_boxTitle);
        pack_start(m_btnOpenFolder);
        pack_end(m_btnHelp);
    }

    void HeaderBar::setTitle(const std::string& title)
    {
        m_lblTitle.set_text(title);
    }

    void HeaderBar::setSubtitle(const std::string& subtitle)
    {
        m_boxTitle.remove(m_lblSubtitle);
        if(!subtitle.empty())
        {
            m_boxTitle.append(m_lblSubtitle);
            m_lblSubtitle.set_text(subtitle);
        }
    }

    Gtk::Button& HeaderBar::getBtnOpenFolder()
    {
        return m_btnOpenFolder;
    }

    const std::shared_ptr<Gio::SimpleAction>& HeaderBar::getActionCheckForUpdates() const
    {
        return m_actionCheckForUpdates;
    }

    const std::shared_ptr<Gio::SimpleAction>& HeaderBar::getActionGitHubRepo() const
    {
        return m_actionGitHubRepo;
    }

    const std::shared_ptr<Gio::SimpleAction>& HeaderBar::getActionReportABug() const
    {
        return m_actionReportABug;
    }

    const std::shared_ptr<Gio::SimpleAction>& HeaderBar::getActionSettings() const
    {
        return m_actionSettings;
    }

    const std::shared_ptr<Gio::SimpleAction>& HeaderBar::getActionShortcuts() const
    {
        return m_actionShortcuts;
    }

    const std::shared_ptr<Gio::SimpleAction>& HeaderBar::getActionChangelog() const
    {
        return m_actionChangelog;
    }

    const std::shared_ptr<Gio::SimpleAction>& HeaderBar::getActionAbout() const
    {
        return m_actionAbout;
    }
}
