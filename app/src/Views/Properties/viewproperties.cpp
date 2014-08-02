#include "viewproperties.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewProperties::ViewProperties()
{
    _prepareView();
}

//Functions
void Views::ViewProperties::setViewHidden(const std::string &viewName, bool hidden) const
{
    bool currentView = false;

    if(viewName == "Data")
    {
        _mnuNavBarDataItem->setHidden(hidden);
        if(_stkMain->currentWidget() == _qtvData)
            currentView = true;
    }
    else if(viewName == "Tags")
    {
        _mnuNavBarTagsItem->setHidden(hidden);
        if(_stkMain->currentWidget() == _cntTags)
            currentView = true;
    }
    else if(viewName == "Notes")
    {
        _mnuNavBarNotesItem->setHidden(hidden);
        if(_stkMain->currentWidget() == _qtvNotes)
            currentView = true;
    }
    else if(viewName == "Groups Privileges")
    {
        _mnuNavBarGroupsPrivilegesItem->setHidden(hidden);
        if(_stkMain->currentWidget() == _cntGroupsPrivileges)
            currentView = true;
    }

    if(hidden)
    {
        if(currentView)
            _stkMain->setCurrentWidget(_cntNoProperties);
    }
}

void Views::ViewProperties::updateTagsView()
{
    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query;

    int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

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

void Views::ViewProperties::updateGroupsPrivilegesView()
{
    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::Privilege>> query;

    int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

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

const std::string Views::ViewProperties::currentView() const
{
    std::string currentView = "";

    if(_stkMain->currentWidget() == _qtvData)
        currentView = "Data";
    else if(_stkMain->currentWidget() == _cntTags)
        currentView = "Tags";
    else if(_stkMain->currentWidget() == _qtvNotes)
        currentView = "Notes";
    else if(_stkMain->currentWidget() == _cntGroupsPrivileges)
        currentView = "Groups Privileges";

    return currentView;
}

void Views::ViewProperties::setCurrentView(const std::string &viewName) const
{
    setViewHidden(viewName, false);

    if(viewName == "Data")
        _mnuNavBarDataItem->select();
    else if(viewName == "Tags")
        _mnuNavBarTagsItem->select();
    else if(viewName == "Notes")
        _mnuNavBarNotesItem->select();
    else if(viewName == "Groups Privileges")
        _mnuNavBarGroupsPrivilegesItem->select();
}

//UI
Ms::Widgets::MQueryTableViewWidget<Database::DboData> *Views::ViewProperties::qtvData()
{
    return _qtvData;
}

Ms::Widgets::MQueryTableViewWidget<Database::Tag> *Views::ViewProperties::qtvTags()
{
    return _qtvTags;
}

Ms::Widgets::MQueryTableViewWidget<Database::Tag> *Views::ViewProperties::qtvAssignedTags()
{
    return _qtvAssignedTags;
}

Ms::Widgets::MQueryTableViewWidget<Database::Note> *Views::ViewProperties::qtvNotes()
{
    return _qtvNotes;
}

Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *Views::ViewProperties::qtvGroupsPrivileges()
{
    return _qtvGroupsPrivileges;
}

Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *Views::ViewProperties::qtvGroupsAssignedPrivileges()
{
    return _qtvGroupsAssignedPrivileges;
}

//Signals
Wt::Signal<std::string> &Views::ViewProperties::viewExposed()
{
    return _viewExposed;
}

Wt::Signal<> &Views::ViewProperties::addDataRequested()
{
    return _addDataRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::DboData>>> &Views::ViewProperties::removeDataRequested()
{
    return _removeDataRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &Views::ViewProperties::addTagRequested()
{
    return _addTagRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &Views::ViewProperties::removeTagRequested()
{
    return _removeTagRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &Views::ViewProperties::filterByTagRequested()
{
    return _filterByTagRequested;
}

Wt::Signal<> &Views::ViewProperties::clearTagFilterRequested()
{
    return _clearTagFilterRequested;
}

Wt::Signal<> &Views::ViewProperties::addNoteRequested()
{
    return _addNoteRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Note>>> &Views::ViewProperties::removeNoteRequested()
{
    return _removeNoteRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &Views::ViewProperties::addGroupPrivilegeRequested()
{
    return _addGroupPrivilegeRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &Views::ViewProperties::removeGroupPrivilegeRequested()
{
    return _removeGroupPrivilegeRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &Views::ViewProperties::filterGroupByPrivilegeRequested()
{
    return _filterGroupByPrivilegeRequested;
}

Wt::Signal<> &Views::ViewProperties::clearGroupPrivilegeFilterRequested()
{
    return _clearGroupPrivilegeFilterRequested;
}

//Slots
void Views::ViewProperties::_mnuNavBarDataItemTriggered()
{
    _stkMain->setCurrentWidget(_qtvData);
    _viewExposed("Data");
}

void Views::ViewProperties::_mnuNavBarTagsItemTriggered()
{
    _stkMain->setCurrentWidget(_cntTags);
    _viewExposed("Tags");
}

void Views::ViewProperties::_mnuNavBarNotesItemTriggered()
{
    _stkMain->setCurrentWidget(_qtvNotes);
    _viewExposed("Notes");
}

void Views::ViewProperties::_mnuNavBarGroupsPrivilegesItemTriggered()
{
    _stkMain->setCurrentWidget(_cntGroupsPrivileges);
    _viewExposed("Groups Privileges");
}

void Views::ViewProperties::_btnAddDataClicked()
{
    _addDataRequested();
}

void Views::ViewProperties::_btnRemoveDataClicked()
{
    _removeDataRequested(_qtvData->selectedItems());
}

void Views::ViewProperties::_btnAddTagClicked()
{
    _addTagRequested(_qtvTags->selectedItems());
}

void Views::ViewProperties::_btnRemoveTagClicked()
{
    _removeTagRequested(_qtvAssignedTags->selectedItems());
}

void Views::ViewProperties::_btnFilterTagClicked()
{
    _filterByTagRequested(_qtvTags->selectedItems());
}

void Views::ViewProperties::_btnClearFilterTagClicked()
{
    _clearTagFilterRequested();
}

void Views::ViewProperties::_btnAddNoteClicked()
{
    _addNoteRequested();
}

void Views::ViewProperties::_btnRemoveNoteClicked()
{
    _removeNoteRequested(_qtvNotes->selectedItems());;
}

void Views::ViewProperties::_btnAddGroupsPrivilegesClicked()
{
    _addGroupPrivilegeRequested(_qtvGroupsPrivileges->selectedItems());
}

void Views::ViewProperties::_btnRemoveGroupsPrivilegesClicked()
{
    _removeGroupPrivilegeRequested(_qtvGroupsAssignedPrivileges->selectedItems());
}

void Views::ViewProperties::_btnFilterGroupsPrivilegesClicked()
{
    _filterGroupByPrivilegeRequested(_qtvGroupsPrivileges->selectedItems());;
}

void Views::ViewProperties::_btnClearFilterGroupsPrivilegesClicked()
{
    _clearGroupPrivilegeFilterRequested();
}

//Functions
void Views::ViewProperties::_createDataTableView()
{
    _qtvData = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::DboData>(&Database::DatabaseManager::instance());
    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvData->createToolButton("", "icons/Add.png", "Add A New Field");
        btn->clicked().connect(this, &Views::ViewProperties::_btnAddDataClicked);
    }
    else
        _qtvData->setImportCSVFetureEnabled(false);

    //requires "remove" privilege
//    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
//    {
//        Wt::WPushButton *btn = _qtvData->createToolButton("", "icons/Remove.png", "Remove Selected Fields");
//        btn->clicked().connect(this, &Views::ViewProperties::_btnRemoveDataClicked);
//    }
}

void Views::ViewProperties::_createTagsTableView()
{
    _qtvTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(&Database::DatabaseManager::instance());
    _qtvTags->setImportCSVFetureEnabled(false);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvTags->createToolButton("", "icons/AddTo.png", "Add selected tags to selected items");
        btn->clicked().connect(this, &Views::ViewProperties::_btnAddTagClicked);
    }

    Wt::WPushButton *btnFilter = _qtvTags->createToolButton("", "icons/Filter.png", "Filter active view by selected tags");
    btnFilter->clicked().connect(this, &Views::ViewProperties::_btnFilterTagClicked);

    Wt::WPushButton *btnClearFilter = _qtvTags->createToolButton("", "icons/ClearFilter.png", "Clear tags filter on the active view");
    btnClearFilter->clicked().connect(this, &Views::ViewProperties::_btnClearFilterTagClicked);

    updateTagsView();
}

void Views::ViewProperties::_createAssignedTagsTableView()
{
    _qtvAssignedTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(&Database::DatabaseManager::instance());
    _qtvAssignedTags->setImportCSVFetureEnabled(false);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvAssignedTags->createToolButton("", "icons/RemoveFrom.png", "Remove selected tags from selected items");
        btn->clicked().connect(this, &Views::ViewProperties::_btnRemoveTagClicked);
    }
}

void Views::ViewProperties::_createNotesTableView()
{
    _qtvNotes = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Note>(&Database::DatabaseManager::instance());
    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvNotes->createToolButton("", "icons/Add.png", "Add A New Note");
        btn->clicked().connect(this, &Views::ViewProperties::_btnAddNoteClicked);
    }
    else
        _qtvNotes->setImportCSVFetureEnabled(false);

    //requires "remove" privilege
//    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
//    {
//        Wt::WPushButton *btn = _qtvNotes->createToolButton("", "icons/Remove.png", "Remove Selected Notes");
//        btn->clicked().connect(this, &Views::ViewProperties::_btnRemoveNotesClicked);
//    }
}

void Views::ViewProperties::_createGroupsPrivilegesTableView()
{
    _qtvGroupsPrivileges = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::Privilege>(&Database::DatabaseManager::instance());
    _qtvGroupsPrivileges->setImportCSVFetureEnabled(false);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvGroupsPrivileges->createToolButton("", "icons/AddTo.png", "Add selected privileges to selected groups");
        btn->clicked().connect(this, &Views::ViewProperties::_btnAddGroupsPrivilegesClicked);
    }

    Wt::WPushButton *btnFilter = _qtvGroupsPrivileges->createToolButton("", "icons/Filter.png", "Filter groups view by selected privileges");
    btnFilter->clicked().connect(this, &Views::ViewProperties::_btnFilterGroupsPrivilegesClicked);

    Wt::WPushButton *btnClearFilter = _qtvGroupsPrivileges->createToolButton("", "icons/ClearFilter.png", "Clear privileges filter on the groups view");
    btnClearFilter->clicked().connect(this, &Views::ViewProperties::_btnClearFilterGroupsPrivilegesClicked);

    updateGroupsPrivilegesView();
}

void Views::ViewProperties::_createGroupsAssignedPrivilegesTableView()
{
    _qtvGroupsAssignedPrivileges = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::Privilege>(&Database::DatabaseManager::instance());
    _qtvGroupsAssignedPrivileges->setImportCSVFetureEnabled(false);

    //requires "edit" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvGroupsAssignedPrivileges->createToolButton("", "icons/RemoveFrom.png", "Remove selected privileges from selected groups");
        btn->clicked().connect(this, &Views::ViewProperties::_btnRemoveGroupsPrivilegesClicked);
    }
}

template<typename T>
void Views::ViewProperties::_addExtraColumns(Ms::Widgets::MQueryTableViewWidget<T> *widget, Wt::WFlags<Wt::ItemFlag> flags, int editRank)
{
    widget->addColumn(Ms::Widgets::MTableViewColumn("View_Rank", "View Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Edit_Rank", "Edit Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Remove_Rank", "Remove Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Date_Created", "Date Created", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Created_By", "Created By", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Last_Modified_Date", "Last Modified Date", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Last_Modified_By", "Last Modified By", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Active", "Active", Wt::ItemIsSelectable | Wt::ItemIsUserCheckable, new Ms::Widgets::Delegates::MCheckBoxDelegate(editRank)));
}

void Views::ViewProperties::_prepareView()
{
    _layCntMain = new Wt::WVBoxLayout();
    _layCntMain->setContentsMargins(0,0,0,0);
    _layCntMain->setSpacing(0);

    setLayout(_layCntMain);

    _navBarMain = new Wt::WNavigationBar();

    _layCntMain->addWidget(_navBarMain);

    _mnuNavBar = new Wt::WMenu(Wt::Horizontal);

    _navBarMain->addMenu(_mnuNavBar);

    _mnuNavBarDataItem = new Wt::WMenuItem("Data");
    _mnuNavBarDataItem->triggered().connect(this, &Views::ViewProperties::_mnuNavBarDataItemTriggered);
    _mnuNavBar->addItem(_mnuNavBarDataItem);

    _mnuNavBarTagsItem = new Wt::WMenuItem("Tags");
    _mnuNavBarTagsItem->triggered().connect(this, &Views::ViewProperties::_mnuNavBarTagsItemTriggered);
    _mnuNavBar->addItem(_mnuNavBarTagsItem);

    _mnuNavBarNotesItem = new Wt::WMenuItem("Notes");
    _mnuNavBarNotesItem->triggered().connect(this, &Views::ViewProperties::_mnuNavBarNotesItemTriggered);
    _mnuNavBar->addItem(_mnuNavBarNotesItem);

    _mnuNavBarGroupsPrivilegesItem = new Wt::WMenuItem("Privileges");
    _mnuNavBarGroupsPrivilegesItem->triggered().connect(this, &Views::ViewProperties::_mnuNavBarGroupsPrivilegesItemTriggered);
    _mnuNavBar->addItem(_mnuNavBarGroupsPrivilegesItem);

    _stkMain = new Wt::WStackedWidget();
    _stkMain->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);

    _layCntMain->addWidget(_stkMain, 1);

    //Data Table View
    _createDataTableView();
    _stkMain->addWidget(_qtvData);

    //Tags/AssignedTags Table Views
    _cntTags = new Wt::WContainerWidget();
    _stkMain->addWidget(_cntTags);

    _layCntTags = new Wt::WVBoxLayout();
    _layCntTags->setContentsMargins(0,0,0,0);
    _layCntTags->setSpacing(0);

    _cntTags->setLayout(_layCntTags);

    _cntAssignedTags = new Ms::Widgets::MContainerWidget();
    _cntAssignedTags->setTitle("<b><i>Assigned Tags</i></b>");

    _layCntTags->addWidget(_cntAssignedTags);

    Wt::WVBoxLayout *_layCntAssignedTags = dynamic_cast<Wt::WVBoxLayout*>(_cntAssignedTags->layout());
    _layCntAssignedTags->setContentsMargins(0,14,0,0);

    _createAssignedTagsTableView();
    _layCntAssignedTags->addWidget(_qtvAssignedTags, 1);

    //Available Tags
    _cntAvailableTags = new Ms::Widgets::MContainerWidget();
    _cntAvailableTags->setTitle("<b><i>Available Tags</i></b>");

    _layCntTags->addWidget(_cntAvailableTags);

     Wt::WVBoxLayout *_layCntAvailableTags = dynamic_cast<Wt::WVBoxLayout*>(_cntAvailableTags->layout());
    _layCntAvailableTags->setContentsMargins(0,14,0,0);

    //Tags Table View
    _createTagsTableView();
    _layCntAvailableTags->addWidget(_qtvTags, 1);

    //Notes Table View
    _createNotesTableView();
    _stkMain->addWidget(_qtvNotes);

    //GroupsPrivileges/GroupsAssignedPrivileges Table Views
    _cntGroupsPrivileges = new Wt::WContainerWidget();
    _stkMain->addWidget(_cntGroupsPrivileges);

    _layCntGroupsPrivileges = new Wt::WVBoxLayout();
    _layCntGroupsPrivileges->setContentsMargins(0,0,0,0);
    _layCntGroupsPrivileges->setSpacing(0);

    _cntGroupsPrivileges->setLayout(_layCntGroupsPrivileges);

    _cntGroupsAssignedPrivileges = new Ms::Widgets::MContainerWidget();
    _cntGroupsAssignedPrivileges->setTitle("<b><i>Assigned Privileges</i></b>");

    _layCntGroupsPrivileges->addWidget(_cntGroupsAssignedPrivileges);

    Wt::WVBoxLayout *_layCntGroupsAssignedPrivileges = dynamic_cast<Wt::WVBoxLayout*>(_cntGroupsAssignedPrivileges->layout());
    _layCntGroupsAssignedPrivileges->setContentsMargins(0,14,0,0);

    _cntGroupsAssignedPrivileges->setLayout(_layCntGroupsAssignedPrivileges);

    _createGroupsAssignedPrivilegesTableView();
    _layCntGroupsAssignedPrivileges->addWidget(_qtvGroupsAssignedPrivileges, 1);

    //Available Groups Privileges
    _cntGroupsAvailablePrivileges = new Ms::Widgets::MContainerWidget();
    _cntGroupsAvailablePrivileges->setTitle("<b><i>Available Privileges</i></b>");

    _layCntGroupsPrivileges->addWidget(_cntGroupsAvailablePrivileges);

    Wt::WVBoxLayout *_layCntGroupsAvailablePrivileges = dynamic_cast<Wt::WVBoxLayout*>(_cntGroupsAvailablePrivileges->layout());
    _layCntGroupsAvailablePrivileges->setContentsMargins(0,14,0,0);

    _createGroupsPrivilegesTableView();
    _layCntGroupsAvailablePrivileges->addWidget(_qtvGroupsPrivileges, 1);

    //No properties view
    _cntNoProperties = new Wt::WContainerWidget();
    _stkMain->addWidget(_cntNoProperties);

    //Default Selected Tab
    _mnuNavBar->select(_mnuNavBarDataItem);
}
