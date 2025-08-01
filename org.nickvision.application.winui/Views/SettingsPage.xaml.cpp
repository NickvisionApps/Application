#include "Views/SettingsPage.xaml.h"
#if __has_include("Views/SettingsPage.g.cpp")
#include "Views/SettingsPage.g.cpp"
#endif
#include <libnick/localization/gettext.h>

using namespace ::Nickvision::Application::Shared::Controllers;
using namespace ::Nickvision::Application::Shared::Models;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;

namespace winrt::Nickvision::Application::WinUI::Views::implementation
{
    SettingsPage::SettingsPage()
        : m_constructing{ false }
    {
        InitializeComponent();
        //Localize Strings
        LblSettings().Text(winrt::to_hstring(_("Settings")));
        NavUserInterface().Text(winrt::to_hstring(_("User Interface")));
        RowTheme().Title(winrt::to_hstring(_("Theme")));
        CmbTheme().Items().Append(winrt::box_value(winrt::to_hstring(_("Light"))));
        CmbTheme().Items().Append(winrt::box_value(winrt::to_hstring(_("Dark"))));
        CmbTheme().Items().Append(winrt::box_value(winrt::to_hstring(_("System"))));
        RowLanguage().Title(winrt::to_hstring(_("Translation Language")));
        RowLanguage().Description(winrt::to_hstring(_("An application restart is required for change to take effect")));
        RowUpdates().Title(winrt::to_hstring(_("Automatically Check for Updates")));
        TglUpdates().OnContent(winrt::box_value(winrt::to_hstring(_("On"))));
        TglUpdates().OffContent(winrt::box_value(winrt::to_hstring(_("Off"))));
    }

    void SettingsPage::Controller(const std::shared_ptr<PreferencesViewController>& controller)
    {
        m_controller = controller;
        //Load
        m_constructing = true;
        CmbTheme().SelectedIndex(static_cast<int>(m_controller->getTheme()));
        for(const std::string& language : m_controller->getAvailableTranslationLanguages())
        {
            Windows::Foundation::IInspectable item{ winrt::box_value(winrt::to_hstring(language)) };
            CmbLanguage().Items().Append(item);
            if(language == m_controller->getTranslationLanguage())
            {
                CmbLanguage().SelectedItem(item);
            }
        }
        TglUpdates().IsOn(m_controller->getAutomaticallyCheckForUpdates());
        m_constructing = false;
    }

    void SettingsPage::OnNavViewSelectionChanged(const SelectorBar& sender, const SelectorBarSelectionChangedEventArgs& args)
    {
        uint32_t index;
        if(sender.Items().IndexOf(sender.SelectedItem(), index))
        {
            ViewStack().CurrentPageIndex(static_cast<int>(index));
        }
    }

    void SettingsPage::OnCmbChanged(const IInspectable& sender, const SelectionChangedEventArgs& args)
    {
        ApplyChanges();
    }

    void SettingsPage::OnSwitchToggled(const IInspectable& sender, const RoutedEventArgs& args)
    {
        ApplyChanges();
    }

    void SettingsPage::ApplyChanges()
    {
        if(m_constructing)
        {
            return;
        }
        m_controller->setTheme(static_cast<Theme>(CmbTheme().SelectedIndex()));
        m_controller->setTranslationLanguage(winrt::to_string(CmbLanguage().SelectedItem().as<winrt::hstring>()));
        m_controller->setAutomaticallyCheckForUpdates(TglUpdates().IsOn());
        m_controller->saveConfiguration();
    }
}