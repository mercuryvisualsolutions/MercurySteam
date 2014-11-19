#include "viewdbodata.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewDboData::ViewDboData()
{
    _prepareView();

    adjustUIPrivileges();
}

Ms::Widgets::MQueryTableViewWidget<Database::DboData> *Views::ViewDboData::qtvData()
{
    return _qtvData;
}

void Views::ViewDboData::adjustUIPrivileges()
{
    Wt::Dbo::ptr<Users::User> user = Session::SessionManager::instance().user();

    bool hasCreateDboPriv = user->hasPrivilege("Create DBO");

    _btnCreateData->setHidden(!hasCreateDboPriv);

    _qtvData->setImportCSVFeatureEnabled(hasCreateDboPriv);
}

Wt::Signal<> &Views::ViewDboData::addDataRequested()
{
    return _addDataRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::DboData>>> &Views::ViewDboData::removeDataRequested()
{
    return _removeDataRequested;
}

void Views::ViewDboData::_btnCreateDataClicked()
{
    _addDataRequested();
}

void Views::ViewDboData::_btnRemoveDataClicked()
{
    _removeDataRequested(_qtvData->selectedItems());
}

void Views::ViewDboData::_createDataTableView()
{
    _qtvData = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::DboData>(Session::SessionManager::instance().dboSession());

    _btnCreateData = _qtvData->createToolButton("", "icons/Add.png", "Add A New Field");
    _btnCreateData->clicked().connect(this, &Views::ViewDboData::_btnCreateDataClicked);

//    Wt::WPushButton *btn = _qtvData->createToolButton("", "icons/Remove.png", "Remove Selected Fields");
//    btn->clicked().connect(this, &Views::ViewDboData::_btnRemoveDataClicked);
}

void Views::ViewDboData::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _createDataTableView();

    _layMain->addWidget(_qtvData);
}
