#include "views/mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>
#include <libnick/helpers/codehelpers.h>
#include <libnick/localization/gettext.h>
#include <libnick/notifications/shellnotification.h>
#include "controls/aboutdialog.h"
#include "helpers/qthelpers.h"
#include "views/settingsdialog.h"

using namespace Nickvision::App;
using namespace Nickvision::Application::QT::Controls;
using namespace Nickvision::Application::QT::Helpers;
using namespace Nickvision::Application::Shared::Controllers;
using namespace Nickvision::Events;
using namespace Nickvision::Helpers;
using namespace Nickvision::Notifications;
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
        setAcceptDrops(true);
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
        connect(m_ui->actionOpenFolder, &QAction::triggered, this, &MainWindow::openFolder);
        connect(m_ui->actionCloseFolder, &QAction::triggered, this, &MainWindow::closeFolder);
        connect(m_ui->actionExit, &QAction::triggered, this, &MainWindow::exit);
        connect(m_ui->actionSettings, &QAction::triggered, this, &MainWindow::settings);
        connect(m_ui->actionCheckForUpdates, &QAction::triggered, this, &MainWindow::checkForUpdates);
        connect(m_ui->actionGitHubRepo, &QAction::triggered, this, &MainWindow::gitHubRepo);
        connect(m_ui->actionReportABug, &QAction::triggered, this, &MainWindow::reportABug);
        connect(m_ui->actionDiscussions, &QAction::triggered, this, &MainWindow::discussions);
        connect(m_ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
        connect(m_ui->btnHomeOpenFolder, &QPushButton::clicked, this, &MainWindow::openFolder);
        connect(m_ui->btnFolderOpenFolder, &QPushButton::clicked, this, &MainWindow::openFolder);
        connect(m_ui->btnFolderCloseFolder, &QPushButton::clicked, this, &MainWindow::closeFolder);
        m_controller->notificationSent() += [&](const NotificationSentEventArgs& args) { QTHelpers::dispatchToMainThread([this, args]() { onNotificationSent(args); }); };
        m_controller->shellNotificationSent() += [&](const ShellNotificationSentEventArgs& args) { onShellNotificationSent(args); };
        m_controller->folderChanged() += [&](const EventArgs& args) { onFolderChanged(args); };
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

    void MainWindow::dragEnterEvent(QDragEnterEvent* event)
    {
        if(event->mimeData()->hasUrls())
        {
            event->acceptProposedAction();
        }
    }

    void MainWindow::dropEvent(QDropEvent* event)
    {
        if(event->mimeData()->hasUrls())
        {
            m_controller->openFolder(event->mimeData()->urls()[0].toLocalFile().toStdString());
        }
    }

    void MainWindow::openFolder()
    {
        QString path{ QFileDialog::getExistingDirectory(this, _("Open Folder"), {}, QFileDialog::ShowDirsOnly) };
        m_controller->openFolder(path.toStdString());
    }

    void MainWindow::closeFolder()
    {
        m_controller->closeFolder();
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

    void MainWindow::checkForUpdates()
    {
        m_controller->checkForUpdates();
    }

#ifdef _WIN32
    void MainWindow::windowsUpdate()
    {
        m_controller->windowsUpdate();
    }
#endif

    void MainWindow::gitHubRepo()
    {
        QDesktopServices::openUrl(QString::fromStdString(m_controller->getAppInfo().getSourceRepo()));
    }

    void MainWindow::reportABug()
    {
        QDesktopServices::openUrl(QString::fromStdString(m_controller->getAppInfo().getIssueTracker()));
    }

    void MainWindow::discussions()
    {
        QDesktopServices::openUrl(QString::fromStdString(m_controller->getAppInfo().getSupportUrl()));
    }

    void MainWindow::about()
    {
        AboutDialog dialog{ m_controller->getAppInfo(), m_controller->getDebugInformation(), this };
        dialog.exec();
    }

    void MainWindow::onNotificationSent(const NotificationSentEventArgs& args)
    {
        QMessageBox::Icon icon{ QMessageBox::Icon::NoIcon };
        switch(args.getSeverity())
        {
        case NotificationSeverity::Informational:
        case NotificationSeverity::Success:
            icon = QMessageBox::Icon::Information;
            break;
        case NotificationSeverity::Warning:
            icon = QMessageBox::Icon::Warning;
            break;
        case NotificationSeverity::Error:
            icon = QMessageBox::Icon::Critical;
            break;
        }
        QMessageBox msgBox{ icon, QString::fromStdString(m_controller->getAppInfo().getShortName()), QString::fromStdString(args.getMessage()), QMessageBox::StandardButton::Ok, this };
        if(args.getAction() == "update")
        {
            QPushButton* updateButton{ msgBox.addButton(_("Update"), QMessageBox::ButtonRole::ActionRole) };
            connect(updateButton, &QPushButton::clicked, this, &MainWindow::checkForUpdates);
        }
        else if(args.getAction() == "close")
        {
            QPushButton* closeButton{ msgBox.addButton(_("Close"), QMessageBox::ButtonRole::ActionRole) };
            connect(closeButton, &QPushButton::clicked, this, &MainWindow::closeFolder);
        }
        msgBox.exec();
    }

    void MainWindow::onShellNotificationSent(const ShellNotificationSentEventArgs& args)
    {
        m_controller->log(Logging::LogLevel::Info, "ShellNotification sent. (" + args.getMessage() + ")");
#ifdef _WIN32
        ShellNotification::send(args, reinterpret_cast<HWND>(winId()));
#elif defined(__linux__)
        ShellNotification::send(args, m_controller->getAppInfo().getId(), _("Open"));
#endif
    }

    void MainWindow::onFolderChanged(const EventArgs& args)
    {
        m_ui->viewStack->setCurrentIndex(m_controller->isFolderOpened() ? 1 : 0);
        m_ui->lblFiles->setText(QString::fromStdString(std::vformat(_n("There is {} file in the folder.", "There are {} files in the folder.", m_controller->getFiles().size()), std::make_format_args(CodeHelpers::unmove(m_controller->getFiles().size())))));
        for(const std::filesystem::path& file : m_controller->getFiles())
        {
            m_ui->listFiles->addItem(QString::fromStdString(file.filename().string()));
        }
    }
}