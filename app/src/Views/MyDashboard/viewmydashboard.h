#ifndef VIEWMYDASHBOARD_H
#define VIEWMYDASHBOARD_H

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

#include "Database/dbtables.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>

namespace Views
{
    class ViewMyDashboard : public Wt::WContainerWidget
    {
    public:
        ViewMyDashboard();

        //functions
        void updateView();
        void updateTasksView();
        bool isTasksViewShown() const;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *tasksQueryTableView() const;

        //Signals
        Wt::Signal<> &onTabMyTasksSelected();

    private:
        //variables
        //Signals
        Wt::Signal<> _onTabMyTasksSelected;

        /*******************--Main--********************/
        Wt::WText *_txtMyDashboardPanelTitle;
        Wt::WContainerWidget *_cntTxtMyDashboardPanelTitle;
        Wt::WNavigationBar *_navBarMain;
        Wt::WContainerWidget *_cntNavBarMain;
        Wt::WMenu *_mnuNavBarMain;
        Wt::WMenuItem *_mnuNavBarMainMyTasksItem;
        Wt::WStackedWidget *_stkMain;

        /*******************--MyTasks--********************/
        Wt::WVBoxLayout *_layMain;

        Wt::WVBoxLayout *_layTasks;
        Wt::WContainerWidget *_cntTasks;//container for task view
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *_qtvTasks;

        /*******************--Tasks--********************/
        //functions
        void _createTasksTableView();

        //slots
        void _btnTasksFilesClicked();
        void _btnTasksReloadClicked();
        void _mnuNavBarMainMyTasksItemTriggered();

        void _prepareView();
    };
}

#endif // VIEWMYDASHBOARD_H
