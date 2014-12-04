#include "dlgselectactivitytemplate.h"

#include <Wt/WBreak>

#include "../../Database/dbosession.h"
#include "../../Settings/appsettings.h"

Views::DlgSelectActivityTemplate::DlgSelectActivityTemplate()
{
    _prepareView();
}

Wt::Dbo::ptr<Projects::ProjectActivityTemplate> Views::DlgSelectActivityTemplate::activityTemplate()
{
    return _mdlCmbTemplate->resultRow(_cmbTemplate->currentIndex());
}

void Views::DlgSelectActivityTemplate::_prepareView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    this->setCaption("Select Template");
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

    _createCmbTemplate();
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Template:", _cntCmbTemplate));

    _layLeft->addWidget(new Wt::WBreak());

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->clicked().connect(this, &Views::DlgSelectActivityTemplate::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
    _btnCancel->setFocus();

    transaction.commit();
}

void Views::DlgSelectActivityTemplate::_btnOkClicked()
{
    if(_validate())
        this->accept();
}

void Views::DlgSelectActivityTemplate::_createCmbTemplate()
{
    _cmbTemplate = new Wt::WComboBox();
    _cmbTemplate->setMinimumSize(20, 30);
    _cntCmbTemplate = new Wt::WContainerWidget();
    _cntCmbTemplate->addWidget(_cmbTemplate);

    _mdlCmbTemplate = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectActivityTemplate>>();

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectActivityTemplate>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectActivityTemplate>();
    else
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectActivityTemplate>().where("Active = ?").bind(true);

    _mdlCmbTemplate->setQuery(query);

    _mdlCmbTemplate->reload();

    _mdlCmbTemplate->addColumn("Name", Wt::ItemIsSelectable);

    _cmbTemplate->setModel(_mdlCmbTemplate);

    if(_mdlCmbTemplate->rowCount() > 0)
        _cmbTemplate->setCurrentIndex(0);
}

bool Views::DlgSelectActivityTemplate::_validate()
{
    if(_cmbTemplate->currentIndex() == -1)
	return false;
    
    return true;
}
