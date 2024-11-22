#include "views/settingspage.h"
#include "ui_settingspage.h"
#include <libnick/localization/gettext.h>

using namespace Nickvision::Application::Shared::Controllers;
using namespace Nickvision::Application::Shared::Models;

namespace Nickvision::Application::QT::Views
{
    SettingsPage::SettingsPage(const std::shared_ptr<PreferencesViewController>& controller, QWidget* parent)
        : QWidget{ parent },
        m_ui{ new Ui::SettingsPage() },
        m_controller{ controller }
    {
        m_ui->setupUi(this);
        //Localize Strings
        m_ui->tabs->setTabText(0, _("User Interface"));
        m_ui->lblUpdates->setText(_("Automatically Check for Updates"));
        //Load Settings
        m_ui->tabs->setCurrentIndex(0);
        m_ui->chkUpdates->setChecked(m_controller->getAutomaticallyCheckForUpdates());
    }
    
    SettingsPage::~SettingsPage()
    {
        delete m_ui;
    }

    void SettingsPage::closeEvent(QCloseEvent* event)
    {
        m_controller->setAutomaticallyCheckForUpdates(m_ui->chkUpdates->isChecked());
        m_controller->saveConfiguration();
        event->accept();
    }
}