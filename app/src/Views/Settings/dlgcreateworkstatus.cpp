#include "dlgcreateworkstatus.h"
#include "../../Settings/appsettings.h"

#include <Wt/WBreak>

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
    return _cmbActive->currentText() == "Yes" ? true : false;
}

void Views::DlgCreateWorkStatus::_prepareView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    this->setCaption("Create Work Status");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(2);

    this->contents()->setLayout(_layMain);

    _txtType = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Type:", _txtType));

    _layMain->addWidget(new Wt::WBreak());

    _createCmbType();
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Belongs To:", _cntCmbType));

    _cmbActive = new Wt::WComboBox();
    _cmbActive->addItem("Yes");
    _cmbActive->addItem("No");
    _cmbActive->setCurrentIndex(0);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", _cmbActive), 1);

    _layMain->addWidget(new Wt::WBreak(), 1);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::DlgCreateWorkStatus::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);

    transaction.commit();
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

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectWorkStatusType>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatusType>();
    else
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatusType>().where("Active = ?").bind(true);

    _mdlCmbType->setQuery(query);

    _mdlCmbType->reload();

    _mdlCmbType->addColumn("Work_Status_Type", Wt::ItemIsSelectable);

    _cmbType->setModel(_mdlCmbType);
}
