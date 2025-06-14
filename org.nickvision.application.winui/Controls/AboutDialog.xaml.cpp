#include "Controls/AboutDialog.xaml.h"
#if __has_include("Controls/AboutDialog.g.cpp")
#include "Controls/AboutDialog.g.cpp"
#endif
#include <libnick/localization/gettext.h>

namespace winrt::Nickvision::Application::WinUI::Controls::implementation
{
    AboutDialog::AboutDialog()
    {
        InitializeComponent();
        Title(winrt::box_value(winrt::to_hstring(_("About Application"))));
        CloseButtonText(winrt::to_hstring(_("OK")));
    }
}
