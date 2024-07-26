#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QCloseEvent>
#include <QMainWindow>
#include "controllers/mainwindowcontroller.h"

namespace Ui { class MainWindow; }

namespace Nickvision::Application::QT::Views
{
    /**
     * @brief The main window for the application.
     */
    class MainWindow : public QMainWindow
    {
    Q_OBJECT

    public:
        /**
         * @brief Constructs a MainWindow.
         * @param controller The MainWindowController
         * @param parent The parent widget
         */
        MainWindow(const std::shared_ptr<Shared::Controllers::MainWindowController>& controller, QWidget* parent = nullptr);
        /**
         * @brief Destructs a MainWindow.
         */
        ~MainWindow();
        /**
         * @brief Shows the MainWindow.
         */
        void show();

    protected:
        /**
         * @brief Handles when the window is closed.
         * @param event QCloseEvent
         */
        void closeEvent(QCloseEvent* event) override;

    private slots:
        /**
         * @brief Prompts the user to open a folder.
         */
        void openFolder();
        /**
         * @brief Closes the folder if one is open.
         */
        void closeFolder();
        /**
         * @brief Exits the application.
         */
        void exit();
        /**
         * @brief Displays the settings dialog.
         */
        void settings();
        /**
         * @brief Checks for application updates.
         */
        void checkForUpdates();
        /**
         * @brief Opens the application's GitHub repo in the browser.
         */
        void gitHubRepo();
        /**
         * @brief Opens the application's issue tracker in the browser.
         */
        void reportABug();
        /**
         * @brief Opens the application's discussions page in the browser.
         */
        void discussions();
        /**
         * @brief Displays the about dialog.
         */
        void about();

    private:
        void onNotificationSent(const Notifications::NotificationSentEventArgs& args);
        void onShellNotificationSent(const Notifications::ShellNotificationSentEventArgs& args);
        void onFolderChanged(const Events::EventArgs& args);
        Ui::MainWindow* m_ui;
        std::shared_ptr<Shared::Controllers::MainWindowController> m_controller;
    };
}

#endif //MAINWINDOW_H