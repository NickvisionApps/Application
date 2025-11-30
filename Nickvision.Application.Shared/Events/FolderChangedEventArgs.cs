using System;
using System.Collections.Generic;

namespace Nickvision.Application.Shared.Events;

public class FolderChangedEventArgs : EventArgs
{
    public string? Path { get; init; }
    public List<string> Files { get; init; }

    public bool IsOpen => !string.IsNullOrEmpty(Path);

    public FolderChangedEventArgs(string? path, List<string>? files = default)
    {
        Path = path;
        Files = files ?? [];
    }
}
