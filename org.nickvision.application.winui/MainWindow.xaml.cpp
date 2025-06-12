#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include <stdexcept>
#include <libnick/localization/gettext.h>

using namespace ::Nickvision::Application::Shared::Controllers;
using namespace ::Nickvision::Application::Shared::Models;
using namespace ::Nickvision::Events;
using namespace ::Nickvision::Notifications;
using namespace winrt::Microsoft::UI::Dispatching;
using namespace winrt::Microsoft::UI::Windowing;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::UI::Xaml::Controls::Primitives;
using namespace winrt::Microsoft::UI::Xaml::Input;
using namespace winrt::Windows::Graphics;

namespace winrt::Nickvision::Application::WinUI::implementation
{
    MainWindow::MainWindow()
        : m_opened{ false },
        m_notificationClickToken{ 0 }
    {
        InitializeComponent();
        this->m_inner.as<::IWindowNative>()->get_WindowHandle(&m_hwnd);
        ExtendsContentIntoTitleBar(true);
        SetTitleBar(TitleBar());
        AppWindow().TitleBar().PreferredHeightOption(TitleBarHeightOption::Tall);
    }

    void MainWindow::Controller(const std::shared_ptr<MainWindowController>& controller)
    {
        m_controller = controller;
        //Register Events
        AppWindow().Closing({ this, &MainWindow::OnClosing });
        m_controller->configurationSaved() += [&](const EventArgs& args){ OnConfigurationSaved(args); };
        m_controller->notificationSent() += [&](const NotificationSentEventArgs& args){ DispatcherQueue().TryEnqueue([this, args](){ OnNotificationSent(args); }); };
        //Localize Strings
        LblAppTitle().Text(winrt::to_hstring(m_controller->getAppInfo().getShortName()));
        NavViewHome().Content(winrt::box_value(winrt::to_hstring(_("Home"))));
        NavViewFolder().Content(winrt::box_value(winrt::to_hstring(_("Folder"))));
        NavViewHelp().Content(winrt::box_value(winrt::to_hstring(_("Help"))));
        NavViewSettings().Content(winrt::box_value(winrt::to_hstring(_("Settings"))));
        MenuCheckForUpdates().Text(winrt::to_hstring(_("Check for Updates")));
        MenuGitHubRepo().Text(winrt::to_hstring(_("GitHub Repo")));
        MenuReportABug().Text(winrt::to_hstring(_("Report a Bug")));
        MenuDiscussions().Text(winrt::to_hstring(_("Discussions")));
        MenuAbout().Text(winrt::to_hstring(_("About")));
    }

    void MainWindow::SystemTheme(ElementTheme theme)
    {
        m_systemTheme = theme;
    }

    void MainWindow::OnLoaded(const IInspectable& sender, const RoutedEventArgs& args)
    {
        if (!m_controller)
        {
            throw std::logic_error("MainWindow::SetController() must be called before using the window.");
        }
        if(m_opened)
        {
            return;
        }
        //Startup
        const StartupInformation& info{ m_controller->startup(m_hwnd) };
        if(info.getWindowGeometry().isMaximized())
        {
            ShowWindow(m_hwnd, SW_MAXIMIZE);
        }
        else
        {
            RectInt32 size;
            size.Width = info.getWindowGeometry().getWidth();
            size.Height = info.getWindowGeometry().getHeight();
            size.X = info.getWindowGeometry().getX();
            size.Y = info.getWindowGeometry().getY();
            AppWindow().MoveAndResize(size);
        }
        NavViewHome().IsSelected(true);
        m_opened = true;
    }

    void MainWindow::OnClosing(const Microsoft::UI::Windowing::AppWindow& sender, const AppWindowClosingEventArgs& args)
    {
        if(!m_controller->canShutdown())
        {
            args.Cancel(true);
            return;
        }
        m_controller->shutdown({ AppWindow().Size().Width, AppWindow().Size().Height, static_cast<bool>(IsZoomed(m_hwnd)), AppWindow().Position().X, AppWindow().Position().Y });
    }

    void MainWindow::OnConfigurationSaved(const ::Nickvision::Events::EventArgs&)
    {
        switch (m_controller->getTheme())
        {
        case Theme::Light:
            MainGrid().RequestedTheme(ElementTheme::Light);
            break;
        case Theme::Dark:
            MainGrid().RequestedTheme(ElementTheme::Dark);
            break;
        default:
            MainGrid().RequestedTheme(m_systemTheme);
            break;
        }
    }

    void MainWindow::OnNotificationSent(const NotificationSentEventArgs& args)
    {
        InfoBar().Message(winrt::to_hstring(args.getMessage()));
        switch(args.getSeverity())
        {
        case NotificationSeverity::Success:
            InfoBar().Severity(InfoBarSeverity::Success);
            break;
        case NotificationSeverity::Warning:
            InfoBar().Severity(InfoBarSeverity::Warning);
            break;
        case NotificationSeverity::Error:
            InfoBar().Severity(InfoBarSeverity::Error);
            break;
        default:
            InfoBar().Severity(InfoBarSeverity::Informational);
            break;
        }
        if(m_notificationClickToken)
        {
            BtnInfoBar().Click(m_notificationClickToken);
        }
        if(args.getAction() == "close")
        {
            BtnInfoBar().Content(winrt::box_value(winrt::to_hstring(_("Close"))));
            //TODO: m_notificationClickToken = BtnInfoBar().Click({ this, &MainWindow::CloseFolder });
        }
        else if(args.getAction() == "update")
        {
            BtnInfoBar().Content(winrt::box_value(winrt::to_hstring(_("Update"))));
            m_notificationClickToken = BtnInfoBar().Click([this](const IInspectable&, const RoutedEventArgs&)
            {
                m_controller->windowsUpdate();
            });
        }
        BtnInfoBar().Visibility(!args.getAction().empty() ? Visibility::Visible : Visibility::Collapsed);
        InfoBar().IsOpen(true);
    }

    void MainWindow::OnNavViewSelectionChanged(const NavigationView& sender, const NavigationViewSelectionChangedEventArgs& args)
    {

    }

    void MainWindow::OnNavViewItemTapped(const IInspectable& sender, const TappedRoutedEventArgs& args)
    {
        FlyoutBase::ShowAttachedFlyout(sender.as<FrameworkElement>());
    }
}