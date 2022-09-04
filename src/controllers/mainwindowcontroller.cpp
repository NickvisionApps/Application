#include "mainwindowcontroller.hpp"
#include <chrono>
#include <filesystem>
#include <thread>

using namespace NickvisionApplication::Controllers;
using namespace NickvisionApplication::Models;

MainWindowController::MainWindowController(AppInfo& appInfo, Configuration& configuration) : m_appInfo{ appInfo }, m_configuration{ configuration }, m_folderPath{ "No Folder Opened" }
{

}

const AppInfo& MainWindowController::getAppInfo() const
{
    return m_appInfo;
}

PreferencesDialogController MainWindowController::createPreferencesDialogController() const
{
    return { m_configuration };
}

void MainWindowController::startup()
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

const std::string& MainWindowController::getFolderPath() const
{
    return m_folderPath;
}

bool MainWindowController::getIsFolderValid() const
{
    return std::filesystem::exists(m_folderPath);
}

void MainWindowController::registerFolderChangedCallback(const std::function<void()>& callback)
{
    m_folderChangedCallback = callback;
}


bool MainWindowController::openFolder(const std::string& folderPath)
{
    m_folderPath = folderPath;
    bool isValid = getIsFolderValid();
    if(!isValid)
    {
        m_folderPath = "No Folder Opened";
    }
    m_folderChangedCallback();
    return isValid;
}  

void MainWindowController::closeFolder()
{
    m_folderPath = "No Folder Opened";
    m_folderChangedCallback();
}
