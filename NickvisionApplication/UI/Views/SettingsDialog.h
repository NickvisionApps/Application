#pragma once

#include <QDialog>
#include "ui_SettingsDialog.h"
#include "../../Models/Configuration.h"

namespace NickvisionApplication::UI::Views
{
	/// <summary>
	/// A dialog for managing application settings
	/// </summary>
	class SettingsDialog : public QDialog
	{
		Q_OBJECT

	public:
		/// <summary>
		/// Constructs a SettingsDialog
		/// </summary>
		/// <param name="parent">The parent of the dialog</param>
		SettingsDialog(QWidget* parent);

	private slots:
		/// <summary>
		/// Change the viewStack page based on treeNav's selected item
		/// </summary>
		void on_treeNav_itemSelectionChanged();
		/// <summary>
		/// Saves the configuration and closes the dialog
		/// </summary>
		void on_btnSave_clicked();
		/// <summary>
		/// Disgards the changes to the configuration and closes the dialog
		/// </summary>
		void on_btnCancel_clicked();

	private:
		NickvisionApplication::Models::Configuration& m_configuration;
		//==UI==//
		Ui::SettingsDialog m_ui;
	};
}
