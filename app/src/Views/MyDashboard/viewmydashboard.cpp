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
    m_logger = Session::SessionManager::instance().logger();
    m_propertiesPanel = Session::SessionManager::instance().propertiesPanel();

    prepareView();

    m_mnuNavBarMain->select(m_mnuNavBarMainMyTasksItem);
}

void Views::ViewMyDashboard::updateView()
{
    updateNotificationsCount();

    if(m_stkMain->currentWidget() == m_qtvTasks)
        updateTasksView();
    else if(m_stkMain->currentWidget() == m_qtvNotifications)
    {
        updateNotificationsView();

        resetUserLastSeenNotificationDate();
    }
}

void Views::ViewMyDashboard::updateTasksView()
{
    if(!Session::SessionManager::instance().login().user().isValid())
        return;

    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::ptr<Users::User> user = Session::SessionManager::instance().user();

        int editRank = user->editRank();

        bool canEdit = user->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTask>> query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTask>();

        //load inactive data if selected from settings
        if(AppSettings::instance().isLoadInactiveData())
            query.where("Task_User_Name = ?").bind(user->name());
        else
            query.where("Task_User_Name = ? AND Active = ?").bind(user->name()).bind(true);

        m_qtvTasks->setQuery(query);

        transaction.commit();

        m_qtvTasks->clearColumns();

        //add columns
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false, true));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true),
         "Status", editRank)));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Date_Created", "Date Created", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false, true));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Start_Date", "Start Date", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("End_Date", "End Date", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Sequence_Sequence_Project_Project_Name", "Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Shot_Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Asset_Asset_Project_Project_Name", "Asset Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description"));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Priority", "Priority", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Accepted_By_User", "Accepted", Wt::ItemIsSelectable | Wt::ItemIsUserCheckable, new Ms::Widgets::Delegates::MCheckBoxDelegate(editRank), false, true));

        m_qtvTasks->updateView();
    }
    catch(...)
    {
        std::cerr << "Error occured while trying to update mytasks view" << std::endl;
    }
}

void Views::ViewMyDashboard::updateNotificationsView()
{
    if(!Session::SessionManager::instance().login().user().isValid())
        return;

    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::ptr<Users::User> user = Session::SessionManager::instance().user();

        std::string queryStr = "SELECT n FROM notification n WHERE n.id IN (SELECT un.notification_id FROM rel_user_notifications un WHERE un.user_Name = '" + user->name() + "')";
        //std::string queryStr = "SELECT n FROM notification n";

        if(!AppSettings::instance().isLoadInactiveData())
            queryStr += " And Active = True";

        Wt::Dbo::Query<Wt::Dbo::ptr<Database::Notification>> query = Session::SessionManager::instance().dboSession().query<Wt::Dbo::ptr<Database::Notification>>(queryStr);

        //std::string notificationIdsSelect = "SELECT un.notification_id FROM rel_user_notifications un WHERE un.user_Name = '" + user->name() + "'";

        //only load active data if selected from settings

        query.orderBy("n.Date_Created DESC").limit(5000);//order by newer notifications and set results limit to 500

        m_qtvNotifications->setQuery(query);

        transaction.commit();

        m_qtvNotifications->clearColumns();

        //add columns
        m_qtvNotifications->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false, true));
        m_qtvNotifications->addColumn(Ms::Widgets::MQueryTableViewColumn("Message", "Message", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, true));

        m_qtvNotifications->addColumn(Ms::Widgets::MQueryTableViewColumn("Date_Created", "Date Created", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false, true));
        m_qtvNotifications->addColumn(Ms::Widgets::MQueryTableViewColumn("Created_By", "Created By", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate, false, true));

        m_qtvNotifications->updateView();
    }
    catch(Wt::Dbo::Exception ex)
    {
        std::cerr << "Error occured while trying to update notifications view" << std::endl << ex.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Error occured while trying to update notifications view" << std::endl;
    }
}

void Views::ViewMyDashboard::showPropertiesView()
{
    m_propertiesPanel->showView("MyDashboard");
}

bool Views::ViewMyDashboard::isTasksViewShown() const
{
    return m_stkMain->currentWidget() == m_qtvTasks;
}

bool Views::ViewMyDashboard::isNotificationsViewShown() const
{
    return m_stkMain->currentWidget() == m_qtvNotifications;
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *Views::ViewMyDashboard::tasksQueryTableView() const
{
    return m_qtvTasks;
}

Ms::Widgets::MQueryTableViewWidget<Database::Notification> *Views::ViewMyDashboard::notificationsQueryTableView() const
{
    return m_qtvNotifications;
}

void Views::ViewMyDashboard::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasViewFilesPriv = user->hasPrivilege("View Files");
    bool hasCheckInPriv = user->hasPrivilege("Check In");
    bool hasCheckOutPriv = user->hasPrivilege("Check Out");
    bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

    bool showTaskFilesButton = hasViewFilesPriv || hasCheckInPriv || hasCheckOutPriv || hasCreateRepoPriv;//if have any of the privileges
    m_btnTaskFiles->setHidden(!showTaskFilesButton);
}

Wt::Signal<> &Views::ViewMyDashboard::onTabMyTasksSelected()
{
    return m_onTabMyTasksSelected;
}

Wt::Signal<> &Views::ViewMyDashboard::onTabNotificationsSelected()
{
    return m_onTabNotificationsSelected;
}

void Views::ViewMyDashboard::createTasksTableView()
{
    m_qtvTasks = new Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask>(Session::SessionManager::instance().dboSession());
    m_qtvTasks->setRowHeight(160);
    m_qtvTasks->setSelectionMode(Wt::ExtendedSelection);
    m_qtvTasks->setDynamicSortFilter(true);
    m_qtvTasks->setFilterRole(Wt::DisplayRole);
    m_qtvTasks->setDefaultFilterColumnIndex(0);
    m_qtvTasks->setIgnoreNumFilterColumns(1);
    m_qtvTasks->setImportCSVFeatureEnabled(false);

    m_btnTaskFiles = m_qtvTasks->createToolButton("", "icons/Files.png", "Files Manager");
    m_btnTaskFiles->clicked().connect(this, &Views::ViewMyDashboard::btnTasksFilesClicked);

    updateTasksView();
}

void Views::ViewMyDashboard::btnTasksFilesClicked()
{
    if(m_qtvTasks->table()->selectedIndexes().size() != 1)
    {
        m_logger->log("Please select only one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgTaskSelectDbo *dlgSelectDbo = new Views::DlgTaskSelectDbo();

    dlgSelectDbo->finished().connect(std::bind([=]()
    {
        if(dlgSelectDbo->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            Wt::Dbo::ptr<Projects::ProjectTask> taskPtr =  m_qtvTasks->selectedItems().at(0);

            std::string path = "";

            if(dlgSelectDbo->type() == "Project")
            {
                if(taskPtr->project().get())
                    path = Projects::ProjectsIO::getRelativeProjectTaskDir(taskPtr->project()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                else
                    m_logger->log("Task does not belong to a project.", Ms::Log::LogMessageType::Warning);m_stkMain->setCurrentWidget(m_qtvNotifications);
                updateNotificationsView();

                m_onTabNotificationsSelected();
            }
            else if(dlgSelectDbo->type() == "Sequence")
            {
                if(taskPtr->sequence().get())
                    path = Projects::ProjectsIO::getRelativeSequenceTaskDir(taskPtr->sequence()->projectName(), taskPtr->sequence()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                else
                    m_logger->log("Task does not belong to a sequence.", Ms::Log::LogMessageType::Warning);
            }
            else if(dlgSelectDbo->type() == "Shot")
            {
                if(taskPtr->shot().get())
                    path = Projects::ProjectsIO::getRelativeShotTaskDir(taskPtr->shot()->projectName(), taskPtr->shot()->sequenceName(), taskPtr->shot()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                else
                    m_logger->log("Task does not belong to a shot.", Ms::Log::LogMessageType::Warning);
            }
            else if(dlgSelectDbo->type() == "Asset")
            {
                if(taskPtr->asset().get())
                    path = Projects::ProjectsIO::getRelativeAssetTaskDir(taskPtr->asset()->projectName(), taskPtr->asset()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                else
                    m_logger->log("Task does not belong to an asset.", Ms::Log::LogMessageType::Warning);
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

void Views::ViewMyDashboard::mnuNavBarMainMyTasksItemTriggered()
{
    m_stkMain->setCurrentWidget(m_qtvTasks);
    updateTasksView();

    m_onTabMyTasksSelected();
}

void Views::ViewMyDashboard::createNotificationsTableView()
{
    m_qtvNotifications = new Ms::Widgets::MQueryTableViewWidget<Database::Notification>(Session::SessionManager::instance().dboSession());
    m_qtvNotifications->setRowHeight(25);
    m_qtvNotifications->setSelectionMode(Wt::ExtendedSelection);
    m_qtvNotifications->setDynamicSortFilter(true);
    m_qtvNotifications->setFilterRole(Wt::DisplayRole);
    m_qtvNotifications->setDefaultFilterColumnIndex(0);
    m_qtvNotifications->setImportCSVFeatureEnabled(false);

    updateNotificationsView();
}

void Views::ViewMyDashboard::updateNotificationsCount()
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::ptr<Users::User> user = Session::SessionManager::instance().user();

        std::string queryStr = "SELECT COUNT(n.id) FROM notification n WHERE n.id IN (SELECT un.notification_id FROM rel_user_notifications un WHERE un.user_Name = '" + user->name() +
                "') AND n.Date_Created > ?";

        //only load active data if selected from settings
        if(!AppSettings::instance().isLoadInactiveData())
            queryStr += " And Active = True";

        Wt::Dbo::Query<int> notifCountQuery = Session::SessionManager::instance().dboSession().query<int>(queryStr).bind(user->lastSeenNotificationsDate());


        notifCountQuery.limit(500);

        //transaction.commit();

        int count = notifCountQuery.resultValue();

        m_mnuNavBarMainNotificationsItem->setText("Notifications-[" + std::to_string(count) + "]");

    }
    catch(Wt::Dbo::Exception ex)
    {
        std::cout << "Error occured while updating my dashboard view!" << std::endl << ex.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Error occured while updating my dashboard view!" << std::endl;
    }
}

void Views::ViewMyDashboard::resetUserLastSeenNotificationDate()
{
    Wt::Dbo::ptr<Users::User> user = Session::SessionManager::instance().user();

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    //reset user last seen notification date when user clicks the notification tab
    user.modify()->setLastSeenNotificationsDate(Wt::WDateTime::currentDateTime());

    transaction.commit();
}

void Views::ViewMyDashboard::m_mnuNavBarMainNotificationsItemTriggered()
{
    m_stkMain->setCurrentWidget(m_qtvNotifications);

    updateNotificationsView();

    resetUserLastSeenNotificationDate();

    m_onTabNotificationsSelected();
}

void Views::ViewMyDashboard::createPropertiesView()
{
    m_cntPropertiesMain = new Wt::WContainerWidget();
    m_propertiesPanel->addPropertiesView("MyDashboard", m_cntPropertiesMain);
}

void Views::ViewMyDashboard::prepareView()
{
    /*******************--MyTasks--********************/
    setTitle("<b><i>My Dashboard</i></b>");

    Wt::WVBoxLayout *m_layMain = dynamic_cast<Wt::WVBoxLayout*>(layout());
    m_layMain->setContentsMargins(14,0,14,14);

    m_navBarMain = new Wt::WNavigationBar();

    m_cntNavBarMain = new Wt::WContainerWidget();
    m_cntNavBarMain->addWidget(m_navBarMain);

    //add our navigation bar to the view
    m_layMain->addWidget(m_cntNavBarMain);

    m_mnuNavBarMain = new Wt::WMenu(Wt::Horizontal);

    m_navBarMain->addMenu(m_mnuNavBarMain);

    m_mnuNavBarMainMyTasksItem = new Wt::WMenuItem("My Tasks");
    m_mnuNavBarMainMyTasksItem->triggered().connect(this, &Views::ViewMyDashboard::mnuNavBarMainMyTasksItemTriggered);

    m_mnuNavBarMain->addItem(m_mnuNavBarMainMyTasksItem);

    m_mnuNavBarMainNotificationsItem = new Wt::WMenuItem("Notifications");
    m_mnuNavBarMainNotificationsItem->triggered().connect(this, &Views::ViewMyDashboard::m_mnuNavBarMainNotificationsItemTriggered);

    m_mnuNavBarMain->addItem(m_mnuNavBarMainNotificationsItem);

    m_stkMain = new Wt::WStackedWidget();
    m_stkMain->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);
    m_layMain->addWidget(m_stkMain, 1);

    createTasksTableView();
    m_stkMain->addWidget(m_qtvTasks);

    createNotificationsTableView();
    m_stkMain->addWidget(m_qtvNotifications);

    updateNotificationsCount();

    /*******************--Properties--********************/
    createPropertiesView();
}
