#include "views/settingsdialog.h"
#include <QApplication>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QStackedWidget>
#include <QStyleHints>
#include <libnick/localization/gettext.h>
#include <oclero/qlementine/widgets/Switch.hpp>
#include "helpers/qthelpers.h"

using namespace Nickvision::Application::Shared::Controllers;
using namespace oclero::qlementine;

namespace Ui 
{
    class SettingsDialog
    {
    public:
        void setupUi(Nickvision::Application::Qt::Views::SettingsDialog* parent)
        {
            viewStack = new QStackedWidget(parent);
            //User Interface Page
            QLabel* lblTheme{ new QLabel(parent) };
            lblTheme->setText(_("Theme"));
            cmbTheme = new QComboBox(parent);
            cmbTheme->addItem(_("Light"));
            cmbTheme->addItem(_("Dark"));
            cmbTheme->addItem(_("System"));
            QLabel* lblUpdates{ new QLabel(parent) };
            lblUpdates->setText(_("Automatically Check for Updates"));
            chkUpdates = new Switch(parent);
            QFormLayout* layoutUserInterface{ new QFormLayout(parent) };
            layoutUserInterface->addRow(lblTheme, cmbTheme);
            layoutUserInterface->addRow(lblUpdates, chkUpdates);
            QWidget* userInterfacePage{ new QWidget(parent) };
            userInterfacePage->setLayout(layoutUserInterface);
            viewStack->addWidget(userInterfacePage);
            //Navigation List
            listNavigation = new QListWidget(parent);
            listNavigation->setMaximumWidth(160);
            listNavigation->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
            listNavigation->setDropIndicatorShown(false);
            listNavigation->addItem(new QListWidgetItem(QLEMENTINE_ICON(Navigation_UiPanelLeft), _("User Interface"), listNavigation));
            QObject::connect(listNavigation, &QListWidget::currentRowChanged, [this]()
            {
                viewStack->setCurrentIndex(listNavigation->currentRow());
            });
            //Main Layout
            QHBoxLayout* layout{ new QHBoxLayout(parent) };
            layout->addWidget(listNavigation);
            layout->addWidget(viewStack);
            parent->setLayout(layout);
        }

        QListWidget* listNavigation;
        QStackedWidget* viewStack;
        QComboBox* cmbTheme;
        Switch* chkUpdates;
    };
}

namespace Nickvision::Application::Qt::Views
{
    SettingsDialog::SettingsDialog(const std::shared_ptr<PreferencesViewController>& controller, oclero::qlementine::ThemeManager* themeManager, QWidget* parent)
        : QDialog{ parent },
        m_ui{ new Ui::SettingsDialog() },
        m_controller{ controller },
        m_themeManager{ themeManager }
    {
        //Dialog Settings
        setWindowTitle(_("Settings"));
        setMinimumSize(600, 400);
        setModal(true);
        //Load Ui
        m_ui->setupUi(this);
        m_ui->cmbTheme->setCurrentIndex(static_cast<int>(m_controller->getTheme()));
        m_ui->chkUpdates->setChecked(m_controller->getAutomaticallyCheckForUpdates());
        m_ui->listNavigation->setCurrentRow(0);
        //Signals
        connect(m_ui->cmbTheme, &QComboBox::currentIndexChanged, this, &SettingsDialog::onThemeChanged);
    }

    SettingsDialog::~SettingsDialog()
    {
        delete m_ui;
    }

    void SettingsDialog::closeEvent(QCloseEvent* event)
    {
        m_controller->setTheme(static_cast<Shared::Models::Theme>(m_ui->cmbTheme->currentIndex()));
        m_controller->setAutomaticallyCheckForUpdates(m_ui->chkUpdates->isChecked());
        m_controller->saveConfiguration();
        event->accept();
    }

    void SettingsDialog::onThemeChanged()
    {
        switch (static_cast<Shared::Models::Theme>(m_ui->cmbTheme->currentIndex()))
        {
        case Shared::Models::Theme::Light:
            QApplication::styleHints()->setColorScheme(::Qt::ColorScheme::Light);
            m_themeManager->setCurrentTheme("Light");
            break;
        case Shared::Models::Theme::Dark:
            QApplication::styleHints()->setColorScheme(::Qt::ColorScheme::Dark);
            m_themeManager->setCurrentTheme("Dark");
            break;
        default:
            QApplication::styleHints()->unsetColorScheme();
            m_themeManager->setCurrentTheme(QApplication::styleHints()->colorScheme() == ::Qt::ColorScheme::Light ? "Light" : "Dark");
            break;
        }
    }
}
