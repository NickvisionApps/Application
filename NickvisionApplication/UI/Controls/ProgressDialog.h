#pragma once

#include <functional>
#include <thread>
#include <QDialog>
#include <QString>
#include "ui_ProgressDialog.h"

namespace NickvisionApplication::UI::Controls
{
	class ProgressDialog : public QDialog
	{
		Q_OBJECT

	public:
		ProgressDialog(QWidget* parent, const QString& description, const std::function<void()>& work);
		int exec();

	private:
		//==Vars==//
		bool m_isFinished;
		std::function<void()> m_work;
		//==UI==//
		Ui::ProgressDialog m_ui;
	};
}
