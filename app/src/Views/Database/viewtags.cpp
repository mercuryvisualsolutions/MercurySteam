#include "viewtags.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewTags::ViewTags()
{
    _prepareView();
}

const Ms::Widgets::MQueryTableViewWidget<Database::Tag> *Views::ViewTags::qtvTags() const
{
    return _qtvTags;
}

const Ms::Widgets::MQueryTableViewWidget<Database::Tag> *Views::ViewTags::qtvAssignedTags() const
{
    return _qtvAssignedTags;
}

bool Views::ViewTags::isCreateOptionHidden() const
{
    return _btnCreateTag->isHidden();
}

void Views::ViewTags::setCreateOptionHidden(bool hidden)
{
    _btnCreateTag->setHidden(hidden);
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

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &Views::ViewTags::filterByTagsRequested()
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

void Views::ViewTags::_btnTagsFilterClicked()
{
    _filterByTagsRequested(_qtvTags->selectedItems());
}

void Views::ViewTags::_btnClearTagsFilterClicked()
{
    _clearTagsFilterRequested();
}

void Views::ViewTags::_createTagsTableView()
{
    _qtvTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(&Database::DatabaseManager::instance());
    _qtvTags->setImportCSVFeatureEnabled(false);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        _btnCreateTag = _qtvTags->createToolButton("", "icons/Add.png", "Create A Custom Tag");
        _btnCreateTag->clicked().connect(this, &Views::ViewTags::_btnCreateTagClicked);

        Wt::WPushButton *btnAssign = _qtvTags->createToolButton("", "icons/AddTo.png", "Add selected tags to selected items");
        btnAssign->clicked().connect(this, &Views::ViewTags::_btnAssignTagsClicked);
    }

    Wt::WPushButton *btnFilter = _qtvTags->createToolButton("", "icons/Filter.png", "Filter active view by selected tags");
    btnFilter->clicked().connect(this, &Views::ViewTags::_btnTagsFilterClicked);

    Wt::WPushButton *btnClearFilter = _qtvTags->createToolButton("", "icons/ClearFilter.png", "Clear tags filter on the active view");
    btnClearFilter->clicked().connect(this, &Views::ViewTags::_btnClearTagsFilterClicked);
}

void Views::ViewTags::_createAssignedTagsTableView()
{
    _qtvAssignedTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(&Database::DatabaseManager::instance());
    _qtvAssignedTags->setImportCSVFeatureEnabled(false);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvAssignedTags->createToolButton("", "icons/RemoveFrom.png", "Remove selected tags from selected items");
        btn->clicked().connect(this, &Views::ViewTags::_btnUnassignTagsClicked);
    }
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
    _layCntTags->setSpacing(0);

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
}
