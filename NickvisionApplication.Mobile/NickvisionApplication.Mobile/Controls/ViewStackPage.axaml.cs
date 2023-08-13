using Avalonia;
using Avalonia.Controls;
using Avalonia.Data;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace NickvisionApplication.Mobile.Controls;

/// <summary>
/// A page for a ViewStack
/// </summary>
public partial class ViewStackPage : UserControl, INotifyPropertyChanged
{
    public static StyledProperty<string> NameProperty { get; } = AvaloniaProperty.Register<ViewStackPage, string>(nameof(Name), "", false, BindingMode.TwoWay);
    
    public event PropertyChangedEventHandler? PropertyChanged;
    
    /// <summary>
    /// Constructs a ViewStackPage
    /// </summary>
    public ViewStackPage()
    {
        InitializeComponent();
        DataContext = this;
    }
    
    /// <summary>
    /// The name of the page
    /// </summary>
    public string Name
    {
        get => GetValue(NameProperty);

        set
        {
            SetValue(NameProperty, value);
            NotifyPropertyChanged();
        }
    }
    
    private void NotifyPropertyChanged([CallerMemberName] string propertyName = "") => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
}