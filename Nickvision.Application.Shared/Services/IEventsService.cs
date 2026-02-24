using Nickvision.Application.Shared.Events;
using Nickvision.Desktop.Filesystem;
using Nickvision.Desktop.Notifications;
using System;

namespace Nickvision.Application.Shared.Services;

public interface IEventsService
{
    event EventHandler<AppNotificationSentEventArgs>? AppNotificationSent;
    event EventHandler<FolderChangedEventArgs> FolderChanged;
    event EventHandler<JsonFileSavedEventArgs>? JsonFileSaved;
}
