#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pch.h"
#include "MainWindow.g.h"
#include <memory>
#include "controllers/mainwindowcontroller.h"

namespace winrt::Nickvision::Application::WinUI::implementation
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

    private:
        HWND m_hwnd;
        std::shared_ptr<::Nickvision::Application::Shared::Controllers::MainWindowController> m_controller;
        Microsoft::UI::Xaml::ElementTheme m_systemTheme;
        bool m_opened;
    };
}

namespace winrt::Nickvision::Application::WinUI::factory_implementation
{
    struct MainWindow : public MainWindowT<MainWindow, implementation::MainWindow> { };
}

#endif //MAINWINDOW_H