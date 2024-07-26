#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include "controllers/mainwindowcontroller.h"

namespace Ui { class MainWindow; }

namespace Nickvision::Application::QT::Views
{
    class MainWindow : public QMainWindow
    {
    Q_OBJECT

    public:
        MainWindow(const std::shared_ptr<Shared::Controllers::MainWindowController>& controller, QWidget *parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow* m_ui;
        std::shared_ptr<Shared::Controllers::MainWindowController> m_controller;
    };
}

#endif //MAINWINDOW_H