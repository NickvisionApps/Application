#include "Views/MainWindow.xaml.h"
#if __has_include("Views/MainWindow.g.cpp")
#include "Views/MainWindow.g.cpp"
#endif
#include <cmath>
#include <stdexcept>
#include <libnick/localization/gettext.h>
#include <libnick/notifications/appnotification.h>
#include "Views/SettingsPage.xaml.h"

using namespace ::Nickvision::Application::Shared::Controllers;
using namespace ::Nickvision::Application::Shared::Models;
using namespace ::Nickvision::Events;
using namespace ::Nickvision::Notifications;
using namespace ::Nickvision::Update;
using namespace winrt::Microsoft::UI::Dispatching;
using namespace winrt::Microsoft::UI::Windowing;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::UI::Xaml::Controls::Primitives;
using namespace winrt::Microsoft::UI::Xaml::Input;
using namespace winrt::Windows::ApplicationModel::DataTransfer;
using namespace winrt::Windows::Graphics;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Pickers;
using namespace winrt::Windows::System;

enum MainWindowPage
{
    Home = 0,
    Folder,
    UpdateCenter,
    Custom
};

enum UpdateCenterPage
{
    NoUpdates = 0,
    UpdatesAvailable,
    DownloadingUpdate,
};

namespace winrt::Nickvision::Application::WinUI::Views::implementation
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
        m_controller->configurationSaved() += [this](const EventArgs& args){ DispatcherQueue().TryEnqueue([this, args](){ OnConfigurationSaved(args); }); };
        m_controller->notificationSent() += [this](const NotificationSentEventArgs& args){ DispatcherQueue().TryEnqueue([this, args](){ OnNotificationSent(args); }); };
        m_controller->appUpdateAvailable() += [this](const ParamEventArgs<Version>& args){ DispatcherQueue().TryEnqueue([this, args](){ OnAppUpdateAvailable(args); }); };
        m_controller->appUpdateProgressChanged() += [this](const ParamEventArgs<double>& args){ DispatcherQueue().TryEnqueue([this, args](){ OnAppUpdateProgressChanged(args); }); };
        m_controller->folderChanged() += [this](const EventArgs& args){ OnFolderChanged(args); };
        //Localize Strings
        TitleBar().Title(winrt::to_hstring(m_controller->getAppInfo().getShortName()));
        TitleBar().Subtitle(m_controller->getAppInfo().getVersion().getVersionType() == VersionType::Preview ? winrt::to_hstring(_("Preview")) : L"");
        NavViewHome().Content(winrt::box_value(winrt::to_hstring(_("Home"))));
        NavViewFolder().Content(winrt::box_value(winrt::to_hstring(_("Folder"))));
        NavViewUpdates().Content(winrt::box_value(winrt::to_hstring(_("Updates"))));
        NavViewSettings().Content(winrt::box_value(winrt::to_hstring(_("Settings"))));
        LblGreeting().Text(winrt::to_hstring(m_controller->getGreeting()));
        LblGettingStarted().Text(winrt::to_hstring(_("Open a folder (or drag one into the app) to get started")));
        LblHomeStart().Text(winrt::to_hstring(_("Start")));
        LblHomeOpenFolder().Text(winrt::to_hstring(_("Open Folder")));
        BtnHomeGitHubRepo().Label(winrt::to_hstring(_("GitHub Repo")));
        BtnHomeReportABug().Label(winrt::to_hstring(_("Report a Bug")));
        BtnHomeDiscussions().Label(winrt::to_hstring(_("Discussions")));
        LblFolder().Text(winrt::to_hstring(_("Folder")));
        LblFolderOpenFolder().Text(winrt::to_hstring(_("Open")));
        BtnFolderCloseFolder().Label(winrt::to_hstring(_("Close")));
        LblFiles().Text(winrt::to_hstring(_("No Folder Opened")));
        LblUpdateCenter().Text(winrt::to_hstring(_("Updates")));
        LblNoUpdates().Text(winrt::to_hstring(_("You're up to date")));
        LblNoUpdatesDetails().Text(winrt::to_hstring(_("We will let you know once app updates are available to install")));
        LblUpdatesAvailable().Text(winrt::to_hstring(_("There is an update available")));
        LblDownloadUpdate().Text(winrt::to_hstring(_("Download")));
        LblUpdatesDownloading().Text(winrt::to_hstring(_("Downloading the update")));
        LblUpdatesOptions().Text(winrt::to_hstring(_("Options")));
        RowUpdatesBeta().Title(winrt::to_hstring(_("Receive Beta App Updates")));
        RowUpdatesBeta().Description(winrt::to_hstring(_f("Check for pre-release (beta) versions of {}", m_controller->getAppInfo().getShortName())));
        TglUpdatesBeta().OnContent(winrt::box_value(winrt::to_hstring(_("On"))));
        TglUpdatesBeta().OffContent(winrt::box_value(winrt::to_hstring(_("Off"))));
        LblUpdatesChangelogTitle().Text(winrt::to_hstring(_("Changelog")));
        LblUpdatesChangelogVersion().Text(winrt::to_hstring(_f("Version {}", m_controller->getAppInfo().getVersion().str())));
        LblUpdatesChangelog().Text(winrt::to_hstring(m_controller->getAppInfo().getChangelog()));
        BtnCopyDebugInformation().Label(winrt::to_hstring(_("Copy Debug Information")));
        //Load
        TglUpdatesBeta().IsOn(m_controller->getPreferredUpdateType() == VersionType::Preview);
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
        //Load UI
        ViewStackUpdateCenter().CurrentPageIndex(UpdateCenterPage::NoUpdates);
        NavViewHome().IsSelected(true);
        //Startup
        const StartupInformation& info{ m_controller->startup() };
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

    void MainWindow::OnPaneToggleRequested(const Microsoft::UI::Xaml::Controls::TitleBar& sender, const IInspectable& args)
    {
        NavView().IsPaneOpen(!NavView().IsPaneOpen());
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
            m_notificationClickToken = BtnInfoBar().Click({ this, &MainWindow::CloseFolder });
        }
        BtnInfoBar().Visibility(!args.getAction().empty() ? Visibility::Visible : Visibility::Collapsed);
        InfoBar().IsOpen(true);
    }

    void MainWindow::OnAppUpdateAvailable(const ParamEventArgs<Version>& args)
    {
        InfoBadgeUpdates().Visibility(Visibility::Visible);
        ViewStackUpdateCenter().CurrentPageIndex(UpdateCenterPage::UpdatesAvailable);
        LblUpdatesAvailableDetails().Text(winrt::to_hstring(_f("{} version {} is available to download and install", m_controller->getAppInfo().getShortName(), (*args).str())));
    }

    void MainWindow::OnAppUpdateProgressChanged(const ParamEventArgs<double>& args)
    {
        ViewStackUpdateCenter().CurrentPageIndex(UpdateCenterPage::DownloadingUpdate);
        LblUpdatesDownloadingDetails().Text(winrt::to_hstring(_("The installer will start once the download is complete")));
        LblUpdatesDownloadingProgress().Text(winrt::to_hstring(_f("Downloading {}%", static_cast<int>(std::round((*args) * 100.0)))));
        if(*args == 1.0)
        {
            InfoBadgeUpdates().Visibility(Visibility::Collapsed);
            ViewStackUpdateCenter().CurrentPageIndex(UpdateCenterPage::NoUpdates);
        }
    }

    void MainWindow::OnAppUpdateBetaToggled(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args)
    {
        m_controller->setPreferredUpdateType(TglUpdatesBeta().IsOn() ? VersionType::Preview : VersionType::Stable);
    }

    void MainWindow::OnNavViewSelectionChanged(const NavigationView& sender, const NavigationViewSelectionChangedEventArgs& args)
    {
        winrt::hstring tag{ NavView().SelectedItem().as<NavigationViewItem>().Tag().as<winrt::hstring>() };
        if(tag == L"Home")
        {
            ViewStack().CurrentPageIndex(MainWindowPage::Home);
        }
        else if(tag == L"Folder")
        {
            ViewStack().CurrentPageIndex(MainWindowPage::Folder);
        }
        else if(tag == L"Updates")
        {
            ViewStack().CurrentPageIndex(MainWindowPage::UpdateCenter);
        }
        else if(tag == L"Settings")
        {
            ViewStack().CurrentPageIndex(MainWindowPage::Custom);
            PageCustom().Content(winrt::make<implementation::SettingsPage>());
            PageCustom().Content().as<implementation::SettingsPage>()->Controller(m_controller->createPreferencesViewController());
        }
    }

    void MainWindow::OnNavViewItemTapped(const IInspectable& sender, const TappedRoutedEventArgs& args)
    {
        FlyoutBase::ShowAttachedFlyout(sender.as<FrameworkElement>());
    }

    Windows::Foundation::IAsyncAction MainWindow::GitHubRepo(const IInspectable& sender, const RoutedEventArgs& args)
    {
        co_await Launcher::LaunchUriAsync(Windows::Foundation::Uri{ winrt::to_hstring(m_controller->getAppInfo().getSourceRepo()) });
    }

    Windows::Foundation::IAsyncAction MainWindow::ReportABug(const IInspectable& sender, const RoutedEventArgs& args)
    {
        co_await Launcher::LaunchUriAsync(Windows::Foundation::Uri{ winrt::to_hstring(m_controller->getAppInfo().getIssueTracker()) });
    }

    Windows::Foundation::IAsyncAction MainWindow::Discussions(const IInspectable& sender, const RoutedEventArgs& args)
    {
        co_await Launcher::LaunchUriAsync(Windows::Foundation::Uri{ winrt::to_hstring(m_controller->getAppInfo().getSupportUrl()) });
    }

    void MainWindow::CopyDebugInformation(const IInspectable& sender, const RoutedEventArgs& args)
    {
        DataPackage dataPackage;
        dataPackage.SetText(winrt::to_hstring(m_controller->getDebugInformation()));
        Clipboard::SetContent(dataPackage);
        AppNotification::send({ _("Debug information copied to clipboard"), NotificationSeverity::Success });
    }

    void MainWindow::DownloadUpdate(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args)
    {
        m_controller->startWindowsUpdate();
    }

    void MainWindow::OnFolderChanged(const EventArgs& args)
    {
        NavViewHome().IsSelected(!m_controller->isFolderOpened());
        NavViewFolder().IsSelected(m_controller->isFolderOpened());
        ListFiles().Items().Clear();
        if(m_controller->isFolderOpened())
        {
            LblFolder().Text(winrt::to_hstring(_f("Folder: {}", m_controller->getFolderPath().string())));
            LblFiles().Text(winrt::to_hstring(_fn("There is {} file in the folder.", "There are {} files in the folder.", m_controller->getFiles().size(), m_controller->getFiles().size())));
            for(const std::filesystem::path& file : m_controller->getFiles())
            {
                ListFiles().Items().Append(winrt::box_value(winrt::to_hstring(file.filename().string())));
            }
        }
        else
        {
            LblFolder().Text(winrt::to_hstring(_("Folder")));
            LblFiles().Text(winrt::to_hstring(_("No Folder Opened")));
        }
    }

    Windows::Foundation::IAsyncAction MainWindow::OpenFolder(const IInspectable& sender, const RoutedEventArgs& args)
    {
        FolderPicker picker;
        picker.as<::IInitializeWithWindow>()->Initialize(m_hwnd);
        picker.FileTypeFilter().Append(L"*");
        StorageFolder folder{ co_await picker.PickSingleFolderAsync() };
        if(folder)
        {
            m_controller->openFolder(winrt::to_string(folder.Path()));
        }
    }

    void MainWindow::CloseFolder(const IInspectable& sender, const RoutedEventArgs& args)
    {
        InfoBar().IsOpen(false);
        m_controller->closeFolder();
    }
}
