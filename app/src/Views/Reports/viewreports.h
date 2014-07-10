#ifndef VIEWREPORTS_H
#define VIEWREPORTS_H

#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WCheckBox>
#include <Wt/WStackedWidget>
#include <Wt/WNavigationBar>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include <Wt/WApplication>

namespace Views
{
    class ViewReports : public Wt::WContainerWidget
    {
    public:
        ViewReports();
        //functions
        void showView(const std::string &view);

    private:
        //variables
        /*******************--Reports--********************/
        Wt::WVBoxLayout *_layMain;//main layout
        Wt::WStackedWidget *_stkReports; //for switching reports
        Wt::WNavigationBar *_navBarReports;//reports navigation bar
        Wt::WContainerWidget *_cntNavBarReports;//container for navBarReports
        Wt::WMenu *_mnuReports;//reports menu (holding, Projects, Users..etc)
        Wt::WMenuItem *_mnuReportsProjectsItem;
        Wt::WMenuItem *_mnuReportsUsersItem;

        //Projects
        Wt::WVBoxLayout *_layReportsProjects;
        Wt::WContainerWidget *_cntReportsProjects;//container for projects reports view

        //users
        Wt::WVBoxLayout *_layReportsUsers;
        Wt::WContainerWidget *_cntReportsUsers;//container for users reports view

        //functions
        //slots
        //general
        void _mnuReportsProjectsItemTriggered();
        void _mnuReportsUsersItemTriggered();
        void _prepareView();
    };
}

#endif // VIEWREPORTS_H
