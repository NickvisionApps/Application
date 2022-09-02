#include "mainwindowcontroller.hpp"
#include <filesystem>

using namespace NickvisionApplication::Controllers;
using namespace NickvisionApplication::Models;

MainWindowController::MainWindowController(AppInfo& appInfo) : m_appInfo{ appInfo }, m_folderPath{ "No Folder Opened" }
{

}

const std::string& MainWindowController::getApplicationShortName() const
{
    return m_appInfo.getShortName();
}

const std::string& MainWindowController::getFolderPath() const
{
    return m_folderPath;
}

bool MainWindowController::openFolder(const std::string& folderPath)
{
    if(std::filesystem::exists(folderPath))
    {
        m_folderPath = folderPath;
        return true;
    }
    m_folderPath = "No Folder Opened";
    return false;
}  

void MainWindowController::closeFolder()
{
    m_folderPath = "No Folder Opened";
}
