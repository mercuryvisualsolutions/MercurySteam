#include "dlgcreateuser.h"
#include "../../Settings/appsettings.h"

#include <Wt/WBreak>

Views::DlgCreateUser::DlgCreateUser()
{
    prepareView();
}

std::string Views::DlgCreateUser::userName() const
{
    return m_txtUserName->text().toUTF8();
}

std::string Views::DlgCreateUser::password() const
{
    return m_txtPassword->text().toUTF8();
}

std::string Views::DlgCreateUser::emailAddress() const
{
    return m_txtEmailAddress->text().toUTF8();
}

std::string Views::DlgCreateUser::phoneNumber() const
{
    return m_txtPhoneNumber->text().toUTF8();
}

std::string Views::DlgCreateUser::idNumber() const
{
    return m_txtIdNumber->text().toUTF8();
}

std::string Views::DlgCreateUser::address() const
{
    return m_txtAddress->text().toUTF8();
}

Wt::Dbo::ptr<Users::Group> Views::DlgCreateUser::group() const
{
    return m_mdlCmbGroups->resultRow(m_cmbGroups->currentIndex());
}

Wt::Dbo::ptr<Users::UserTitle> Views::DlgCreateUser::title() const
{
    return m_mdlCmbTitles->resultRow(m_cmbTitles->currentIndex());
}

bool Views::DlgCreateUser::isActive() const
{
    return m_cmbActive->currentText() == "Yes" ? true : false;
}

void Views::DlgCreateUser::prepareView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    this->setCaption("Create User");
    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WHBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    this->contents()->setLayout(m_layMain);

    m_layLeft = new Wt::WVBoxLayout();
    m_layLeft->setContentsMargins(0,0,0,0);
    m_layLeft->setSpacing(2);

    m_cntLeft = new Wt::WContainerWidget();
    m_cntLeft->setLayout(m_layLeft);

    m_layMain->addWidget(m_cntLeft);

    m_layRight = new Wt::WVBoxLayout();
    m_layRight->setContentsMargins(16,0,0,0);
    m_layRight->setSpacing(2);

    m_cntRight = new Wt::WContainerWidget();
    m_cntRight->setLayout(m_layRight);

    m_layMain->addWidget(m_cntRight);

    m_txtUserName = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{3,150}");
    m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", m_txtUserName));

    m_layLeft->addWidget(new Wt::WBreak());

    m_txtPassword = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9_-]{6,14}");
    m_txtPassword->setEchoMode(Wt::WLineEdit::Password);
    m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Password:", m_txtPassword));

    m_layLeft->addWidget(new Wt::WBreak());

    m_txtEmailAddress = Ms::Widgets::MWidgetFactory::createLineEdit("", false);
    m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Email:", m_txtEmailAddress));

    m_layLeft->addWidget(new Wt::WBreak());

    m_txtPhoneNumber = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[0-9]{7,255}", false);
    m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Phone Number:", m_txtPhoneNumber));

    m_layLeft->addWidget(new Wt::WBreak());

    m_txtIdNumber = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[0-9]{1,255}", false);
    m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Id Number:", m_txtIdNumber));

    m_layMain->addWidget(new Wt::WBreak());

    m_txtAddress = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{6,255}", false);
    m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Address:", m_txtAddress));

    m_layRight->addWidget(new Wt::WBreak());

    createCmbGroups();
    m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Group:", m_cntCmbGroups));

    m_layRight->addWidget(new Wt::WBreak());

    createCmbTitles();
    m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Title:", m_cntCmbTitles));

    m_layRight->addWidget(new Wt::WBreak());

    m_cmbActive = new Wt::WComboBox();
    m_cmbActive->addItem("Yes");
    m_cmbActive->addItem("No");
    m_cmbActive->setCurrentIndex(0);
    m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", m_cmbActive));

    m_layRight->addWidget(new Wt::WBreak(), 1);

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->setDefault(true);
    m_btnOk->clicked().connect(this, &Views::DlgCreateUser::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);

    transaction.commit();
}

void Views::DlgCreateUser::createCmbGroups()
{
    m_cmbGroups = new Wt::WComboBox();
    m_cmbGroups->setMinimumSize(20, 30);
    m_cntCmbGroups = new Wt::WContainerWidget();
    m_cntCmbGroups->addWidget(m_cmbGroups);

    m_mdlCmbGroups = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::Group>>();

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::Group>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Users::Group>();
    else
        query = Session::SessionManager::instance().dboSession().find<Users::Group>().where("Active = ?").bind(true);

    m_mdlCmbGroups->setQuery(query);

    m_mdlCmbGroups->reload();

    m_mdlCmbGroups->addColumn("Name", Wt::ItemIsSelectable);

    m_cmbGroups->setModel(m_mdlCmbGroups);

    if(m_mdlCmbGroups->rowCount() > 0)
        m_cmbGroups->setCurrentIndex(0);
}

void Views::DlgCreateUser::createCmbTitles()
{
    m_cmbTitles = new Wt::WComboBox();
    m_cmbTitles->setMinimumSize(20, 30);
    m_cntCmbTitles = new Wt::WContainerWidget();
    m_cntCmbTitles->addWidget(m_cmbTitles);

    m_mdlCmbTitles = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::UserTitle>>();

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::UserTitle>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Users::UserTitle>();
    else
        query = Session::SessionManager::instance().dboSession().find<Users::UserTitle>().where("Active = ?").bind(true);

    m_mdlCmbTitles->setQuery(query);

    m_mdlCmbTitles->reload();

    m_mdlCmbTitles->addColumn("Name", Wt::ItemIsSelectable);

    m_cmbTitles->setModel(m_mdlCmbTitles);

    if(m_mdlCmbTitles->rowCount() > 0)
        m_cmbTitles->setCurrentIndex(0);
}

bool Views::DlgCreateUser::validate()
{
    if((m_txtUserName->validate() != Wt::WValidator::Valid) ||
            (m_txtPassword->validate() != Wt::WValidator::Valid) ||
            (m_txtIdNumber->validate() != Wt::WValidator::Valid) ||
            (m_txtAddress->validate() != Wt::WValidator::Valid) ||
            (m_cmbGroups->currentIndex() == -1))
        return false;

    return true;
}

void Views::DlgCreateUser::btnOkClicked()
{
    if(validate())
        this->accept();
}
