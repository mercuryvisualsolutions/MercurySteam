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
        void showPropertiesView();

        bool isTasksViewShown() const;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *tasksQueryTableView() const;

        //functions
        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

        //Signals
        Wt::Signal<> &onTabMyTasksSelected();

    private:
        //variables
        
        Log::Logger *m_logger;
        Ms::Widgets::MPropertiesPanel *m_propertiesPanel;

        //Signals
        Wt::Signal<> m_onTabMyTasksSelected;

        /*******************--Main--********************/
        Wt::WNavigationBar *m_navBarMain;
        Wt::WContainerWidget *m_cntNavBarMain;
        Wt::WMenu *m_mnuNavBarMain;
        Wt::WMenuItem *m_mnuNavBarMainMyTasksItem;
        Wt::WStackedWidget *m_stkMain;

        /*******************--MyTasks--********************/
        Wt::WVBoxLayout *m_layTasks;
        Wt::WContainerWidget *m_cntTasks;//container for task view
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *m_qtvTasks;
        Wt::WPushButton *m_btnTaskFiles;

        /*******************--Properties--********************/
        Wt::WContainerWidget *m_cntPropertiesMain;

        /*******************--Tasks--********************/
        //functions
        void createTasksTableView();

        //slots
        void btnTasksFilesClicked();
        void mnuNavBarMainMyTasksItemTriggered();

        /*******************--Properties--********************/
        void createPropertiesView();

        void prepareView();
    };
}

#endif // VIEWMYDASHBOARD_H
