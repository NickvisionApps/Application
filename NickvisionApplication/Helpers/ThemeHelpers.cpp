#include "ThemeHelpers.h"
#include <dwmapi.h>
#include <uxtheme.h>
#include "../Models/Configuration.h"

using namespace NickvisionApplication::Models;

namespace NickvisionApplication::Helpers
{
    QPalette ThemeHelpers::getLightPalette()
    {
        QPalette lightPalette;
        lightPalette.setColor(QPalette::Window, QColor(255, 255, 255));
        lightPalette.setColor(QPalette::WindowText, Qt::black);
        lightPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
        lightPalette.setColor(QPalette::Base, QColor(250, 250, 250));
        lightPalette.setColor(QPalette::AlternateBase, QColor(243, 243, 243));
        lightPalette.setColor(QPalette::ToolTipBase, Qt::black);
        lightPalette.setColor(QPalette::ToolTipText, Qt::black);
        lightPalette.setColor(QPalette::Text, Qt::black);
        lightPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
        lightPalette.setColor(QPalette::Button, QColor(250, 250, 250));
        lightPalette.setColor(QPalette::ButtonText, Qt::black);
        lightPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
        lightPalette.setColor(QPalette::BrightText, Qt::red);
        lightPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        lightPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        lightPalette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
        lightPalette.setColor(QPalette::HighlightedText, Qt::white);
        lightPalette.setColor(QPalette::PlaceholderText, Qt::darkGray);
        lightPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));
        return lightPalette;
    }

	QPalette ThemeHelpers::getDarkPalette()
	{
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
        darkPalette.setColor(QPalette::Base, QColor(43, 43, 43));
        darkPalette.setColor(QPalette::AlternateBase, QColor(55, 55, 55));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
        darkPalette.setColor(QPalette::Button, QColor(43, 43, 43));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
        darkPalette.setColor(QPalette::HighlightedText, Qt::white);
        darkPalette.setColor(QPalette::PlaceholderText, Qt::gray);
        darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));
        return darkPalette;
	}

	void ThemeHelpers::applyWin32Theming(QWidget* widget)
	{
        BOOL enabled;
        if (Configuration::getInstance().getTheme() == Theme::Light)
        {
            enabled = FALSE;
            SetWindowTheme((HWND)widget->winId(), L"Explorer", NULL);
        }
        else
        {
            enabled = TRUE;
            SetWindowTheme((HWND)widget->winId(), L"DarkMode_Explorer", NULL);
        }
		DwmSetWindowAttribute((HWND)widget->winId(), 20, &enabled, sizeof(enabled));
	}
}