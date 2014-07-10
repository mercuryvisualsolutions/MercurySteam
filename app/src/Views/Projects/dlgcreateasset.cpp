#include "dlgcreateasset.h"

#include <Wt/WBreak>

#include "../../Database/databasemanager.h"
#include "../../Settings/appsettings.h"

Views::DlgCreateAsset::DlgCreateAsset()
{
    _prepareView();
}

std::string Views::DlgCreateAsset::assetName() const
{
    return _txtAssetName->text().toUTF8();
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Views::DlgCreateAsset::status() const
{
    return _mdlCmbStatus->resultRow(_cmbStatus->currentIndex());
}

Wt::Dbo::ptr<Projects::ProjectAssetType> Views::DlgCreateAsset::assetType() const
{
    return _mdlCmbType->resultRow(_cmbType->currentIndex());
}

Wt::WDate Views::DlgCreateAsset::startDate() const
{
    return _datStartDate->date();
}

Wt::WDate Views::DlgCreateAsset::endDate() const
{
    return _datEndDate->date();
}

std::string Views::DlgCreateAsset::description() const
{
    return _txtDescription->text().toUTF8();
}

bool Views::DlgCreateAsset::isActive() const
{
    return _chkActive->isChecked();
}

void Views::DlgCreateAsset::_prepareView()
{
    this->setCaption("Create Asset");
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

    _txtAssetName = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{4,150}");
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", _txtAssetName));

    _layLeft->addWidget(new Wt::WBreak());

    _datStartDate = Ms::Widgets::MWidgetFactory::createDateEdit();
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Start Date:", _datStartDate));

    _layLeft->addWidget(new Wt::WBreak());

    _datEndDate = Ms::Widgets::MWidgetFactory::createDateEdit();
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("End Date:", _datEndDate));

    _layLeft->addWidget(new Wt::WBreak());

    _createCmbStatus();
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Status:", _cntCmbStatus));

    _layLeft->addWidget(new Wt::WBreak());

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
    _btnOk->clicked().connect(this, &Views::DlgCreateAsset::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

void Views::DlgCreateAsset::_btnOkClicked()
{
    if(_validate())
        this->accept();
}

void Views::DlgCreateAsset::_createCmbType()
{
    _cmbType = new Wt::WComboBox();
    _cmbType->setMinimumSize(20, 30);
    _cntCmbType = new Wt::WContainerWidget();
    _cntCmbType->addWidget(_cmbType);

    _mdlCmbType = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectAssetType>>();

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectAssetType>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Database::DatabaseManager::instance().session()->find<Projects::ProjectAssetType>();
    else
        query = Database::DatabaseManager::instance().session()->find<Projects::ProjectAssetType>().where("Active = ?").bind(true);

    _mdlCmbType->setQuery(query);

    Database::DatabaseManager::instance().commitTransaction();

    _mdlCmbType->reload();

    _mdlCmbType->addColumn("Type", Wt::ItemIsSelectable);

    _cmbType->setModel(_mdlCmbType);
}

void Views::DlgCreateAsset::_createCmbStatus()
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

bool Views::DlgCreateAsset::_validate()
{
    if((_txtAssetName->validate() != Wt::WValidator::Valid) ||
            (_datStartDate->validate() != Wt::WDateValidator::Valid) ||
            (_datEndDate->validate() != Wt::WDateValidator::Valid) ||
            (_cmbStatus->currentIndex() == -1) ||
            (_cmbType->currentIndex() == -1))
        return false;

    return true;
}
