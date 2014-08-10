#include "dlgcreateuser.h"
#include "../../Database/databasemanager.h"
#include "../../Settings/appsettings.h"

#include <Wt/WBreak>

Views::DlgCreateUser::DlgCreateUser()
{
    _prepareView();
}

std::string Views::DlgCreateUser::userName() const
{
    return _txtUserName->text().toUTF8();
}

std::string Views::DlgCreateUser::password() const
{
    return _txtPassword->text().toUTF8();
}

std::string Views::DlgCreateUser::emailAddress() const
{
    return _txtEmailAddress->text().toUTF8();
}

std::string Views::DlgCreateUser::phoneNumber() const
{
    return _txtPhoneNumber->text().toUTF8();
}

std::string Views::DlgCreateUser::idNumber() const
{
    return _txtIdNumber->text().toUTF8();
}

std::string Views::DlgCreateUser::address() const
{
    return _txtAddress->text().toUTF8();
}

Wt::Dbo::ptr<Users::Group> Views::DlgCreateUser::group() const
{
    return _mdlCmbGroups->resultRow(_cmbGroups->currentIndex());
}

Wt::Dbo::ptr<Users::UserTitle> Views::DlgCreateUser::title() const
{
    return _mdlCmbTitles->resultRow(_cmbTitles->currentIndex());
}

bool Views::DlgCreateUser::isActive() const
{
    return _cmbActive->currentText() == "Yes" ? true : false;
}

void Views::DlgCreateUser::_prepareView()
{
    this->setCaption("Create User");
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

    _txtUserName = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{3,150}");
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", _txtUserName));

    _layLeft->addWidget(new Wt::WBreak());

    _txtPassword = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9_-]{6,14}");
    _txtPassword->setEchoMode(Wt::WLineEdit::Password);
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Password:", _txtPassword));

    _layLeft->addWidget(new Wt::WBreak());

    _txtEmailAddress = Ms::Widgets::MWidgetFactory::createLineEdit("", false);
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Email:", _txtEmailAddress));

    _layLeft->addWidget(new Wt::WBreak());

    _txtPhoneNumber = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[0-9]{7,255}", false);
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Phone Number:", _txtPhoneNumber));

    _layLeft->addWidget(new Wt::WBreak());

    _txtIdNumber = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[0-9]{1,255}", false);
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Id Number:", _txtIdNumber));

    _layMain->addWidget(new Wt::WBreak());

    _txtAddress = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{6,255}", false);
    _layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Address:", _txtAddress));

    _layRight->addWidget(new Wt::WBreak());

    _createCmbGroups();
    _layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Group:", _cntCmbGroups));

    _layRight->addWidget(new Wt::WBreak());

    _createCmbTitles();
    _layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Title:", _cntCmbTitles));

    _layRight->addWidget(new Wt::WBreak());

    _cmbActive = new Wt::WComboBox();
    _cmbActive->addItem("Yes");
    _cmbActive->addItem("No");
    _cmbActive->setCurrentIndex(0);
    _layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", _cmbActive));

    _layRight->addWidget(new Wt::WBreak(), 1);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::DlgCreateUser::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

void Views::DlgCreateUser::_createCmbGroups()
{
    _cmbGroups = new Wt::WComboBox();
    _cmbGroups->setMinimumSize(20, 30);
    _cntCmbGroups = new Wt::WContainerWidget();
    _cntCmbGroups->addWidget(_cmbGroups);

    _mdlCmbGroups = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::Group>>();

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::Group>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Database::DatabaseManager::instance().session()->find<Users::Group>();
    else
        query = Database::DatabaseManager::instance().session()->find<Users::Group>().where("Active = ?").bind(true);

    _mdlCmbGroups->setQuery(query);

    Database::DatabaseManager::instance().commitTransaction();

    _mdlCmbGroups->reload();

    _mdlCmbGroups->addColumn("Name", Wt::ItemIsSelectable);

    _cmbGroups->setModel(_mdlCmbGroups);

    if(_mdlCmbGroups->rowCount() > 0)
        _cmbGroups->setCurrentIndex(0);
}

void Views::DlgCreateUser::_createCmbTitles()
{
    _cmbTitles = new Wt::WComboBox();
    _cmbTitles->setMinimumSize(20, 30);
    _cntCmbTitles = new Wt::WContainerWidget();
    _cntCmbTitles->addWidget(_cmbTitles);

    _mdlCmbTitles = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::UserTitle>>();

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::UserTitle>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Database::DatabaseManager::instance().session()->find<Users::UserTitle>();
    else
        query = Database::DatabaseManager::instance().session()->find<Users::UserTitle>().where("Active = ?").bind(true);

    _mdlCmbTitles->setQuery(query);

    Database::DatabaseManager::instance().commitTransaction();

    _mdlCmbTitles->reload();

    _mdlCmbTitles->addColumn("Name", Wt::ItemIsSelectable);

    _cmbTitles->setModel(_mdlCmbTitles);

    if(_mdlCmbTitles->rowCount() > 0)
        _cmbTitles->setCurrentIndex(0);
}

bool Views::DlgCreateUser::_validate()
{
    if((_txtUserName->validate() != Wt::WValidator::Valid) ||
            (_txtPassword->validate() != Wt::WValidator::Valid) ||
            (_txtIdNumber->validate() != Wt::WValidator::Valid) ||
            (_txtAddress->validate() != Wt::WValidator::Valid) ||
            (_cmbGroups->currentIndex() == -1))
        return false;

    return true;
}

void Views::DlgCreateUser::_btnOkClicked()
{
    if(_validate())
        this->accept();
}
