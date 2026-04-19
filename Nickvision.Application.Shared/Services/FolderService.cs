using Microsoft.Extensions.Logging;
using Nickvision.Application.Shared.Events;
using Nickvision.Desktop.Globalization;
using Nickvision.Desktop.Notifications;
using System;
using System.Collections.Generic;
using System.IO;

namespace Nickvision.Application.Shared.Services;

public class FolderService : IFolderService
{
    private readonly ILogger<FolderService> _logger;
    private readonly INotificationService _notificationService;
    private readonly ITranslationService _translationService;

    public List<string> Files { get; init; }
    public string? Path { get; private set; }

    public event EventHandler<FolderChangedEventArgs>? Changed;

    public FolderService(ILogger<FolderService> logger, INotificationService notificationService, ITranslationService translationService)
    {
        _logger = logger;
        _notificationService = notificationService;
        _translationService = translationService;
        Files = new List<string>();
        Path = null;
    }

    public void Open(string path)
    {
        _logger.LogDebug($"Opening folder: {path}");
        if (!Directory.Exists(path))
        {
            _logger.LogError("Folder does not exist.");
            Path = null;
            return;
        }
        Path = path;
        Files.Clear();
        foreach (var file in Directory.EnumerateFiles(Path))
        {
            Files.Add(file);
        }
        _logger.LogInformation($"Folder opened with {Files.Count} file(s).");
        _notificationService.Send(new AppNotification(_translationService._("Folder opened: {0}", Path), NotificationSeverity.Success)
        {
            Action = "close"
        });
        _notificationService.Send(new ShellNotification(_translationService._("Folder"), _translationService._("Loaded {0} file(s)", Files.Count), NotificationSeverity.Information)
        {
            Action = "open",
            ActionParam = Path
        });
        Changed?.Invoke(this, new FolderChangedEventArgs(Path, Files));
    }

    public void Close()
    {
        _logger.LogDebug($"Closing folder: {Path}");
        Path = null;
        Files.Clear();
        _logger.LogInformation("Folder closed.");
        _notificationService.Send(new AppNotification(_translationService._("Folder closed"), NotificationSeverity.Warning));
        Changed?.Invoke(this, new FolderChangedEventArgs(null));
    }
}
