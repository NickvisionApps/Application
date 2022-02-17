#pragma once

#include <string>

namespace NickvisionApplication::Models
{
	class Configuration
	{
	public:
		Configuration();
		bool GetPreferLightTheme() const;
		void SetPreferLightTheme(bool preferLightTheme);
		void Save() const;

	private:
		std::string m_configDir;
		bool m_preferLightTheme;
	};
}

