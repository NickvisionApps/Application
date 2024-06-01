#ifndef TITLEBAR_H
#define TITLEBAR_H

#include "includes.h"
#include "Controls/TitleBar.g.h"

namespace winrt::Nickvision::Application::WinUI::Controls::implementation 
{
    /**
     * @brief A modern titlebar control.
     */
    class TitleBar : public TitleBarT<TitleBar>
    {
    public:
        /**
         * @brief Constructs a TitleBar. 
         */
        TitleBar();
        /**
         * @brief Gets the title for the titlebar.
         * @return The titlebar title 
         */
        winrt::hstring Title() const;
        /**
         * @brief Sets the title for the titlebar.
         * @param title The new title 
         */
        void Title(const winrt::hstring& title);
        /**
         * @brief Gets the foreground brush for the titlebar.
         * @return The titlebar foreground brush
         */
        Microsoft::UI::Xaml::Media::Brush TitleForeground() const;
        /**
         * @brief Sets the foreground brush for the titlebar.
         * @param foreground The new foreground brush 
         */
        void TitleForeground(const Microsoft::UI::Xaml::Media::Brush& foreground);
        /**
         * @brief Gets the visibility of the search box control.
         * @return The search box control visibility
         */
        Microsoft::UI::Xaml::Visibility SearchVisibility() const;
        /**
         * @brief Sets the visibility of the search box control.
         * @param visibility The new search box control visibility 
         */
        void SearchVisibility(const Microsoft::UI::Xaml::Visibility& visibility);
        /**
         * @brief Gets the app window for the titlebar.
         * @return The app window 
         */
        Microsoft::UI::Windowing::AppWindow AppWindow();
        /**
         * @brief Sets the app window for the titlebar.
         * @param appWindow The new app window 
         */
        void AppWindow(const Microsoft::UI::Windowing::AppWindow& appWindow);
        /**
         * @brief Subscribes a handler to the search changed event.
         * @return The token for the newly subscribed handler.
         */
        winrt::event_token SearchChanged(const Windows::Foundation::EventHandler<winrt::hstring>& handler);
        /**
         * @brief Unsubscribes a handler from the search changed event.
         * @param token The token of the handler to unsubscribe. 
         */
        void SearchChanged(const winrt::event_token& token);
        /**
         * @brief Subscribes a handler to the property changed event.
         * @return The token for the newly subscribed handler.
         */
        winrt::event_token PropertyChanged(const Microsoft::UI::Xaml::Data::PropertyChangedEventHandler& handler);
        /**
         * @brief Unsubscribes a handler from the property changed event.
         * @param token The token of the handler to unsubscribe. 
         */
        void PropertyChanged(const winrt::event_token& token);
        /**
         * @brief Handles when the titlebar is loaded.
         * @param sender IInspectable
         * @param args Microsoft::UI::Xaml::RoutedEventArgs 
         */
        void OnLoaded(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args);
        /**
         * @brief Handles when the titlebar's size is changed.
         * @param sender IInspectable
         * @param args Windows::UI::Xaml::SizeChangedEventArgs 
         */
        void OnSizeChanged(const IInspectable& sender, const Microsoft::UI::Xaml::SizeChangedEventArgs& args);
        /**
         * @brief Handles when the titlebar's theme is changed.
         * @param sender Microsoft::UI::Xaml::FrameworkElement
         * @param args IInspectable 
         */
        void OnThemeChanged(const Microsoft::UI::Xaml::FrameworkElement& sender, const IInspectable& args);
        /**
         * @brief Handles when the titlebar's search text is changed.
         * @param sender Microsoft::UI::Xaml::Controls::AutoSuggestBox
         * @param args Microsoft::UI::Xaml::Controls::AutoSuggestBoxTextChangedEventArgs 
         */
        void OnSearchTextChanged(const Microsoft::UI::Xaml::Controls::AutoSuggestBox& sender, const Microsoft::UI::Xaml::Controls::AutoSuggestBoxTextChangedEventArgs& args);

    private:
        /**
         * @brief Sets the drag region for the titlebar. 
         */
        void SetDragRegion();
        Microsoft::UI::Windowing::AppWindow m_appWindow;
        bool m_loaded;
        winrt::event<Windows::Foundation::EventHandler<winrt::hstring>> m_searchChangedEvent;
        winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChangedEvent;

    public:
        /**
         * @brief Gets the titlebar's title dependency property.
         * @return The title dependency property
         */
        static const Microsoft::UI::Xaml::DependencyProperty& TitleProperty();
        /**
         * @brief Gets the titlebar's foreground dependency property.
         * @return The foreground dependency property
         */
        static const Microsoft::UI::Xaml::DependencyProperty& TitleForegroundProperty();
        /**
         * @brief Gets the titlebar's search visibility dependency property.
         * @return The search visibility dependency property
         */
        static const Microsoft::UI::Xaml::DependencyProperty& SearchVisibilityProperty();
        /**
         * @brief Handles when a property of the row is changed.
         * @param d Microsoft::UI::Xaml::DependencyObject
         * @param args Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs
         */
        static void OnPropertyChanged(const Microsoft::UI::Xaml::DependencyObject& d, const Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs& args);

    private:
        static Microsoft::UI::Xaml::DependencyProperty m_titleProperty;
        static Microsoft::UI::Xaml::DependencyProperty m_titleForegroundProperty;
        static Microsoft::UI::Xaml::DependencyProperty m_searchVisibilityProperty;
    };
}

namespace winrt::Nickvision::Application::WinUI::Controls::factory_implementation 
{
    class TitleBar : public TitleBarT<TitleBar, implementation::TitleBar>
    {

    };
}

#endif //TITLEBAR_H