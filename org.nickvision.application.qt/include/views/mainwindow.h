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
         * @brief Exits the application.
         */
        void exit();
        /**
         * @brief Displays the settings dialog.
         */
        void settings();
        /**
         * @brief Displays the about dialog.
         */
        void about();

    private:
        Ui::MainWindow* m_ui;
        std::shared_ptr<Shared::Controllers::MainWindowController> m_controller;
    };
}

#endif //MAINWINDOW_H