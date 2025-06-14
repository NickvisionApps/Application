#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "pch.h"
#include "Controls/AboutDialog.g.h"

namespace winrt::Nickvision::Application::WinUI::Controls::implementation
{
    /**
     * @brief A dialog for showing application information.
     */
    struct AboutDialog : public AboutDialogT<AboutDialog>
    {
    public:
        /**
         * @brief Constructs a AboutDialog.
         */
        AboutDialog();
    };
}

namespace winrt::Nickvision::Application::WinUI::Controls::factory_implementation
{
    struct AboutDialog : public AboutDialogT<AboutDialog, implementation::AboutDialog> { };
}

#endif //ABOUTDIALOG_H
