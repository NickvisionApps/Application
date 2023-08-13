using Avalonia;
using Avalonia.Collections;
using Avalonia.Controls;
using Avalonia.Data;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;

namespace NickvisionApplication.Mobile.Controls;

/// <summary>
/// A ViewStack
/// </summary>
public partial class ViewStack : UserControl, INotifyPropertyChanged
{
    private IEnumerable<ViewStackPage> _pages;
    
    public static StyledProperty<string> CurrentPageNameProperty { get; } = AvaloniaProperty.Register<ViewStack, string>(nameof(CurrentPageName), "", false, BindingMode.TwoWay);
    public static DirectProperty<ViewStack, IEnumerable<ViewStackPage>> PagesProperty { get; } = AvaloniaProperty.RegisterDirect<ViewStack, IEnumerable<ViewStackPage>>(nameof(Pages), o => o.Pages, (o, v) => o.Pages = v);
    
    public event PropertyChangedEventHandler? PropertyChanged;
    
    /// <summary>
    /// Constructs a ViewStack
    /// </summary>
    public ViewStack()
    {
        InitializeComponent();
        _pages = new AvaloniaList<ViewStackPage>();
        DataContext = this;
    }

    /// <summary>
    /// The object of the page shown
    /// </summary>
    public ViewStackPage? CurrentPage
    {
        get
        {
            foreach (var page in _pages)
            {
                if (page.Name == CurrentPageName)
                {
                    return page;
                }
            }
            return null;
        }
    }
    
    /// <summary>
    /// The name of the page shown
    /// </summary>
    public string CurrentPageName
    {
        get => GetValue(CurrentPageNameProperty);

        set
        {
            SetValue(CurrentPageNameProperty, value);
            NotifyPropertyChanged();
            NotifyPropertyChanged("CurrentPage");
        }
    }

    /// <summary>
    /// The list of pages in the stack
    /// </summary>
    public IEnumerable<ViewStackPage> Pages
    {
        get => _pages;

        set
        {
            _pages = value;
            NotifyPropertyChanged();
            CurrentPageName = value.First().Name;
        }
    }
    
    private void NotifyPropertyChanged([CallerMemberName] string propertyName = "") => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
}