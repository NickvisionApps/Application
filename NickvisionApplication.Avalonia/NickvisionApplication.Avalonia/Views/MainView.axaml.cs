using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Interactivity;
using FluentAvalonia.UI.Controls;
using NickvisionApplication.Avalonia.Controls;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Events;
using System;
using static NickvisionApplication.Shared.Helpers.Gettext;

namespace NickvisionApplication.Avalonia.Views;

/// <summary>
/// The MainView for the application
/// </summary>
public partial class MainView : UserControl
{
    private readonly MainWindowController _controller;
    private readonly IApplicationLifetime _lifetime;
    private bool _opened;
    
    /// <summary>
    /// Constructs a MainView
    /// </summary>
    /// <param name="controller">MainWindowController</param>
    /// <param name="lifetime">IApplicationLifetime</param>
    public MainView(MainWindowController controller, IApplicationLifetime lifetime)
    {
        InitializeComponent();
        InitializeComponent();
        _controller = controller;
        _lifetime = lifetime;
        _opened = false;
        //Register Events
        _controller.NotificationSent += NotificationSent;
        //Header
        BtnOpenFolder.Label = _("Open");
        ToolTip.SetTip(BtnSettings, _("Settings"));
        ToolTip.SetTip(BtnAbout, _("About Application"));
        //Greeting
        GreetingTitle.Text = _controller.Greeting;
        GreetingDescription.Text = _("Open a folder to get started");
    }
    
    /// <summary>
    /// Occurs when the main view is loaded
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">RoutedEventArgs</param>
    private void OnLoaded(object? sender, RoutedEventArgs e)
    {
        if (!_opened)
        {
            _controller.Startup(_lifetime is IClassicDesktopStyleApplicationLifetime desktop ? (desktop.Args ?? Array.Empty<string>()) : Array.Empty<string>());
            _opened = true;
        }
    }

    /// <summary>
    /// Occurs when a notification is sent from the controller
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">NotificationSentEventArgs</param>
    private void NotificationSent(object? sender, NotificationSentEventArgs e)
    {
        InfoBar.Title = e.Message;
        InfoBar.Severity = e.Severity switch
        {
            NotificationSeverity.Success => InfoBarSeverity.Success,
            NotificationSeverity.Error => InfoBarSeverity.Error,
            NotificationSeverity.Warning => InfoBarSeverity.Warning,
            _ => InfoBarSeverity.Informational
        };
        if (e.Action == "close")
        {
            var button = new Button()
            {
                Content = _("Close")
            };
            button.Click += (s, ex) => CloseFolder();
            InfoBar.ActionButton = button;
        }
        else
        {
            InfoBar.ActionButton = null;
        }
        InfoBar.IsOpen = true;
    }

    private void CloseFolder()
    {
        
    }

    private async void About(object? sender, RoutedEventArgs e)
    {
        var aboutDialog = new AboutDialog(_controller.AppInfo);
        await aboutDialog.ShowAsync();
    }
}