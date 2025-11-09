using System;

namespace Nickvision.Application.Shared.Events;

public class FolderChangedEventArgs : EventArgs
{
    public string? Path { get; init; }

    public FolderChangedEventArgs(string? path)
    {
        Path = path;
    }
}
