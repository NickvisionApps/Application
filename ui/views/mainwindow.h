#pragma once

#include <adwaita.h>
#include "../widget.h"
#include "../../models/configuration.h"
#include "../../update/updater.h"
#include "../messenger.h"
#include "homepage.h"
#include "formpage.h"

namespace NickvisionApplication::UI::Views
{
    class MainWindow : public NickvisionApplication::UI::Widget
    {
    public:
        MainWindow(NickvisionApplication::Models::Configuration& configuration);
        ~MainWindow();
        GtkWidget* gobj() override;
        void showMaximized();

    private:
        NickvisionApplication::Models::Configuration& m_configuration;
        NickvisionApplication::Update::Updater m_updater;
        NickvisionApplication::UI::Messenger m_messenger;
        bool m_opened;
        //==Help Actions==//
        GSimpleAction* m_gio_actUpdate;
        GSimpleAction* m_gio_actGitHubRepo;
        GSimpleAction* m_gio_actReportABug;
        GSimpleAction* m_gio_actPreferences;
        GSimpleAction* m_gio_actChangelog;
        GSimpleAction* m_gio_actAbout;
        //==Pages==//
        HomePage m_homePage;
        FormPage m_formPage;
        //==Signals==//
        void onStartup();
        void update();
        void gitHubRepo();
        void reportABug();
        void preferences();
        void changelog();
        void about();
        //==Messages==//
        void sendToast(const std::string& message);
    };
}
