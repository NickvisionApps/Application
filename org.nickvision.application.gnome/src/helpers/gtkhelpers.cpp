#include "helpers/gtkhelpers.h"

namespace Nickvision::Application::GNOME::Helpers
{
    void GtkHelpers::dispatchToMainThread(const std::function<void()>& func)
    {
        g_idle_add(+[](gpointer data) -> int
        {
            std::function<void()>* function{ static_cast<std::function<void()>*>(data) };
            (*function)();
            delete function;
            return G_SOURCE_REMOVE;
        }, new std::function<void()>(func));
    }

    void GtkHelpers::setAccelForAction(GtkApplication* app, const char* action, const char* accel)
    {
        const char* accels[2] { accel, nullptr };
        gtk_application_set_accels_for_action(app, action, accels);
    }

    void GtkHelpers::setComboRowModel(AdwComboRow* row, const std::vector<std::string>& strs, const std::string& selected, bool allowEllipse)
    {
        size_t selectedIndex{ 0 };
        GtkStringList* list{ gtk_string_list_new(nullptr) };
        for(size_t i = 0; i < strs.size(); i++)
        {
            const std::string& str{ strs[i] };
            gtk_string_list_append(list, str.c_str());
            if(str == selected)
            {
                selectedIndex = i;
            }
        }
        if(!allowEllipse)
        {
            g_signal_connect_after(adw_combo_row_get_factory(row), "setup", G_CALLBACK(+[](GtkListItemFactory* factory, GtkListItem* item, gpointer data)
            {
                GtkWidget* box{ gtk_list_item_get_child(item) };
                GtkLabel* lbl{ GTK_LABEL(gtk_widget_get_first_child(box)) };
                gtk_label_set_ellipsize(lbl, PANGO_ELLIPSIZE_NONE);
            }), nullptr);
        }
        adw_combo_row_set_model(row, G_LIST_MODEL(list));
        g_object_unref(list);
        adw_combo_row_set_selected(row, selectedIndex);
    }

    void GtkHelpers::setComboRowModel(AdwComboRow* row, const std::vector<std::string>& strs, const size_t& selected, bool allowEllipse)
    {
        GtkStringList* list{ gtk_string_list_new(nullptr) };
        for(const std::string& str : strs)
        {
            gtk_string_list_append(list, str.c_str());
        }
        if(!allowEllipse)
        {
            g_signal_connect_after(adw_combo_row_get_factory(row), "setup", G_CALLBACK(+[](GtkListItemFactory* factory, GtkListItem* item, gpointer data)
            {
                GtkWidget* box{ gtk_list_item_get_child(item) };
                GtkLabel* lbl{ GTK_LABEL(gtk_widget_get_first_child(box)) };
                gtk_label_set_ellipsize(lbl, PANGO_ELLIPSIZE_NONE);
            }), nullptr);
        }
        adw_combo_row_set_model(row, G_LIST_MODEL(list));
        g_object_unref(list);
        if(selected < strs.size())
        {
            adw_combo_row_set_selected(row, selected);
        }
        else
        {
            adw_combo_row_set_selected(row, 0);
        }
    }
}
