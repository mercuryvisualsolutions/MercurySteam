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
        Wt::WVBoxLayout *m_layMain;//main layout
        Wt::WStackedWidget *m_stkReports; //for switching reports
        Wt::WNavigationBar *m_navBarReports;//reports navigation bar
        Wt::WContainerWidget *m_cntNavBarReports;//container for navBarReports
        Wt::WMenu *m_mnuReports;//reports menu (holding, Projects, Users..etc)
        Wt::WMenuItem *m_mnuReportsProjectsItem;
        Wt::WMenuItem *m_mnuReportsUsersItem;

        //Projects
        Wt::WVBoxLayout *m_layReportsProjects;
        Wt::WContainerWidget *m_cntReportsProjects;//container for projects reports view

        //users
        Wt::WVBoxLayout *m_layReportsUsers;
        Wt::WContainerWidget *m_cntReportsUsers;//container for users reports view

        //functions
        //slots
        //general
        void mnuReportsProjectsItemTriggered();
        void mnuReportsUsersItemTriggered();
        void prepareView();
    };
}

#endif // VIEWREPORTS_H
