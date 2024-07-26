#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui { class MainWindow; }

namespace Nickvision::Application::QT::Views
{
    class MainWindow : public QMainWindow
    {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow* m_ui;
    };
}

#endif //MAINWINDOW_H