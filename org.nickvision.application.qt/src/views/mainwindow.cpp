#include "views/mainwindow.h"
#include "ui_mainwindow.h"

namespace Nickvision::Application::QT::Views
{
    MainWindow::MainWindow(QWidget *parent) 
        : QMainWindow{ parent },
        m_ui{ new Ui::MainWindow }
    {
        m_ui->setupUi(this);
    }

    MainWindow::~MainWindow()
    {
        delete m_ui;
    }
}