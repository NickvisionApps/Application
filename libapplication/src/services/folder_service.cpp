#include "services/folder_service.h"

using namespace application::events;
using namespace desktop::app;
using namespace desktop::events;
using namespace desktop::notifications;

namespace application::services
{
	folder_service::folder_service(std::shared_ptr<notification_service> notification_service, std::shared_ptr<translation_service> translation_service)
	    : m_notification_service{ std::move(notification_service) },
	      m_translation_service{ std::move(translation_service) }
	{
	}

	const event<folder_service, folder_changed_event_args>& folder_service::get_folder_changed_event() const
	{
		return m_folder_changed_event;
	}

	const std::filesystem::path& folder_service::get_path() const
	{
		return m_path;
	}

	const std::vector<std::filesystem::path>& folder_service::get_files() const
	{
		return m_files;
	}

	void folder_service::open(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
		{
			m_path = std::filesystem::path();
			m_files.clear();
			return;
		}
		m_path = path;
		m_files.clear();
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator{ path })
		{
			m_files.push_back(entry.path());
		}
		m_notification_service->send(
		    app_notification{ m_translation_service->_("Folder opened: {}", m_path.string()), notification_severity::success, "close" });
		m_notification_service->send(shell_notification{ m_translation_service->_("Folder"), m_translation_service->_("Loaded {} file(s)", m_files.size()),
		                                                 notification_severity::information, "open", m_path.string() });
		m_folder_changed_event.invoke(*this, folder_changed_event_args{ m_path, m_files });
	}

	void folder_service::close()
	{
		m_path = std::filesystem::path();
		m_files.clear();
		m_notification_service->send(app_notification{ m_translation_service->_("Folder closed"), notification_severity::warning });
		m_folder_changed_event.invoke(*this, folder_changed_event_args{ m_path });
	}
}