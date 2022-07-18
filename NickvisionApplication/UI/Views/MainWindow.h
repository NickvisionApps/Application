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

    private:
        //==Vars==//
        NickvisionApplication::Models::Theme m_currentTheme;
        NickvisionApplication::Update::Updater m_updater;
        //==UI==//
        Ui::MainWindow m_ui;
        HomePage m_homePage;
        //==Functions==//
        /// <summary>
        /// Opens the SettingsDialog
        /// </summary>
        void settings();
        /// <summary>
        /// Checks for an application update
        /// </summary>
        void checkForUpdates();
        /// <summary>
        /// Changes the page on the window
        /// </summary>
        /// <param name="page">The page to change to</param>
        void changePage(Pages page);
    };
}
