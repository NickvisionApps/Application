#pragma once

#include <adwaita.h>

namespace NickvisionApplication::UI::Views
{
    class PreferencesDialog
    {
    public:
    	PreferencesDialog(GtkWindow* parent);
    	~PreferencesDialog();
    	GtkWidget* gobj() const;
    	void show();

    private:
	GtkWidget* m_gobj{ nullptr };
	GtkWidget* m_mainBox{ nullptr };
	GtkWidget* m_headerBar{ nullptr };
	GtkWidget* m_btnCancel{ nullptr };
	GtkWidget* m_btnSave{ nullptr };
	void onCancel();
        void onSave();
        void onRowIsFirstTimeOpenActivate();
    };
}