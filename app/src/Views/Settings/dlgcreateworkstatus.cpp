#include "dlgcreateworkstatus.h"

#include <Wt/WBreak>

#include "../../Database/databasemanager.h"
#include "../../Settings/appsettings.h"

Views::DlgCreateWorkStatus::DlgCreateWorkStatus()
{
    _prepareView();
}

std::string Views::DlgCreateWorkStatus::type() const
{
    return _txtType->text().toUTF8();
}

Wt::Dbo::ptr<Projects::ProjectWorkStatusType> Views::DlgCreateWorkStatus::belongsToType() const
{
    return _mdlCmbType->resultRow(_cmbType->currentIndex());
}

bool Views::DlgCreateWorkStatus::isActive() const
{
    return _chkActive->isChecked();
}

void Views::DlgCreateWorkStatus::_prepareView()
{
    this->setCaption("Create Work Status");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(6);

    this->contents()->setLayout(_layMain);

    _txtType = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Type:", _txtType));

    _layMain->addWidget(new Wt::WBreak());

    _createCmbType();
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Belongs To:", _cntCmbType));

    _chkActive = new Wt::WCheckBox("Active");
    _chkActive->setChecked(true);
    _layMain->addWidget(_chkActive);

    _layMain->addWidget(new Wt::WBreak());

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::DlgCreateWorkStatus::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::DlgCreateWorkStatus::_validate()
{
    if(_txtType->validate() != Wt::WValidator::Valid ||
            (_cmbType->currentIndex() == -1))
        return false;

    return true;
}

void Views::DlgCreateWorkStatus::_btnOkClicked()
{
    if(_validate())
        this->accept();
}


void Views::DlgCreateWorkStatus::_createCmbType()
{
    _cmbType = new Wt::WComboBox();
    _cmbType->setMinimumSize(20, 30);
    _cntCmbType = new Wt::WContainerWidget();
    _cntCmbType->addWidget(_cmbType);

    _mdlCmbType = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatusType>>();

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectWorkStatusType>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatusType>();
    else
        query = Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatusType>().where("Active = ?").bind(true);

    _mdlCmbType->setQuery(query);

    Database::DatabaseManager::instance().commitTransaction();

    _mdlCmbType->reload();

    _mdlCmbType->addColumn("Work_Status_Type", Wt::ItemIsSelectable);

    _cmbType->setModel(_mdlCmbType);
}
