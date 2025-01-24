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
#include "views/settingspage.h"

using namespace Nickvision::App;
using namespace Nickvision::Application::Qt::Controls;
using namespace Nickvision::Application::Qt::Helpers;
using namespace Nickvision::Application::Shared::Controllers;
using namespace Nickvision::Application::Shared::Models;
using namespace Nickvision::Events;
using namespace Nickvision::Helpers;
using namespace Nickvision::Notifications;
using namespace Nickvision::Update;

namespace Nickvision::Application::Qt::Views
{
    MainWindow::MainWindow(const std::shared_ptr<MainWindowController>& controller, QWidget* parent) 
        : QMainWindow{ parent },
        m_ui{ new Ui::MainWindow() },
        m_infoBar{ new InfoBar(this) },
        m_controller{ controller }
    {
        m_ui->setupUi(this);
        setWindowTitle(m_controller->getAppInfo().getVersion().getVersionType() == VersionType::Stable ? _("Application") : _("Application (Preview)"));
        addDockWidget(::Qt::TopDockWidgetArea, m_infoBar);
        //Home Page
        m_ui->lblHomeGreeting->setText(QString::fromStdString(m_controller->getGreeting()));
        m_ui->lblHomeDescription->setText(_("Open a folder (or drag one into the app) to get started"));
        m_ui->btnHomeOpenFolder->setText(_("Open Folder"));
        //Folder Page
        m_ui->btnFolderOpenFolder->setText(_("Open"));
        //Signals
        connect(m_ui->btnHomeOpenFolder, &QPushButton::clicked, this, &MainWindow::openFolder);
        connect(m_ui->btnFolderOpenFolder, &QPushButton::clicked, this, &MainWindow::openFolder);
        connect(m_ui->btnFolderCloseFolder, &QPushButton::clicked, this, &MainWindow::closeFolder);
        m_controller->notificationSent() += [&](const NotificationSentEventArgs& args) { QtHelpers::dispatchToMainThread([this, args]() { onNotificationSent(args); }); };
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
#ifdef _WIN32
        const StartupInformation& info{ m_controller->startup(reinterpret_cast<HWND>(winId())) };
#elif defined(__linux__)
        const StartupInformation& info{ m_controller->startup(m_controller->getAppInfo().getId() + ".desktop") };
#else
        const StartupInformation& info{ m_controller->startup() };
#endif
        if(info.getWindowGeometry().isMaximized())
        {
            showMaximized();
        }
        else
        {
            setGeometry(QWidget::geometry().x(), QWidget::geometry().y(), info.getWindowGeometry().getWidth(), info.getWindowGeometry().getHeight());
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
        std::string extraDebug;
        extraDebug += "Qt " + std::string(qVersion()) + "\n";
        AboutDialog dialog{ m_controller->getAppInfo(), m_controller->getDebugInformation(extraDebug), this };
        dialog.exec();
    }

    void MainWindow::onNotificationSent(const NotificationSentEventArgs& args)
    {
        QString actionText;
        std::function<void()> actionCallback;
        if(args.getAction() == "close")
        {
            actionText = _("Close");
            actionCallback = [this]() { closeFolder(); };
        }
#ifdef _WIN32
        else if(args.getAction() == "update")
        {
            actionText = _("Update");
            actionCallback = [this]() { windowsUpdate(); };
        }
#endif
        m_infoBar->show(args, actionText, actionCallback);
    }

    void MainWindow::onShellNotificationSent(const ShellNotificationSentEventArgs& args)
    {
        m_controller->log(Logging::LogLevel::Info, "ShellNotification sent. (" + args.getMessage() + ")");
#ifdef _WIN32
        ShellNotification::send(args, reinterpret_cast<HWND>(winId()));
#elif defined(__linux__)
        ShellNotification::send(args, m_controller->getAppInfo().getId(), _("Open"));
#else
        ShellNotification::send(args);
#endif
    }

    void MainWindow::onFolderChanged(const EventArgs& args)
    {
        if(m_controller->isFolderOpened())
        {
            m_ui->lblFiles->setText(QString::fromStdString(std::vformat(_n("There is {} file in the folder.", "There are {} files in the folder.", m_controller->getFiles().size()), std::make_format_args(CodeHelpers::unmove(m_controller->getFiles().size())))));
            for(const std::filesystem::path& file : m_controller->getFiles())
            {
                m_ui->listFiles->addItem(QString::fromStdString(file.filename().string()));
            }
        }
        else
        {
            m_ui->listFiles->clear();
        }
    }
}
