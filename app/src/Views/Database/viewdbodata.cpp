#include "viewdbodata.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewDboData::ViewDboData()
{
    prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Database::DboData> *Views::ViewDboData::qtvData()
{
    return m_qtvData;
}

void Views::ViewDboData::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasCreateDboPriv = user->hasPrivilege("Create DBO");

    m_btnCreateData->setHidden(!hasCreateDboPriv);

    m_qtvData->setImportCSVFeatureEnabled(hasCreateDboPriv);
}

Wt::Signal<> &Views::ViewDboData::addDataRequested()
{
    return m_addDataRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::DboData>>> &Views::ViewDboData::removeDataRequested()
{
    return m_removeDataRequested;
}

void Views::ViewDboData::btnCreateDataClicked()
{
    m_addDataRequested();
}

void Views::ViewDboData::btnRemoveDataClicked()
{
    m_removeDataRequested(m_qtvData->selectedItems());
}

void Views::ViewDboData::createDataTableView()
{
    m_qtvData = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::DboData>(Session::SessionManager::instance().dboSession());

    m_btnCreateData = m_qtvData->createToolButton("", "icons/Add.png", "Add A New Field");
    m_btnCreateData->clicked().connect(this, &Views::ViewDboData::btnCreateDataClicked);

//    Wt::WPushButton *btn = m_qtvData->createToolButton("", "icons/Remove.png", "Remove Selected Fields");
//    btn->clicked().connect(this, &Views::ViewDboData::btnRemoveDataClicked);
}

void Views::ViewDboData::prepareView()
{
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    createDataTableView();

    m_layMain->addWidget(m_qtvData);
}
