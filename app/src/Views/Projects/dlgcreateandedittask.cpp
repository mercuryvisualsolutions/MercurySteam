#include "dlgcreateandedittask.h"

#include <Wt/WBreak>

#include "../../Database/dbosession.h"
#include "../../Settings/appsettings.h"

Views::DlgCreateAndEditTask::DlgCreateAndEditTask(bool editing) :
    _editing(editing)
{
    _prepareView();
}

Wt::Dbo::ptr<Projects::ProjectTaskType> Views::DlgCreateAndEditTask::type() const
{
    return _mdlCmbType->resultRow(_cmbType->currentIndex());
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Views::DlgCreateAndEditTask::status() const
{
    return _mdlCmbStatus->resultRow(_cmbStatus->currentIndex());
}

Wt::Dbo::ptr<Users::User> Views::DlgCreateAndEditTask::user() const
{
    return _mdlCmbUser->resultRow(_cmbUser->currentIndex());
}

Wt::WDate Views::DlgCreateAndEditTask::startDate() const
{
    return _datStartDate->date();
}

Wt::WDate Views::DlgCreateAndEditTask::endDate() const
{
    return _datEndDate->date();
}

int Views::DlgCreateAndEditTask::priority() const
{
    return _spnPriority->value();
}

std::string Views::DlgCreateAndEditTask::description() const
{
    return _txtDescription->text().toUTF8();
}

bool Views::DlgCreateAndEditTask::isActive() const
{
    return _cmbActive->currentText() == "Yes" ? true : false;
}

bool Views::DlgCreateAndEditTask::isEditing()
{
    return _editing;
}

bool Views::DlgCreateAndEditTask::editedStartDate() const
{
    if(_editing)
    {
        if(_datStartDate->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditTask::editedEndDate() const
{
    if(_editing)
    {
        if(_datEndDate->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditTask::editedPriority() const
{
    if(_editing)
    {
        if(_spnPriority->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditTask::editedType() const
{
    if(_editing)
    {
        if(_cmbType->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditTask::editedStatus() const
{
    if(_editing)
    {
        if(_cmbStatus->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditTask::editedUser() const
{
    if(_editing)
    {
        if(_cmbUser->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditTask::editedDescription() const
{
    if(_editing)
    {
        if(_txtDescription->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditTask::editedActive() const
{
    if(_editing)
    {
        if(_cmbActive->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

void Views::DlgCreateAndEditTask::_prepareView()
{
    if(!_editing)
        this->setCaption("Create Task");
    else
        this->setCaption("Edit Tasks");

    this->rejectWhenEscapePressed();

    _layMain = new Wt::WHBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(2);

    this->contents()->setLayout(_layMain);

    _layLeft = new Wt::WVBoxLayout();
    _layLeft->setContentsMargins(0,0,0,0);
    _layLeft->setSpacing(2);

    _cntLeft = new Wt::WContainerWidget();
    _cntLeft->setLayout(_layLeft);

    _layMain->addWidget(_cntLeft);

    _layRight = new Wt::WVBoxLayout();
    _layRight->setContentsMargins(16,0,0,0);
    _layRight->setSpacing(1);

    _cntRight = new Wt::WContainerWidget();
    _cntRight->setLayout(_layRight);

    _layMain->addWidget(_cntRight);

    _datStartDate = Ms::Widgets::MWidgetFactory::createDateEdit();
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Start Date:", _datStartDate));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Start Date:", _datStartDate));

    _layLeft->addWidget(new Wt::WBreak());

    _datEndDate = Ms::Widgets::MWidgetFactory::createDateEdit();
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("End Date:", _datEndDate));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("End Date:", _datEndDate));

    _layLeft->addWidget(new Wt::WBreak());

    _spnPriority = Ms::Widgets::MWidgetFactory::createSpinBox(0, INT_MAX, 0);
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Priority:", _spnPriority));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Priority:", _spnPriority));

    _layLeft->addWidget(new Wt::WBreak());

    _createCmbType();
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Type:", _cntCmbType));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Type:", _cntCmbType));

    _layLeft->addWidget(new Wt::WBreak());

    _createCmbStatus();
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Status:", _cntCmbStatus));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Status:", _cntCmbStatus));

    _layLeft->addWidget(new Wt::WBreak());

    _createCmbUser();
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("User:", _cntCmbUser));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("User:", _cntCmbUser));

    _layLeft->addWidget(new Wt::WBreak(), 1);

    _txtDescription = Ms::Widgets::MWidgetFactory::createTextArea("", false);
    if(_editing)
        _layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Description:", _txtDescription));
    else
        _layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Description:", _txtDescription));

    _layRight->addWidget(new Wt::WBreak());

    _cmbActive = new Wt::WComboBox();
    _cmbActive->addItem("Yes");
    _cmbActive->addItem("No");
    _cmbActive->setCurrentIndex(0);
    if(_editing)
        _layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Active:", _cmbActive));
    else
        _layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", _cmbActive));

    _layRight->addWidget(new Wt::WBreak(), 1);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->clicked().connect(this, &Views::DlgCreateAndEditTask::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
    _btnCancel->setFocus();
}

void Views::DlgCreateAndEditTask::_btnOkClicked()
{
    if(_validate())
        this->accept();
}

void Views::DlgCreateAndEditTask::_createCmbUser()
{
    _cmbUser = new Wt::WComboBox();
    _cmbUser->setMinimumSize(20, 30);
    _cntCmbUser = new Wt::WContainerWidget();
    _cntCmbUser->addWidget(_cmbUser);

    _mdlCmbUser = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::User>>();

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::User>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Users::User>();
    else
        query = Session::SessionManager::instance().dboSession().find<Users::User>().where("Active = ?").bind(true);

    _mdlCmbUser->setQuery(query);

    transaction.commit();

    _mdlCmbUser->reload();

    _mdlCmbUser->addColumn("Name", Wt::ItemIsSelectable);

    _cmbUser->setModel(_mdlCmbUser);

    if(_mdlCmbUser->rowCount() > 0)
        _cmbUser->setCurrentIndex(0);
}

void Views::DlgCreateAndEditTask::_createCmbStatus()
{
    _cmbStatus = new Wt::WComboBox();
    _cmbStatus->setMinimumSize(20, 30);
    _cntCmbStatus = new Wt::WContainerWidget();
    _cntCmbStatus->addWidget(_cmbStatus);

    _mdlCmbStatus = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatus>>();

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>();
    else
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true);

    _mdlCmbStatus->setQuery(query);

    transaction.commit();

    _mdlCmbStatus->reload();

    _mdlCmbStatus->addColumn("Status", Wt::ItemIsSelectable);

    _cmbStatus->setModel(_mdlCmbStatus);

    if(_mdlCmbStatus->rowCount() > 0)
        _cmbStatus->setCurrentIndex(0);
}

void Views::DlgCreateAndEditTask::_createCmbType()
{
    _cmbType = new Wt::WComboBox();
    _cmbType->setMinimumSize(20, 30);
    _cntCmbType = new Wt::WContainerWidget();
    _cntCmbType->addWidget(_cmbType);

    _mdlCmbType = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskType>>();

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskType>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskType>();
    else
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskType>().where("Active = ?").bind(true);

    _mdlCmbType->setQuery(query);

    transaction.commit();

    _mdlCmbType->reload();

    _mdlCmbType->addColumn("Type", Wt::ItemIsSelectable);

    _cmbType->setModel(_mdlCmbType);

    if(_mdlCmbType->rowCount() > 0)
        _cmbType->setCurrentIndex(0);
}

bool Views::DlgCreateAndEditTask::_validate()
{
    bool result = true;

    if(!_editing)
    {
        if((_datStartDate->validate() != Wt::WDateValidator::Valid) ||
                (_datEndDate->validate() != Wt::WDateValidator::Valid) ||
                (_spnPriority->validate() != Wt::WIntValidator::Valid) ||
                (_cmbUser->currentIndex() == -1) ||
                (_cmbType->currentIndex() == -1) ||
                (_cmbStatus->currentIndex() == -1))
        {
            result = false;
        }
    }
    else
    {
        if(((_datStartDate->isEnabled()) && (_datStartDate->validate() != Wt::WValidator::Valid)) ||
                ((_datEndDate->isEnabled()) && (_datEndDate->validate() != Wt::WValidator::Valid)) ||
                ((_datEndDate->isEnabled()) && (_spnPriority->validate() != Wt::WIntValidator::Valid)) ||
                ((_cmbUser->isEnabled()) && (_cmbUser->currentIndex() == -1)) ||
                ((_cmbType->isEnabled()) && (_cmbType->currentIndex() == -1)) ||
                ((_cmbStatus->isEnabled()) && (_cmbStatus->currentIndex() == -1)))
        {
            result = false;
        }
    }

    return result;
}
