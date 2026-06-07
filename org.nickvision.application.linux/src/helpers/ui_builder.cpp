#include "helpers/ui_builder.h"
#include <filesystem>
#include <libxml++/libxml++.h>
#include <stdexcept>

using namespace desktop::app;
using namespace desktop::system;

namespace application::linux::helpers
{
	ui_builder::ui_builder(const std::string& name, const std::shared_ptr<translation_service>& translation_service)
	{
		std::filesystem::path path{ environment::get_executable_directory() / "ui" / (name + ".ui") };
		if (!std::filesystem::exists(path))
		{
			throw std::invalid_argument("UI file not found: " + path.string());
		}
		xmlpp::DomParser xml{ path.string() };
		xmlpp::Element* root{ xml.get_document()->get_root_node() };
		for (xmlpp::Node* node : root->find("//text()"))
		{
			xmlpp::Element* e{ node->get_parent() };
			if (e->get_attribute("translatable"))
			{
				xmlpp::TextNode* t{ dynamic_cast<xmlpp::TextNode*>(node) };
				e->remove_attribute("translatable");
				std::string context{ e->get_attribute_value("context") };
				if (!context.empty())
				{
					std::string p{ context + "\004" + t->get_content() };
					t->set_content(translation_service->_p(p.c_str(), t->get_content().c_str()));
				}
				else
				{
					t->set_content(translation_service->_(t->get_content().c_str()));
				}
			}
		}
		m_builder = gtk_builder_new_from_string(xml.get_document()->write_to_string().c_str(), -1);
		if (!m_builder)
		{
			throw std::runtime_error("Failed to create builder");
		}
	}

	ui_builder::~ui_builder()
	{
		g_object_unref(m_builder);
	}

	GtkBuilder* ui_builder::gobj() const
	{
		return m_builder;
	}
}