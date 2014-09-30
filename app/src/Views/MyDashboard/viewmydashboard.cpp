#include "viewmydashboard.h"

#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"
#include "../Main/viewmain.h"
#include "../../Auth/authmanager.h"
#include "../../Users/usersmanager.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Projects/projectsio.h"
#include "../../Projects/projectsmanager.h"
#include "../../Log/logmanager.h"

#include <Ms/Widgets/Delegates/MDelegates>
#include <Ms/IO/IO.h>

Views::ViewMyDashboard::ViewMyDashboard() :
    Ms::Widgets::MContainerWidget()
{
    _logger = Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId());
    _propertiesPanel = Session::SessionManager::instance().getSessionPropertiesPanel(Wt::WApplication::instance()->sessionId());

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

        int editRank = Auth::AuthManager::instance().currentUser()->editRank();

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTask>> query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTask>();

        //load inactive data if selected from settings
        if(AppSettings::instance().isLoadInactiveData())
            query.where("Task_User_Name = ?").bind(Auth::AuthManager::instance().login().user().identity(Wt::Auth::Identity::LoginName));
        else
            query.where("Task_User_Name = ? AND Active = ?").bind(Auth::AuthManager::instance().login().user().identity(Wt::Auth::Identity::LoginName)).bind(true);

        _qtvTasks->setQuery(query);

        _qtvTasks->clearColumns();

        //add columns
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false, true));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Date_Created", "Date Created", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false, true));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Start_Date", "Start Date", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("End_Date", "End Date", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Sequence_Sequence_Project_Project_Name", "Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Shot_Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Asset_Asset_Project_Project_Name", "Asset Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description"));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Priority", "Priority", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Accepted_By_User", "Accepted", Wt::ItemIsSelectable | Wt::ItemIsUserCheckable, new Ms::Widgets::Delegates::MCheckBoxDelegate(editRank), false, true));

        _qtvTasks->updateView();
    }
    catch(...)
    {
        std::cerr << "Error occured while trying to update mytasks view" << std::endl;
    }
}

void Views::ViewMyDashboard::showPropertiesView()
{
    _propertiesPanel->showView("MyDashboard");
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
    _qtvTasks->setImportCSVFeatureEnabled(false);

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
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

    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
        dlg->setCreateDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In"))
        dlg->setCheckInDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out"))
        dlg->setCheckOutDisabled(true);

    dlg->show();
}

void Views::ViewMyDashboard::_mnuNavBarMainMyTasksItemTriggered()
{
    _stkMain->setCurrentWidget(_qtvTasks);
    updateTasksView();

    _onTabMyTasksSelected();
}

void Views::ViewMyDashboard::_createPropertiesView()
{
    _cntPropertiesMain = new Wt::WContainerWidget();
    _propertiesPanel->addPropertiesView("MyDashboard", _cntPropertiesMain);
}

void Views::ViewMyDashboard::_prepareView()
{
    /*******************--MyTasks--********************/
    setTitle("<b><i>My Dashboard</i></b>");

    Wt::WVBoxLayout *_layMain = dynamic_cast<Wt::WVBoxLayout*>(layout());
    _layMain->setContentsMargins(14,14,14,14);

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
    _stkMain->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);
    _layMain->addWidget(_stkMain, 1);

    _createTasksTableView();
    _stkMain->addWidget(_qtvTasks);

    /*******************--Properties--********************/
    _createPropertiesView();
}
