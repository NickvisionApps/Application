#pragma once

#include <libdesktop.h>
#include <memory>
#include <tuple>
#include "events/folder_changed_event_args.h"
#include "folder_service.h"

namespace application::services
{
	class events_service
	{
	public:
		using dependencies = std::tuple<desktop::app::configuration_service, folder_service, desktop::notifications::notification_service>;
		events_service(std::shared_ptr<desktop::app::configuration_service> configuration_service, std::shared_ptr<folder_service> folder_service,
		               std::shared_ptr<desktop::notifications::notification_service> notification_service);
		~events_service() = default;
		events_service(const events_service&) = delete;
		events_service(events_service&&) = delete;
		const desktop::events::event<desktop::notifications::notification_service, desktop::notifications::app_notification_sent_event_args>&
		get_app_notification_sent_event() const;
		const desktop::events::event<desktop::app::configuration_service, desktop::app::configuration_saved_event_args>& get_configuration_saved_event() const;
		const desktop::events::event<folder_service, events::folder_changed_event_args>& get_folder_changed_event() const;
		events_service& operator=(const events_service&) = delete;
		events_service& operator=(events_service&&) = delete;

	private:
		std::shared_ptr<desktop::app::configuration_service> m_configuration_service;
		std::shared_ptr<folder_service> m_folder_service;
		std::shared_ptr<desktop::notifications::notification_service> m_notification_service;
	};
}