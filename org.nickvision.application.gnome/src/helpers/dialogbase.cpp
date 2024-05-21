#include "helpers/dialogbase.h"
#include "helpers/builder.h"

namespace Nickvision::Application::GNOME::Helpers
{
    DialogBase::DialogBase(GtkWindow* parent, const std::string& fileName, const std::string& rootName)
        : m_builder{ BuilderHelpers::fromBlueprint(fileName) },
        m_parent{ parent },
        m_dialog{ ADW_DIALOG(gtk_builder_get_object(m_builder, rootName.c_str())) }
    {

    }

    DialogBase::~DialogBase()
    {
        adw_dialog_force_close(m_dialog);
        g_object_unref(m_builder);
    }

    AdwDialog* DialogBase::get()
    {
        return m_dialog;
    }

    void DialogBase::present() const
    {
        adw_dialog_present(m_dialog, GTK_WIDGET(m_parent));
    }
}