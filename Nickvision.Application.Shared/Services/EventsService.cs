using Nickvision.Application.Shared.Events;
using Nickvision.Desktop.Filesystem;
using Nickvision.Desktop.Notifications;
using System;

namespace Nickvision.Application.Shared.Services;

public class EventsService : IEventsService
{
    private readonly IFolderService _folderService;
    private readonly IJsonFileService _jsonFileService;
    private readonly INotificationService _notificationService;

    public EventsService(IFolderService folderService, IJsonFileService jsonFileService, INotificationService notificationService)
    {
        _folderService = folderService;
        _jsonFileService = jsonFileService;
        _notificationService = notificationService;
    }

    public event EventHandler<AppNotificationSentEventArgs>? AppNotificationSent
    {
        add => _notificationService.AppNotificationSent += value;

        remove => _notificationService.AppNotificationSent -= value;
    }

    public event EventHandler<FolderChangedEventArgs> FolderChanged
    {
        add => _folderService.Changed += value;

        remove => _folderService.Changed -= value;
    }

    public event EventHandler<JsonFileSavedEventArgs>? JsonFileSaved
    {
        add => _jsonFileService.Saved += value;

        remove => _jsonFileService.Saved -= value;
    }
}
