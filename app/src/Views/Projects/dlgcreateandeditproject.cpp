#include "dlgcreateandeditproject.h"

#include <Wt/WBreak>

#include "../../Database/dbosession.h"
#include "../../Settings/appsettings.h"

Views::DlgCreateAndEditProject::DlgCreateAndEditProject(bool editing) :
    m_editing(editing)
{
    prepareView();
}

std::string Views::DlgCreateAndEditProject::projectName() const
{
    return m_txtProjectName->text().toUTF8();
}

Wt::WDate Views::DlgCreateAndEditProject::startDate() const
{
    return m_datStartDate->date();
}

Wt::WDate Views::DlgCreateAndEditProject::endDate() const
{
    return m_datEndDate->date();
}

int Views::DlgCreateAndEditProject::duration() const
{
    return m_spnDuration->value();
}

float Views::DlgCreateAndEditProject::fps() const
{
    return m_spnFPS->value();
}

int Views::DlgCreateAndEditProject::frameWidth() const
{
    return m_spnWidth->value();
}

int Views::DlgCreateAndEditProject::frameHeight() const
{
    return m_spnHeight->value();
}

int Views::DlgCreateAndEditProject::priority() const
{
    return m_spnPriority->value();
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Views::DlgCreateAndEditProject::status() const
{
    return m_mdlCmbStatus->resultRow(m_cmbStatus->currentIndex());
}

Wt::Dbo::ptr<Users::User> Views::DlgCreateAndEditProject::manager() const
{
    return m_mdlCmbManager->resultRow(m_cmbManager->currentIndex());
}

std::string Views::DlgCreateAndEditProject::description() const
{
    return m_txtDescription->text().toUTF8();
}

bool Views::DlgCreateAndEditProject::isActive() const
{
    return m_cmbActive->currentText() == "Yes" ? true : false;
}

bool Views::DlgCreateAndEditProject::isEditing()
{
    return m_editing;
}

bool Views::DlgCreateAndEditProject::editedStartDate() const
{
    if(m_editing)
    {
        if(m_datStartDate->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedEndDate() const
{
    if(m_editing)
    {
        if(m_datEndDate->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedDuration() const
{
    if(m_editing)
    {
        if(m_spnDuration->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedFps() const
{
    if(m_editing)
    {
        if(m_spnFPS->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedFrameWidth() const
{
    if(m_editing)
    {
        if(m_spnWidth->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedFrameHeight() const
{
    if(m_editing)
    {
        if(m_spnHeight->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedPriority() const
{
    if(m_editing)
    {
        if(m_spnPriority->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedStatus() const
{
    if(m_editing)
    {
        if(m_cmbStatus->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedManager() const
{
    if(m_editing)
    {
        if(m_cmbManager->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedDescription() const
{
    if(m_editing)
    {
        if(m_txtDescription->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedActive() const
{
    if(m_editing)
    {
        if(m_cmbActive->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

void Views::DlgCreateAndEditProject::prepareView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(!m_editing)
        this->setCaption("Create Project");
    else
        this->setCaption("Edit Projects");

    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WHBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    this->contents()->setLayout(m_layMain);

    m_layLeft = new Wt::WVBoxLayout();
    m_layLeft->setContentsMargins(0,0,0,0);
    m_layLeft->setSpacing(2);

    m_cntLeft = new Wt::WContainerWidget();
    m_cntLeft->setLayout(m_layLeft);

    m_layMain->addWidget(m_cntLeft);

    m_layRight = new Wt::WVBoxLayout();
    m_layRight->setContentsMargins(16,0,0,0);
    m_layRight->setSpacing(2);

    m_cntRight = new Wt::WContainerWidget();
    m_cntRight->setLayout(m_layRight);

    m_layMain->addWidget(m_cntRight);

    if(!m_editing)
    {
        m_txtProjectName = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{4,150}");
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", m_txtProjectName));

        m_layLeft->addWidget(new Wt::WBreak());
    }

    m_datStartDate = Ms::Widgets::MWidgetFactory::createDateEdit();
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Start Date:", m_datStartDate));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Start Date:", m_datStartDate));

    m_layLeft->addWidget(new Wt::WBreak());

    m_datEndDate = Ms::Widgets::MWidgetFactory::createDateEdit();
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("End Date:", m_datEndDate));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("End Date:", m_datEndDate));

    m_layLeft->addWidget(new Wt::WBreak());

    m_spnDuration = Ms::Widgets::MWidgetFactory::createSpinBox(0, INT_MAX, 0);
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Duration In Frames:", m_spnDuration));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Duration In Frames:", m_spnDuration));

    m_layLeft->addWidget(new Wt::WBreak());

    m_spnFPS = Ms::Widgets::MWidgetFactory::createDoubleSpinBox(1.0, DBL_MAX, 25.0);
    m_spnFPS->setValidator(Ms::Widgets::MWidgetFactory::createDoubleValidator(1.0, DBL_MAX));
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("FPS:", m_spnFPS));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("FPS:", m_spnFPS));

    m_layLeft->addWidget(new Wt::WBreak());

    m_spnWidth = Ms::Widgets::MWidgetFactory::createSpinBox(1, INT_MAX, 1920);
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Frame Width:", m_spnWidth));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Frame Width:", m_spnWidth));

    m_layLeft->addWidget(new Wt::WBreak());

    m_spnHeight = Ms::Widgets::MWidgetFactory::createSpinBox(1, INT_MAX, 1080);
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Frame Height:", m_spnHeight));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Frame Height:", m_spnHeight));

    m_layLeft->addWidget(new Wt::WBreak());

    m_spnPriority = Ms::Widgets::MWidgetFactory::createSpinBox(0, INT_MAX, 0);
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Priority:", m_spnPriority));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Priority:", m_spnPriority));

    m_layLeft->addWidget(new Wt::WBreak(), 1);

    createCmbStatus();
    if(m_editing)
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Status:", m_cntCmbStatus));
    else
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Status:", m_cntCmbStatus));

    m_layRight->addWidget(new Wt::WBreak());

    createCmbManager();
    if(m_editing)
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Manager:", m_cntCmbManager));
    else
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Manager:", m_cntCmbManager));

    m_layRight->addWidget(new Wt::WBreak());

    m_txtDescription = Ms::Widgets::MWidgetFactory::createTextArea("", false);
    if(m_editing)
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Description:", m_txtDescription));
    else
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Description:", m_txtDescription));

    m_cmbActive = new Wt::WComboBox();
    m_cmbActive->addItem("Yes");
    m_cmbActive->addItem("No");
    m_cmbActive->setCurrentIndex(0);
    if(m_editing)
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Active:", m_cmbActive));
    else
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", m_cmbActive));

    m_layRight->addWidget(new Wt::WBreak(), 1);

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->clicked().connect(this, &Views::DlgCreateAndEditProject::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
    m_btnCancel->setFocus();

    transaction.commit();
}

void Views::DlgCreateAndEditProject::btnOkClicked()
{
    if(validate())
        this->accept();
}

void Views::DlgCreateAndEditProject::createCmbManager()
{
    m_cmbManager = new Wt::WComboBox();
    m_cmbManager->setMinimumSize(20, 30);
    m_cntCmbManager = new Wt::WContainerWidget();
    m_cntCmbManager->addWidget(m_cmbManager);

    m_mdlCmbManager = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::User>>();

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::User>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Users::User>();
    else
        query = Session::SessionManager::instance().dboSession().find<Users::User>().where("Active = ?").bind(true);

    m_mdlCmbManager->setQuery(query);

    m_mdlCmbManager->reload();

    m_mdlCmbManager->addColumn("Name", Wt::ItemIsSelectable);

    m_cmbManager->setModel(m_mdlCmbManager);

    if(m_mdlCmbManager->rowCount() > 0)
        m_cmbManager->setCurrentIndex(0);
}

void Views::DlgCreateAndEditProject::createCmbStatus()
{
    m_cmbStatus = new Wt::WComboBox();
    m_cmbStatus->setMinimumSize(20, 30);
    m_cntCmbStatus = new Wt::WContainerWidget();
    m_cntCmbStatus->addWidget(m_cmbStatus);

    m_mdlCmbStatus = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatus>>();

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>();
    else
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true);

    m_mdlCmbStatus->setQuery(query);

    m_mdlCmbStatus->reload();

    m_mdlCmbStatus->addColumn("Status", Wt::ItemIsSelectable);

    m_cmbStatus->setModel(m_mdlCmbStatus);

    if(m_mdlCmbStatus->rowCount() > 0)
        m_cmbStatus->setCurrentIndex(0);
}

bool Views::DlgCreateAndEditProject::validate()
{
    bool result = true;

    if(!m_editing)
    {
        if((m_txtProjectName->validate() != Wt::WValidator::Valid) ||
                (m_datStartDate->validate() != Wt::WDateValidator::Valid) ||
                (m_datEndDate->validate() != Wt::WDateValidator::Valid) ||
                (m_spnDuration->validate() != Wt::WIntValidator::Valid) ||
                (m_spnFPS->validate() != Wt::WDoubleValidator::Valid) ||
                (m_spnWidth->validate() != Wt::WIntValidator::Valid) ||
                (m_spnHeight->validate() != Wt::WIntValidator::Valid) ||
                (m_spnPriority->validate() != Wt::WIntValidator::Valid) ||
                (m_cmbStatus->currentIndex() == -1) ||
                (m_cmbManager->currentIndex() == -1))
        {
            result = false;
        }
    }
    else
    {
        if(((m_datStartDate->isEnabled()) && (m_datStartDate->validate() != Wt::WValidator::Valid)) ||
                ((m_datEndDate->isEnabled()) && (m_datEndDate->validate() != Wt::WValidator::Valid)) ||
                ((m_spnDuration->isEnabled()) && (m_spnDuration->validate() != Wt::WValidator::Valid)) ||
                ((m_spnFPS->isEnabled()) && (m_spnFPS->validate() != Wt::WValidator::Valid)) ||
                ((m_spnWidth->isEnabled()) && (m_spnWidth->validate() != Wt::WValidator::Valid)) ||
                ((m_spnHeight->isEnabled()) && (m_spnHeight->validate() != Wt::WValidator::Valid)) ||
                ((m_spnPriority->isEnabled()) && (m_spnPriority->validate() != Wt::WIntValidator::Valid)) ||
                ((m_cmbStatus->isEnabled()) && (m_cmbStatus->currentIndex() == -1)) ||
                ((m_cmbManager->isEnabled()) && (m_cmbManager->currentIndex() == -1)))
        {
            result = false;
        }
    }

    return result;
}
