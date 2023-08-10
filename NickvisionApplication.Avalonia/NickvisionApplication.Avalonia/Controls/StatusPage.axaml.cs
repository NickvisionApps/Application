using Avalonia;
using Avalonia.Controls;
using Avalonia.Data;
using FluentAvalonia.UI.Controls;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace NickvisionApplication.Avalonia.Controls;

/// <summary>
/// A status page control
/// </summary>
public partial class StatusPage : UserControl, INotifyPropertyChanged
{
    public static StyledProperty<Symbol> SymbolProperty { get; } = AvaloniaProperty.Register<StatusPage, Symbol>(nameof(Symbol), 0, false, BindingMode.TwoWay);
    public static StyledProperty<string> TitleProperty { get; } = AvaloniaProperty.Register<StatusPage, string>(nameof(Title), "", false, BindingMode.TwoWay);
    public static StyledProperty<string> DescriptionProperty { get; } = AvaloniaProperty.Register<StatusPage, string>(nameof(Description), "", false, BindingMode.TwoWay);
    public static StyledProperty<Control?> ChildProperty { get; } = AvaloniaProperty.Register<StatusPage, Control?>(nameof(Child), null, false, BindingMode.TwoWay);

    public event PropertyChangedEventHandler? PropertyChanged;
    
    /// <summary>
    /// Constructs a StatusPage
    /// </summary>
    public StatusPage()
    {
        InitializeComponent();
        DataContext = this;
    }
    
    /// <summary>
    /// The symbol for the status page
    /// </summary>
    public Symbol Symbol
    {
        get => GetValue(SymbolProperty);

        set
        {
            SetValue(SymbolProperty, value);
            NotifyPropertyChanged();
        }
    }

    /// <summary>
    /// The title of the status
    /// </summary>
    public string Title
    {
        get => GetValue(TitleProperty);

        set
        {
            SetValue(TitleProperty, value);
            NotifyPropertyChanged();
        }
    }

    /// <summary>
    /// The message of the status
    /// </summary>
    public string Description
    {
        get => GetValue(DescriptionProperty);

        set
        {
            SetValue(DescriptionProperty, value);
            NotifyPropertyChanged();
        }
    }

    /// <summary>
    /// The extra child of the page
    /// </summary>
    public Control? Child
    {
        get => GetValue(ChildProperty);

        set
        {
            SetValue(ChildProperty, value);
            NotifyPropertyChanged();
        }
    }
    
    private void NotifyPropertyChanged([CallerMemberName] string propertyName = "") => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
}