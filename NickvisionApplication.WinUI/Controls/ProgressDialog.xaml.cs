using Microsoft.UI.Xaml.Controls;
using System;
using System.Threading.Tasks;

namespace NickvisionApplication.WinUI.Controls;

/// <summary>
/// A control for running a long task and displaying Progress in a blocking dialog.
/// </summary>
public sealed partial class ProgressDialog : ContentDialog
{
    private readonly Func<Task> _action;
    private bool _isFinished;

    /// <summary>
    /// Constructs a ProgressDialog.
    /// </summary>
    /// <param name="description">The description of the task</param>
    /// <param name="action">The long task</param>
    public ProgressDialog(string description, Func<Task> action)
    {
        InitializeComponent();
        _action = action;
        _isFinished = false;
        LblDescription.Text = description;
    }

    /// <summary>
    /// Occurs when the dialog is opened.
    /// </summary>
    /// <param name="sender">Unused.</param>
    /// <param name="args">Unused.</param>
    private async void Dialog_Opened(ContentDialog sender, ContentDialogOpenedEventArgs args)
    {
        await _action();
        _isFinished = true;
        Hide();
    }

    /// <summary>
    /// Occurs when the dialog is closed.
    /// </summary>
    /// <param name="sender">Unused.</param>
    /// <param name="args">Unused.</param>
    private void Dialog_Closing(ContentDialog sender, ContentDialogClosingEventArgs args)
    {
        if (!_isFinished)
        {
            args.Cancel = true;
        }
    }
}