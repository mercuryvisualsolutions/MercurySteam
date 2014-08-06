#include "viewprivileges.h"

#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewPrivileges::ViewPrivileges()
{
    _prepareView();
}

const Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *Views::ViewPrivileges::qtvPrivileges() const
{
    return _qtvPrivileges;
}

const Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *Views::ViewPrivileges::qtvAssignedPrivileges() const
{
    return _qtvAssignedPrivileges;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &Views::ViewPrivileges::assignPrivilegesRequested()
{
    return _assignPrivilegesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &Views::ViewPrivileges::unassignPrivilegesRequested()
{
    return _unassignPrivilegesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &Views::ViewPrivileges::filterByPrivilegesRequested()
{
    return _filterByPrivilegesRequested;
}

Wt::Signal<> &Views::ViewPrivileges::clearPrivilegesFilterRequested()
{
    return _clearPrivilegesFilterRequested;
}

void Views::ViewPrivileges::_btnAssignPrivilegesClicked()
{
    _assignPrivilegesRequested(_qtvPrivileges->selectedItems());
}

void Views::ViewPrivileges::_btnUnassignPrivilegesClicked()
{
    _unassignPrivilegesRequested(_qtvAssignedPrivileges->selectedItems());
}

void Views::ViewPrivileges::_btnPrivilegesFilterClicked()
{
    _filterByPrivilegesRequested(_qtvPrivileges->selectedItems());
}

void Views::ViewPrivileges::_btnClearPrivilegesFilterClicked()
{
    _clearPrivilegesFilterRequested();
}

void Views::ViewPrivileges::_createPrivilegesTableView()
{
    _qtvPrivileges = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::Privilege>(&Database::DatabaseManager::instance());
    _qtvPrivileges->setImportCSVFeatureEnabled(false);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btnAssign = _qtvPrivileges->createToolButton("", "icons/AddTo.png", "Add selected Privileges to selected items");
        btnAssign->clicked().connect(this, &Views::ViewPrivileges::_btnAssignPrivilegesClicked);
    }

    Wt::WPushButton *btnFilter = _qtvPrivileges->createToolButton("", "icons/Filter.png", "Filter active view by selected Privileges");
    btnFilter->clicked().connect(this, &Views::ViewPrivileges::_btnPrivilegesFilterClicked);

    Wt::WPushButton *btnClearFilter = _qtvPrivileges->createToolButton("", "icons/ClearFilter.png", "Clear Privileges filter on the active view");
    btnClearFilter->clicked().connect(this, &Views::ViewPrivileges::_btnClearPrivilegesFilterClicked);
}

void Views::ViewPrivileges::_createAssignedPrivilegesTableView()
{
    _qtvAssignedPrivileges = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::Privilege>(&Database::DatabaseManager::instance());
    _qtvAssignedPrivileges->setImportCSVFeatureEnabled(false);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvAssignedPrivileges->createToolButton("", "icons/RemoveFrom.png", "Remove selected Privileges from selected items");
        btn->clicked().connect(this, &Views::ViewPrivileges::_btnUnassignPrivilegesClicked);
    }
}

void Views::ViewPrivileges::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    //Privileges/AssignedPrivileges Table Views
    _cntPrivileges = new Wt::WContainerWidget();
    _layMain->addWidget(_cntPrivileges);

    _layCntPrivileges = new Wt::WVBoxLayout();
    _layCntPrivileges->setContentsMargins(0,0,0,0);
    _layCntPrivileges->setSpacing(0);

    _cntPrivileges->setLayout(_layCntPrivileges);

    _cntAssignedPrivileges = new Ms::Widgets::MContainerWidget();
    _cntAssignedPrivileges->setTitle("<b><i>Assigned Privileges</i></b>");

    _layCntPrivileges->addWidget(_cntAssignedPrivileges);

    Wt::WVBoxLayout *_layCntAssignedPrivileges = dynamic_cast<Wt::WVBoxLayout*>(_cntAssignedPrivileges->layout());
    _layCntAssignedPrivileges->setContentsMargins(0,14,0,0);

    _createAssignedPrivilegesTableView();
    _layCntAssignedPrivileges->addWidget(_qtvAssignedPrivileges, 1);

    //Available Privileges
    _cntAvailablePrivileges = new Ms::Widgets::MContainerWidget();
    _cntAvailablePrivileges->setTitle("<b><i>Available Privileges</i></b>");

    _layCntPrivileges->addWidget(_cntAvailablePrivileges);

     Wt::WVBoxLayout *_layCntAvailablePrivileges = dynamic_cast<Wt::WVBoxLayout*>(_cntAvailablePrivileges->layout());
    _layCntAvailablePrivileges->setContentsMargins(0,14,0,0);

    //Privileges Table View
    _createPrivilegesTableView();
    _layCntAvailablePrivileges->addWidget(_qtvPrivileges, 1);
}
