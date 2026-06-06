#include "events/folder_changed_event_args.h"

namespace application::events
{
	folder_changed_event_args::folder_changed_event_args(std::filesystem::path path, std::vector<std::filesystem::path> files)
	    : m_path{ std::move(path) },
	      m_files{ std::move(files) }
	{
	}

	const std::filesystem::path& folder_changed_event_args::get_path() const
	{
		return m_path;
	}

	const std::vector<std::filesystem::path>& folder_changed_event_args::get_files() const
	{
		return m_files;
	}
}