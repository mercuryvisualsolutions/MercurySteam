#include "viewtags.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewTags::ViewTags()
{
    prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Database::Tag> *Views::ViewTags::qtvTags()
{
    return m_qtvTags;
}

Ms::Widgets::MQueryTableViewWidget<Database::Tag> *Views::ViewTags::qtvAssignedTags()
{
    return m_qtvAssignedTags;
}

bool Views::ViewTags::isCreateOptionHidden() const
{
    if(m_btnCreateTag)
        return m_btnCreateTag->isHidden();

    return false;
}

void Views::ViewTags::setCreateOptionHidden(bool hidden)
{
    if(m_btnCreateTag)
        m_btnCreateTag->setHidden(hidden);
}

void Views::ViewTags::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasCreateDboPriv = user->hasPrivilege("Create DBO");
    bool hasEditPriv = user->hasPrivilege("Edit");

    m_btnCreateTag->setHidden(!hasCreateDboPriv);
    m_btnAssignTags->setHidden(!hasEditPriv);
    m_btnUnassignTags->setHidden(!hasEditPriv);
}

Wt::Signal<> &Views::ViewTags::createTagRequested()
{
    return m_createTagRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &Views::ViewTags::assignTagsRequested()
{
    return m_assignTagsRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &Views::ViewTags::unassignTagsRequested()
{
    return m_unassignTagsRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag> >, bool, bool> &Views::ViewTags::filterByTagsRequested()
{
    return m_filterByTagsRequested;
}

Wt::Signal<> &Views::ViewTags::clearTagsFilterRequested()
{
    return m_clearTagsFilterRequested;
}

void Views::ViewTags::btnCreateTagClicked()
{
    createTagRequested();
}

void Views::ViewTags::btnAssignTagsClicked()
{
    m_assignTagsRequested(m_qtvTags->selectedItems());
}

void Views::ViewTags::btnUnassignTagsClicked()
{
    m_unassignTagsRequested(m_qtvAssignedTags->selectedItems());
}

void Views::ViewTags::btnClearTagsFilterClicked()
{
    clearTagsFilterRequested();
}

void Views::ViewTags::mnuFilterByTagsExactSelectionItemTriggered()
{
    m_filterByTagsRequested(m_qtvTags->selectedItems(), true, false);
}

void Views::ViewTags::mnuFilterByTagsAnyOfSelectionItemTriggered()
{
    m_filterByTagsRequested(m_qtvTags->selectedItems(), false, false);
}

void Views::ViewTags::mnuFilterByTagsNotInSelectionItemTriggered()
{
    m_filterByTagsRequested(m_qtvTags->selectedItems(), false, true);
}

void Views::ViewTags::createTagsTableView()
{
    m_qtvTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(Session::SessionManager::instance().dboSession());
    m_qtvTags->setImportCSVFeatureEnabled(false);

    m_btnCreateTag = m_qtvTags->createToolButton("", "icons/Add.png", "Create A Custom Tag");
    m_btnCreateTag->clicked().connect(this, &Views::ViewTags::btnCreateTagClicked);

    m_btnAssignTags = m_qtvTags->createToolButton("", "icons/AddTo.png", "Add selected tags to selected items");
    m_btnAssignTags->clicked().connect(this, &Views::ViewTags::btnAssignTagsClicked);

    m_btnFilterByTags = m_qtvTags->createToolButton("", "icons/Filter.png", "Filter active view by selected tags");

    m_mnuFilterByTags = new Wt::WPopupMenu();

    m_mnuFilterByTagsExactSelectionItem = new Wt::WPopupMenuItem("Exact Selection");
    m_mnuFilterByTagsExactSelectionItem->triggered().connect(this, &Views::ViewTags::mnuFilterByTagsExactSelectionItemTriggered);
    m_mnuFilterByTags->addItem(m_mnuFilterByTagsExactSelectionItem);

    m_mnuFilterByTagsAnyOfSelectionItem = new Wt::WPopupMenuItem("Any Of Selection");
    m_mnuFilterByTagsAnyOfSelectionItem->triggered().connect(this, &Views::ViewTags::mnuFilterByTagsAnyOfSelectionItemTriggered);
    m_mnuFilterByTags->addItem(m_mnuFilterByTagsAnyOfSelectionItem);

    m_mnuFilterByTagsNotInSelectionItem = new Wt::WPopupMenuItem("Not In Selection");
    m_mnuFilterByTagsNotInSelectionItem->triggered().connect(this, &Views::ViewTags::mnuFilterByTagsNotInSelectionItemTriggered);
    m_mnuFilterByTags->addItem(m_mnuFilterByTagsNotInSelectionItem);

    m_btnFilterByTags->setMenu(m_mnuFilterByTags);

    m_btnClearTagsFilter = m_qtvTags->createToolButton("", "icons/ClearFilter.png", "Clear tags filter on the active view");
    m_btnClearTagsFilter->clicked().connect(this, &Views::ViewTags::btnClearTagsFilterClicked);
}

void Views::ViewTags::createAssignedTagsTableView()
{
    m_qtvAssignedTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(Session::SessionManager::instance().dboSession());
    m_qtvAssignedTags->setImportCSVFeatureEnabled(false);

    m_btnUnassignTags = m_qtvAssignedTags->createToolButton("", "icons/RemoveFrom.png", "Remove selected tags from selected items");
    m_btnUnassignTags->clicked().connect(this, &Views::ViewTags::btnUnassignTagsClicked);
}

void Views::ViewTags::prepareView()
{
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    //Tags/AssignedTags Table Views
    m_cntTags = new Wt::WContainerWidget();
    m_layMain->addWidget(m_cntTags);

    m_layCntTags = new Wt::WVBoxLayout();
    m_layCntTags->setContentsMargins(0,0,0,0);
    m_layCntTags->setSpacing(6);

    m_cntTags->setLayout(m_layCntTags);

    m_cntAssignedTags = new Ms::Widgets::MContainerWidget();
    m_cntAssignedTags->setTitle("<b><i>Assigned Tags</i></b>");

    m_layCntTags->addWidget(m_cntAssignedTags);

    Wt::WVBoxLayout *_layCntAssignedTags = dynamic_cast<Wt::WVBoxLayout*>(m_cntAssignedTags->layout());

    createAssignedTagsTableView();
    _layCntAssignedTags->addWidget(m_qtvAssignedTags, 1);

    //Available Tags
    m_cntAvailableTags = new Ms::Widgets::MContainerWidget();
    m_cntAvailableTags->setTitle("<b><i>Available Tags</i></b>");

    m_layCntTags->addWidget(m_cntAvailableTags);

     Wt::WVBoxLayout *_layCntAvailableTags = dynamic_cast<Wt::WVBoxLayout*>(m_cntAvailableTags->layout());

    //Tags Table View
    createTagsTableView();
    _layCntAvailableTags->addWidget(m_qtvTags, 1);

    m_layCntTags->setResizable(0, true);
}
