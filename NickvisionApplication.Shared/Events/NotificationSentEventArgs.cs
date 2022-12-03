using System;

namespace NickvisionApplication.Shared.Events;

/// <summary>
/// Event args for when a notification is sent
/// </summary>
public class NotificationSentEventArgs : EventArgs
{
    /// <summary>
    /// The message of the notification
    /// </summary>
    public string Message { get; set; }

    /// <summary>
    /// Constructs a NotificationSentEventArgs
    /// </summary>
    /// <param name="message">The message of the notification</param>
    public NotificationSentEventArgs(string message = "")
    {
        Message = message;
    }
}
