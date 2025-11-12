using Nickvision.Application.Shared.Events;
using Nickvision.Desktop.Globalization;
using Nickvision.Desktop.Notifications;
using System;
using System.Collections.Generic;
using System.IO;

namespace Nickvision.Application.Shared.Services;

public class FolderService : IFolderService
{
    private readonly INotificationService _notificationService;
    private readonly ITranslationService _translationService;

    public List<string> Files { get; init; }
    public string? Path { get; private set; }

    public event EventHandler<FolderChangedEventArgs>? Changed;

    public FolderService(INotificationService notificationService, ITranslationService translationService)
    {
        _notificationService = notificationService;
        _translationService = translationService;
        Files = new List<string>();
        Path = null;
    }

    public void Open(string path)
    {
        if (!Directory.Exists(path))
        {
            Path = null;
            return;
        }
        Path = path;
        Files.Clear();
        foreach (var file in Directory.EnumerateFiles(Path))
        {
            Files.Add(file);
        }
        _notificationService.Send(new AppNotification(_translationService._("Folder Opened: {0}", Path), NotificationSeverity.Success)
        {
            Action = "close"
        });
        Changed?.Invoke(this, new FolderChangedEventArgs(Path));
    }

    public void Close()
    {
        Path = null;
        Files.Clear();
        _notificationService.Send(new AppNotification(_translationService._("Folder closed"), NotificationSeverity.Warning));
        Changed?.Invoke(this, new FolderChangedEventArgs(null));
    }
}
