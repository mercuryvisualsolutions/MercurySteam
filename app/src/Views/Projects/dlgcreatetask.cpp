#include "dlgcreatetask.h"

#include <Wt/WBreak>

#include "../../Database/databasemanager.h"
#include "../../Settings/appsettings.h"

Views::DlgCreateTask::DlgCreateTask()
{
    _prepareView();
}

Wt::Dbo::ptr<Projects::ProjectTaskType> Views::DlgCreateTask::type() const
{
    return _mdlCmbType->resultRow(_cmbType->currentIndex());
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Views::DlgCreateTask::status() const
{
    return _mdlCmbStatus->resultRow(_cmbStatus->currentIndex());
}

Wt::Dbo::ptr<Users::User> Views::DlgCreateTask::user() const
{
    return _mdlCmbUser->resultRow(_cmbUser->currentIndex());
}

Wt::WDate Views::DlgCreateTask::startDate() const
{
    return _datStartDate->date();
}

Wt::WDate Views::DlgCreateTask::endDate() const
{
    return _datEndDate->date();
}

std::string Views::DlgCreateTask::description() const
{
    return _txtDescription->text().toUTF8();
}

bool Views::DlgCreateTask::isActive() const
{
    return _chkActive->isChecked();
}

void Views::DlgCreateTask::_prepareView()
{
    this->setCaption("Create Task");
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
    _layRight->setContentsMargins(0,0,0,0);
    _layRight->setSpacing(1);

    _cntRight = new Wt::WContainerWidget();
    _cntRight->setLayout(_layRight);

    _layMain->addWidget(_cntRight);

    _datStartDate = Ms::Widgets::MWidgetFactory::createDateEdit();
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Start Date:", _datStartDate));

    _layLeft->addWidget(new Wt::WBreak());

    _datEndDate = Ms::Widgets::MWidgetFactory::createDateEdit();
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("End Date:", _datEndDate));

    _layLeft->addWidget(new Wt::WBreak());

    _createCmbStatus();
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Status:", _cntCmbStatus));

    _layLeft->addWidget(new Wt::WBreak());

    _createCmbUser();
//    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("User:", _cntCmbUser));

//    _layLeft->addWidget(new Wt::WBreak());

    _createCmbType();
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Type:", _cntCmbType));

    _layLeft->addWidget(new Wt::WBreak(), 1);

    _txtDescription = Ms::Widgets::MWidgetFactory::createTextArea("", false);
    _layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Description:", _txtDescription), 1);

    _chkActive = new Wt::WCheckBox("active");
    _chkActive->setChecked(true);

    _layRight->addWidget(_chkActive);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::DlgCreateTask::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

void Views::DlgCreateTask::_btnOkClicked()
{
    if(_validate())
        this->accept();
}

void Views::DlgCreateTask::_createCmbUser()
{
    _cmbUser = new Wt::WComboBox();
    _cmbUser->setMinimumSize(20, 30);
    _cntCmbUser = new Wt::WContainerWidget();
    _cntCmbUser->addWidget(_cmbUser);

    _mdlCmbUser = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::User>>();

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::User>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Database::DatabaseManager::instance().session()->find<Users::User>();
    else
        query = Database::DatabaseManager::instance().session()->find<Users::User>().where("Active = ?").bind(true);

    _mdlCmbUser->setQuery(query);

    Database::DatabaseManager::instance().commitTransaction();

    _mdlCmbUser->reload();

    _mdlCmbUser->addColumn("Name", Wt::ItemIsSelectable);

    _cmbUser->setModel(_mdlCmbUser);
}

void Views::DlgCreateTask::_createCmbStatus()
{
    _cmbStatus = new Wt::WComboBox();
    _cmbStatus->setMinimumSize(20, 30);
    _cntCmbStatus = new Wt::WContainerWidget();
    _cntCmbStatus->addWidget(_cmbStatus);

    _mdlCmbStatus = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatus>>();

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>();
    else
        query = Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true);

    _mdlCmbStatus->setQuery(query);

    Database::DatabaseManager::instance().commitTransaction();

    _mdlCmbStatus->reload();

    _mdlCmbStatus->addColumn("Status", Wt::ItemIsSelectable);

    _cmbStatus->setModel(_mdlCmbStatus);
}

void Views::DlgCreateTask::_createCmbType()
{
    _cmbType = new Wt::WComboBox();
    _cmbType->setMinimumSize(20, 30);
    _cntCmbType = new Wt::WContainerWidget();
    _cntCmbType->addWidget(_cmbType);

    _mdlCmbType = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskType>>();

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskType>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskType>();
    else
        query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskType>().where("Active = ?").bind(true);

    _mdlCmbType->setQuery(query);

    Database::DatabaseManager::instance().commitTransaction();

    _mdlCmbType->reload();

    _mdlCmbType->addColumn("Type", Wt::ItemIsSelectable);

    _cmbType->setModel(_mdlCmbType);
}

bool Views::DlgCreateTask::_validate()
{
    if((_datStartDate->validate() != Wt::WDateValidator::Valid) ||
            (_datEndDate->validate() != Wt::WDateValidator::Valid) ||
            /*(cmbUser->currentIndex() == -1) ||*/
            (_cmbStatus->currentIndex() == -1) ||
            (_cmbType->currentIndex() == -1))
        return false;

    return true;
}
