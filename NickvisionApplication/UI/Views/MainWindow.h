#pragma once

#include <QMainWindow>
#include <QLabel>
#include "ui_MainWindow.h"
#include "../../Models/Theme.h"
#include "../../Update/Updater.h"
#include "HomePage.h"
#include "Pages.h"

namespace NickvisionApplication::UI::Views
{
    /// <summary>
    /// The MainWindow
    /// </summary>
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        /// <summary>
        /// Constructs a MainWindow
        /// </summary>
        /// <param name="parent">The parent of the widget, if any</param>
        MainWindow(QWidget* parent = nullptr);

    private slots:
        /// <summary>
        /// Creates a new file to edit
        /// </summary>
        void on_menuNewFile_triggered();
        /// <summary>
        /// Opens a file to edit
        /// </summary>
        void on_menuOpenFile_triggered();
        /// <summary>
        /// Closes the current file being edited
        /// </summary>
        void on_menuCloseFile_triggered();
        /// <summary>
        /// Closes the window
        /// </summary>
        void on_menuExit_triggered();
        /// <summary>
        /// Opens the SettingsDialog
        /// </summary>
        void on_menuSettings_triggered();
        /// <summary>
        /// Checks for an application update
        /// </summary>
        void on_menuCheckForUpdates_triggered();
        /// <summary>
        /// Displays information about the runnning application
        /// </summary>
        void on_menuAbout_triggered();

    private:
        //==Vars==//
        NickvisionApplication::Models::Theme m_currentTheme;
        NickvisionApplication::Update::Updater m_updater;
        //==UI==//
        Ui::MainWindow m_ui;
        HomePage m_homePage;
        //==Functions==//
        /// <summary>
        /// Changes the page on the window
        /// </summary>
        /// <param name="page">The page to change to</param>
        void changePage(Pages page);
    };
}
