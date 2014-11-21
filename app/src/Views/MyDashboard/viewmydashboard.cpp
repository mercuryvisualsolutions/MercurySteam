#include "viewmydashboard.h"

#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"
#include "../Main/viewmain.h"
#include "../../Auth/authmanager.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Projects/projectsio.h"
#include "../../Log/logmanager.h"
#include "../Projects/dlgtaskselectdbo.h"

#include <Ms/Widgets/Delegates/MDelegates>
#include <Ms/IO/IO.h>

Views::ViewMyDashboard::ViewMyDashboard() :
    Ms::Widgets::MContainerWidget()
{
    _logger = Session::SessionManager::instance().logger();
    _propertiesPanel = Session::SessionManager::instance().propertiesPanel();

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
    if(!Session::SessionManager::instance().login().user().isValid())
        return;

    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        int editRank = Session::SessionManager::instance().user()->editRank();

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTask>> query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTask>();

        //load inactive data if selected from settings
        if(AppSettings::instance().isLoadInactiveData())
            query.where("Task_User_Name = ?").bind(Session::SessionManager::instance().login().user().identity(Wt::Auth::Identity::LoginName));
        else
            query.where("Task_User_Name = ? AND Active = ?").bind(Session::SessionManager::instance().login().user().identity(Wt::Auth::Identity::LoginName)).bind(true);

        _qtvTasks->setQuery(query);

        transaction.commit();

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

void Views::ViewMyDashboard::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasViewFilesPriv = user->hasPrivilege("View Files");
    bool hasCheckInPriv = user->hasPrivilege("Check In");
    bool hasCheckOutPriv = user->hasPrivilege("Check Out");
    bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

    bool showTaskFilesButton = hasViewFilesPriv || hasCheckInPriv || hasCheckOutPriv || hasCreateRepoPriv;//if have any of the privileges
    _btnTaskFiles->setHidden(!showTaskFilesButton);
}

Wt::Signal<> &Views::ViewMyDashboard::onTabMyTasksSelected()
{
    return _onTabMyTasksSelected;
}

void Views::ViewMyDashboard::_createTasksTableView()
{
    _qtvTasks = new Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask>(Session::SessionManager::instance().dboSession());
    _qtvTasks->setHeaderHeight(24);
    _qtvTasks->setRowHeight(24);
    _qtvTasks->setSelectionMode(Wt::ExtendedSelection);
    _qtvTasks->setDynamicSortFilter(true);
    _qtvTasks->setFilterRole(Wt::DisplayRole);
    _qtvTasks->setFilterRegExp("[^$]{0,255}");
    _qtvTasks->setDefaultFilterColumnIndex(0);
    _qtvTasks->setIgnoreNumFilterColumns(0);
    _qtvTasks->setImportCSVFeatureEnabled(false);

    _btnTaskFiles = _qtvTasks->createToolButton("", "icons/Files.png", "Files Manager");
    _btnTaskFiles->clicked().connect(this, &Views::ViewMyDashboard::_btnTasksFilesClicked);

    updateTasksView();
}

void Views::ViewMyDashboard::_btnTasksFilesClicked()
{
    if(_qtvTasks->table()->selectedIndexes().size() != 1)
    {
        _logger->log("Please select only one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgTaskSelectDbo *dlgSelectDbo = new Views::DlgTaskSelectDbo();

    dlgSelectDbo->finished().connect(std::bind([=]()
    {
        if(dlgSelectDbo->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            Wt::Dbo::ptr<Projects::ProjectTask> taskPtr =  _qtvTasks->selectedItems().at(0);

            std::string path = "";

            if(dlgSelectDbo->type() == "Project")
            {
                if(taskPtr->project().get())
                    path = Projects::ProjectsIO::getRelativeProjectTaskDir(taskPtr->project()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                else
                    _logger->log("Task does not belong to a project.", Ms::Log::LogMessageType::Warning);
            }
            else if(dlgSelectDbo->type() == "Sequence")
            {
                if(taskPtr->sequence().get())
                    path = Projects::ProjectsIO::getRelativeSequenceTaskDir(taskPtr->sequence()->projectName(), taskPtr->sequence()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                else
                    _logger->log("Task does not belong to a sequence.", Ms::Log::LogMessageType::Warning);
            }
            else if(dlgSelectDbo->type() == "Shot")
            {
                if(taskPtr->shot().get())
                    path = Projects::ProjectsIO::getRelativeShotTaskDir(taskPtr->shot()->projectName(), taskPtr->shot()->sequenceName(), taskPtr->shot()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                else
                    _logger->log("Task does not belong to a shot.", Ms::Log::LogMessageType::Warning);
            }
            else if(dlgSelectDbo->type() == "Asset")
            {
                if(taskPtr->asset().get())
                    path = Projects::ProjectsIO::getRelativeAssetTaskDir(taskPtr->asset()->projectName(), taskPtr->asset()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                else
                    _logger->log("Task does not belong to an asset.", Ms::Log::LogMessageType::Warning);
            }

            if(path != "")
            {
                DlgFilesManager *dlgFiles = new DlgFilesManager(path);
                dlgFiles->finished().connect(std::bind([=]()
                {
                    delete dlgFiles;
                }));

                Wt::Dbo::ptr<Users::User> user = Session::SessionManager::instance().user();

                bool hasViewFilesPriv = user->hasPrivilege("View Files");
                bool hasCheckInPriv = user->hasPrivilege("Check In");
                bool hasCheckOutPriv = user->hasPrivilege("Check Out");
                bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

                dlgFiles->setViewDisabled(!hasViewFilesPriv);
                dlgFiles->setCreateDisabled(!hasCreateRepoPriv);
                dlgFiles->setCheckInDisabled(!hasCheckInPriv);
                dlgFiles->setCheckOutDisabled(!hasCheckOutPriv);

                dlgFiles->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
            }

            transaction.commit();
        }

        delete dlgSelectDbo;
    }));

    dlgSelectDbo->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
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
    _layMain->setContentsMargins(14,0,14,14);

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
