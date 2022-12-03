using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;

namespace NickvisionApplication.WinUI.Controls;

/// <summary>
/// A StatusPage control
/// </summary>
public sealed partial class StatusPage : UserControl
{
    public static DependencyProperty GlyphProperty { get; } = DependencyProperty.Register("Glyph", typeof(string), typeof(StatusPage), new PropertyMetadata(null));
    public static DependencyProperty TitleProperty { get; } = DependencyProperty.Register("Title", typeof(string), typeof(StatusPage), new PropertyMetadata(null));
    public static DependencyProperty MessageProperty { get; } = DependencyProperty.Register("Message", typeof(string), typeof(StatusPage), new PropertyMetadata(null));

    public StatusPage()
    {
        InitializeComponent();
    }

    public string Glyph
    {
        get => (string)GetValue(GlyphProperty);

        set => SetValue(GlyphProperty, value);
    }

    public string Title
    {
        get => (string)GetValue(TitleProperty);

        set => SetValue(TitleProperty, value);
    }

    public string Message
    {
        get => (string)GetValue(MessageProperty);

        set => SetValue(MessageProperty, value);
    }
}
