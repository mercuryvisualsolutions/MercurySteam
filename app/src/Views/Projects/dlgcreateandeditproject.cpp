#include "dlgcreateandeditproject.h"

#include <Wt/WBreak>

#include "../../Database/databasemanager.h"
#include "../../Settings/appsettings.h"

Views::DlgCreateAndEditProject::DlgCreateAndEditProject(bool editing) :
    _editing(editing)
{
    _prepareView();
}

std::string Views::DlgCreateAndEditProject::projectName() const
{
    return _txtProjectName->text().toUTF8();
}

Wt::WDate Views::DlgCreateAndEditProject::startDate() const
{
    return _datStartDate->date();
}

Wt::WDate Views::DlgCreateAndEditProject::endDate() const
{
    return _datEndDate->date();
}

int Views::DlgCreateAndEditProject::duration() const
{
    return _spnDuration->value();
}

float Views::DlgCreateAndEditProject::fps() const
{
    return _spnFPS->value();
}

int Views::DlgCreateAndEditProject::frameWidth() const
{
    return _spnWidth->value();
}

int Views::DlgCreateAndEditProject::frameHeight() const
{
    return _spnHeight->value();
}

int Views::DlgCreateAndEditProject::priority() const
{
    return _spnPriority->value();
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Views::DlgCreateAndEditProject::status() const
{
    return _mdlCmbStatus->resultRow(_cmbStatus->currentIndex());
}

Wt::Dbo::ptr<Users::User> Views::DlgCreateAndEditProject::manager() const
{
    return _mdlCmbManager->resultRow(_cmbManager->currentIndex());
}

std::string Views::DlgCreateAndEditProject::description() const
{
    return _txtDescription->text().toUTF8();
}

bool Views::DlgCreateAndEditProject::isActive() const
{
    return _cmbActive->currentText() == "Yes" ? true : false;
}

bool Views::DlgCreateAndEditProject::isEditing()
{
    return _editing;
}

bool Views::DlgCreateAndEditProject::editedStartDate() const
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

bool Views::DlgCreateAndEditProject::editedEndDate() const
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

bool Views::DlgCreateAndEditProject::editedDuration() const
{
    if(_editing)
    {
        if(_spnDuration->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedFps() const
{
    if(_editing)
    {
        if(_spnFPS->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedFrameWidth() const
{
    if(_editing)
    {
        if(_spnWidth->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedFrameHeight() const
{
    if(_editing)
    {
        if(_spnHeight->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedPriority() const
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

bool Views::DlgCreateAndEditProject::editedStatus() const
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

bool Views::DlgCreateAndEditProject::editedManager() const
{
    if(_editing)
    {
        if(_cmbManager->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditProject::editedDescription() const
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

bool Views::DlgCreateAndEditProject::editedActive() const
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

void Views::DlgCreateAndEditProject::_prepareView()
{
    if(!_editing)
        this->setCaption("Create Project");
    else
        this->setCaption("Edit Projects");

    this->rejectWhenEscapePressed();

    _layMain = new Wt::WHBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    this->contents()->setLayout(_layMain);

    _layLeft = new Wt::WVBoxLayout();
    _layLeft->setContentsMargins(0,0,0,0);
    _layLeft->setSpacing(2);

    _cntLeft = new Wt::WContainerWidget();
    _cntLeft->setLayout(_layLeft);

    _layMain->addWidget(_cntLeft);

    _layRight = new Wt::WVBoxLayout();
    _layRight->setContentsMargins(16,0,0,0);
    _layRight->setSpacing(2);

    _cntRight = new Wt::WContainerWidget();
    _cntRight->setLayout(_layRight);

    _layMain->addWidget(_cntRight);

    if(!_editing)
    {
        _txtProjectName = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{4,150}");
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", _txtProjectName));

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

    _spnDuration = Ms::Widgets::MWidgetFactory::createSpinBox(0, INT_MAX, 0);
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Duration In Frames:", _spnDuration));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Duration In Frames:", _spnDuration));

    _layLeft->addWidget(new Wt::WBreak());

    _spnFPS = Ms::Widgets::MWidgetFactory::createDoubleSpinBox(1.0, DBL_MAX, 25.0);
    _spnFPS->setValidator(Ms::Widgets::MWidgetFactory::createDoubleValidator(1.0, DBL_MAX));
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("FPS:", _spnFPS));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("FPS:", _spnFPS));

    _layLeft->addWidget(new Wt::WBreak());

    _spnWidth = Ms::Widgets::MWidgetFactory::createSpinBox(1, INT_MAX, 1920);
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Frame Width:", _spnWidth));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Frame Width:", _spnWidth));

    _layLeft->addWidget(new Wt::WBreak());

    _spnHeight = Ms::Widgets::MWidgetFactory::createSpinBox(1, INT_MAX, 1080);
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Frame Height:", _spnHeight));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Frame Height:", _spnHeight));

    _layLeft->addWidget(new Wt::WBreak());

    _spnPriority = Ms::Widgets::MWidgetFactory::createSpinBox(0, INT_MAX, 0);
    if(_editing)
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Priority:", _spnPriority));
    else
        _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Priority:", _spnPriority));

    _layLeft->addWidget(new Wt::WBreak(), 1);

    _createCmbStatus();
    if(_editing)
        _layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Status:", _cntCmbStatus));
    else
        _layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Status:", _cntCmbStatus));

    _layRight->addWidget(new Wt::WBreak());

    _createCmbManager();
    if(_editing)
        _layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Manager:", _cntCmbManager));
    else
        _layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Manager:", _cntCmbManager));

    _layRight->addWidget(new Wt::WBreak());

    _txtDescription = Ms::Widgets::MWidgetFactory::createTextArea("", false);
    if(_editing)
        _layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Description:", _txtDescription));
    else
        _layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Description:", _txtDescription));

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
    _btnOk->clicked().connect(this, &Views::DlgCreateAndEditProject::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
    _btnCancel->setFocus();
}

void Views::DlgCreateAndEditProject::_btnOkClicked()
{
    if(_validate())
        this->accept();
}

void Views::DlgCreateAndEditProject::_createCmbManager()
{
    _cmbManager = new Wt::WComboBox();
    _cmbManager->setMinimumSize(20, 30);
    _cntCmbManager = new Wt::WContainerWidget();
    _cntCmbManager->addWidget(_cmbManager);

    _mdlCmbManager = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::User>>();

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::User>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Database::DatabaseManager::instance().session()->find<Users::User>();
    else
        query = Database::DatabaseManager::instance().session()->find<Users::User>().where("Active = ?").bind(true);

    _mdlCmbManager->setQuery(query);

    Database::DatabaseManager::instance().commitTransaction();

    _mdlCmbManager->reload();

    _mdlCmbManager->addColumn("Name", Wt::ItemIsSelectable);

    _cmbManager->setModel(_mdlCmbManager);

    if(_mdlCmbManager->rowCount() > 0)
        _cmbManager->setCurrentIndex(0);
}

void Views::DlgCreateAndEditProject::_createCmbStatus()
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

bool Views::DlgCreateAndEditProject::_validate()
{
    bool result = true;

    if(!_editing)
    {
        if((_txtProjectName->validate() != Wt::WValidator::Valid) ||
                (_datStartDate->validate() != Wt::WDateValidator::Valid) ||
                (_datEndDate->validate() != Wt::WDateValidator::Valid) ||
                (_spnDuration->validate() != Wt::WIntValidator::Valid) ||
                (_spnFPS->validate() != Wt::WDoubleValidator::Valid) ||
                (_spnWidth->validate() != Wt::WIntValidator::Valid) ||
                (_spnHeight->validate() != Wt::WIntValidator::Valid) ||
                (_spnPriority->validate() != Wt::WIntValidator::Valid) ||
                (_cmbStatus->currentIndex() == -1) ||
                (_cmbManager->currentIndex() == -1))
        {
            result = false;
        }
    }
    else
    {
        if(((_datStartDate->isEnabled()) && (_datStartDate->validate() != Wt::WValidator::Valid)) ||
                ((_datEndDate->isEnabled()) && (_datEndDate->validate() != Wt::WValidator::Valid)) ||
                ((_spnDuration->isEnabled()) && (_spnDuration->validate() != Wt::WValidator::Valid)) ||
                ((_spnFPS->isEnabled()) && (_spnFPS->validate() != Wt::WValidator::Valid)) ||
                ((_spnWidth->isEnabled()) && (_spnWidth->validate() != Wt::WValidator::Valid)) ||
                ((_spnHeight->isEnabled()) && (_spnHeight->validate() != Wt::WValidator::Valid)) ||
                ((_spnPriority->isEnabled()) && (_spnPriority->validate() != Wt::WIntValidator::Valid)) ||
                ((_cmbStatus->isEnabled()) && (_cmbStatus->currentIndex() == -1)) ||
                ((_cmbManager->isEnabled()) && (_cmbManager->currentIndex() == -1)))
        {
            result = false;
        }
    }

    return result;
}
