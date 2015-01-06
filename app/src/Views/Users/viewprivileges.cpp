#include "viewprivileges.h"

#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewPrivileges::ViewPrivileges()
{
    prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *Views::ViewPrivileges::qtvPrivileges()
{
    return m_qtvPrivileges;
}

Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *Views::ViewPrivileges::qtvAssignedPrivileges()
{
    return m_qtvAssignedPrivileges;
}

void Views::ViewPrivileges::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasEditPriv = user->hasPrivilege("Edit");

    m_btnAssignPrivileges->setHidden(!hasEditPriv);
    m_btnUnassignPrivileges->setHidden(!hasEditPriv);
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &Views::ViewPrivileges::assignPrivilegesRequested()
{
    return m_assignPrivilegesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &Views::ViewPrivileges::unassignPrivilegesRequested()
{
    return m_unassignPrivilegesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege> >, bool, bool> &Views::ViewPrivileges::filterByPrivilegesRequested()
{
    return m_filterByPrivilegesRequested;
}

Wt::Signal<> &Views::ViewPrivileges::clearPrivilegesFilterRequested()
{
    return m_clearPrivilegesFilterRequested;
}

void Views::ViewPrivileges::btnAssignPrivilegesClicked()
{
    m_assignPrivilegesRequested(m_qtvPrivileges->selectedItems());
}

void Views::ViewPrivileges::btnUnassignPrivilegesClicked()
{
    m_unassignPrivilegesRequested(m_qtvAssignedPrivileges->selectedItems());
}

void Views::ViewPrivileges::btnClearPrivilegesFilterClicked()
{
    m_clearPrivilegesFilterRequested();
}

void Views::ViewPrivileges::mnuFilterByPrivilegesExactSelectionItemTriggered()
{
    m_filterByPrivilegesRequested(m_qtvPrivileges->selectedItems(), true, false);
}

void Views::ViewPrivileges::mnuFilterByPrivilegesAnyOfSelectionItemTriggered()
{
    m_filterByPrivilegesRequested(m_qtvPrivileges->selectedItems(), false, false);
}

void Views::ViewPrivileges::mnuFilterByPrivilegesNotInSelectionItemTriggered()
{
    m_filterByPrivilegesRequested(m_qtvPrivileges->selectedItems(), false, true);
}

void Views::ViewPrivileges::createPrivilegesTableView()
{
    m_qtvPrivileges = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::Privilege>(Session::SessionManager::instance().dboSession());
    m_qtvPrivileges->setImportCSVFeatureEnabled(false);

    m_btnAssignPrivileges = m_qtvPrivileges->createToolButton("", "icons/AddTo.png", "Add selected Privileges to selected items");
    m_btnAssignPrivileges->clicked().connect(this, &Views::ViewPrivileges::btnAssignPrivilegesClicked);

    m_btnFilterByPrivileges = m_qtvPrivileges->createToolButton("", "icons/Filter.png", "Filter active view by selected Privileges");

    m_mnuFilterByPrivileges = new Wt::WPopupMenu();

    m_mnuFilterByPrivilegesExactSelectionItem = new Wt::WPopupMenuItem("Exact Selection");
    m_mnuFilterByPrivilegesExactSelectionItem->triggered().connect(this, &Views::ViewPrivileges::mnuFilterByPrivilegesExactSelectionItemTriggered);
    m_mnuFilterByPrivileges->addItem(m_mnuFilterByPrivilegesExactSelectionItem);

    m_mnuFilterByPrivilegesAnyOfSelectionItem = new Wt::WPopupMenuItem("Any Of Selection");
    m_mnuFilterByPrivilegesAnyOfSelectionItem->triggered().connect(this, &Views::ViewPrivileges::mnuFilterByPrivilegesAnyOfSelectionItemTriggered);
    m_mnuFilterByPrivileges->addItem(m_mnuFilterByPrivilegesAnyOfSelectionItem);

    m_mnuFilterByPrivilegesNotInSelectionItem = new Wt::WPopupMenuItem("Not In Selection");
    m_mnuFilterByPrivilegesNotInSelectionItem->triggered().connect(this, &Views::ViewPrivileges::mnuFilterByPrivilegesNotInSelectionItemTriggered);
    m_mnuFilterByPrivileges->addItem(m_mnuFilterByPrivilegesNotInSelectionItem);

    m_btnFilterByPrivileges->setMenu(m_mnuFilterByPrivileges);

    m_btnClearPrivilegeFilter = m_qtvPrivileges->createToolButton("", "icons/ClearFilter.png", "Clear Privileges filter on the active view");
    m_btnClearPrivilegeFilter->clicked().connect(this, &Views::ViewPrivileges::btnClearPrivilegesFilterClicked);
}

void Views::ViewPrivileges::createAssignedPrivilegesTableView()
{
    m_qtvAssignedPrivileges = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::Privilege>(Session::SessionManager::instance().dboSession());
    m_qtvAssignedPrivileges->setImportCSVFeatureEnabled(false);

    m_btnUnassignPrivileges = m_qtvAssignedPrivileges->createToolButton("", "icons/RemoveFrom.png", "Remove selected Privileges from selected items");
    m_btnUnassignPrivileges->clicked().connect(this, &Views::ViewPrivileges::btnUnassignPrivilegesClicked);
}

void Views::ViewPrivileges::prepareView()
{
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    //Privileges/AssignedPrivileges Table Views
    m_cntPrivileges = new Wt::WContainerWidget();
    m_layMain->addWidget(m_cntPrivileges);

    m_layCntPrivileges = new Wt::WVBoxLayout();
    m_layCntPrivileges->setContentsMargins(0,0,0,0);
    m_layCntPrivileges->setSpacing(6);

    m_cntPrivileges->setLayout(m_layCntPrivileges);

    m_cntAssignedPrivileges = new Ms::Widgets::MContainerWidget();
    m_cntAssignedPrivileges->setTitle("<b><i>Assigned Privileges</i></b>");

    m_layCntPrivileges->addWidget(m_cntAssignedPrivileges);

    Wt::WVBoxLayout *_layCntAssignedPrivileges = dynamic_cast<Wt::WVBoxLayout*>(m_cntAssignedPrivileges->layout());

    createAssignedPrivilegesTableView();
    _layCntAssignedPrivileges->addWidget(m_qtvAssignedPrivileges, 1);

    //Available Privileges
    m_cntAvailablePrivileges = new Ms::Widgets::MContainerWidget();
    m_cntAvailablePrivileges->setTitle("<b><i>Available Privileges</i></b>");

    m_layCntPrivileges->addWidget(m_cntAvailablePrivileges);

     Wt::WVBoxLayout *_layCntAvailablePrivileges = dynamic_cast<Wt::WVBoxLayout*>(m_cntAvailablePrivileges->layout());

    //Privileges Table View
    createPrivilegesTableView();
    _layCntAvailablePrivileges->addWidget(m_qtvPrivileges, 1);

    m_layCntPrivileges->setResizable(0, true);
}
