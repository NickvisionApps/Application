#include "Controls/ViewStackPage.xaml.h"
#if __has_include("Controls/ViewStackPage.g.cpp")
#include "Controls/ViewStackPage.g.cpp"
#endif

using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::Nickvision::Application::WinUI::Controls::implementation 
{
    DependencyProperty ViewStackPage::m_pageNameProperty = DependencyProperty::Register(L"PageName", winrt::xaml_typename<winrt::hstring>(), winrt::xaml_typename<Nickvision::Application::WinUI::Controls::ViewStackPage>(), PropertyMetadata{ winrt::box_value(L"") });

    ViewStackPage::ViewStackPage()
    {
        InitializeComponent();
    }

    winrt::hstring ViewStackPage::PageName() const noexcept
    {
        return winrt::unbox_value<winrt::hstring>(GetValue(m_pageNameProperty));
    }

    void ViewStackPage::PageName(const winrt::hstring& pageName) noexcept
    {
        SetValue(m_pageNameProperty, winrt::box_value(pageName));
    }

    const DependencyProperty& ViewStackPage::PageNameProperty() noexcept
    {
        return m_pageNameProperty;
    }
}