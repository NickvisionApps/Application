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
        Gtk::Button& getBtnOpenFolder();
        Gtk::Button& getBtnSettings();
        std::shared_ptr<Gio::SimpleAction>& getActionCheckForUpdates();
        std::shared_ptr<Gio::SimpleAction>& getActionGitHubRepo();
        std::shared_ptr<Gio::SimpleAction>& getActionReportABug();
        std::shared_ptr<Gio::SimpleAction>& getActionChangelog();
        std::shared_ptr<Gio::SimpleAction>& getActionAbout();

    private:
        //==Open Folder==//
        Gtk::Button m_btnOpenFolder;
        //==Settings==//
        Gtk::Button m_btnSettings;
        //==Help==//
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
