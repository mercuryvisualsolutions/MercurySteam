#ifndef VIEWMYDASHBOARD_H
#define VIEWMYDASHBOARD_H

#include "Database/dbtables.h"
#include "../../Log/logger.h"
#include "../../Session/sessionmanager.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WStackedWidget>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include <Wt/WToolBar>
#include <Wt/WNavigationBar>
#include <Wt/WTableView>
#include <Wt/Dbo/QueryModel>
#include <Wt/WSortFilterProxyModel>
#include <Wt/WStandardItemModel>
#include <Wt/Auth/User>

namespace Views
{
    class ViewMyDashboard : public Ms::Widgets::MContainerWidget
    {
    public:
        ViewMyDashboard();

        //functions
        void updateView();
        void updateTasksView();
        void updateNotificationsView();
        void showPropertiesView();

        bool isTasksViewShown() const;
        bool isNotificationsViewShown() const;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *tasksQueryTableView() const;
        Ms::Widgets::MQueryTableViewWidget<Database::Notification> *notificationsQueryTableView() const;

        //functions
        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);
        void resetUserLastSeenNotificationDate();

        //Signals
        Wt::Signal<> &onTabMyTasksSelected();
        Wt::Signal<> &onTabNotificationsSelected();

    private:
        //variables
        
        Log::Logger *m_logger;
        Ms::Widgets::MPropertiesPanel *m_propertiesPanel;

        //Signals
        Wt::Signal<> m_onTabMyTasksSelected;
        Wt::Signal<> m_onTabNotificationsSelected;

        /*******************--Main--********************/
        Wt::WNavigationBar *m_navBarMain;
        Wt::WContainerWidget *m_cntNavBarMain;
        Wt::WMenu *m_mnuNavBarMain;
        Wt::WMenuItem *m_mnuNavBarMainMyTasksItem;
        Wt::WMenuItem *m_mnuNavBarMainNotificationsItem;
        Wt::WStackedWidget *m_stkMain;

        /*******************--MyTasks--********************/
        Wt::WVBoxLayout *m_layTasks;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *m_qtvTasks;
        Wt::WPushButton *m_btnTaskFiles;

        /*******************--Notifications--********************/
        Wt::WVBoxLayout *m_layNotifications;
        Ms::Widgets::MQueryTableViewWidget<Database::Notification> *m_qtvNotifications;

        /*******************--Properties--********************/
        Wt::WContainerWidget *m_cntPropertiesMain;

        /*******************--Tasks--********************/
        //functions
        void createTasksTableView();

        //slots
        void btnTasksFilesClicked();
        void mnuNavBarMainMyTasksItemTriggered();

        /*******************--Notifications--********************/
        //functions
        void createNotificationsTableView();
        void updateNotificationsCount();

        //slots
        void m_mnuNavBarMainNotificationsItemTriggered();

        /*******************--Properties--********************/
        void createPropertiesView();

        void prepareView();
    };
}

#endif // VIEWMYDASHBOARD_H
