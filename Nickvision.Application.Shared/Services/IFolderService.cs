using Nickvision.Application.Shared.Events;
using Nickvision.Desktop;
using System;
using System.Collections.Generic;

namespace Nickvision.Application.Shared.Services;

public interface IFolderService : IService
{
    public List<string> Files { get; }
    public string? Path { get; }

    public event EventHandler<FolderChangedEventArgs>? Changed;

    public void Open(string path);

    public void Close();
}
