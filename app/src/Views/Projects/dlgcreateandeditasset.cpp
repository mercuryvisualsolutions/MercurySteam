#include "dlgcreateandeditasset.h"

#include <Wt/WBreak>

#include "../../Database/databasemanager.h"
#include "../../Settings/appsettings.h"

Views::DlgCreateAndEditAsset::DlgCreateAndEditAsset(bool editing) :
    _editing(editing)
{
    _prepareView();
}

std::string Views::DlgCreateAndEditAsset::assetName() const
{
    return _txtAssetName->text().toUTF8();
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Views::DlgCreateAndEditAsset::status() const
{
    return _mdlCmbStatus->resultRow(_cmbStatus->currentIndex());
}

Wt::Dbo::ptr<Projects::ProjectAssetType> Views::DlgCreateAndEditAsset::assetType() const
{
    return _mdlCmbType->resultRow(_cmbType->currentIndex());
}

Wt::WDate Views::DlgCreateAndEditAsset::startDate() const
{
    return _datStartDate->date();
}

Wt::WDate Views::DlgCreateAndEditAsset::endDate() const
{
    return _datEndDate->date();
}

int Views::DlgCreateAndEditAsset::priority() const
{
    return _spnPriority->value();
}

std::string Views::DlgCreateAndEditAsset::description() const
{
    return _txtDescription->text().toUTF8();
}

bool Views::DlgCreateAndEditAsset::isActive() const
{
    return _cmbActive->currentText() == "Yes" ? true : false;
}

bool Views::DlgCreateAndEditAsset::isEditing()
{
    return _editing;
}

bool Views::DlgCreateAndEditAsset::editedStartDate() const
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

bool Views::DlgCreateAndEditAsset::editedEndDate() const
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

bool Views::DlgCreateAndEditAsset::editedPriority() const
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

bool Views::DlgCreateAndEditAsset::editedType() const
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

bool Views::DlgCreateAndEditAsset::editedStatus() const
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

bool Views::DlgCreateAndEditAsset::editedDescription() const
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

bool Views::DlgCreateAndEditAsset::editedActive() const
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

void Views::DlgCreateAndEditAsset::_prepareView()
{
    if(!_editing)
        this->setCaption("Create Asset");
    else
        this->setCaption("Edit Assets");

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

    if(!_editing)
    {
        _txtAssetName = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{4,150}");
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", _txtAssetName));

        _layLeft->addWidget(new Wt::WBreak());
    }

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

    _spnPriority = Ms::Widgets::MWidgetFactory::createSpinBox(0, INT_MAX, 0);
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Priority:", _spnPriority));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Priority:", _spnPriority));

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
    _btnOk->clicked().connect(this, &Views::DlgCreateAndEditAsset::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
    _btnCancel->setFocus();
}

void Views::DlgCreateAndEditAsset::_btnOkClicked()
{
    if(_validate())
        this->accept();
}

void Views::DlgCreateAndEditAsset::_createCmbType()
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

    if(_mdlCmbType->rowCount() > 0)
        _cmbType->setCurrentIndex(0);
}

void Views::DlgCreateAndEditAsset::_createCmbStatus()
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

    if(_mdlCmbStatus->rowCount() > 0)
        _cmbStatus->setCurrentIndex(0);
}

bool Views::DlgCreateAndEditAsset::_validate()
{
    bool result = true;

    if(!_editing)
    {
        if((_datStartDate->validate() != Wt::WDateValidator::Valid) ||
                (_datEndDate->validate() != Wt::WDateValidator::Valid) ||
                (_spnPriority->validate() != Wt::WIntValidator::Valid) ||
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
                ((_cmbType->isEnabled()) && (_cmbType->currentIndex() == -1)) ||
                ((_cmbStatus->isEnabled()) && (_cmbStatus->currentIndex() == -1)))
        {
            result = false;
        }
    }

    return result;
}
