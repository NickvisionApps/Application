#include "views/mainwindow.h"
#include <QAction>
#include <QDesktopServices>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>
#include <QScrollArea>
#include <QToolBar>
#include <QVBoxLayout>
#include <libnick/helpers/codehelpers.h>
#include <libnick/localization/gettext.h>
#include <libnick/notifications/shellnotification.h>
#include "controls/aboutdialog.h"
#include "controls/infobar.h"
#include "helpers/qthelpers.h"
#include "views/settingsdialog.h"

using namespace Nickvision::App;
using namespace Nickvision::Application::Qt::Controls;
using namespace Nickvision::Application::Qt::Helpers;
using namespace Nickvision::Application::Shared::Controllers;
using namespace Nickvision::Application::Shared::Models;
using namespace Nickvision::Events;
using namespace Nickvision::Helpers;
using namespace Nickvision::Notifications;
using namespace Nickvision::Update;

namespace Ui
{
    class MainWindow
    {
    public:
        void setupUi(Nickvision::Application::Qt::Views::MainWindow* parent) 
        {
            //Actions
            actionOpenFolder = new QAction(parent);
            actionOpenFolder->setText(_("Open Folder"));
            actionOpenFolder->setIcon(QLEMENTINE_ICON(File_FolderOpen));
            actionOpenFolder->setShortcut(Qt::CTRL | Qt::Key_O);
            actionCloseFolder = new QAction(parent);
            actionCloseFolder->setText(_("Close Folder"));
            actionCloseFolder->setIcon(QLEMENTINE_ICON(File_Folder));
            actionCloseFolder->setShortcut(Qt::CTRL | Qt::Key_W);
            actionExit = new QAction(parent);
            actionExit->setText(_("Exit"));
            actionExit->setIcon(QLEMENTINE_ICON(Action_Close));
            actionExit->setShortcut(Qt::CTRL | Qt::Key_Q);
            actionSettings = new QAction(parent);
            actionSettings->setText(_("Settings"));
            actionSettings->setIcon(QLEMENTINE_ICON(Navigation_Settings));
            actionSettings->setShortcut(Qt::CTRL | Qt::Key_Comma);
            actionCheckForUpdates = new QAction(parent);
            actionCheckForUpdates->setText(_("Check for Updates"));
            actionCheckForUpdates->setIcon(QLEMENTINE_ICON(Action_Update));
            actionGitHubRepo = new QAction(parent);
            actionGitHubRepo->setText(_("GitHub Repo"));
            actionGitHubRepo->setIcon(QLEMENTINE_ICON(Software_VersionControl));
            actionReportABug = new QAction(parent);
            actionReportABug->setText(_("Report a Bug"));
            actionReportABug->setIcon(QLEMENTINE_ICON(Misc_Debug));
            actionDiscussions = new QAction(parent);
            actionDiscussions->setText(_("Discussions"));
            actionDiscussions->setIcon(QLEMENTINE_ICON(Misc_Users));
            actionAbout = new QAction(parent);
            actionAbout->setText(_("About Application"));
            actionAbout->setIcon(QLEMENTINE_ICON(Misc_Info));
            actionAbout->setShortcut(Qt::Key_F1);
            //InfoBar
            infoBar = new Nickvision::Application::Qt::Controls::InfoBar(parent);
            parent->addDockWidget(::Qt::BottomDockWidgetArea, infoBar);
            //MenuBar
            QMenu* menuFile{ new QMenu(parent) };
            menuFile->setTitle(_("File"));
            menuFile->addAction(actionOpenFolder);
            menuFile->addAction(actionCloseFolder);
            menuFile->addSeparator();
            menuFile->addAction(actionExit);
            QMenu* menuEdit{ new QMenu(parent) };
            menuEdit->setTitle(_("Edit"));
            menuEdit->addAction(actionSettings);
            QMenu* menuHelp{ new QMenu(parent) };
            menuHelp->setTitle(_("Help"));
            menuHelp->addAction(actionCheckForUpdates);
            menuHelp->addSeparator();
            menuHelp->addAction(actionGitHubRepo);
            menuHelp->addAction(actionReportABug);
            menuHelp->addAction(actionDiscussions);
            menuHelp->addSeparator();
            menuHelp->addAction(actionAbout);
            parent->menuBar()->addMenu(menuFile);
            parent->menuBar()->addMenu(menuEdit);
            parent->menuBar()->addMenu(menuHelp);
            //ToolBar
            QToolBar* toolBar{ new QToolBar(parent) };
            toolBar->setAllowedAreas(::Qt::ToolBarArea::TopToolBarArea);
            toolBar->setMovable(false);
            toolBar->setFloatable(false);
            toolBar->addAction(actionOpenFolder);
            toolBar->addAction(actionCloseFolder);
            parent->addToolBar(toolBar);
            //Files View
            listFiles = new QListWidget(parent);
            listFiles->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
            lblFiles = new QLabel(parent);
            lblFiles->setAlignment(::Qt::AlignmentFlag::AlignCenter);
            lblFiles->setText(_("No Folder Opened"));
            //Main Layout
            QWidget* centralWidget{ new QWidget(parent) };
            QHBoxLayout* layoutMain{ new QHBoxLayout(parent) };
            QScrollArea* scrollFiles{ new QScrollArea(parent) };
            scrollFiles->setMaximumWidth(300);
            scrollFiles->setWidgetResizable(true);
            scrollFiles->setWidget(listFiles);
            layoutMain->addWidget(scrollFiles);
            layoutMain->addWidget(lblFiles);
            centralWidget->setLayout(layoutMain);
            parent->setCentralWidget(centralWidget);
        }

        QAction* actionOpenFolder;
        QAction* actionCloseFolder;
        QAction* actionExit;
        QAction* actionSettings;
        QAction* actionCheckForUpdates;
        QAction* actionGitHubRepo;
        QAction* actionReportABug;
        QAction* actionDiscussions;
        QAction* actionAbout;
        Nickvision::Application::Qt::Controls::InfoBar* infoBar;
        QListWidget* listFiles;
        QLabel* lblFiles;
    };
}

namespace Nickvision::Application::Qt::Views
{
    MainWindow::MainWindow(const std::shared_ptr<MainWindowController>& controller, oclero::qlementine::ThemeManager* themeManager, QWidget* parent) 
        : QMainWindow{ parent },
        m_ui{ new Ui::MainWindow() },
        m_controller{ controller },
        m_themeManager{ themeManager }
    {
        //Window Settings
        bool stable{ m_controller->getAppInfo().getVersion().getVersionType() == VersionType::Stable };
        setWindowTitle(stable ? _("Application") : _("Application (Preview)"));
        setWindowIcon(QIcon(":/icon.svg"));
        setAcceptDrops(true);
        //Load Ui
        m_ui->setupUi(this);
        //Signals
        connect(m_ui->actionOpenFolder, &QAction::triggered, this, &MainWindow::openFolder);
        connect(m_ui->actionCloseFolder, &QAction::triggered, this, &MainWindow::closeFolder);
        connect(m_ui->actionExit, &QAction::triggered, this, &MainWindow::close);
        connect(m_ui->actionSettings, &QAction::triggered, this, &MainWindow::settings);
        connect(m_ui->actionCheckForUpdates, &QAction::triggered, this, &MainWindow::checkForUpdates);
        connect(m_ui->actionGitHubRepo, &QAction::triggered, this, &MainWindow::gitHubRepo);
        connect(m_ui->actionReportABug, &QAction::triggered, this, &MainWindow::reportABug);
        connect(m_ui->actionDiscussions, &QAction::triggered, this, &MainWindow::discussions);
        connect(m_ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
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
        setGeometry(QWidget::geometry().x(), QWidget::geometry().y(), info.getWindowGeometry().getWidth(), info.getWindowGeometry().getHeight());
        if(info.getWindowGeometry().isMaximized())
        {
            showMaximized();
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

    void MainWindow::settings()
    {
        SettingsDialog dialog{ m_controller->createPreferencesViewController(), m_themeManager, this };
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
        m_ui->infoBar->show(args, actionText, actionCallback);
    }

    void MainWindow::onShellNotificationSent(const ShellNotificationSentEventArgs& args)
    {
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
            m_ui->lblFiles->setText(_("No Folder Opened"));
        }
    }
}
