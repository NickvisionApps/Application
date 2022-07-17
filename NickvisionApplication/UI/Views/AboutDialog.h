#pragma once

#include <QDialog>
#include "ui_AboutDialog.h"

namespace NickvisionApplication::UI::Views
{
	class AboutDialog : public QDialog
	{
		Q_OBJECT

	public:
		AboutDialog(QWidget* parent);

	private slots:
		/// <summary>
		/// Opens the default browser to the application's GitHub repo
		/// </summary>
		void on_btnGitHubRepo_clicked();
		/// <summary>
		/// Opens the default browser to the application's issue tracker
		/// </summary>
		void on_btnReportABug_clicked();
		/// <summary>
		/// Closes the dialog
		/// </summary>
		void on_btnClose_clicked();

	private:
		Ui::AboutDialog m_ui;
	};
}
