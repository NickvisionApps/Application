#pragma once

#include <filesystem>
#include <libdesktop.h>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include "events/folder_changed_event_args.h"

namespace application::services
{
	class folder_service
	{
	public:
		using dependencies = std::tuple<desktop::notifications::notification_service, desktop::app::translation_service>;
		folder_service(std::shared_ptr<desktop::notifications::notification_service> notification_service,
		               std::shared_ptr<desktop::app::translation_service> translation_service);
		~folder_service() = default;
		folder_service(const folder_service&) = delete;
		folder_service(folder_service&&) = delete;
		const desktop::events::event<folder_service, events::folder_changed_event_args>& get_folder_changed_event() const;
		const std::filesystem::path& get_path() const;
		const std::vector<std::filesystem::path>& get_files() const;
		void open(const std::filesystem::path& path);
		void close();
		folder_service& operator=(const folder_service&) = delete;
		folder_service& operator=(folder_service&&) = delete;

	private:
		std::shared_ptr<desktop::notifications::notification_service> m_notification_service;
		std::shared_ptr<desktop::app::translation_service> m_translation_service;
		std::filesystem::path m_path;
		std::vector<std::filesystem::path> m_files;
		desktop::events::event<folder_service, events::folder_changed_event_args> m_folder_changed_event;
	};
}