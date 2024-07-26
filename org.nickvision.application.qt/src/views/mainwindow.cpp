#include "views/mainwindow.h"
#include <libnick/localization/gettext.h>
#include "ui_mainwindow.h"

using namespace Nickvision::Application::Shared::Controllers;
using namespace Nickvision::Update;

namespace Nickvision::Application::QT::Views
{
    MainWindow::MainWindow(const std::shared_ptr<MainWindowController>& controller, QWidget *parent) 
        : QMainWindow{ parent },
        m_ui{ new Ui::MainWindow() },
        m_controller{ controller }
    {
        m_ui->setupUi(this);
        setWindowTitle(m_controller->getAppInfo().getVersion().getVersionType() == VersionType::Stable ? _("Application") : _("Application (Preview)"));
        //Localize Menu Strings
        m_ui->menuFile->setTitle(_("File"));
        m_ui->actionOpenFolder->setText(_("Open Folder"));
        m_ui->actionCloseFolder->setText(_("Close Folder"));
        m_ui->actionExit->setText(_("Exit"));
        m_ui->menuEdit->setTitle(_("Edit"));
        m_ui->actionSettings->setText(_("Settings"));
        m_ui->menuHelp->setTitle(_("Help"));
        m_ui->actionCheckForUpdates->setText(_("Check for Updates"));
        m_ui->actionGitHubRepo->setText(_("GitHub Repo"));
        m_ui->actionReportABug->setText(_("Report a Bug"));
        m_ui->actionDiscussions->setText(_("Discussions"));
        m_ui->actionAbout->setText(_("About Application"));
        //Localize Home Page
        m_ui->lblHomeGreeting->setText(QString::fromStdString(m_controller->getGreeting()));
        m_ui->lblHomeDescription->setText(_("Open a folder (or drag one into the app) to get started"));
        m_ui->btnHomeOpenFolder->setText(_("Open Folder"));
    }

    MainWindow::~MainWindow()
    {
        delete m_ui;
    }
}