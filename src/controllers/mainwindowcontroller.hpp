#pragma once

#include <functional>
#include <string>
#include "../models/appinfo.hpp"

namespace NickvisionApplication::Controllers
{
    /**
     * A controller for the MainWindow
     */
    class MainWindowController
    {
    public:
    	/**
    	 * Constructs a MainWindowController
    	 *
    	 * @param appInfo The AppInfo for the application (Stored as a reference)
    	 */
    	MainWindowController(NickvisionApplication::Models::AppInfo& appInfo);
    	/**
    	 * Gets the short name for the application
    	 *
    	 * @returns The short name for the application
    	 */
    	const std::string& getApplicationShortName() const;
    	/**
    	 * Gets the opened folder path
    	 *
    	 * @returns The opened folder path or "No Folder Path" if no folder is opened
    	 */
    	const std::string& getFolderPath() const;
    	/**
    	 * Gets whether or not the folder is valid
    	 *
    	 * @returns True if folder is valid, else false
    	 */
    	bool getIsFolderValid() const;
    	/**
    	 * Registers a callback for when the folder is changed
    	 *
    	 * @param callback A void() function
    	 */
    	void registerFolderChangedCallback(const std::function<void()>& callback);
    	/**
    	 * Opens a folder with the given path
    	 * 
    	 * @param folderPath The path to the folder to open
    	 * @returns True if the folderPath is valid and the folder was opened, else false
    	 */
    	bool openFolder(const std::string& folderPath);
    	/**
    	 * Closes the folder if one is open
    	 */
    	void closeFolder();
    	
    private:
    	NickvisionApplication::Models::AppInfo& m_appInfo;
    	std::string m_folderPath;
    	std::function<void()> m_folderChangedCallback;
    };
}