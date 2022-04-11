#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <adwaita.h>
#include "../widget.h"
#include "../../update/updater.h"
#include "welcomepage.h"
#include "formpage.h"

namespace NickvisionApplication::UI::Views
{
    class MainWindow : public NickvisionApplication::UI::Widget
    {
    public:
        MainWindow();
        ~MainWindow();
        GtkWidget* gobj() override;

    private:
        NickvisionApplication::Update::Updater m_updater;
        bool m_opened;
        //==Help Actions==//
        GSimpleAction* m_gio_actUpdate = nullptr;
        GSimpleAction* m_gio_actGitHubRepo = nullptr;
        GSimpleAction* m_gio_actReportABug = nullptr;
        GSimpleAction* m_gio_actPreferences = nullptr;
        GSimpleAction* m_gio_actChangelog = nullptr;
        GSimpleAction* m_gio_actAbout = nullptr;
        //==Pages==//
        WelcomePage m_welcomePage;
        FormPage m_formPage;
        //==Signals==//
        void onStartup();
        void update();
        void gitHubRepo();
        void reportABug();
        void preferences();
        void changelog();
        void about();
        void onNavigationChanged();
        //==Other Functions==//
        void sendToast(const std::string& message);
        void changePage(const std::string& pageName);
    };
}

#endif // MAINWINDOW_H
