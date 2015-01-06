#include "dlgselecttasktemplate.h"

#include <Wt/WBreak>

#include "../../Database/dbosession.h"
#include "../../Settings/appsettings.h"

Views::DlgSelectTaskTemplate::DlgSelectTaskTemplate()
{
    prepareView();
}

Wt::Dbo::ptr<Projects::ProjectTaskTemplate> Views::DlgSelectTaskTemplate::taskTemplate() const
{
    return m_mdlCmbTemplate->resultRow(m_cmbTemplate->currentIndex());
}

void Views::DlgSelectTaskTemplate::prepareView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    this->setCaption("Select Template");
    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WHBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(2);

    this->contents()->setLayout(m_layMain);

    m_layLeft = new Wt::WVBoxLayout();
    m_layLeft->setContentsMargins(0,0,0,0);
    m_layLeft->setSpacing(2);

    m_cntLeft = new Wt::WContainerWidget();
    m_cntLeft->setLayout(m_layLeft);

    m_layMain->addWidget(m_cntLeft);

    createCmbTemplate();
    m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Template:", m_cntCmbTemplate));

    m_layLeft->addWidget(new Wt::WBreak());

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->clicked().connect(this, &Views::DlgSelectTaskTemplate::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
    m_btnCancel->setFocus();

    transaction.commit();
}

void Views::DlgSelectTaskTemplate::btnOkClicked()
{
    if(validate())
        this->accept();
}

void Views::DlgSelectTaskTemplate::createCmbTemplate()
{
    m_cmbTemplate = new Wt::WComboBox();
    m_cmbTemplate->setMinimumSize(20, 30);
    m_cntCmbTemplate = new Wt::WContainerWidget();
    m_cntCmbTemplate->addWidget(m_cmbTemplate);

    m_mdlCmbTemplate = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>>();

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskTemplate>();
    else
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskTemplate>().where("Active = ?").bind(true);

    m_mdlCmbTemplate->setQuery(query);

    m_mdlCmbTemplate->reload();

    m_mdlCmbTemplate->addColumn("Name", Wt::ItemIsSelectable);

    m_cmbTemplate->setModel(m_mdlCmbTemplate);

    if(m_mdlCmbTemplate->rowCount() > 0)
        m_cmbTemplate->setCurrentIndex(0);
}

bool Views::DlgSelectTaskTemplate::validate()
{
    if(m_cmbTemplate->currentIndex() == -1)
	return false;
    
    return true;
}
