using Nickvision.Application.Shared.Events;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.Notifications;
using System;

namespace Nickvision.Application.Shared.Services;

public class EventsService : IEventsService
{
    private readonly IConfigurationService _configurationService;
    private readonly IDatabaseService _databaseService;
    private readonly IFolderService _folderService;
    private readonly INotificationService _notificationService;

    public EventsService(IConfigurationService configurationService, IDatabaseService databaseService, IFolderService folderService, INotificationService notificationService)
    {
        _configurationService = configurationService;
        _databaseService = databaseService;
        _folderService = folderService;
        _notificationService = notificationService;
    }

    public event EventHandler<AppNotificationSentEventArgs>? AppNotificationSent
    {
        add => _notificationService.AppNotificationSent += value;

        remove => _notificationService.AppNotificationSent -= value;
    }

    public event EventHandler<ConfigurationSavedEventArgs>? ConfigurationSaved
    {
        add => _configurationService.Saved += value;

        remove => _configurationService.Saved -= value;
    }

    public event EventHandler<FolderChangedEventArgs> FolderChanged
    {
        add => _folderService.Changed += value;

        remove => _folderService.Changed -= value;
    }
}
