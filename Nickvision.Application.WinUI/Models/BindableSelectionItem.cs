using WinRT;

namespace Nickvision.Application.WinUI.Models;

[GeneratedBindableCustomProperty]
public sealed partial class BindableSelectionItem(string label)
{
    public string Label { get; } = label;
}
