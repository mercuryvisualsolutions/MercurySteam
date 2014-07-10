#include "viewproperties.h"

#include "../../Database/databasemanager.h"
#include "../../Users/usersmanager.h"
#include "../../Settings/appsettings.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewProperties::ViewProperties()
{
    _prepareView();

    _mnuPropertiesNavBar->select(_mnuPropertiesNavBarDataItem);
}

const Ms::Widgets::MQueryTableViewWidget<Database::DboData> *Views::ViewProperties::dataQueryTableView() const
{
    return _qtvData;
}

const Ms::Widgets::MQueryTableViewWidget<Database::Tag> *Views::ViewProperties::tagsQueryTableView() const
{
    return _qtvTags;
}

const Ms::Widgets::MQueryTableViewWidget<Database::Tag> *Views::ViewProperties::assignedTagsQueryTableView() const
{
    return _qtvAssignedTags;
}

const Ms::Widgets::MQueryTableViewWidget<Database::Note> *Views::ViewProperties::notesQueryTableView() const
{
    return _qtvNotes;
}

const Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *Views::ViewProperties::groupsPrivilegesQueryTableView() const
{
    return _qtvGroupsPrivileges;
}

const Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *Views::ViewProperties::groupsAssignedPrivilegesQueryTableView() const
{
    return _qtvGroupsAssignedPrivileges;
}

void Views::ViewProperties::updateTagsView() const
{
    if(_cntAvailableTags->isHidden())
        return;

    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query;

        int viewRank = Users::UsersManager::instance().getCurrentUserRank(Database::RankFlag::ViewRank);

        if(AppSettings::instance().isLoadInactiveData())
            query = Database::DatabaseManager::instance().session()->find<Database::Tag>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Database::DatabaseManager::instance().session()->find<Database::Tag>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        _qtvTags->setQuery(query);

        _qtvTags->clearColumns();

        //add columns
        _qtvTags->addColumn(Ms::Widgets::MTableViewColumn("Tag_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), true));
        _qtvTags->addColumn(Ms::Widgets::MTableViewColumn("Tag_Content", "Content", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), true));

        if(AppSettings::instance().isShowExtraColumns())
            _addExtraColumns<Database::Tag>(_qtvTags, Wt::ItemIsSelectable, 0);

        _qtvTags->removeColumn(Ms::Widgets::MTableViewColumn("Active"));

        _qtvTags->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating tags table view" << std::endl;
    }
}

void Views::ViewProperties::updateGroupsPrivilegesView() const
{
    if(_cntGroupsAvailablePrivileges->isHidden())
        return;

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::Privilege>> query;

    int viewRank = Users::UsersManager::instance().getCurrentUserRank(Database::RankFlag::ViewRank);

    if(AppSettings::instance().isLoadInactiveData())
        query = Database::DatabaseManager::instance().session()->find<Users::Privilege>().where("View_Rank <= ?").bind(viewRank);
    else
        query = Database::DatabaseManager::instance().session()->find<Users::Privilege>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

    _qtvGroupsPrivileges->setQuery(query);

    _qtvGroupsPrivileges->clearColumns();

    //add columns
    _qtvGroupsPrivileges->addColumn(Ms::Widgets::MTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), true));

    if(AppSettings::instance().isShowExtraColumns())
        _addExtraColumns<Users::Privilege>(_qtvGroupsPrivileges, Wt::ItemIsSelectable, 0);

    _qtvGroupsPrivileges->removeColumn(Ms::Widgets::MTableViewColumn("Active"));

    _qtvGroupsPrivileges->updateView();
}

void Views::ViewProperties::updateGroupsAssignedPrivilegesView(std::vector<Wt::Dbo::ptr<Users::Group>> groupVec) const
{
    if(_cntGroupsAssignedPrivileges->isHidden())
        return;

    if(groupVec.size() == 0)
        return;

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    std::string groupsSelectSql = "gp.group_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Users::Group>(groupVec).at(0) + ")";

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::Privilege>> query = Database::DatabaseManager::instance().session()->find<Users::Privilege>().where(
                "Name IN (SELECT gp.privilege_Name FROM rel_group_privileges gp WHERE " + groupsSelectSql + ")");

    if(!AppSettings::instance().isLoadInactiveData())
        query.where("Active = ?").bind(true);

    _qtvGroupsAssignedPrivileges->setQuery(query);

    _qtvGroupsAssignedPrivileges->clearColumns();

    //add columns
    _qtvGroupsAssignedPrivileges->addColumn(Ms::Widgets::MTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), true));

    if(AppSettings::instance().isShowExtraColumns())
        _addExtraColumns<Users::Privilege>(_qtvGroupsAssignedPrivileges, Wt::ItemIsSelectable, 0);

    _qtvGroupsAssignedPrivileges->removeColumn(Ms::Widgets::MTableViewColumn("Active"));

    _qtvGroupsAssignedPrivileges->updateView();
}

Wt::Signal<> &Views::ViewProperties::onAddData()
{
    return _onAddData;
}

Wt::Signal<> &Views::ViewProperties::onRemoveData()
{
    return _onRemoveData;
}

Wt::Signal<> &Views::ViewProperties::onAddTag()
{
    return _onAddTag;
}

Wt::Signal<> &Views::ViewProperties::onRemoveTag()
{
    return _onRemoveTag;
}

Wt::Signal<> &Views::ViewProperties::onAddNote()
{
    return _onAddNote;
}

Wt::Signal<> &Views::ViewProperties::onRemoveNote()
{
    return _onRemoveNote;
}

Wt::Signal<> &Views::ViewProperties::onAddPrivilegeToGroup()
{
    return _onAddPrivilegeToGroup;
}

Wt::Signal<> &Views::ViewProperties::onRemovePrivilegeFromGroup()
{
    return _onRemovePrivilegeFromGroup;
}

Wt::Signal<> &Views::ViewProperties::onTabDataSelected()
{
    return _onTabDataSelected;
}

Wt::Signal<> &Views::ViewProperties::onTabTagsSelected()
{
    return _onTabTagsSelected;
}

Wt::Signal<> &Views::ViewProperties::onTabNotesSelected()
{
    return _onTabNotesSelected;
}

Wt::Signal<> &Views::ViewProperties::onTabGroupsPrivilegesSelected()
{
    return _onTabGroupsPrivilegesSelected;
}

void Views::ViewProperties::setSubViewHidden(const std::string &viewName, bool hidden)
{
    if(viewName == "Data")
    {
        _mnuPropertiesNavBarDataItem->setHidden(hidden);

        if(hidden)
        {
            if(_stkProperties->currentWidget() == _qtvData)
                _stkProperties->setCurrentWidget(_cntNoPropertiesView);
        }
        else
        {
            if(_stkProperties->currentWidget() == _cntNoPropertiesView)
            {
                _stkProperties->setCurrentWidget(_qtvData);
                _mnuPropertiesNavBar->select(_mnuPropertiesNavBarDataItem);
            }
        }
    }
    else if(viewName == "Tags")
    {
        _mnuPropertiesNavBarTagsItem->setHidden(hidden);

        if(hidden)
        {
            if(_stkProperties->currentWidget() == _cntTags)
                _stkProperties->setCurrentWidget(_cntNoPropertiesView);
        }
        else
        {
            if(_stkProperties->currentWidget() == _cntNoPropertiesView)
            {
                _stkProperties->setCurrentWidget(_cntTags);
                _mnuPropertiesNavBar->select(_mnuPropertiesNavBarTagsItem);
            }
        }
    }
    else if(viewName == "Notes")
    {
        _mnuPropertiesNavBarNotesItem->setHidden(hidden);

        if(hidden)
        {
            if(_stkProperties->currentWidget() == _qtvNotes)
                _stkProperties->setCurrentWidget(_cntNoPropertiesView);
        }
        else
        {
            if(_stkProperties->currentWidget() == _cntNoPropertiesView)
            {
                _stkProperties->setCurrentWidget(_qtvNotes);
                _mnuPropertiesNavBar->select(_mnuPropertiesNavBarNotesItem);
            }
        }
    }
    else if(viewName == "GroupsPrivileges")
    {
        _mnuPropertiesNavBarGroupsPrivilegesItem->setHidden(hidden);

        if(hidden)
        {
            if(_stkProperties->currentWidget() == _cntGroupsPrivileges)
                _stkProperties->setCurrentWidget(_cntNoPropertiesView);
        }
        else
        {
            if(_stkProperties->currentWidget() == _cntNoPropertiesView)
            {
                _stkProperties->setCurrentWidget(_cntGroupsPrivileges);
                _mnuPropertiesNavBar->select(_mnuPropertiesNavBarGroupsPrivilegesItem);
            }
        }
    }
}

void Views::ViewProperties::_createDataTableView()
{
    _qtvData = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::DboData>(&Database::DatabaseManager::instance());
    //requires "create" privilege
    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Create))
    {
        Wt::WPushButton *btn = _qtvData->createToolButton("", "icons/Add.png", "Add A New Field");
        btn->clicked().connect(this, &Views::ViewProperties::_btnAddDataClicked);

        _qtvData->setImportOptionVisible(true);
    }
    else
        _qtvData->setImportOptionVisible(false);

    //requires "remove" privilege
//    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Remove))
//    {
//        Wt::WPushButton *btn = _qtvData->createToolButton("", "icons/Remove.png", "Remove Selected Fields");
//        btn->clicked().connect(this, &Views::ViewProperties::_btnRemoveDataClicked);
//    }

    updateDboDataView(std::vector<Wt::Dbo::ptr<Database::DboData>>());
}

void Views::ViewProperties::_createTagsTableView()
{
    _qtvTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(&Database::DatabaseManager::instance());
    _qtvTags->setImportOptionVisible(false);

    //requires "create" privilege
    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Create))
    {
        Wt::WPushButton *btn = _qtvTags->createToolButton("", "icons/AddTo.png", "Add selected tags to selected items");
        btn->clicked().connect(this, &Views::ViewProperties::_btnAddTagClicked);
    }

    updateTagsView();
}

void Views::ViewProperties::_createAssignedTagsTableView()
{
    _qtvAssignedTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(&Database::DatabaseManager::instance());
    _qtvAssignedTags->setImportOptionVisible(false);

    //requires "create" privilege
    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Edit))
    {
        Wt::WPushButton *btn = _qtvAssignedTags->createToolButton("", "icons/RemoveFrom.png", "Remove selected tags from selected items");
        btn->clicked().connect(this, &Views::ViewProperties::_btnRemoveTagClicked);
    }

    updateDboTagsView(std::vector<Wt::Dbo::ptr<Database::Tag>>());
}

void Views::ViewProperties::_createNotesTableView()
{
    _qtvNotes = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Note>(&Database::DatabaseManager::instance());

    //requires "create" privilege
    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Create))
    {
        Wt::WPushButton *btn = _qtvNotes->createToolButton("", "icons/Add.png", "Create A New Note");
        btn->clicked().connect(this, &Views::ViewProperties::_btnAddNoteClicked);

        _qtvNotes->setImportOptionVisible(true);
    }
    else
        _qtvNotes->setImportOptionVisible(false);

    //requires "remove" privilege
//    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Remove))
//    {
//        Wt::WPushButton *btn = _qtvNotes->createToolButton("", "icons/Remove.png", "Remove Selected Note");
//        btn->clicked().connect(this, &Views::ViewProperties::_btnRemoveNoteClicked);
//    }

    updateDboNotesView(std::vector<Wt::Dbo::ptr<Database::Note>>());
}

void Views::ViewProperties::_createGroupsPrivilegesTableView()
{
    _qtvGroupsPrivileges = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::Privilege>(&Database::DatabaseManager::instance());
    _qtvGroupsPrivileges->setImportOptionVisible(false);

    //requires "create" privilege
    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Create))
    {
        Wt::WPushButton *btn = _qtvGroupsPrivileges->createToolButton("", "icons/AddTo.png", "Add selected privileges to selected groups");
        btn->clicked().connect(this, &Views::ViewProperties::_btnAddGroupPrivilegeClicked);
    }

    updateGroupsPrivilegesView();
}

void Views::ViewProperties::_createGroupsAssignedPrivilegesTableView()
{
    _qtvGroupsAssignedPrivileges = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::Privilege>(&Database::DatabaseManager::instance());
    _qtvGroupsAssignedPrivileges->setImportOptionVisible(false);

    //requires "edit" privilege
    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Create))
    {
        Wt::WPushButton *btn = _qtvGroupsAssignedPrivileges->createToolButton("", "icons/RemoveFrom.png", "Remove selected privileges from selected groups");
        btn->clicked().connect(this, &Views::ViewProperties::_btnRemoveGroupPrivilegeClicked);
    }

    updateGroupsAssignedPrivilegesView(std::vector<Wt::Dbo::ptr<Users::Group>>());
}

void Views::ViewProperties::_mnuPropertiesNavBarDataItemTriggered()
{
    _stkProperties->setCurrentWidget(_qtvData);
    _onTabDataSelected.emit();
}

void Views::ViewProperties::_mnuPropertiesNavBarTagsItemTriggered()
{
    _stkProperties->setCurrentWidget(_cntTags);

    _onTabTagsSelected.emit();
}

void Views::ViewProperties::_mnuPropertiesNavBarNotesItemTriggered()
{
    _stkProperties->setCurrentWidget(_qtvNotes);
    _onTabNotesSelected.emit();
}

void Views::ViewProperties::_mnuPropertiesNavBarGroupsPrivilegesItemTriggered()
{
    _stkProperties->setCurrentWidget(_cntGroupsPrivileges);

    _onTabGroupsPrivilegesSelected.emit();
}

void Views::ViewProperties::_btnAddDataClicked()
{
    _onAddData.emit();
}

void Views::ViewProperties::_btnRemoveDataClicked()
{
    _onRemoveData.emit();
}

void Views::ViewProperties::_btnAddTagClicked()
{
    _onAddTag.emit();
}

void Views::ViewProperties::_btnRemoveTagClicked()
{
    _onRemoveTag.emit();
}

void Views::ViewProperties::_btnAddNoteClicked()
{
    _onAddNote.emit();
}

void Views::ViewProperties::_btnRemoveNoteClicked()
{
    _onRemoveNote.emit();
}

void Views::ViewProperties::_btnAddGroupPrivilegeClicked()
{
    _onAddPrivilegeToGroup.emit();
}

void Views::ViewProperties::_btnRemoveGroupPrivilegeClicked()
{
    _onRemovePrivilegeFromGroup.emit();
}

void Views::ViewProperties::_prepareView()
{
    setMaximumSize(520, Wt::WLength::Auto);
    setMinimumSize(520, Wt::WLength::Auto);

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    this->setLayout(_layMain);

    _cntTxtPropertiesTitle = new Wt::WContainerWidget();
    _cntTxtPropertiesTitle->setStyleClass("toolbar");
    _cntTxtPropertiesTitle->setContentAlignment(Wt::AlignCenter);
    _cntTxtPropertiesTitle->setMinimumSize(Wt::WLength::Auto, 25);

    _layMain->addWidget(_cntTxtPropertiesTitle);

    _txtPropertiesTitle = new Wt::WText("<b><i>Properties</i></b>");
    _txtPropertiesTitle->setStyleClass("soft-text");

    _cntTxtPropertiesTitle->addWidget(_txtPropertiesTitle);

    _navBarPropertiesMain = new Wt::WNavigationBar();

    _layMain->addWidget(_navBarPropertiesMain);

    _mnuPropertiesNavBar = new Wt::WMenu(Wt::Horizontal);

    _navBarPropertiesMain->addMenu(_mnuPropertiesNavBar);

    _mnuPropertiesNavBarDataItem = new Wt::WMenuItem("Data");
    _mnuPropertiesNavBarDataItem->triggered().connect(this, &Views::ViewProperties::_mnuPropertiesNavBarDataItemTriggered);
    _mnuPropertiesNavBar->addItem(_mnuPropertiesNavBarDataItem);

    _mnuPropertiesNavBarTagsItem = new Wt::WMenuItem("Tags");
    _mnuPropertiesNavBarTagsItem->triggered().connect(this, &Views::ViewProperties::_mnuPropertiesNavBarTagsItemTriggered);
    _mnuPropertiesNavBar->addItem(_mnuPropertiesNavBarTagsItem);

    _mnuPropertiesNavBarNotesItem = new Wt::WMenuItem("Notes");
    _mnuPropertiesNavBarNotesItem->triggered().connect(this, &Views::ViewProperties::_mnuPropertiesNavBarNotesItemTriggered);
    _mnuPropertiesNavBar->addItem(_mnuPropertiesNavBarNotesItem);

    _mnuPropertiesNavBarGroupsPrivilegesItem = new Wt::WMenuItem("Privileges");
    _mnuPropertiesNavBarGroupsPrivilegesItem->triggered().connect(this, &Views::ViewProperties::_mnuPropertiesNavBarGroupsPrivilegesItemTriggered);
    _mnuPropertiesNavBar->addItem(_mnuPropertiesNavBarGroupsPrivilegesItem);

    _stkProperties = new Wt::WStackedWidget();

    _layMain->addWidget(_stkProperties, 1);

    //Data Table View
    _createDataTableView();
    _stkProperties->addWidget(_qtvData);

    //Tags/AssignedTags Table Views
    _cntTags = new Wt::WContainerWidget();
    _stkProperties->addWidget(_cntTags);

    _layCntTags = new Wt::WVBoxLayout();
    _layCntTags->setContentsMargins(0,0,0,0);
    _layCntTags->setSpacing(0);

    _cntTags->setLayout(_layCntTags);

    _cntAssignedTags = new Wt::WContainerWidget();

    _layCntTags->addWidget(_cntAssignedTags);

    _layCntAssignedTags = new Wt::WVBoxLayout();
    _layCntAssignedTags->setContentsMargins(0,0,0,0);
    _layCntAssignedTags->setSpacing(0);

    _cntAssignedTags->setLayout(_layCntAssignedTags);

    _cntTxtAssignedTagsLabel = new Wt::WContainerWidget();
    _cntTxtAssignedTagsLabel->setStyleClass("toolbar");
    _cntTxtAssignedTagsLabel->setContentAlignment(Wt::AlignCenter);
    _cntTxtAssignedTagsLabel->setMinimumSize(Wt::WLength::Auto, 25);

    _layCntAssignedTags->addWidget(_cntTxtAssignedTagsLabel);

    _txtAssignedTagsLabel = new Wt::WText("<b><i>Assigned Tags</i></b>");
    _txtAssignedTagsLabel->setStyleClass("soft-text");
    _cntTxtAssignedTagsLabel->addWidget(_txtAssignedTagsLabel);

    _createAssignedTagsTableView();
    _layCntAssignedTags->addWidget(_qtvAssignedTags, 1);

    //Available Tags
    _cntAvailableTags = new Wt::WContainerWidget();

    _layCntTags->addWidget(_cntAvailableTags);

    _layCntAvailableTags = new Wt::WVBoxLayout();
    _layCntAvailableTags->setContentsMargins(0,0,0,0);
    _layCntAvailableTags->setSpacing(0);

    _cntAvailableTags->setLayout(_layCntAvailableTags);

    _cntTxtAvailableTagsLabel = new Wt::WContainerWidget();
    _cntTxtAvailableTagsLabel->setStyleClass("toolbar");
    _cntTxtAvailableTagsLabel->setContentAlignment(Wt::AlignCenter);
    _cntTxtAvailableTagsLabel->setMinimumSize(Wt::WLength::Auto, 25);

    _layCntAvailableTags->addWidget(_cntTxtAvailableTagsLabel);

    _txtAvailableTagsLabel = new Wt::WText("<b><i>Available Tags</i></b>");
    _txtAvailableTagsLabel->setStyleClass("soft-text");
    _cntTxtAvailableTagsLabel->addWidget(_txtAvailableTagsLabel);

    _createTagsTableView();
    _layCntAvailableTags->addWidget(_qtvTags, 1);

    //GroupsPrivileges/GroupsAssignedPrivileges Table Views
    _cntGroupsPrivileges = new Wt::WContainerWidget();
    _stkProperties->addWidget(_cntGroupsPrivileges);

    _layCntGroupsPrivileges = new Wt::WVBoxLayout();
    _layCntGroupsPrivileges->setContentsMargins(0,0,0,0);
    _layCntGroupsPrivileges->setSpacing(0);

    _cntGroupsPrivileges->setLayout(_layCntGroupsPrivileges);

    _cntGroupsAssignedPrivileges = new Wt::WContainerWidget();

    _layCntGroupsPrivileges->addWidget(_cntGroupsAssignedPrivileges);

    _layCntGroupsAssignedPrivileges = new Wt::WVBoxLayout();
    _layCntGroupsAssignedPrivileges->setContentsMargins(0,0,0,0);
    _layCntGroupsAssignedPrivileges->setSpacing(0);

    _cntGroupsAssignedPrivileges->setLayout(_layCntGroupsAssignedPrivileges);

    _cntTxtGroupsAssignedPrivilegesLabel = new Wt::WContainerWidget();
    _cntTxtGroupsAssignedPrivilegesLabel->setStyleClass("toolbar");
    _cntTxtGroupsAssignedPrivilegesLabel->setContentAlignment(Wt::AlignCenter);
    _cntTxtGroupsAssignedPrivilegesLabel->setMinimumSize(Wt::WLength::Auto, 25);

    _layCntGroupsAssignedPrivileges->addWidget(_cntTxtGroupsAssignedPrivilegesLabel);

    _txtGroupsAssignedPrivilegesLabel = new Wt::WText("<b><i>Assigned Privileges</i></b>");
    _txtGroupsAssignedPrivilegesLabel->setStyleClass("soft-text");
    _cntTxtGroupsAssignedPrivilegesLabel->addWidget(_txtGroupsAssignedPrivilegesLabel);

    _createGroupsAssignedPrivilegesTableView();
    _layCntGroupsAssignedPrivileges->addWidget(_qtvGroupsAssignedPrivileges, 1);

    //Available Groups Privileges
    _cntGroupsAvailablePrivileges = new Wt::WContainerWidget();

    _layCntGroupsPrivileges->addWidget(_cntGroupsAvailablePrivileges);

    _layCntGroupsAvailablePrivileges = new Wt::WVBoxLayout();
    _layCntGroupsAvailablePrivileges->setContentsMargins(0,0,0,0);
    _layCntGroupsAvailablePrivileges->setSpacing(0);

    _cntGroupsAvailablePrivileges->setLayout(_layCntGroupsAvailablePrivileges);

    _cntTxtGroupsAvailablePrivilegesLabel = new Wt::WContainerWidget();
    _cntTxtGroupsAvailablePrivilegesLabel->setStyleClass("toolbar");
    _cntTxtGroupsAvailablePrivilegesLabel->setContentAlignment(Wt::AlignCenter);
    _cntTxtGroupsAvailablePrivilegesLabel->setMinimumSize(Wt::WLength::Auto, 25);

    _layCntGroupsAvailablePrivileges->addWidget(_cntTxtGroupsAvailablePrivilegesLabel);

    _txtGroupsAvailablePrivilegesLabel = new Wt::WText("<b><i>Available Privileges</i></b>");
    _txtGroupsAvailablePrivilegesLabel->setStyleClass("soft-text");
    _cntTxtGroupsAvailablePrivilegesLabel->addWidget(_txtGroupsAvailablePrivilegesLabel);

    _createGroupsPrivilegesTableView();
    _layCntGroupsAvailablePrivileges->addWidget(_qtvGroupsPrivileges, 1);

    //Notes Table View
    _createNotesTableView();
    _stkProperties->addWidget(_qtvNotes);

    //No Properties view
    _cntNoPropertiesView = new Wt::WContainerWidget();
    _stkProperties->addWidget(_cntNoPropertiesView);
}
