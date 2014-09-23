#include "dlgcreateandeditasset.h"

#include <Wt/WBreak>

#include "../../Database/databasemanager.h"
#include "../../Settings/appsettings.h"

Views::DlgSelectTaskPipeline::DlgSelectTaskPipeline()
{
    _prepareView();
}

Wt::Dbo::ptr<Projects::ProjectTaskPipeline> Views::DlgSelectTaskPipeline::pipeline() const
{
    return _mdlCmbPipeline->resultRow(_cmbPipeline->currentIndex());
}

void Views::DlgSelectTaskPipeline::_prepareView()
{
    this->setCaption("Select Pipeline");
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

    _createCmbPipeline();
    _layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Pipeline:", _cntCmbPipeline));

    _layLeft->addWidget(new Wt::WBreak());

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->clicked().connect(this, &Views::DlgSelectTaskPipeline::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
    _btnCancel->setFocus();
}

void Views::DlgSelectTaskPipeline::_btnOkClicked()
{
    if(_validate())
        this->accept();
}

void Views::DlgSelectTaskPipeline::_createCmbStatus()
{
    _cmbPipeline = new Wt::WComboBox();
    _cmbPipeline->setMinimumSize(20, 30);
    _cntCmbPipeline = new Wt::WContainerWidget();
    _cntCmbPipeline->addWidget(_cmbPipeline);

    _mdlCmbPipeline = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskPipeline>>();

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskPipeline>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskPipeline>();
    else
        query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskPipeline>().where("Active = ?").bind(true);

    _mdlCmbPipeline->setQuery(query);

    Database::DatabaseManager::instance().commitTransaction();

    _mdlCmbPipeline->reload();

    _mdlCmbPipeline->addColumn("Name", Wt::ItemIsSelectable);

    _cmbPipeline->setModel(_mdlCmbPipeline);

    if(_mdlCmbPipeline->rowCount() > 0)
        _cmbPipeline->setCurrentIndex(0);
}

bool Views::DlgSelectTaskPipeline::_validate()
{
    if(_cmbPipeline->currentIndex() == -1)
	return false;
    
    return true;
}
