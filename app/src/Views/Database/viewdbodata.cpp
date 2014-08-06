#include "viewdbodata.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewDboData::ViewDboData()
{
    _prepareView();
}

const Ms::Widgets::MQueryTableViewWidget<Database::DboData> *Views::ViewDboData::qtvData() const
{
    return _qtvData;
}

Wt::Signal<> &Views::ViewDboData::addDataRequested()
{
    return _addDataRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::DboData>>> &Views::ViewDboData::removeDataRequested()
{
    return _removeDataRequested;
}

void Views::ViewDboData::_btnAddDataClicked()
{
    _addDataRequested();
}

void Views::ViewDboData::_btnRemoveDataClicked()
{
    _removeDataRequested(_qtvData->selectedItems());
}

void Views::ViewDboData::_createDataTableView()
{
    _qtvData = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::DboData>(&Database::DatabaseManager::instance());
    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvData->createToolButton("", "icons/Add.png", "Add A New Field");
        btn->clicked().connect(this, &Views::ViewDboData::_btnAddDataClicked);
    }
    else
        _qtvData->setImportCSVFeatureEnabled(false);

    //requires "remove" privilege
//    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
//    {
//        Wt::WPushButton *btn = _qtvData->createToolButton("", "icons/Remove.png", "Remove Selected Fields");
//        btn->clicked().connect(this, &Views::ViewDboData::_btnRemoveDataClicked);
//    }
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
