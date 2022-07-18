#pragma once

#include <QWidget>
#include "ui_HomePage.h"

namespace NickvisionApplication::UI::Views
{
	/// <summary>
	/// A home page for the application
	/// </summary>
	class HomePage : public QWidget
	{
		Q_OBJECT

	public:
		/// <summary>
		/// Constructs a HomePage
		/// </summary>
		/// <param name="parent">The parent of the widget, if any</param>
		HomePage(QWidget* parent = nullptr);

	private slots:
		/// <summary>
		/// Sends the MainWindow.settings message
		/// </summary>
		void on_btnSettings_clicked();
		/// <summary>
		/// Sends the MainWindow.checkForUpdates message
		/// </summary>
		void on_btnCheckForUpdates_clicked();
		/// <summary>
		/// Displays an AboutDialog
		/// </summary>
		void on_btnAbout_clicked();

	private:
		//==UI==//
		Ui::HomePage m_ui;
	};
}
