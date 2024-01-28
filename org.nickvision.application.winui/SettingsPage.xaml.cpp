#include "SettingsPage.xaml.h"
#if __has_include("SettingsPage.g.cpp")
#include "SettingsPage.g.cpp"
#endif
#include <libnick/localization/gettext.h>

using namespace ::Nickvision::Application::Shared::Controllers;
using namespace ::Nickvision::Application::Shared::Models;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::Windows::AppLifecycle;

namespace winrt::Nickvision::Application::WinUI::implementation 
{
    SettingsPage::SettingsPage()
        : m_constructing{ true }
    {
        InitializeComponent();
        //Localize Strings
        LblTitle().Text(winrt::to_hstring(_("Settings")));
        LblUserInterface().Text(winrt::to_hstring(_("User Interface")));
        RowTheme().Title(winrt::to_hstring(_("Theme")));
        RowTheme().Description(winrt::to_hstring(_("An application restart is required to change the theme.")));
        CmbTheme().Items().Append(winrt::box_value(winrt::to_hstring(_p("Theme", "Light"))));
        CmbTheme().Items().Append(winrt::box_value(winrt::to_hstring(_p("Theme", "Dark"))));
        CmbTheme().Items().Append(winrt::box_value(winrt::to_hstring(_p("Theme", "System"))));
        RowAutomaticallyCheckForUpdates().Title(winrt::to_hstring(_("Automatically Check for Updates")));
        TglAutomaticallyCheckForUpdates().OnContent(winrt::box_value(winrt::to_hstring(_("On"))));
        TglAutomaticallyCheckForUpdates().OffContent(winrt::box_value(winrt::to_hstring(_("Off"))));
    }

    void SettingsPage::SetController(const std::shared_ptr<PreferencesViewController>& controller)
    {
        m_controller = controller;
        //Load
        m_constructing = true;
        CmbTheme().SelectedIndex(static_cast<int>(m_controller->getTheme()));
        TglAutomaticallyCheckForUpdates().IsOn(m_controller->getAutomaticallyCheckForUpdates());
        m_constructing = false;
    }

    Windows::Foundation::IAsyncAction SettingsPage::OnThemeChanged(const IInspectable& sender, const SelectionChangedEventArgs& args)
    {
        co_await ApplyChangesAsync();
    }

    Windows::Foundation::IAsyncAction SettingsPage::OnUpdatesToggled(const IInspectable& sender, const RoutedEventArgs& args)
    {
        co_await ApplyChangesAsync();
    }

    Windows::Foundation::IAsyncAction SettingsPage::ApplyChangesAsync()
    {
        if(!m_constructing)
        {
            bool needsRestart{ false };
            if(m_controller->getTheme() != static_cast<Theme>(CmbTheme().SelectedIndex()))
            {
                m_controller->setTheme(static_cast<Theme>(CmbTheme().SelectedIndex()));
                needsRestart = true;
            }
            m_controller->setAutomaticallyCheckForUpdates(TglAutomaticallyCheckForUpdates().IsOn());
            m_controller->saveConfiguration();
            if(needsRestart)
            {
                ContentDialog restartDialog;
                restartDialog.Title(winrt::box_value(winrt::to_hstring(_("Restart To Apply Theme?"))));
                restartDialog.Content(winrt::box_value(winrt::to_hstring(_("Would you like to restart the app to apply the new theme?\nAny unsaved work will be lost."))));
                restartDialog.PrimaryButtonText(winrt::to_hstring(_("Yes")));
                restartDialog.CloseButtonText(winrt::to_hstring(_("No")));
                restartDialog.DefaultButton(ContentDialogButton::Primary);
                restartDialog.XamlRoot(XamlRoot());
                ContentDialogResult res{ co_await restartDialog.ShowAsync() };
                if(res == ContentDialogResult::Primary)
                {
                    AppInstance::Restart(L"Apply new theme");
                }
            }
        }
    }
}
