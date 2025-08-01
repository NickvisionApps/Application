#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pch.h"
#include "Controls/TitleBar.g.h"
#include "Controls/ViewStack.g.h"
#include "Views/MainWindow.g.h"
#include <memory>
#include "controllers/mainwindowcontroller.h"

namespace winrt::Nickvision::Application::WinUI::Views::implementation
{
    /**
     * @brief The main window for the application.
     */
    struct MainWindow : MainWindowT<MainWindow>
    {
    public:
        /**
         * @brief Constructs a MainWindow.
         */
        MainWindow();
        /**
         * @brief Sets the controller for the main window.
         * @param controller The MainWindowController
         */
        void Controller(const std::shared_ptr<::Nickvision::Application::Shared::Controllers::MainWindowController>& controller);
        /**
         * @brief Sets the system theme for the main window.
         * @param theme The Microsoft::UI::Xaml::ElementTheme
         */
        void SystemTheme(Microsoft::UI::Xaml::ElementTheme theme);
        /**
         * @brief Handles when the main window is loaded.
         * @param sender IInspectable
         * @param args Microsoft::UI::Xaml::RoutedEventArgs
         * @throw std::logic_error Thrown if the controller has not been set before loading the window
         */
        void OnLoaded(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args);
        /**
         * @brief Handles when the main window is closing.
         * @param sender Microsoft::UI::Windowing::AppWindow
         * @param args Microsoft::UI::Windowing::AppWindowClosingEventArgs
         */
        void OnClosing(const Microsoft::UI::Windowing::AppWindow& sender, const Microsoft::UI::Windowing::AppWindowClosingEventArgs& args);\
        /**
         * @brief Handles when the main window is activated.
         * @param sender IInspectable
         * @param args Microsoft::UI::Xaml::WindowActivatedEventArgs
         */
        void OnActivated(const IInspectable& sender, const Microsoft::UI::Xaml::WindowActivatedEventArgs& args);
        /**
         * @brief Handles when the app's configuration is saved.
         * @param args Nickvision::Events::EventArgs
         */
        void OnConfigurationSaved(const ::Nickvision::Events::EventArgs& args);
        /**
         * @brief Handles when a notification is sent.
         * @param args Nickvision::Notifications::NotificationSentEventArgs
         */
        void OnNotificationSent(const ::Nickvision::Notifications::NotificationSentEventArgs& args);
        /**
        * @brief Handles when a change in the window's navigation occurs.
        * @param sender Microsoft::UI::Xaml::Controls::NavigationView
        * @param args Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs
        */
        void OnNavViewSelectionChanged(const Microsoft::UI::Xaml::Controls::NavigationView& sender, const Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs& args);
        /**
         * @brief Handles when a navigation item is tapped (to display it's flyout).
         * @param sender IInspectable
         * @param args Microsoft::UI::Xaml::Input::TappedRoutedEventArgs
         */
        void OnNavViewItemTapped(const IInspectable& sender, const Microsoft::UI::Xaml::Input::TappedRoutedEventArgs& args);
        /**
         * @brief Checks for an update to the application.
         * @param sender IInspectable
         * @param args Microsoft::UI::Xaml::RoutedEventArgs
         */
        void CheckForUpdates(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args);
        /**
         * @brief Opens the application's GitHub repo.
         * @param sender IInspectable
         * @param args Microsoft::UI::Xaml::RoutedEventArgs
         */
        Windows::Foundation::IAsyncAction GitHubRepo(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args);
        /**
         * @brief Opens the application's issue tracker.
         * @param sender IInspectable
         * @param args Microsoft::UI::Xaml::RoutedEventArgs
         */
        Windows::Foundation::IAsyncAction ReportABug(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args);
        /**
         * @brief Opens the application's support page.
         * @param sender IInspectable
         * @param args Microsoft::UI::Xaml::RoutedEventArgs
         */
        Windows::Foundation::IAsyncAction Discussions(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args);
        /**
        * @brief Opens the about dialog.
        * @param sender IInspectable
        * @param args Microsoft::UI::Xaml::RoutedEventArgs
        */
        Windows::Foundation::IAsyncAction About(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args);
        /**
         * @brief Handles when the folder is changed.
         * @param args Nickvision::Events::EventArgs
         */
        void OnFolderChanged(const ::Nickvision::Events::EventArgs& args);
        /**
        * @brief Prompts the user to open a folder.
        * @param sender IInspectable
        * @param args Microsoft::UI::Xaml::RoutedEventArgs
        */
        Windows::Foundation::IAsyncAction OpenFolder(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args);
        /**
        * @brief Closes the currently opened folder.
        * @param sender IInspectable
        * @param args Microsoft::UI::Xaml::RoutedEventArgs
        */
        void CloseFolder(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args);

    private:
        HWND m_hwnd;
        std::shared_ptr<::Nickvision::Application::Shared::Controllers::MainWindowController> m_controller;
        Microsoft::UI::Xaml::ElementTheme m_systemTheme;
        bool m_opened;
        winrt::event_token m_notificationClickToken;
    };
}

namespace winrt::Nickvision::Application::WinUI::Views::factory_implementation
{
    struct MainWindow : public MainWindowT<MainWindow, implementation::MainWindow> { };
}

#endif //MAINWINDOW_H
