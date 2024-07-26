#include "views/mainwindow.h"
#include "ui_mainwindow.h"
#include <libnick/localization/gettext.h>
#include "controls/aboutdialog.h"
#include "views/settingsdialog.h"

using namespace Nickvision::App;
using namespace Nickvision::Application::Shared::Controllers;
using namespace Nickvision::Application::QT::Controls;
using namespace Nickvision::Update;

namespace Nickvision::Application::QT::Views
{
    MainWindow::MainWindow(const std::shared_ptr<MainWindowController>& controller, QWidget* parent) 
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
        //Localize Folder Page
        m_ui->btnFolderOpenFolder->setText(_("Open"));
        //Signals
        connect(m_ui->actionExit, &QAction::triggered, this, &MainWindow::exit);
        connect(m_ui->actionSettings, &QAction::triggered, this, &MainWindow::settings);
        connect(m_ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    }

    MainWindow::~MainWindow()
    {
        delete m_ui;
    }

    void MainWindow::show()
    {
        QMainWindow::show();
        m_ui->viewStack->setCurrentIndex(0);
#ifdef _WIN32
        WindowGeometry geometry{ m_controller->startup(reinterpret_cast<HWND>(winId())) };
#elif defined(__linux__)
        WindowGeometry geometry{ m_controller->startup(m_controller->getAppInfo().getId() + ".desktop") };
#else
        WindowGeometry geometry{ m_controller->startup() };
#endif
        if(geometry.isMaximized())
        {
            showMaximized();
        }
        else
        {
            setGeometry(QWidget::geometry().x(), QWidget::geometry().y(), geometry.getWidth(), geometry.getHeight());
        }
    }

    void MainWindow::closeEvent(QCloseEvent* event)
    {
        if(!m_controller->canShutdown())
        {
            return event->ignore();
        }
        m_controller->shutdown({ geometry().width(), geometry().height(), isMaximized() });
        event->accept();
    }

    void MainWindow::exit()
    {
        close();
    }

    void MainWindow::settings()
    {
        SettingsDialog dialog{ m_controller->createPreferencesViewController(), this };
        dialog.exec();
    }

    void MainWindow::about()
    {
        AboutDialog dialog{ m_controller->getAppInfo(), m_controller->getDebugInformation(), this };
        dialog.exec();
    }
}