#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include <stdexcept>
#include <libnick/localization/gettext.h>

using namespace ::Nickvision::Application::Shared::Controllers;
using namespace ::Nickvision::Application::Shared::Models;
using namespace winrt::Microsoft::UI::Windowing;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Windows::Graphics;

namespace winrt::Nickvision::Application::WinUI::implementation
{
    MainWindow::MainWindow()
        : m_opened{ false }
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
        //Register Events
        //Localize Strings
        LblAppTitle().Text(winrt::to_hstring(m_controller->getAppInfo().getShortName()));
        MenuFile().Title(winrt::to_hstring(_("File")));
        MenuEdit().Title(winrt::to_hstring(_("Edit")));
        MenuHelp().Title(winrt::to_hstring(_("Help")));
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
        m_opened = true;
    }
}