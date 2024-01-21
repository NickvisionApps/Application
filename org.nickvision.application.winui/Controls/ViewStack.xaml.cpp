#include "Controls/ViewStack.xaml.h"
#if __has_include("Controls/ViewStack.g.cpp")
#include "Controls/ViewStack.g.cpp"
#endif

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Nickvision::Application::WinUI::Controls::implementation 
{
    DependencyProperty ViewStack::m_pagesProperty = DependencyProperty::Register(L"Pages", winrt::xaml_typename<IObservableVector<IInspectable>>(), winrt::xaml_typename<Nickvision::Application::WinUI::Controls::ViewStack>(), PropertyMetadata{ winrt::box_value(winrt::single_threaded_observable_vector<IInspectable>()) });

    ViewStack::ViewStack()
    {
        InitializeComponent();
    }

    winrt::hstring ViewStack::CurrentPage() const noexcept
    {
        return m_currentPage;
    }

    void ViewStack::CurrentPage(const winrt::hstring& pageName) noexcept
    {
        for(const IInspectable& obj : Pages())
        {
            if(Nickvision::Application::WinUI::Controls::ViewStackPage page{ obj.try_as<Nickvision::Application::WinUI::Controls::ViewStackPage>() })
            {
                if(page.PageName() == pageName)
                {
                    m_currentPage = pageName;
                    Content(obj);
                    m_pageChangedEvent(*this, m_currentPage);
                    break;
                }
            }
        }
    }

    IObservableVector<IInspectable> ViewStack::Pages() const noexcept
    {
        return winrt::unbox_value<IObservableVector<IInspectable>>(GetValue(m_pagesProperty));
    }

    winrt::event_token ViewStack::PageChanged(const Windows::Foundation::EventHandler<winrt::hstring>& handler)
    {
        return m_pageChangedEvent.add(handler);
    }

    void ViewStack::PageChanged(const winrt::event_token& token) noexcept
    {
        m_pageChangedEvent.remove(token);
    }

    const DependencyProperty& ViewStack::PagesProperty() noexcept
    {
        return m_pagesProperty;
    }
}