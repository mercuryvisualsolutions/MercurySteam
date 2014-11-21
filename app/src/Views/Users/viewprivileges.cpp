#include "viewprivileges.h"

#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewPrivileges::ViewPrivileges()
{
    _prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *Views::ViewPrivileges::qtvPrivileges()
{
    return _qtvPrivileges;
}

Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *Views::ViewPrivileges::qtvAssignedPrivileges()
{
    return _qtvAssignedPrivileges;
}

void Views::ViewPrivileges::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasEditPriv = user->hasPrivilege("Edit");

    _btnAssignPrivileges->setHidden(!hasEditPriv);
    _btnUnassignPrivileges->setHidden(!hasEditPriv);
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &Views::ViewPrivileges::assignPrivilegesRequested()
{
    return _assignPrivilegesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &Views::ViewPrivileges::unassignPrivilegesRequested()
{
    return _unassignPrivilegesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege> >, bool, bool> &Views::ViewPrivileges::filterByPrivilegesRequested()
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

void Views::ViewPrivileges::_btnClearPrivilegesFilterClicked()
{
    _clearPrivilegesFilterRequested();
}

void Views::ViewPrivileges::_mnuFilterByPrivilegesExactSelectionItemTriggered()
{
    _filterByPrivilegesRequested(_qtvPrivileges->selectedItems(), true, false);
}

void Views::ViewPrivileges::_mnuFilterByPrivilegesAnyOfSelectionItemTriggered()
{
    _filterByPrivilegesRequested(_qtvPrivileges->selectedItems(), false, false);
}

void Views::ViewPrivileges::_mnuFilterByPrivilegesNotInSelectionItemTriggered()
{
    _filterByPrivilegesRequested(_qtvPrivileges->selectedItems(), false, true);
}

void Views::ViewPrivileges::_createPrivilegesTableView()
{
    _qtvPrivileges = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::Privilege>(Session::SessionManager::instance().dboSession());
    _qtvPrivileges->setImportCSVFeatureEnabled(false);

    _btnAssignPrivileges = _qtvPrivileges->createToolButton("", "icons/AddTo.png", "Add selected Privileges to selected items");
    _btnAssignPrivileges->clicked().connect(this, &Views::ViewPrivileges::_btnAssignPrivilegesClicked);

    _btnFilterByPrivileges = _qtvPrivileges->createToolButton("", "icons/Filter.png", "Filter active view by selected Privileges");

    _mnuFilterByPrivileges = new Wt::WPopupMenu();

    _mnuFilterByPrivilegesExactSelectionItem = new Wt::WPopupMenuItem("Exact Selection");
    _mnuFilterByPrivilegesExactSelectionItem->triggered().connect(this, &Views::ViewPrivileges::_mnuFilterByPrivilegesExactSelectionItemTriggered);
    _mnuFilterByPrivileges->addItem(_mnuFilterByPrivilegesExactSelectionItem);

    _mnuFilterByPrivilegesAnyOfSelectionItem = new Wt::WPopupMenuItem("Any Of Selection");
    _mnuFilterByPrivilegesAnyOfSelectionItem->triggered().connect(this, &Views::ViewPrivileges::_mnuFilterByPrivilegesAnyOfSelectionItemTriggered);
    _mnuFilterByPrivileges->addItem(_mnuFilterByPrivilegesAnyOfSelectionItem);

    _mnuFilterByPrivilegesNotInSelectionItem = new Wt::WPopupMenuItem("Not In Selection");
    _mnuFilterByPrivilegesNotInSelectionItem->triggered().connect(this, &Views::ViewPrivileges::_mnuFilterByPrivilegesNotInSelectionItemTriggered);
    _mnuFilterByPrivileges->addItem(_mnuFilterByPrivilegesNotInSelectionItem);

    _btnFilterByPrivileges->setMenu(_mnuFilterByPrivileges);

    _btnClearPrivilegeFilter = _qtvPrivileges->createToolButton("", "icons/ClearFilter.png", "Clear Privileges filter on the active view");
    _btnClearPrivilegeFilter->clicked().connect(this, &Views::ViewPrivileges::_btnClearPrivilegesFilterClicked);
}

void Views::ViewPrivileges::_createAssignedPrivilegesTableView()
{
    _qtvAssignedPrivileges = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::Privilege>(Session::SessionManager::instance().dboSession());
    _qtvAssignedPrivileges->setImportCSVFeatureEnabled(false);

    _btnUnassignPrivileges = _qtvAssignedPrivileges->createToolButton("", "icons/RemoveFrom.png", "Remove selected Privileges from selected items");
    _btnUnassignPrivileges->clicked().connect(this, &Views::ViewPrivileges::_btnUnassignPrivilegesClicked);
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
    _layCntPrivileges->setSpacing(6);

    _cntPrivileges->setLayout(_layCntPrivileges);

    _cntAssignedPrivileges = new Ms::Widgets::MContainerWidget();
    _cntAssignedPrivileges->setTitle("<b><i>Assigned Privileges</i></b>");

    _layCntPrivileges->addWidget(_cntAssignedPrivileges);

    Wt::WVBoxLayout *_layCntAssignedPrivileges = dynamic_cast<Wt::WVBoxLayout*>(_cntAssignedPrivileges->layout());

    _createAssignedPrivilegesTableView();
    _layCntAssignedPrivileges->addWidget(_qtvAssignedPrivileges, 1);

    //Available Privileges
    _cntAvailablePrivileges = new Ms::Widgets::MContainerWidget();
    _cntAvailablePrivileges->setTitle("<b><i>Available Privileges</i></b>");

    _layCntPrivileges->addWidget(_cntAvailablePrivileges);

     Wt::WVBoxLayout *_layCntAvailablePrivileges = dynamic_cast<Wt::WVBoxLayout*>(_cntAvailablePrivileges->layout());

    //Privileges Table View
    _createPrivilegesTableView();
    _layCntAvailablePrivileges->addWidget(_qtvPrivileges, 1);

    _layCntPrivileges->setResizable(0, true);
}
