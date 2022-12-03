using Microsoft.UI;
using Microsoft.UI.Composition;
using Microsoft.UI.Composition.SystemBackdrops;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Media;
using NickvisionApplication.Shared.Models;
using System;
using Vanara.PInvoke;
using WinRT;
using WinRT.Interop;

namespace NickvisionApplication.WinUI.Views;

/// <summary>
/// The MainWindow for the application
/// </summary>
public sealed partial class MainWindow : Window
{
    private readonly IntPtr _hwnd;
    private readonly AppWindow _appWindow;
    private bool _isActived;
    private readonly SystemBackdropConfiguration _backdropConfiguration;
    private readonly MicaController? _micaController;
    private readonly DesktopAcrylicController? _acrylicController;

    /// <summary>
    /// Constructs a MainWindow
    /// </summary>
    public MainWindow()
    {
        InitializeComponent();
        //Initialize Vars
        _hwnd = WindowNative.GetWindowHandle(this);
        _appWindow = AppWindow.GetFromWindowId(Win32Interop.GetWindowIdFromWindow(_hwnd));
        _isActived = true;
        //Register Events
        _appWindow.Closing += Window_Closing;
        //Set TitleBar
        TitleBarTitle.Text = AppInfo.Current.ShortName;
        _appWindow.Title = TitleBarTitle.Text;
        if (AppWindowTitleBar.IsCustomizationSupported())
        {
            _appWindow.TitleBar.ExtendsContentIntoTitleBar = true;
            TitleBarLeftPaddingColumn.Width = new GridLength(_appWindow.TitleBar.LeftInset);
            TitleBarRightPaddingColumn.Width = new GridLength(_appWindow.TitleBar.RightInset);
            _appWindow.TitleBar.ButtonBackgroundColor = Colors.Transparent;
            _appWindow.TitleBar.ButtonInactiveBackgroundColor = Colors.Transparent;
        }
        else
        {
            TitleBar.Visibility = Visibility.Collapsed;
            NavView.Margin = new Thickness(0, 0, 0, 0);
        }
        //Setup Backdrop
        WindowsSystemDispatcherQueueHelper.EnsureWindowsSystemDispatcherQueueController();
        _backdropConfiguration = new SystemBackdropConfiguration()
        {
            IsInputActive = true,
            Theme = ((FrameworkElement)Content).ActualTheme switch
            {
                ElementTheme.Default => SystemBackdropTheme.Default,
                ElementTheme.Light => SystemBackdropTheme.Light,
                ElementTheme.Dark => SystemBackdropTheme.Dark,
                _ => SystemBackdropTheme.Default
            }
        };
        if (MicaController.IsSupported())
        {
            _micaController = new MicaController();
            _micaController.AddSystemBackdropTarget(this.As<ICompositionSupportsSystemBackdrop>());
            _micaController.SetSystemBackdropConfiguration(_backdropConfiguration);
        }
        else if (DesktopAcrylicController.IsSupported())
        {
            _acrylicController = new DesktopAcrylicController();
            _acrylicController.AddSystemBackdropTarget(this.As<ICompositionSupportsSystemBackdrop>());
            _acrylicController.SetSystemBackdropConfiguration(_backdropConfiguration);
        }
        //Maximize
        User32.ShowWindow(_hwnd, ShowWindowCommand.SW_SHOWMAXIMIZED);
    }

    /// <summary>
    /// Calls InitializeWithWindow.Initialize on the target object with the MainWindow's hwnd
    /// </summary>
    /// <param name="target">The target object to initialize</param>
    public void InitializeWithWindow(object target) => WinRT.Interop.InitializeWithWindow.Initialize(target, _hwnd);

    /// <summary>
    /// Occurs when the window is activated
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">WindowActivatedEventArgs</param>
    private void Window_Activated(object sender, WindowActivatedEventArgs e)
    {
        _isActived = e.WindowActivationState != WindowActivationState.Deactivated;
        //Update TitleBar
        TitleBarTitle.Foreground = (SolidColorBrush)App.Current.Resources[_isActived ? "WindowCaptionForeground" : "WindowCaptionForegroundDisabled"];
        _appWindow.TitleBar.ButtonForegroundColor = ((SolidColorBrush)App.Current.Resources[_isActived ? "WindowCaptionForeground" : "WindowCaptionForegroundDisabled"]).Color;
        //Update Backdrop
        _backdropConfiguration.IsInputActive = _isActived;
    }

    /// <summary>
    /// Occurs when the window is closing
    /// </summary>
    /// <param name="sender">AppWindow</param>
    /// <param name="e">AppWindowClosingEventArgs</param>
    private void Window_Closing(AppWindow sender, AppWindowClosingEventArgs e)
    {
        _micaController?.Dispose();
        _acrylicController?.Dispose();
    }

    /// <summary>
    /// Occurs when the window's theme is changed
    /// </summary>
    /// <param name="sender">FrameworkElement</param>
    /// <param name="e">object</param>
    private void Window_ActualThemeChanged(FrameworkElement sender, object e)
    {
        //Update TitleBar
        TitleBarTitle.Foreground = (SolidColorBrush)App.Current.Resources[_isActived ? "WindowCaptionForeground" : "WindowCaptionForegroundDisabled"];
        _appWindow.TitleBar.ButtonForegroundColor = ((SolidColorBrush)App.Current.Resources[_isActived ? "WindowCaptionForeground" : "WindowCaptionForegroundDisabled"]).Color;
        //Update Backdrop
        _backdropConfiguration.Theme = sender.ActualTheme switch
        {
            ElementTheme.Default => SystemBackdropTheme.Default,
            ElementTheme.Light => SystemBackdropTheme.Light,
            ElementTheme.Dark => SystemBackdropTheme.Dark,
            _ => SystemBackdropTheme.Default
        };
    }
}
