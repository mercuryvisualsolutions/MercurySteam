#include "viewtags.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewTags::ViewTags()
{
    _prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Database::Tag> *Views::ViewTags::qtvTags()
{
    return _qtvTags;
}

Ms::Widgets::MQueryTableViewWidget<Database::Tag> *Views::ViewTags::qtvAssignedTags()
{
    return _qtvAssignedTags;
}

bool Views::ViewTags::isCreateOptionHidden() const
{
    if(_btnCreateTag)
        return _btnCreateTag->isHidden();

    return false;
}

void Views::ViewTags::setCreateOptionHidden(bool hidden)
{
    if(_btnCreateTag)
        _btnCreateTag->setHidden(hidden);
}

void Views::ViewTags::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasCreateDboPriv = user->hasPrivilege("Create DBO");
    bool hasEditPriv = user->hasPrivilege("Edit");

    _btnCreateTag->setHidden(!hasCreateDboPriv);
    _btnAssignTags->setHidden(!hasEditPriv);
    _btnUnassignTags->setHidden(!hasEditPriv);
}

Wt::Signal<> &Views::ViewTags::createTagRequested()
{
    return _createTagRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &Views::ViewTags::assignTagsRequested()
{
    return _assignTagsRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &Views::ViewTags::unassignTagsRequested()
{
    return _unassignTagsRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag> >, bool> &Views::ViewTags::filterByTagsRequested()
{
    return _filterByTagsRequested;
}

Wt::Signal<> &Views::ViewTags::clearTagsFilterRequested()
{
    return _clearTagsFilterRequested;
}

void Views::ViewTags::_btnCreateTagClicked()
{
    _createTagRequested();
}

void Views::ViewTags::_btnAssignTagsClicked()
{
    _assignTagsRequested(_qtvTags->selectedItems());
}

void Views::ViewTags::_btnUnassignTagsClicked()
{
    _unassignTagsRequested(_qtvAssignedTags->selectedItems());
}

void Views::ViewTags::_btnClearTagsFilterClicked()
{
    _clearTagsFilterRequested();
}

void Views::ViewTags::_mnuFilterByTagsExactSelectionItemTriggered()
{
    _filterByTagsRequested(_qtvTags->selectedItems(), true);
}

void Views::ViewTags::_mnuFilterByTagsAnyOfSelectionItemTriggered()
{
    _filterByTagsRequested(_qtvTags->selectedItems(), false);
}

void Views::ViewTags::_createTagsTableView()
{
    _qtvTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(Session::SessionManager::instance().dboSession());
    _qtvTags->setImportCSVFeatureEnabled(false);

    _btnCreateTag = _qtvTags->createToolButton("", "icons/Add.png", "Create A Custom Tag");
    _btnCreateTag->clicked().connect(this, &Views::ViewTags::_btnCreateTagClicked);

    _btnAssignTags = _qtvTags->createToolButton("", "icons/AddTo.png", "Add selected tags to selected items");
    _btnAssignTags->clicked().connect(this, &Views::ViewTags::_btnAssignTagsClicked);

    _btnFilterByTags = _qtvTags->createToolButton("", "icons/Filter.png", "Filter active view by selected tags");

    _mnuFilterByTags = new Wt::WPopupMenu();

    _mnuFilterByTagsExactSelectionItem = new Wt::WPopupMenuItem("Exact Selection");
    _mnuFilterByTagsExactSelectionItem->triggered().connect(this, &Views::ViewTags::_mnuFilterByTagsExactSelectionItemTriggered);
    _mnuFilterByTags->addItem(_mnuFilterByTagsExactSelectionItem);

    _mnuFilterByTagsAnyOfSelectionItem = new Wt::WPopupMenuItem("Any Of Selection");
    _mnuFilterByTagsAnyOfSelectionItem->triggered().connect(this, &Views::ViewTags::_mnuFilterByTagsAnyOfSelectionItemTriggered);
    _mnuFilterByTags->addItem(_mnuFilterByTagsAnyOfSelectionItem);

    _btnFilterByTags->setMenu(_mnuFilterByTags);

    _btnClearTagsFilter = _qtvTags->createToolButton("", "icons/ClearFilter.png", "Clear tags filter on the active view");
    _btnClearTagsFilter->clicked().connect(this, &Views::ViewTags::_btnClearTagsFilterClicked);
}

void Views::ViewTags::_createAssignedTagsTableView()
{
    _qtvAssignedTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(Session::SessionManager::instance().dboSession());
    _qtvAssignedTags->setImportCSVFeatureEnabled(false);

    _btnUnassignTags = _qtvAssignedTags->createToolButton("", "icons/RemoveFrom.png", "Remove selected tags from selected items");
    _btnUnassignTags->clicked().connect(this, &Views::ViewTags::_btnUnassignTagsClicked);
}

void Views::ViewTags::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    //Tags/AssignedTags Table Views
    _cntTags = new Wt::WContainerWidget();
    _layMain->addWidget(_cntTags);

    _layCntTags = new Wt::WVBoxLayout();
    _layCntTags->setContentsMargins(0,0,0,0);
    _layCntTags->setSpacing(6);

    _cntTags->setLayout(_layCntTags);

    _cntAssignedTags = new Ms::Widgets::MContainerWidget();
    _cntAssignedTags->setTitle("<b><i>Assigned Tags</i></b>");

    _layCntTags->addWidget(_cntAssignedTags);

    Wt::WVBoxLayout *_layCntAssignedTags = dynamic_cast<Wt::WVBoxLayout*>(_cntAssignedTags->layout());

    _createAssignedTagsTableView();
    _layCntAssignedTags->addWidget(_qtvAssignedTags, 1);

    //Available Tags
    _cntAvailableTags = new Ms::Widgets::MContainerWidget();
    _cntAvailableTags->setTitle("<b><i>Available Tags</i></b>");

    _layCntTags->addWidget(_cntAvailableTags);

     Wt::WVBoxLayout *_layCntAvailableTags = dynamic_cast<Wt::WVBoxLayout*>(_cntAvailableTags->layout());

    //Tags Table View
    _createTagsTableView();
    _layCntAvailableTags->addWidget(_qtvTags, 1);

    _layCntTags->setResizable(0, true);
}
