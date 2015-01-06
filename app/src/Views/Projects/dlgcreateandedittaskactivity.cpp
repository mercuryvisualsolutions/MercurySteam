#include "dlgcreateandedittaskactivity.h"

#include <Wt/WBreak>

#include "../../Database/dbosession.h"
#include "../../Settings/appsettings.h"

Views::DlgCreateAndEditTaskActivity::DlgCreateAndEditTaskActivity(bool editing) :
    m_editing(editing)
{
    prepareView();
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Views::DlgCreateAndEditTaskActivity::status() const
{
    return m_mdlCmbStatus->resultRow(m_cmbStatus->currentIndex());
}

Wt::Dbo::ptr<Projects::ProjectTaskActivityType> Views::DlgCreateAndEditTaskActivity::type() const
{
    return m_mdlCmbType->resultRow(m_cmbType->currentIndex());
}

int Views::DlgCreateAndEditTaskActivity::hours() const
{
    return m_spnHours->value();
}

std::string Views::DlgCreateAndEditTaskActivity::description() const
{
    return m_txtDescription->text().toUTF8();
}

bool Views::DlgCreateAndEditTaskActivity::isActive() const
{
    return m_cmbActive->currentText() == "Yes" ? true : false;
}

bool Views::DlgCreateAndEditTaskActivity::isEditing()
{
    return m_editing;
}

bool Views::DlgCreateAndEditTaskActivity::editedHours() const
{
    if(m_editing)
    {
        if(m_spnHours->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditTaskActivity::editedType() const
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

bool Views::DlgCreateAndEditTaskActivity::editedStatus() const
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

bool Views::DlgCreateAndEditTaskActivity::editedDescription() const
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

bool Views::DlgCreateAndEditTaskActivity::editedActive() const
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

void Views::DlgCreateAndEditTaskActivity::prepareView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(!m_editing)
        this->setCaption("Create Task Activity");
    else
        this->setCaption("Edit Task Activity");

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

//    m_layRight = new Wt::WVBoxLayout();
//    m_layRight->setContentsMargins(16,0,0,0);
//    m_layRight->setSpacing(1);

//    m_cntRight = new Wt::WContainerWidget();
//    m_cntRight->setLayout(m_layRight);

//    m_layMain->addWidget(m_cntRight);

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

    m_spnHours = Ms::Widgets::MWidgetFactory::createSpinBox(0, INT_MAX, 0);
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Hours:", m_spnHours));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Hours:", m_spnHours));

    m_layLeft->addWidget(new Wt::WBreak());

    m_txtDescription = Ms::Widgets::MWidgetFactory::createTextArea("", false);
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Description:", m_txtDescription));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Description:", m_txtDescription));

    m_layLeft->addWidget(new Wt::WBreak());

    m_cmbActive = new Wt::WComboBox();
    m_cmbActive->addItem("Yes");
    m_cmbActive->addItem("No");
    m_cmbActive->setCurrentIndex(0);
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Active:", m_cmbActive));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", m_cmbActive));

    m_layLeft->addWidget(new Wt::WBreak(), 1);

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->clicked().connect(this, &Views::DlgCreateAndEditTaskActivity::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
    m_btnCancel->setFocus();

    transaction.commit();
}

void Views::DlgCreateAndEditTaskActivity::btnOkClicked()
{
    if(validate())
        this->accept();
}

void Views::DlgCreateAndEditTaskActivity::createCmbType()
{
    m_cmbType = new Wt::WComboBox();
    m_cmbType->setMinimumSize(20, 30);
    m_cntCmbType = new Wt::WContainerWidget();
    m_cntCmbType->addWidget(m_cmbType);

    m_mdlCmbType = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskActivityType>>();

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskActivityType>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskActivityType>();
    else
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskActivityType>().where("Active = ?").bind(true);

    m_mdlCmbType->setQuery(query);

    m_mdlCmbType->reload();

    m_mdlCmbType->addColumn("Type", Wt::ItemIsSelectable);

    m_cmbType->setModel(m_mdlCmbType);

    if(m_mdlCmbType->rowCount() > 0)
        m_cmbType->setCurrentIndex(0);
}

void Views::DlgCreateAndEditTaskActivity::createCmbStatus()
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

bool Views::DlgCreateAndEditTaskActivity::validate()
{
    bool result = true;

    if(!m_editing)
    {
        if((m_spnHours->validate() != Wt::WIntValidator::Valid) ||
                (m_cmbType->currentIndex() == -1) ||
                (m_cmbStatus->currentIndex() == -1))
        {
            result = false;
        }
    }
    else
    {
        if((m_spnHours->isEnabled() && (m_spnHours->validate() != Wt::WIntValidator::Valid)) ||
                ((m_cmbType->isEnabled()) && (m_cmbType->currentIndex() == -1)) ||
                ((m_cmbStatus->isEnabled()) && (m_cmbStatus->currentIndex() == -1)))
        {
            result = false;
        }
    }

    return result;
}
