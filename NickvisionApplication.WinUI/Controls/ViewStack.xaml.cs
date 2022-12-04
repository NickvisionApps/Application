using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using System.Collections.ObjectModel;
using System.Collections.Specialized;

namespace NickvisionApplication.WinUI.Controls;

public sealed partial class ViewStack : UserControl
{
    public static DependencyProperty PagesProperty { get; } = DependencyProperty.Register("Pages", typeof(ObservableCollection<ViewStackPage>), typeof(ViewStack), new PropertyMetadata(null));

    public ObservableCollection<ViewStackPage> Pages => (ObservableCollection<ViewStackPage>)GetValue(PagesProperty);

    public ViewStack()
    {
        InitializeComponent();
        SetValue(PagesProperty, new ObservableCollection<ViewStackPage>());
        Pages.CollectionChanged += CollectionChanged;
    }

    public bool ChangePage(string pageName)
    {
        foreach(var page in Pages)
        {
            if(page.PageName == pageName)
            {
                Content = page;
                return true;
            }
        }
        return false;
    }

    private void CollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
    {
        if (Pages.Count == 0)
        {
            Content = null;
        }
        else
        {
            Content = Pages[Pages.Count - 1];
        }
    }
}
