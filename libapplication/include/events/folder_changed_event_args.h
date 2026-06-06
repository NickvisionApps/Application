#pragma once

#include <filesystem>
#include <libdesktop.h>
#include <vector>

namespace application::events
{
	class folder_changed_event_args : public desktop::events::event_args
	{
	public:
		folder_changed_event_args(std::filesystem::path path, std::vector<std::filesystem::path> files = {});
		const std::filesystem::path& get_path() const;
		const std::vector<std::filesystem::path>& get_files() const;

	private:
		std::filesystem::path m_path;
		std::vector<std::filesystem::path> m_files;
	};
}