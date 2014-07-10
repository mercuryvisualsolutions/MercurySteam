#include <Wt/WMessageBox>

#include "viewsettings.h"
#include "../../Database/databasemanager.h"
#include "../../Projects/projectsmanager.h"
#include "../../Settings/appsettings.h"
#include "Users/usersmanager.h"

#include <Ms/Widgets/Delegates/MDelegates>
#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewSettings::ViewSettings()
{
    _prepareView();

    updateGeneralSettingsView();

    //default menu selection
    _mnuSettings->select(_mnuSettingsGeneralItem);
    _mnuProjectSettings->select(_mnuProjectSettingsTaskTypeItem);
    _mnuUsersSettings->select(_mnuUsersSettingsUserTitlesItem);
    _mnuGlobalSettings->select(_mnuGlobalSettingsTagsItem);
}

//main
void Views::ViewSettings::_mnuSettingsGeneralItemTriggered()
{
    _stkSettings->setCurrentWidget(_cntGeneralSettings);
    updateGeneralSettingsView();
}

void Views::ViewSettings::_mnuSettingsProjectsItemTriggered()
{
    _stkSettings->setCurrentWidget(_cntProjectsSettings);
    updateProjectsSettingsView();
}

void Views::ViewSettings::_mnuSettingsUsersItemTriggered()
{
    _stkSettings->setCurrentWidget(_cntUsersSettings);
    updateUsersSettingsView();
}

void Views::ViewSettings::_mnuSettingsGlobalItemTriggered()
{
    _stkSettings->setCurrentWidget(_cntGlobalSettings);
    updateGlobalSettingsView();
}

void Views::ViewSettings::updateView()
{
    if(_stkSettings->currentWidget() == _cntGeneralSettings)
        updateGeneralSettingsView();
    else if(_stkSettings->currentWidget() == _cntProjectsSettings)
        updateProjectsSettingsView();
    else if(_stkSettings->currentWidget() == _cntUsersSettings)
        updateProjectsSettingsView();
    else if(_stkSettings->currentWidget() == _cntGlobalSettings)
        updateGlobalSettingsView();
}

void Views::ViewSettings::updateGeneralSettingsView()
{
    _chkLoadInactiveData->setChecked(AppSettings::instance().isLoadInactiveData());
    _chkShowExtraColumns->setChecked(AppSettings::instance().isShowExtraColumns());
}

void Views::ViewSettings::updateProjectsSettingsView()
{
    if(_stkProjectProperties->currentWidget() == _qtvProjectAssetType)
        updateAssetTypeView();
    else if(_stkProjectProperties->currentWidget() == _qtvProjectTaskType)
        updateTaskTypeView();
    else if(_stkProjectProperties->currentWidget() == _qtvProjectWorkStatus)
        updateWorkStatusView();
}

void Views::ViewSettings::updateUsersSettingsView()
{
    if(_stkUsersProperties->currentWidget() == _qtvUserTitle)
        updateUserTitlesView();
}

void Views::ViewSettings::updateGlobalSettingsView()
{
    if(_stkGlobalProperties->currentWidget() == _qtvTags)
        updateTagsView();
}

void Views::ViewSettings::updateTaskTypeView()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskType>> query;

        int viewRank = Users::UsersManager::instance().getCurrentUserRank(Database::RankFlag::ViewRank);

        if(AppSettings::instance().isLoadInactiveData())
            query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskType>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskType>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        _qtvProjectTaskType->setQuery(query);

        bool canEdit = Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Edit);
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Users::UsersManager::instance().getCurrentUserRank(Database::RankFlag::EditRank);

        _qtvProjectTaskType->clearColumns();

        //add columns
        _qtvProjectTaskType->addColumn(Ms::Widgets::MTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            _addExtraColumns<Projects::ProjectTaskType>(_qtvProjectTaskType, flags, editRank);

        _qtvProjectTaskType->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating task type table view" << std::endl;
    }
}

void Views::ViewSettings::updateAssetTypeView()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectAssetType>> query;

        int viewRank = Users::UsersManager::instance().getCurrentUserRank(Database::RankFlag::ViewRank);

        if(AppSettings::instance().isLoadInactiveData())
            query = Database::DatabaseManager::instance().session()->find<Projects::ProjectAssetType>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Database::DatabaseManager::instance().session()->find<Projects::ProjectAssetType>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        _qtvProjectAssetType->setQuery(query);

        bool canEdit = Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Edit);
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Users::UsersManager::instance().getCurrentUserRank(Database::RankFlag::EditRank);

        _qtvProjectAssetType->clearColumns();

        //add columns
        _qtvProjectAssetType->addColumn(Ms::Widgets::MTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            _addExtraColumns<Projects::ProjectAssetType>(_qtvProjectAssetType, flags, editRank);

        _qtvProjectAssetType->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating asset type table view" << std::endl;
    }
}

void Views::ViewSettings::updateWorkStatusView()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> query;

        int viewRank = Users::UsersManager::instance().getCurrentUserRank(Database::RankFlag::ViewRank);

        if(AppSettings::instance().isLoadInactiveData())
            query = Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        _qtvProjectWorkStatus->setQuery(query);

        bool canEdit = Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Edit);
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Users::UsersManager::instance().getCurrentUserRank(Database::RankFlag::EditRank);

        _qtvProjectWorkStatus->clearColumns();

        //add columns
        _qtvProjectWorkStatus->addColumn(Ms::Widgets::MTableViewColumn("Status", "Status", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvProjectWorkStatus->addColumn(Ms::Widgets::MTableViewColumn("Belongs_To_Work_Status_Type", "Belongs To Work Status Type",
        flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatusType>(Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatusType>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatusType>().where("Active = ?").bind(true),
         "Work_Status_Type", editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            _addExtraColumns<Projects::ProjectWorkStatus>(_qtvProjectWorkStatus, flags, editRank);

        _qtvProjectWorkStatus->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating work status table view" << std::endl;
    }
}

void Views::ViewSettings::updateUserTitlesView()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Users::UserTitle>> query;

        int viewRank = Users::UsersManager::instance().getCurrentUserRank(Database::RankFlag::ViewRank);

        if(AppSettings::instance().isLoadInactiveData())
            query = Database::DatabaseManager::instance().session()->find<Users::UserTitle>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Database::DatabaseManager::instance().session()->find<Users::UserTitle>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        _qtvUserTitle->setQuery(query);

        bool canEdit = Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Edit);
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Users::UsersManager::instance().getCurrentUserRank(Database::RankFlag::EditRank);

        _qtvUserTitle->clearColumns();

        //add columns
        _qtvUserTitle->addColumn(Ms::Widgets::MTableViewColumn("Name", "Title", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            _addExtraColumns<Users::UserTitle>(_qtvUserTitle, flags, editRank);

        _qtvUserTitle->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating users titles table view" << std::endl;
    }
}

void Views::ViewSettings::updateTagsView()
{
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

        bool canEdit = Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Edit);
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Users::UsersManager::instance().getCurrentUserRank(Database::RankFlag::EditRank);

        _qtvTags->clearColumns();

        //add columns
        _qtvTags->addColumn(Ms::Widgets::MTableViewColumn("Tag_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvTags->addColumn(Ms::Widgets::MTableViewColumn("Tag_Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            _addExtraColumns<Database::Tag>(_qtvTags, flags, editRank);

        _qtvTags->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating tags table view" << std::endl;
    }
}

Wt::Signal<> &Views::ViewSettings::onTabGeneralSelected()
{
    return _onTabGeneralSelected;
}

Wt::Signal<> &Views::ViewSettings::onTabProjectsSelected()
{
    return _onTabProjectsSelected;
}

Wt::Signal<> &Views::ViewSettings::onTabUsersSelected()
{
    return _onTabUsersSelected;
}

Wt::Signal<> &Views::ViewSettings::onTabGlobalSelected()
{
    return _onTabGlobalSelected;
}

template<typename T>
void Views::ViewSettings::_addExtraColumns(Ms::Widgets::MQueryTableViewWidget<T> *widget, Wt::WFlags<Wt::ItemFlag> flags, int editRank)
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

void Views::ViewSettings::_btnSaveClicked()
{
    AppSettings::instance().setLoadInactiveData(_chkLoadInactiveData->isChecked());
    AppSettings::instance().setShowExtraColumns(_chkShowExtraColumns->isChecked());

    Wt::WMessageBox *msg = new Wt::WMessageBox("Information", "Saved.", Wt::Information, Wt::Ok);
    msg->buttonClicked().connect(std::bind([=]()
    {
        delete msg;
    }));
    msg->show();
}

void Views::ViewSettings::_mnuProjectSettingsTaskTypeItemTriggered()
{
    _stkProjectProperties->setCurrentWidget(_cntTaskType);
    updateTaskTypeView();
}

void Views::ViewSettings::_mnuProjectSettingsAssetTypeItemTriggered()
{
    _stkProjectProperties->setCurrentWidget(_cntAssetType);
    updateAssetTypeView();
}

void Views::ViewSettings::_mnuProjectSettingsWorkStatusItemTriggered()
{
    _stkProjectProperties->setCurrentWidget(_cntWorkStatus);
    updateWorkStatusView();
}

void Views::ViewSettings::_mnuUsersSettingsUserTitlesItemTriggered()
{
    _stkUsersProperties->setCurrentWidget(_cntUserTitles);
    updateUserTitlesView();
}

void Views::ViewSettings::_mnuGlobalSettingsTagsItemTriggered()
{
    _stkGlobalProperties->setCurrentWidget(_cntTags);
    updateTagsView();
}

//TaskType
void Views::ViewSettings::_createTaskTypeTableView()
{
    _qtvProjectTaskType = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTaskType>(&Database::DatabaseManager::instance());

    //requires "create" privilege
    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Create))
    {
        Wt::WPushButton *btn = _qtvProjectTaskType->createToolButton("", "icons/Add.png", "Create A New Task Type");
        btn->clicked().connect(this, &Views::ViewSettings::_btnCreateTaskTypeClicked);

        _qtvProjectTaskType->setImportOptionVisible(true);
    }
    else
        _qtvProjectTaskType->setImportOptionVisible(false);

    //requires "remove" privilege
//    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Remove))
//    {
//        Wt::WPushButton *btn = _qtvProjectTaskType->createToolButton("", "icons/Remove.png", "Remove Selected Task Type");
//        btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveTaskTypeClicked);
//    }

    updateTaskTypeView();
}

void Views::ViewSettings::_btnCreateTaskTypeClicked()
{
    Views::DlgCreateTaskType *dlg = new Views::DlgCreateTaskType();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Projects::ProjectTaskType *type = new Projects::ProjectTaskType(dlg->type());
            type->setActive(dlg->isActive());

            if(Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(type))
                updateTaskTypeView();
        }

        delete dlg;
    }));

    dlg->show();
}

void Views::ViewSettings::_btnRemoveTaskTypeClicked()
{

}

//AssetType
void Views::ViewSettings::_createAssetTypeTableView()
{
    _qtvProjectAssetType = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectAssetType>(&Database::DatabaseManager::instance());

    //requires "create" privilege
    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Create))
    {
        Wt::WPushButton *btn = _qtvProjectAssetType->createToolButton("", "icons/Add.png", "Create A New Asset Type");
        btn->clicked().connect(this, &Views::ViewSettings::_btnCreateAssetTypeClicked);

        _qtvProjectAssetType->setImportOptionVisible(true);
    }
    else
        _qtvProjectAssetType->setImportOptionVisible(false);

    //requires "remove" privilege
//    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Remove))
//    {
//        Wt::WPushButton *btn = _qtvProjectAssetType->createToolButton("", "icons/Remove.png", "Remove Selected Asset Type");
//        btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveAssetTypeClicked);
//    }

    updateAssetTypeView();
}

void Views::ViewSettings::_btnCreateAssetTypeClicked()
{
    Views::DlgCreateAssetType *dlg = new Views::DlgCreateAssetType();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Projects::ProjectAssetType *type = new Projects::ProjectAssetType(dlg->type());
            type->setActive(dlg->isActive());

            if(Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(type))
                updateAssetTypeView();
        }

        delete dlg;
    }));

    dlg->show();
}

void Views::ViewSettings::_btnRemoveAssetTypeClicked()
{

}

//WorkStatus
void Views::ViewSettings::_createWorkStatusTableView()
{
    _qtvProjectWorkStatus = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectWorkStatus>(&Database::DatabaseManager::instance());

    //requires "create" privilege
    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Create))
    {
        Wt::WPushButton *btn = _qtvProjectWorkStatus->createToolButton("", "icons/Add.png", "Create A New Work Status");
        btn->clicked().connect(this, &Views::ViewSettings::_btnCreateWorkStatusClicked);

        _qtvProjectWorkStatus->setImportOptionVisible(true);
    }
    else
        _qtvProjectWorkStatus->setImportOptionVisible(false);

    //requires "remove" privilege
//    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Remove))
//    {
//        Wt::WPushButton *btn = _qtvProjectWorkStatus->createToolButton("", "icons/Remove.png", "Remove Selected Work Status");
//        btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveWorkStatusClicked);
//    }

    updateWorkStatusView();
}

void Views::ViewSettings::_btnCreateWorkStatusClicked()
{
    Views::DlgCreateWorkStatus *dlg = new Views::DlgCreateWorkStatus();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Projects::ProjectWorkStatus *status = new Projects::ProjectWorkStatus(dlg->type());
            status->setWorkStatusType(dlg->belongsToType());
            status->setActive(dlg->isActive());

            if(Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(status))
                updateWorkStatusView();
        }

        delete dlg;
    }));

    dlg->show();
}

void Views::ViewSettings::_btnRemoveWorkStatusClicked()
{

}

//UserTitles
void Views::ViewSettings::_createUsersTitlesTableView()
{
    _qtvUserTitle = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::UserTitle>(&Database::DatabaseManager::instance());

    //requires "create" privilege
    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Create))
    {
        Wt::WPushButton *btn = _qtvUserTitle->createToolButton("", "icons/Add.png", "Create A New User Title");
        btn->clicked().connect(this, &Views::ViewSettings::_btnCreateUserTitleClicked);

        _qtvUserTitle->setImportOptionVisible(true);
    }
    else
        _qtvUserTitle->setImportOptionVisible(false);

    //requires "remove" privilege
    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Remove))
    {
        //Wt::WPushButton *btn = _qtvUserTitle->createToolButton("", "icons/Remove.png", "Remove Selected User Title");
        //btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveUserTitlesClicked);
    }

    updateUserTitlesView();
}

void Views::ViewSettings::_btnCreateUserTitleClicked()
{
    Views::DlgCreateUserTitle *dlg = new Views::DlgCreateUserTitle();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Users::UserTitle *title = new Users::UserTitle(dlg->title());
            title->setActive(dlg->isActive());

            if(Database::DatabaseManager::instance().createDbo<Users::UserTitle>(title))
                updateUserTitlesView();
        }

        delete dlg;
    }));

    dlg->show();
}

void Views::ViewSettings::_btnRemoveUserTitlesClicked()
{

}

//Tags
void Views::ViewSettings::_createTagsTableView()
{
    _qtvTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(&Database::DatabaseManager::instance());

    //requires "create" privilege
    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Create))
    {
        Wt::WPushButton *btn = _qtvTags->createToolButton("", "icons/Add.png", "Create A New Tag");
        btn->clicked().connect(this, &Views::ViewSettings::_btnCreateTagClicked);

        _qtvTags->setImportOptionVisible(true);
    }
    else
        _qtvTags->setImportOptionVisible(false);

    //requires "remove" privilege
    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Remove))
    {
        //Wt::WPushButton *btn = _qtvTags->createToolButton("", "icons/Remove.png", "Remove Selected Tags");
        //btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveTagClicked);
    }

    updateTagsView();
}

void Views::ViewSettings::_btnCreateTagClicked()
{
    Views::DlgCreateTag *dlg = new Views::DlgCreateTag();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Database::Tag *tag = new Database::Tag(dlg->tagName(), dlg->tagContent());
            tag->setActive(dlg->isActive());

            if(Database::DatabaseManager::instance().createDbo<Database::Tag>(tag))
                updateTagsView();
        }

        delete dlg;
    }));

    dlg->show();
}

void Views::ViewSettings::_btnRemoveTagClicked()
{

}

void Views::ViewSettings::_prepareView()
{
    /*******************--Settings--********************/
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _cntTxtSettingsPanelTitle = new Wt::WContainerWidget();
    _cntTxtSettingsPanelTitle->setStyleClass("toolbar");
    _cntTxtSettingsPanelTitle->setContentAlignment(Wt::AlignCenter);
    _cntTxtSettingsPanelTitle->setMinimumSize(Wt::WLength::Auto, 25);

    _layMain->addWidget(_cntTxtSettingsPanelTitle);

    _txtSettingsPanelTitle = new Wt::WText("<b><i>Settings</i></b>");
    _txtSettingsPanelTitle->setStyleClass("soft-text");

    _cntTxtSettingsPanelTitle->addWidget(_txtSettingsPanelTitle);

    _navBarSettings = new Wt::WNavigationBar();

    _cntNavBarSettings = new Wt::WContainerWidget();
    _cntNavBarSettings->addWidget(_navBarSettings);

    //add our navigation bar to the view
    _layMain->addWidget(_cntNavBarSettings);

    _mnuSettings = new Wt::WMenu(Wt::Horizontal);
    _navBarSettings->addMenu(_mnuSettings);

    _mnuSettingsGeneralItem = new Wt::WMenuItem("General");
    _mnuSettingsGeneralItem->triggered().connect(this, &Views::ViewSettings::_mnuSettingsGeneralItemTriggered);
    _mnuSettings->addItem(_mnuSettingsGeneralItem);

    _mnuSettingsProjectsItem = new Wt::WMenuItem("Projects");
    _mnuSettingsProjectsItem->triggered().connect(this, &Views::ViewSettings::_mnuSettingsProjectsItemTriggered);
    _mnuSettings->addItem(_mnuSettingsProjectsItem);

    _mnuSettingsUsersItem = new Wt::WMenuItem("Users");
    _mnuSettingsUsersItem->triggered().connect(this, &Views::ViewSettings::_mnuSettingsUsersItemTriggered);
    _mnuSettings->addItem(_mnuSettingsUsersItem);

    _mnuSettingsGlobalItem = new Wt::WMenuItem("Global");
    _mnuSettingsGlobalItem->triggered().connect(this, &Views::ViewSettings::_mnuSettingsGlobalItemTriggered);
    _mnuSettings->addItem(_mnuSettingsGlobalItem);

    _stkSettings = new Wt::WStackedWidget();
    _layMain->addWidget(_stkSettings, 1);//stack general view

    //General//////////////////////////////////////
    _laySettingsGeneral = new Wt::WVBoxLayout();
    _laySettingsGeneral->setContentsMargins(20,10,20,20);
    _laySettingsGeneral->setSpacing(0);

    _cntGeneralSettings = new Wt::WContainerWidget();
    _cntGeneralSettings->setStyleClass("toolbar");
    _cntGeneralSettings->setLayout(_laySettingsGeneral);

    _grpGeneral = new Wt::WGroupBox();
    _grpGeneral->setTitle("General");
    _laySettingsGeneral->addWidget(_grpGeneral);

    _chkLoadInactiveData = new Wt::WCheckBox();
    _chkLoadInactiveData->setText("Load Inactive Data From Database");

    _grpGeneral->addWidget(_chkLoadInactiveData);

    new Wt::WBreak(_grpGeneral);

    _chkShowExtraColumns = new Wt::WCheckBox();
    _chkShowExtraColumns->setText("Show Extra Columns In Data Views");

    _grpGeneral->addWidget(_chkShowExtraColumns);

    new Wt::WBreak(_grpGeneral);

    _btnSave = new Wt::WPushButton("Save");
    _btnSave->setMinimumSize(60, 20);
    _btnSave->clicked().connect(this, &Views::ViewSettings::_btnSaveClicked);

    _cntBtnSave = new Wt::WContainerWidget();
    _cntBtnSave->setContentAlignment(Wt::AlignLeft);
    _cntBtnSave->setMargin(10);
    _cntBtnSave->addWidget(_btnSave);

    _grpGeneral->addWidget(_cntBtnSave);

    //check privileges
    //requires "edit" privilege
    if(!Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::Edit))
    {
        _chkLoadInactiveData->setDisabled(true);
        _chkShowExtraColumns->setDisabled(true);
        _btnSave->setDisabled(true);
    }

    //add our general view to the Settings view
    _stkSettings->addWidget(_cntGeneralSettings);

    /*******************--Projects Main--********************/
    _layCntProjectsSettings = new Wt::WHBoxLayout();
    _layCntProjectsSettings->setContentsMargins(0,0,0,0);
    _layCntProjectsSettings->setSpacing(0);

    _cntProjectsSettings = new Wt::WContainerWidget();
    _cntProjectsSettings->setLayout(_layCntProjectsSettings);

    _layCntMnuProjectSettings = new Wt::WVBoxLayout();
    _layCntMnuProjectSettings->setContentsMargins(0,0,0,0);
    _layCntMnuProjectSettings->setSpacing(0);

    _cntMnuProjectSettings = new Wt::WContainerWidget();
    _cntMnuProjectSettings->setStyleClass("panel-border-right");
    _cntMnuProjectSettings->setLayout(_layCntMnuProjectSettings);

    _layCntProjectsSettings->addWidget(_cntMnuProjectSettings);

    _mnuProjectSettings = new Wt::WMenu(Wt::Vertical);
    _mnuProjectSettings->addStyleClass("nav-pills nav-stacked");
    _mnuProjectSettings->setWidth(250);

    _mnuProjectSettings->addSectionHeader("Types");

    _mnuProjectSettingsTaskTypeItem = new Wt::WMenuItem("Task Type");
    _mnuProjectSettingsTaskTypeItem->triggered().connect(this, &Views::ViewSettings::_mnuProjectSettingsTaskTypeItemTriggered);
    _mnuProjectSettings->addItem(_mnuProjectSettingsTaskTypeItem);

    _mnuProjectSettingsAssetTypeItem = new Wt::WMenuItem("Asset Type");
    _mnuProjectSettingsAssetTypeItem->triggered().connect(this, &Views::ViewSettings::_mnuProjectSettingsAssetTypeItemTriggered);
    _mnuProjectSettings->addItem(_mnuProjectSettingsAssetTypeItem);

    _mnuProjectSettingsWorkStatusItem = new Wt::WMenuItem("Work Status Type");
    _mnuProjectSettingsWorkStatusItem->triggered().connect(this, &Views::ViewSettings::_mnuProjectSettingsWorkStatusItemTriggered);
    _mnuProjectSettings->addItem(_mnuProjectSettingsWorkStatusItem);

    _layCntMnuProjectSettings->addWidget(_mnuProjectSettings);

    //add our project view to the Settings view
    _stkSettings->addWidget(_cntProjectsSettings);

    //add our Projects properties to our settings view
    _stkProjectProperties = new Wt::WStackedWidget();
    _layCntProjectsSettings->addWidget(_stkProjectProperties, 1);

    /*******************--TaskType--********************/
    _layCntTaskType = new Wt::WVBoxLayout();
    _layCntTaskType->setContentsMargins(0,0,0,0);
    _layCntTaskType->setSpacing(0);

    _cntTaskType = new Wt::WContainerWidget();
    _cntTaskType->setLayout(_layCntTaskType);

    //add our TaskType view to the project settings view
    _stkProjectProperties->addWidget(_cntTaskType);

    _createTaskTypeTableView();

    _layCntTaskType->addWidget(_qtvProjectTaskType, 1);

    /*******************--AssetType--********************/
    _layCntAssetType = new Wt::WVBoxLayout();
    _layCntAssetType->setContentsMargins(0,0,0,0);
    _layCntAssetType->setSpacing(0);

    _cntAssetType = new Wt::WContainerWidget();
    _cntAssetType->setLayout(_layCntAssetType);

    //add our AssetType view to the project settings view
    _stkProjectProperties->addWidget(_cntAssetType);

    _createAssetTypeTableView();

    _layCntAssetType->addWidget(_qtvProjectAssetType, 1);

    /*******************--WorkStatus--********************/
    _layCntWorkStatus = new Wt::WVBoxLayout();
    _layCntWorkStatus->setContentsMargins(0,0,0,0);
    _layCntWorkStatus->setSpacing(0);

    _cntWorkStatus = new Wt::WContainerWidget();
    _cntWorkStatus->setLayout(_layCntWorkStatus);

    //add our WorkStatus view to the project settings view
    _stkProjectProperties->addWidget(_cntWorkStatus);

    _createWorkStatusTableView();

    _layCntWorkStatus->addWidget(_qtvProjectWorkStatus, 1);

    /*******************--Users Main--********************/
    _layCntUsersSettings = new Wt::WHBoxLayout();
    _layCntUsersSettings->setContentsMargins(0,0,0,0);
    _layCntUsersSettings->setSpacing(0);

    _cntUsersSettings = new Wt::WContainerWidget();
    _cntUsersSettings->setLayout(_layCntUsersSettings);

    _layCntMnuUsersSettings = new Wt::WVBoxLayout();
    _layCntMnuUsersSettings->setContentsMargins(0,0,0,0);
    _layCntMnuUsersSettings->setSpacing(0);

    _cntMnuUsersSettings = new Wt::WContainerWidget();
    _cntMnuUsersSettings->setStyleClass("panel-border-right");
    _cntMnuUsersSettings->setLayout(_layCntMnuUsersSettings);

    _layCntUsersSettings->addWidget(_cntMnuUsersSettings);

    _mnuUsersSettings = new Wt::WMenu(Wt::Vertical);
    _mnuUsersSettings->addStyleClass("nav-pills nav-stacked");
    _mnuUsersSettings->setWidth(250);

    _mnuUsersSettings->addSectionHeader("Types");

    _mnuUsersSettingsUserTitlesItem = new Wt::WMenuItem("User Titles");
    _mnuUsersSettingsUserTitlesItem->triggered().connect(this, &Views::ViewSettings::_mnuUsersSettingsUserTitlesItemTriggered);
    _mnuUsersSettings->addItem(_mnuUsersSettingsUserTitlesItem);

    _layCntMnuUsersSettings->addWidget(_mnuUsersSettings);

    //add our User view to the Settings view
    _stkSettings->addWidget(_cntUsersSettings);

    //add our Users properties to our settings view
    _stkUsersProperties = new Wt::WStackedWidget();
    _layCntUsersSettings->addWidget(_stkUsersProperties, 1);

    /*******************--UserTitles--********************/
    _layCntUserTitles = new Wt::WVBoxLayout();
    _layCntUserTitles->setContentsMargins(0,0,0,0);
    _layCntUserTitles->setSpacing(0);

    _cntUserTitles = new Wt::WContainerWidget();
    _cntUserTitles->setLayout(_layCntUserTitles);

    //add our Titles view to the User settings view
    _stkUsersProperties->addWidget(_cntUserTitles);

    _createUsersTitlesTableView();

    _layCntUserTitles->addWidget(_qtvUserTitle, 1);

    /*******************--Global Main--********************/
    _layCntGlobalSettings = new Wt::WHBoxLayout();
    _layCntGlobalSettings->setContentsMargins(0,0,0,0);
    _layCntGlobalSettings->setSpacing(0);

    _cntGlobalSettings = new Wt::WContainerWidget();
    _cntGlobalSettings->setLayout(_layCntGlobalSettings);

    _layCntMnuGlobalSettings = new Wt::WVBoxLayout();
    _layCntMnuGlobalSettings->setContentsMargins(0,0,0,0);
    _layCntMnuGlobalSettings->setSpacing(0);

    _cntMnuGlobalSettings = new Wt::WContainerWidget();
    _cntMnuGlobalSettings->setStyleClass("panel-border-right");
    _cntMnuGlobalSettings->setLayout(_layCntMnuGlobalSettings);

    _layCntGlobalSettings->addWidget(_cntMnuGlobalSettings);

    _mnuGlobalSettings = new Wt::WMenu(Wt::Vertical);
    _mnuGlobalSettings->addStyleClass("nav-pills nav-stacked");
    _mnuGlobalSettings->setWidth(250);

    _mnuGlobalSettings->addSectionHeader("Types");

    _mnuGlobalSettingsTagsItem = new Wt::WMenuItem("Tags");
    _mnuGlobalSettingsTagsItem->triggered().connect(this, &Views::ViewSettings::_mnuGlobalSettingsTagsItemTriggered);
    _mnuGlobalSettings->addItem(_mnuGlobalSettingsTagsItem);

    _layCntMnuGlobalSettings->addWidget(_mnuGlobalSettings);

    //add our Global view to the Settings view
    _stkSettings->addWidget(_cntGlobalSettings);

    //add our Global properties to our settings view
    _stkGlobalProperties = new Wt::WStackedWidget();
    _layCntGlobalSettings->addWidget(_stkGlobalProperties, 1);

    /*******************--Tags--********************/
    _layCntTags = new Wt::WVBoxLayout();
    _layCntTags->setContentsMargins(0,0,0,0);
    _layCntTags->setSpacing(0);

    _cntTags = new Wt::WContainerWidget();
    _cntTags->setLayout(_layCntTags);

    //add our Tags view to the User settings view
    _stkGlobalProperties->addWidget(_cntTags);

    _createTagsTableView();

    _layCntTags->addWidget(_qtvTags, 1);
}
