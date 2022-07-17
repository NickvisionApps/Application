#pragma once

#include <QWidget>
#include <QPalette>

/// <summary>
/// Functions for working with an application's theme
/// </summary>
namespace NickvisionApplication::Helpers::ThemeHelpers
{
	/// <summary>
	/// Gets a light themed QPalette
	/// </summary>
	/// <returns>Light themed QPalette</returns>
	QPalette getLightPalette();
	/// <summary>
	/// Gets a dark themed QPalette
	/// </summary>
	/// <returns>Dark themed QPalette</returns>
	QPalette getDarkPalette();
	/// <summary>
	/// Applys Win32 theming to QWidget
	/// </summary>
	/// <param name="widget">The QWidget object to apply Win32 theming</param>
	void applyWin32Theming(QWidget* widget);
}

