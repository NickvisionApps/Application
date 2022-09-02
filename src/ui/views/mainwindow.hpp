#pragma once

#include <adwaita.h>
#include "../../controllers/mainwindowcontroller.hpp"

namespace NickvisionApplication::UI::Views
{
    /**
     * The MainWindow for the application
     */
    class MainWindow
    {
    public:
    	/**
    	 * Constructs a MainWindow
    	 *
    	 * @param application GtkApplication*
    	 * @param appInfo The AppInfo for the application
    	 */
    	MainWindow(GtkApplication* application, const NickvisionApplication::Controllers::MainWindowController& controller);
    	/**
    	 * Gets the GtkWidget* representing the MainWindow
    	 *
    	 * @returns The GtkWidget* representing the MainWindow 
    	 */
    	GtkWidget* gobj() const;
    	/**
    	 * Shows the MainWindow
    	 */
    	void show();
    	/**
    	 * Prompts the user to open a folder from disk and load it in the app
    	 */
    	void onOpenFolder();
    	/**
    	 * Closes the opened folder
    	 */
	void onCloseFolder();
    	
    	
    private:
    	NickvisionApplication::Controllers::MainWindowController m_controller;
	GtkWidget* m_gobj{ nullptr };
	GtkWidget* m_mainBox{ nullptr };
	GtkWidget* m_headerBar{ nullptr };
	GtkWidget* m_adwTitle{ nullptr };
	GtkWidget* m_btnOpenFolder{ nullptr };
	GtkWidget* m_btnCloseFolder{ nullptr };
	GtkWidget* m_toastOverlay{ nullptr };
	GSimpleAction* m_actOpenFolder{ nullptr };
	GSimpleAction* m_actCloseFolder{ nullptr };
    };
}