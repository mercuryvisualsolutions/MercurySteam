#include "dlgcreateandedittaskactivity.h"

#include <Wt/WBreak>

#include "../../Database/databasemanager.h"
#include "../../Settings/appsettings.h"

Views::DlgCreateAndEditTaskActivity::DlgCreateAndEditTaskActivity(bool editing) :
    _editing(editing)
{
    _prepareView();
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Views::DlgCreateAndEditTaskActivity::status() const
{
    return _mdlCmbStatus->resultRow(_cmbStatus->currentIndex());
}

Wt::Dbo::ptr<Projects::ProjectTaskActivityType> Views::DlgCreateAndEditTaskActivity::type() const
{
    return _mdlCmbType->resultRow(_cmbType->currentIndex());
}

int Views::DlgCreateAndEditTaskActivity::hours() const
{
    return _spnHours->value();
}

std::string Views::DlgCreateAndEditTaskActivity::description() const
{
    return _txtDescription->text().toUTF8();
}

bool Views::DlgCreateAndEditTaskActivity::isActive() const
{
    return _cmbActive->currentText() == "Yes" ? true : false;
}

bool Views::DlgCreateAndEditTaskActivity::isEditing()
{
    return _editing;
}

bool Views::DlgCreateAndEditTaskActivity::editedHours() const
{
    if(_editing)
    {
        if(_spnHours->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditTaskActivity::editedType() const
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

bool Views::DlgCreateAndEditTaskActivity::editedStatus() const
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

bool Views::DlgCreateAndEditTaskActivity::editedDescription() const
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

bool Views::DlgCreateAndEditTaskActivity::editedActive() const
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

void Views::DlgCreateAndEditTaskActivity::_prepareView()
{
    if(!_editing)
        this->setCaption("Create Task Activity");
    else
        this->setCaption("Edit Task Activity");

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

//    _layRight = new Wt::WVBoxLayout();
//    _layRight->setContentsMargins(16,0,0,0);
//    _layRight->setSpacing(1);

//    _cntRight = new Wt::WContainerWidget();
//    _cntRight->setLayout(_layRight);

//    _layMain->addWidget(_cntRight);

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

    _spnHours = Ms::Widgets::MWidgetFactory::createSpinBox(0, INT_MAX, 0);
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Hours:", _spnHours));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Hours:", _spnHours));

    _layLeft->addWidget(new Wt::WBreak());

    _txtDescription = Ms::Widgets::MWidgetFactory::createTextArea("", false);
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Description:", _txtDescription));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Description:", _txtDescription));

    _layLeft->addWidget(new Wt::WBreak());

    _cmbActive = new Wt::WComboBox();
    _cmbActive->addItem("Yes");
    _cmbActive->addItem("No");
    _cmbActive->setCurrentIndex(0);
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Active:", _cmbActive));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", _cmbActive));

    _layLeft->addWidget(new Wt::WBreak(), 1);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->clicked().connect(this, &Views::DlgCreateAndEditTaskActivity::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
    _btnCancel->setFocus();
}

void Views::DlgCreateAndEditTaskActivity::_btnOkClicked()
{
    if(_validate())
        this->accept();
}

void Views::DlgCreateAndEditTaskActivity::_createCmbType()
{
    _cmbType = new Wt::WComboBox();
    _cmbType->setMinimumSize(20, 30);
    _cntCmbType = new Wt::WContainerWidget();
    _cntCmbType->addWidget(_cmbType);

    _mdlCmbType = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskActivityType>>();

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskActivityType>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskActivityType>();
    else
        query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskActivityType>().where("Active = ?").bind(true);

    _mdlCmbType->setQuery(query);

    Database::DatabaseManager::instance().commitTransaction();

    _mdlCmbType->reload();

    _mdlCmbType->addColumn("Type", Wt::ItemIsSelectable);

    _cmbType->setModel(_mdlCmbType);

    if(_mdlCmbType->rowCount() > 0)
        _cmbType->setCurrentIndex(0);
}

void Views::DlgCreateAndEditTaskActivity::_createCmbStatus()
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

bool Views::DlgCreateAndEditTaskActivity::_validate()
{
    bool result = true;

    if(!_editing)
    {
        if((_spnHours->validate() != Wt::WIntValidator::Valid) ||
                (_cmbType->currentIndex() == -1) ||
                (_cmbStatus->currentIndex() == -1))
        {
            result = false;
        }
    }
    else
    {
        if((_spnHours->isEnabled() && (_spnHours->validate() != Wt::WIntValidator::Valid)) ||
                ((_cmbType->isEnabled()) && (_cmbType->currentIndex() == -1)) ||
                ((_cmbStatus->isEnabled()) && (_cmbStatus->currentIndex() == -1)))
        {
            result = false;
        }
    }

    return result;
}
