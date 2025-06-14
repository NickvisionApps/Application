#include "Controls/AboutDialog.xaml.h"
#if __has_include("Controls/AboutDialog.g.cpp")
#include "Controls/AboutDialog.g.cpp"
#endif
#include <libnick/localization/gettext.h>

#include "ViewStack.xaml.h"

using namespace ::Nickvision::App;
using namespace winrt::Microsoft::UI::Xaml::Controls;

namespace winrt::Nickvision::Application::WinUI::Controls::implementation
{
    AboutDialog::AboutDialog()
    {
        InitializeComponent();
        CloseButtonText(winrt::to_hstring(_("OK")));
        NavAbout().Text(winrt::to_hstring(_("About")));
        NavChangelog().Text(winrt::to_hstring(_("Changelog")));
        NavCredits().Text(winrt::to_hstring(_("Credits")));
        NavDebugging().Text(winrt::to_hstring(_("Debugging")));
    }

    void AboutDialog::Info(const AppInfo& appInfo, const std::string& debugInfo)
    {
        Title(winrt::box_value(winrt::to_hstring(appInfo.getShortName())));
        LblChangelog().Text(winrt::to_hstring(appInfo.getChangelog()));
    }

    void AboutDialog::OnNavViewSelectionChanged(const SelectorBar& sender, const SelectorBarSelectionChangedEventArgs& args)
    {
        uint32_t index;
        if(sender.Items().IndexOf(sender.SelectedItem(), index))
        {
            ViewStack().CurrentPageIndex(static_cast<int>(index));
        }
    }
}
