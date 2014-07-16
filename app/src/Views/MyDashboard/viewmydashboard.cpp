#include "viewmydashboard.h"

#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"
#include "../Main/viewmain.h"
#include "../../Auth/authmanager.h"
#include "../../Users/usersmanager.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Projects/projectsio.h"
#include "../../Projects/projectsmanager.h"

#include <Ms/Widgets/Delegates/MDelegates>
#include <Ms/IO/IO.h>

Views::ViewMyDashboard::ViewMyDashboard()
{
    _prepareView();

    _mnuNavBarMain->select(_mnuNavBarMainMyTasksItem);
}

void Views::ViewMyDashboard::updateView()
{
    if(_stkMain->currentWidget() == _qtvTasks)
        updateTasksView();
}

void Views::ViewMyDashboard::updateTasksView()
{
    if(!Auth::AuthManager::instance().login().user().isValid())
        return;

    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTask>> query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTask>();

        //load inactive data if selected from settings
        if(AppSettings::instance().isLoadInactiveData())
            query.where("Task_User_Name = ?").bind(Auth::AuthManager::instance().login().user().identity(Wt::Auth::Identity::LoginName));
        else
            query.where("Task_User_Name = ? AND Active = ?").bind(Auth::AuthManager::instance().login().user().identity(Wt::Auth::Identity::LoginName)).bind(true);

        _qtvTasks->setQuery(query);

        _qtvTasks->clearColumns();

        //add columns
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("id", "ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false, true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Current_Status", "Status", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Date_Created", "Date Created", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false, true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Start_Date", "Start Date", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("End_Date", "End Date", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Shot_Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Asset_Asset_Project_Project_Name", "Asset Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Description", "Description"));

        _qtvTasks->updateView();
    }
    catch(...)
    {
        std::cerr << "Error occured while trying to update mytasks view" << std::endl;
    }
}

bool Views::ViewMyDashboard::isTasksViewShown() const
{
    return _stkMain->currentWidget() == _qtvTasks;
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *Views::ViewMyDashboard::tasksQueryTableView() const
{
    return _qtvTasks;
}

Wt::Signal<> &Views::ViewMyDashboard::onTabMyTasksSelected()
{
    return _onTabMyTasksSelected;
}

void Views::ViewMyDashboard::_createTasksTableView()
{
    _qtvTasks = new Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask>(&Database::DatabaseManager::instance());
    _qtvTasks->setHeaderHeight(24);
    _qtvTasks->setRowHeight(24);
    _qtvTasks->setSelectionMode(Wt::ExtendedSelection);
    _qtvTasks->setDynamicSortFilter(true);
    _qtvTasks->setFilterRole(Wt::DisplayRole);
    _qtvTasks->setFilterRegExp("[^$]{0,255}");
    _qtvTasks->setDefaultFilterColumnIndex(0);
    _qtvTasks->setIgnoreNumFilterColumns(0);
    _qtvTasks->setImportOptionVisible(false);

    //requires "CheckIn or CheckOut" privilege
    if(Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::CheckIn) ||
            Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::CheckOut) ||
            Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::CreateRepos))
    {
        Wt::WPushButton *btn = _qtvTasks->createToolButton("", "icons/Files.png", "Files Manager");
        btn->clicked().connect(this, &Views::ViewMyDashboard::_btnTasksFilesClicked);
    }

    updateTasksView();
}

void Views::ViewMyDashboard::_btnTasksFilesClicked()
{
    if(_qtvTasks->table()->selectedIndexes().size() != 1)
        return;

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::ptr<Projects::ProjectTask> taskPtr =  _qtvTasks->model()->resultRow(_qtvTasks->proxyModel()->mapToSource(
                                                               *_qtvTasks->table()->selectedIndexes().begin()).row());

    if((!taskPtr->asset()) && (!taskPtr->shot()))
        return;

    std::string path = "";

    if(taskPtr->asset())
        path = Projects::ProjectsIO::getAbsoluteAssetTaskDir(taskPtr->asset()->projectName(), taskPtr->asset()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
    else if(taskPtr->shot())
        path = Projects::ProjectsIO::getAbsoluteShotTaskDir(taskPtr->shot()->projectName(), taskPtr->shot()->sequenceName(), taskPtr->shot()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";

    DlgFilesManager *dlg = new DlgFilesManager(path);
    dlg->finished().connect(std::bind([=]()
    {
        delete dlg;
    }));

    if(!Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::CreateRepos))
        dlg->setCreateDisabled(true);
    if(!Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::CheckIn))
        dlg->setCheckInDisabled(true);
    if(!Users::UsersManager::instance().checkCurrentUserPrivileges(Users::PrivilegeFlags::CheckOut))
        dlg->setCheckOutDisabled(true);

    dlg->show();
}

void Views::ViewMyDashboard::_mnuNavBarMainMyTasksItemTriggered()
{
    _stkMain->setCurrentWidget(_qtvTasks);
    updateTasksView();

    _onTabMyTasksSelected();
}

void Views::ViewMyDashboard::_prepareView()
{
    /*******************--MyTasks--********************/
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _cntTxtMyDashboardPanelTitle = new Wt::WContainerWidget();
    _cntTxtMyDashboardPanelTitle->setStyleClass("toolbar");
    _cntTxtMyDashboardPanelTitle->setContentAlignment(Wt::AlignCenter);
    _cntTxtMyDashboardPanelTitle->setMinimumSize(Wt::WLength::Auto, 25);

    _layMain->addWidget(_cntTxtMyDashboardPanelTitle);

    _txtMyDashboardPanelTitle = new Wt::WText("<b><i>My Dashboard</i></b>");
    _txtMyDashboardPanelTitle->setStyleClass("soft-text");

    _cntTxtMyDashboardPanelTitle->addWidget(_txtMyDashboardPanelTitle);

    _navBarMain = new Wt::WNavigationBar();

    _cntNavBarMain = new Wt::WContainerWidget();
    _cntNavBarMain->addWidget(_navBarMain);

    //add our navigation bar to the view
    _layMain->addWidget(_cntNavBarMain);

    _mnuNavBarMain = new Wt::WMenu(Wt::Horizontal);

    _navBarMain->addMenu(_mnuNavBarMain);

    _mnuNavBarMainMyTasksItem = new Wt::WMenuItem("My Tasks");
    _mnuNavBarMainMyTasksItem->triggered().connect(this, &Views::ViewMyDashboard::_mnuNavBarMainMyTasksItemTriggered);

    _mnuNavBarMain->addItem(_mnuNavBarMainMyTasksItem);

    _stkMain = new Wt::WStackedWidget();
    _layMain->addWidget(_stkMain, 1);

    _createTasksTableView();
    _stkMain->addWidget(_qtvTasks);
}
