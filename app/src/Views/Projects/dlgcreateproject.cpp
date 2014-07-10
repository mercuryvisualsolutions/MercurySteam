#include "dlgcreateproject.h"

#include <Wt/WBreak>

#include "../../Database/databasemanager.h"
#include "../../Settings/appsettings.h"

Views::DlgCreateProject::DlgCreateProject()
{
    _prepareView();
}

std::string Views::DlgCreateProject::projectName() const
{
    return _txtProjectName->text().toUTF8();
}

Wt::WDate Views::DlgCreateProject::startDate() const
{
    return _datStartDate->date();
}

Wt::WDate Views::DlgCreateProject::endDate() const
{
    return _datEndDate->date();
}

int Views::DlgCreateProject::duration() const
{
    return _spnDuration->value();
}

float Views::DlgCreateProject::fps() const
{
    return _spnFPS->value();
}

int Views::DlgCreateProject::frameWidth() const
{
    return _spnWidth->value();
}

int Views::DlgCreateProject::frameHeight() const
{
    return _spnHeight->value();
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Views::DlgCreateProject::status() const
{
    return _mdlCmbStatus->resultRow(_cmbStatus->currentIndex());
}

Wt::Dbo::ptr<Users::User> Views::DlgCreateProject::manager() const
{
    return _mdlCmbManager->resultRow(_cmbManager->currentIndex());
}

std::string Views::DlgCreateProject::description() const
{
    return _txtDescription->text().toUTF8();
}

bool Views::DlgCreateProject::isActive() const
{
    return _chkActive->isChecked();
}

void Views::DlgCreateProject::_prepareView()
{
    this->setCaption("Create Project");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WHBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    this->contents()->setLayout(_layMain);

    _layLeft = new Wt::WVBoxLayout();
    _layLeft->setContentsMargins(0,0,0,0);
    _layLeft->setSpacing(0);

    _cntLeft = new Wt::WContainerWidget();
    _cntLeft->setLayout(_layLeft);

    _layMain->addWidget(_cntLeft);

    _layRight = new Wt::WVBoxLayout();
    _layRight->setContentsMargins(0,0,0,0);
    _layRight->setSpacing(1);

    _cntRight = new Wt::WContainerWidget();
    _cntRight->setLayout(_layRight);

    _layMain->addWidget(_cntRight);

    _txtProjectName = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{4,150}");
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", _txtProjectName));

    _layLeft->addWidget(new Wt::WBreak());

    _datStartDate = Ms::Widgets::MWidgetFactory::createDateEdit();
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Start Date:", _datStartDate));

    _layLeft->addWidget(new Wt::WBreak());

    _datEndDate = Ms::Widgets::MWidgetFactory::createDateEdit();
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("End Date:", _datEndDate));

    _layLeft->addWidget(new Wt::WBreak());

    _spnDuration = Ms::Widgets::MWidgetFactory::createSpinBox(1, INT_MAX, 1500);
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Duration In Frames:", _spnDuration));

    _layLeft->addWidget(new Wt::WBreak());

    _spnFPS = Ms::Widgets::MWidgetFactory::createDoubleSpinBox(1.0, DBL_MAX, 25.0);
    _spnFPS->setValidator(Ms::Widgets::MWidgetFactory::createDoubleValidator(1.0, DBL_MAX));

    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("FPS:", _spnFPS));

    _layLeft->addWidget(new Wt::WBreak());

    _spnWidth = Ms::Widgets::MWidgetFactory::createSpinBox(1, INT_MAX, 1920);
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Frame Width:", _spnWidth));

    _layLeft->addWidget(new Wt::WBreak());

    _spnHeight = Ms::Widgets::MWidgetFactory::createSpinBox(1, INT_MAX, 1080);
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Frame Height:", _spnHeight));

    _layLeft->addWidget(new Wt::WBreak(), 1);

    _createCmbStatus();
    _layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Status:", _cntCmbStatus));

    _layRight->addWidget(new Wt::WBreak());

    _createCmbManager();
    _layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Project Manager:", _cntCmbManager));

    _layRight->addWidget(new Wt::WBreak());

    _txtDescription = Ms::Widgets::MWidgetFactory::createTextArea("", false);
    _layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Description:", _txtDescription), 1);

    _chkActive = new Wt::WCheckBox("active");
    _chkActive->setChecked(true);

    _layRight->addWidget(_chkActive);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::DlgCreateProject::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

void Views::DlgCreateProject::_btnOkClicked()
{
    if(_validate())
        this->accept();
}

void Views::DlgCreateProject::_createCmbManager()
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
}

void Views::DlgCreateProject::_createCmbStatus()
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

bool Views::DlgCreateProject::_validate()
{
    if((_txtProjectName->validate() != Wt::WValidator::Valid) ||
            (_datStartDate->validate() != Wt::WDateValidator::Valid) ||
            (_datEndDate->validate() != Wt::WDateValidator::Valid) ||
            (_spnDuration->validate() != Wt::WIntValidator::Valid) ||
            (_spnFPS->validate() != Wt::WDoubleValidator::Valid) ||
            (_spnWidth->validate() != Wt::WIntValidator::Valid) ||
            (_spnHeight->validate() != Wt::WIntValidator::Valid) ||
            (_cmbStatus->currentIndex() == -1) ||
            (_cmbManager->currentIndex() == -1))
        return false;

    return true;
}
