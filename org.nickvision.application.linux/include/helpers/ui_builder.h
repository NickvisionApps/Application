#pragma once

#include <adwaita.h>
#include <libdesktop.h>
#include <memory>
#include <string>

#undef linux

namespace application::linux::helpers
{
	class ui_builder
	{
	public:
		ui_builder(const std::string& name, const std::shared_ptr<desktop::app::translation_service>& translation_service);
		~ui_builder();
		ui_builder(const ui_builder&) = delete;
		ui_builder(ui_builder&&) = delete;
		GtkBuilder* gobj() const;
		template <typename T>
		T* get(const std::string& name) const
		{
			return reinterpret_cast<T*>(gtk_builder_get_object(m_builder, name.c_str()));
		}
		ui_builder& operator=(const ui_builder&) = delete;
		ui_builder& operator=(ui_builder&&) = delete;

	private:
		GtkBuilder* m_builder{ nullptr };
	};
}