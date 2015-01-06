#include "dlgcreateandedittask.h"

#include <Wt/WBreak>

#include "../../Database/dbosession.h"
#include "../../Settings/appsettings.h"

Views::DlgCreateAndEditTask::DlgCreateAndEditTask(bool editing) :
    m_editing(editing)
{
    prepareView();
}

Wt::Dbo::ptr<Projects::ProjectTaskType> Views::DlgCreateAndEditTask::type() const
{
    return m_mdlCmbType->resultRow(m_cmbType->currentIndex());
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Views::DlgCreateAndEditTask::status() const
{
    return m_mdlCmbStatus->resultRow(m_cmbStatus->currentIndex());
}

std::string Views::DlgCreateAndEditTask::thumbnail() const
{
    return m_cmbThumbnail->currentText().toUTF8();
}

Wt::Dbo::ptr<Users::User> Views::DlgCreateAndEditTask::user() const
{
    return m_mdlCmbUser->resultRow(m_cmbUser->currentIndex());
}

Wt::WDate Views::DlgCreateAndEditTask::startDate() const
{
    return m_datStartDate->date();
}

Wt::WDate Views::DlgCreateAndEditTask::endDate() const
{
    return m_datEndDate->date();
}

int Views::DlgCreateAndEditTask::priority() const
{
    return m_spnPriority->value();
}

std::string Views::DlgCreateAndEditTask::description() const
{
    return m_txtDescription->text().toUTF8();
}

bool Views::DlgCreateAndEditTask::isActive() const
{
    return m_cmbActive->currentText() == "Yes" ? true : false;
}

bool Views::DlgCreateAndEditTask::isEditing()
{
    return m_editing;
}

bool Views::DlgCreateAndEditTask::editedStartDate() const
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

bool Views::DlgCreateAndEditTask::editedEndDate() const
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

bool Views::DlgCreateAndEditTask::editedPriority() const
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

bool Views::DlgCreateAndEditTask::editedType() const
{
    if(m_editing)
    {
        if(m_cmbType->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditTask::editedStatus() const
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

bool Views::DlgCreateAndEditTask::editedThumbnail() const
{
    if(m_editing)
    {
        if(m_cmbThumbnail->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditTask::editedUser() const
{
    if(m_editing)
    {
        if(m_cmbUser->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditTask::editedDescription() const
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

bool Views::DlgCreateAndEditTask::editedActive() const
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

void Views::DlgCreateAndEditTask::prepareView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(!m_editing)
        this->setCaption("Create Task");
    else
        this->setCaption("Edit Tasks");

    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WHBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(2);

    this->contents()->setLayout(m_layMain);

    m_layLeft = new Wt::WVBoxLayout();
    m_layLeft->setContentsMargins(0,0,0,0);
    m_layLeft->setSpacing(2);

    m_cntLeft = new Wt::WContainerWidget();
    m_cntLeft->setLayout(m_layLeft);

    m_layMain->addWidget(m_cntLeft);

    m_layRight = new Wt::WVBoxLayout();
    m_layRight->setContentsMargins(16,0,0,0);
    m_layRight->setSpacing(1);

    m_cntRight = new Wt::WContainerWidget();
    m_cntRight->setLayout(m_layRight);

    m_layMain->addWidget(m_cntRight);

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

    m_spnPriority = Ms::Widgets::MWidgetFactory::createSpinBox(0, INT_MAX, 0);
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Priority:", m_spnPriority));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Priority:", m_spnPriority));

    m_layLeft->addWidget(new Wt::WBreak());

    createCmbType();
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Type:", m_cntCmbType));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Type:", m_cntCmbType));

    m_layLeft->addWidget(new Wt::WBreak());

    createCmbStatus();
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Status:", m_cntCmbStatus));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Status:", m_cntCmbStatus));

    m_layLeft->addWidget(new Wt::WBreak());

    m_cmbThumbnail = new Wt::WComboBox();
    m_cmbThumbnail->addItem("Project");
    m_cmbThumbnail->addItem("Sequence");
    m_cmbThumbnail->addItem("Asset");
    m_cmbThumbnail->addItem("Shot");

    m_cmbThumbnail->setCurrentIndex(0);

    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Thumbnail:", m_cmbThumbnail));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Thumbnail:", m_cmbThumbnail));

    m_layLeft->addWidget(new Wt::WBreak(), 1);

    createCmbUser();
    if(m_editing)
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("User:", m_cntCmbUser));
    else
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("User:", m_cntCmbUser));

    m_layRight->addWidget(new Wt::WBreak());

    m_txtDescription = Ms::Widgets::MWidgetFactory::createTextArea("", false);
    if(m_editing)
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Description:", m_txtDescription));
    else
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Description:", m_txtDescription));

    m_layRight->addWidget(new Wt::WBreak());

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
    m_btnOk->clicked().connect(this, &Views::DlgCreateAndEditTask::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
    m_btnCancel->setFocus();

    transaction.commit();
}

void Views::DlgCreateAndEditTask::btnOkClicked()
{
    if(validate())
        this->accept();
}

void Views::DlgCreateAndEditTask::createCmbUser()
{
    m_cmbUser = new Wt::WComboBox();
    m_cmbUser->setMinimumSize(20, 30);
    m_cntCmbUser = new Wt::WContainerWidget();
    m_cntCmbUser->addWidget(m_cmbUser);

    m_mdlCmbUser = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::User>>();

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::User>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Users::User>();
    else
        query = Session::SessionManager::instance().dboSession().find<Users::User>().where("Active = ?").bind(true);

    m_mdlCmbUser->setQuery(query);

    m_mdlCmbUser->reload();

    m_mdlCmbUser->addColumn("Name", Wt::ItemIsSelectable);

    m_cmbUser->setModel(m_mdlCmbUser);

    if(m_mdlCmbUser->rowCount() > 0)
        m_cmbUser->setCurrentIndex(0);
}

void Views::DlgCreateAndEditTask::createCmbStatus()
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

void Views::DlgCreateAndEditTask::createCmbType()
{
    m_cmbType = new Wt::WComboBox();
    m_cmbType->setMinimumSize(20, 30);
    m_cntCmbType = new Wt::WContainerWidget();
    m_cntCmbType->addWidget(m_cmbType);

    m_mdlCmbType = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskType>>();

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskType>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskType>();
    else
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskType>().where("Active = ?").bind(true);

    m_mdlCmbType->setQuery(query);

    m_mdlCmbType->reload();

    m_mdlCmbType->addColumn("Type", Wt::ItemIsSelectable);

    m_cmbType->setModel(m_mdlCmbType);

    if(m_mdlCmbType->rowCount() > 0)
        m_cmbType->setCurrentIndex(0);
}

bool Views::DlgCreateAndEditTask::validate()
{
    bool result = true;

    if(!m_editing)
    {
        if((m_datStartDate->validate() != Wt::WDateValidator::Valid) ||
                (m_datEndDate->validate() != Wt::WDateValidator::Valid) ||
                (m_spnPriority->validate() != Wt::WIntValidator::Valid) ||
                (m_cmbUser->currentIndex() == -1) ||
                (m_cmbType->currentIndex() == -1) ||
                (m_cmbStatus->currentIndex() == -1))
        {
            result = false;
        }
    }
    else
    {
        if(((m_datStartDate->isEnabled()) && (m_datStartDate->validate() != Wt::WValidator::Valid)) ||
                ((m_datEndDate->isEnabled()) && (m_datEndDate->validate() != Wt::WValidator::Valid)) ||
                ((m_datEndDate->isEnabled()) && (m_spnPriority->validate() != Wt::WIntValidator::Valid)) ||
                ((m_cmbUser->isEnabled()) && (m_cmbUser->currentIndex() == -1)) ||
                ((m_cmbType->isEnabled()) && (m_cmbType->currentIndex() == -1)) ||
                ((m_cmbStatus->isEnabled()) && (m_cmbStatus->currentIndex() == -1)))
        {
            result = false;
        }
    }

    return result;
}
