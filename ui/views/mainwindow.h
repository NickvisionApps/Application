#pragma once

#include <string>
#include <adwaita.h>
#include "../widget.h"
#include "../../models/configuration.h"
#include "../../update/updater.h"
#include "../messenger.h"

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
        GSimpleAction* m_gio_actUpdate = nullptr;
        GSimpleAction* m_gio_actGitHubRepo = nullptr;
        GSimpleAction* m_gio_actReportABug = nullptr;
        GSimpleAction* m_gio_actPreferences = nullptr;
        GSimpleAction* m_gio_actChangelog = nullptr;
        GSimpleAction* m_gio_actAbout = nullptr;
        //==Signals==//
        void onStartup();
        void update();
        void gitHubRepo();
        void reportABug();
        void preferences();
        void changelog();
        void about();
        //==Other Functions==//
        void sendToast(const std::string& message);
    };
}
