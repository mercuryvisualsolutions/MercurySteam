#include "dlgcreateworkstatus.h"
#include "../../Settings/appsettings.h"

#include <Wt/WBreak>

Views::DlgCreateWorkStatus::DlgCreateWorkStatus()
{
    prepareView();
}

std::string Views::DlgCreateWorkStatus::type() const
{
    return m_txtType->text().toUTF8();
}

Wt::Dbo::ptr<Projects::ProjectWorkStatusType> Views::DlgCreateWorkStatus::belongsToType() const
{
    return m_mdlCmbType->resultRow(m_cmbType->currentIndex());
}

bool Views::DlgCreateWorkStatus::isActive() const
{
    return m_cmbActive->currentText() == "Yes" ? true : false;
}

void Views::DlgCreateWorkStatus::prepareView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    this->setCaption("Create Work Status");
    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(2);

    this->contents()->setLayout(m_layMain);

    m_txtType = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Type:", m_txtType));

    m_layMain->addWidget(new Wt::WBreak());

    createCmbType();
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Belongs To:", m_cntCmbType));

    m_cmbActive = new Wt::WComboBox();
    m_cmbActive->addItem("Yes");
    m_cmbActive->addItem("No");
    m_cmbActive->setCurrentIndex(0);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", m_cmbActive), 1);

    m_layMain->addWidget(new Wt::WBreak(), 1);

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->setDefault(true);
    m_btnOk->clicked().connect(this, &Views::DlgCreateWorkStatus::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);

    transaction.commit();
}

bool Views::DlgCreateWorkStatus::validate()
{
    if(m_txtType->validate() != Wt::WValidator::Valid ||
            (m_cmbType->currentIndex() == -1))
        return false;

    return true;
}

void Views::DlgCreateWorkStatus::btnOkClicked()
{
    if(validate())
        this->accept();
}


void Views::DlgCreateWorkStatus::createCmbType()
{
    m_cmbType = new Wt::WComboBox();
    m_cmbType->setMinimumSize(20, 30);
    m_cntCmbType = new Wt::WContainerWidget();
    m_cntCmbType->addWidget(m_cmbType);

    m_mdlCmbType = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatusType>>();

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectWorkStatusType>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatusType>();
    else
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatusType>().where("Active = ?").bind(true);

    m_mdlCmbType->setQuery(query);

    m_mdlCmbType->reload();

    m_mdlCmbType->addColumn("Work_Status_Type", Wt::ItemIsSelectable);

    m_cmbType->setModel(m_mdlCmbType);
}
