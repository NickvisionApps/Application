#ifndef STATUSPAGE_H
#define STATUSPAGE_H

#include "includes.h"
#include "Controls/StatusPage.g.h"

namespace winrt::Nickvision::Application::WinUI::Controls::implementation 
{
    /**
     * @brief A page control to display a status of the application.
     */
    class StatusPage : public StatusPageT<StatusPage>
    {
    public:
        /**
         * @brief Constructs a StatusPage. 
         */
        StatusPage();
        /**
         * @brief Gets the icon glyph for the page.
         * @brief This will be used when UseAppIcon is set to false.
         * @return The page icon glyph
         */
        winrt::hstring Glyph() const noexcept;
        /**
         * @brief Sets the icon glyph for the page. 
         * @brief To be used when UseAppIcon is set to false.
         * @param glyph The new icon glyph
         */
        void Glyph(const winrt::hstring& glyph) noexcept;
        /**
         * @brief Gets whether or not the page is displaying the application's icon.
         * @brief If false, the Glyph icon is being used instead.
         * @return True if displaying app icon, else false  
         */
        bool UseAppIcon() const noexcept;
        /**
         * @brief Sets whether or not the page should display the application's icon.
         * @param useAppIcon True to display app icon. False to use the Glyph icon 
         */
        void UseAppIcon(bool useAppIcon) noexcept;
        /**
         * @brief Gets the status title for the page.
         * @returns The status title 
         */
        winrt::hstring Title() const noexcept;
        /**
         * @brief Sets the status title for the page.
         * @param title The new status title
         */
        void Title(const winrt::hstring& title) noexcept;
        /**
         * @brief Gets the status description for the page.
         * @returns The status description 
         */
        winrt::hstring Description() const noexcept;
        /**
         * @brief Sets the status title for the page.
         * @param title The new status title
         */
        void Description(const winrt::hstring& description) noexcept;
        /**
         * @brief Gets the extra child for the page.
         * @returns The extra child
         */
        IInspectable Child() const noexcept;
        /**
         * @brief Sets the extra child for the page.
         * @param title The new extra child
         */
        void Child(const IInspectable& child) noexcept;
        /**
         * @brief Gets whether or not the page is displayed using compact spacing.
         * @return True for compact spacing, else false 
         */
        bool IsCompact() const noexcept;
        /**
         * @brief Sets whether or not the page should use compact spacing.
         * @param isCompact True for compact spacing, else false
         */
        void IsCompact(bool isCompact) noexcept;
/**
         * @brief Subscribes a handler to the property changed event.
         * @return The token for the newly subscribed handler.
         */
        winrt::event_token PropertyChanged(const Microsoft::UI::Xaml::Data::PropertyChangedEventHandler& handler) noexcept;
        /**
         * @brief Unsubscribes a handler from the property changed event.
         * @param token The token of the handler to unsubscribe. 
         */
        void PropertyChanged(const winrt::event_token& token) noexcept;

    private:
        winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

    public:
        /**
         * @brief Gets the page's glyph dependency property.
         * @return The glyph dependency property 
         */
        static const Microsoft::UI::Xaml::DependencyProperty& GlyphProperty() noexcept;
        /**
         * @brief Gets the page's use app icon dependency property.
         * @return The use app icon dependency property 
         */
        static const Microsoft::UI::Xaml::DependencyProperty& UseAppIconProperty() noexcept;
        /**
         * @brief Gets the page's title dependency property.
         * @return The title dependency property 
         */
        static const Microsoft::UI::Xaml::DependencyProperty& TitleProperty() noexcept;
        /**
         * @brief Gets the page's description dependency property.
         * @return The description dependency property 
         */
        static const Microsoft::UI::Xaml::DependencyProperty& DescriptionProperty() noexcept;
        /**
         * @brief Gets the page's child dependency property.
         * @return The child dependency property 
         */
        static const Microsoft::UI::Xaml::DependencyProperty& ChildProperty() noexcept;
        /**
         * @brief Gets the page's is compact dependency property.
         * @return The is compact dependency property 
         */
        static const Microsoft::UI::Xaml::DependencyProperty& IsCompactProperty() noexcept;
        /**
         * @brief Handles when a property of the row is changed.
         * @param d Microsoft::UI::Xaml::DependencyObject
         * @param args Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs
         */
        static void OnPropertyChanged(const Microsoft::UI::Xaml::DependencyObject& d, const Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs& args);

    private:
        static Microsoft::UI::Xaml::DependencyProperty m_glyphProperty;
        static Microsoft::UI::Xaml::DependencyProperty m_useAppIconProperty;
        static Microsoft::UI::Xaml::DependencyProperty m_titleProperty;
        static Microsoft::UI::Xaml::DependencyProperty m_descriptionProperty;
        static Microsoft::UI::Xaml::DependencyProperty m_childProperty;
        static Microsoft::UI::Xaml::DependencyProperty m_isCompactProperty;
    };
}

namespace winrt::Nickvision::Application::WinUI::Controls::factory_implementation 
{
    class StatusPage : public StatusPageT<StatusPage, implementation::StatusPage>
    {

    };
}

#endif //STATUSPAGE_H