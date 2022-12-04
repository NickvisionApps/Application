using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;

namespace NickvisionApplication.WinUI.Controls;

public sealed partial class ViewStackPage : Frame
{
    public static DependencyProperty PageNameProperty { get; } = DependencyProperty.Register("PageName", typeof(string), typeof(ViewStackPage), new PropertyMetadata(null));

    public ViewStackPage()
    {
        InitializeComponent();
    }

    public string PageName
    {
        get => (string)GetValue(PageNameProperty);

        set => SetValue(PageNameProperty, value);
    }
}
