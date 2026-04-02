using Nickvision.Application.Shared.Events;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.Notifications;
using System;

namespace Nickvision.Application.Shared.Services;

public interface IEventsService
{
    event EventHandler<AppNotificationSentEventArgs>? AppNotificationSent;
    event EventHandler<ConfigurationSavedEventArgs>? ConfigurationSaved;
    event EventHandler<PasswordRequiredEventArgs>? DatabasePasswordRequired;
    event EventHandler<FolderChangedEventArgs> FolderChanged;
}
