using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace NickvisionApplication.WinUI.Controls;

/// <summary>
/// A NavigationViewItem for a StoreNavigationView
/// </summary>
public sealed partial class StoreNavigationViewItem : NavigationViewItem, INotifyPropertyChanged
{
    public new static DependencyProperty IconProperty { get; } = DependencyProperty.Register("Icon", typeof(string), typeof(StoreNavigationViewItem), new PropertyMetadata("", (sender, e) => (sender as StoreNavigationViewItem)?.NotifyPropertyChanged(nameof(Icon))));
    public static DependencyProperty FilledIconProperty { get; } = DependencyProperty.Register("FilledIcon", typeof(string), typeof(StoreNavigationViewItem), new PropertyMetadata("", (sender, e) => (sender as StoreNavigationViewItem)?.NotifyPropertyChanged(nameof(FilledIcon))));
    public static DependencyProperty PageNameProperty { get; } = DependencyProperty.Register("PageName", typeof(string), typeof(StoreNavigationViewItem), new PropertyMetadata("", (sender, e) => (sender as StoreNavigationViewItem)?.NotifyPropertyChanged(nameof(PageName))));

    public event PropertyChangedEventHandler? PropertyChanged;

    /// <summary>
    /// Constructs a StoreNavigationViewItem
    /// </summary>
    public StoreNavigationViewItem()
    {
        InitializeComponent();
        Icon = "";
        FilledIcon = "";
        PageName = "";
    }

    /// <summary>
    /// The glyph code for the icon
    /// </summary>
    public new string Icon
    {
        get => (string)GetValue(IconProperty);

        set
        {
            SetValue(IconProperty, value);
            NotifyPropertyChanged();
        }
    }

    /// <summary>
    /// The glyph code for the filled icon
    /// </summary>
    public string FilledIcon
    {
        get => (string)GetValue(FilledIconProperty);

        set
        {
            SetValue(FilledIconProperty, value);
            NotifyPropertyChanged();
        }
    }

    /// <summary>
    /// The name of the page represented by this navigation view item
    /// </summary>
    public string PageName
    {
        get => (string)GetValue(PageNameProperty);

        set
        {
            SetValue(PageNameProperty, value);
            NotifyPropertyChanged();
        }
    }

    /// <summary>
    /// Whether or not the item is selected
    /// </summary>
    public new bool IsSelected
    {
        get => ViewStack.CurrentPageName == "Selected";

        set
        {
            base.IsSelected = value;
            ViewStack.CurrentPageName = value ? "Selected" : "Unselected";
        }
    }

    private void NotifyPropertyChanged([CallerMemberName] string propertyName = "") => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
}
