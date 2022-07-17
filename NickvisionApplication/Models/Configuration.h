#pragma once

#include <string>
#include "Theme.h"

namespace NickvisionApplication::Models
{
	/// <summary>
	/// A model for an application's configuration
	/// </summary>
	class Configuration
	{
	public:
		Configuration(const Configuration&) = delete;
		void operator=(const Configuration&) = delete;
		/// <summary>
		/// Gets the Configuration singleton object
		/// </summary>
		/// <returns>A reference to the AppInfo object</returns>
		static Configuration& getInstance();
		/// <summary>
		/// Gets the theme of the application
		/// </summary>
		/// <returns>The theme of the application</returns>
		Theme getTheme() const;
		/// <summary>
		/// Sets the theme of the application
		/// </summary>
		/// <param name="theme">The theme of the application</param>
		void setTheme(Theme theme);
		/// <summary>
		/// Saves the configuration file to disk
		/// </summary>
		void save() const;

	private:
		/// <summary>
		/// Constructs a Configuration object
		/// </summary>
		Configuration();
		std::string m_configDir;
		Theme m_theme;
	};
}

