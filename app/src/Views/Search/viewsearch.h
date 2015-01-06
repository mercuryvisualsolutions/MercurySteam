#ifndef VIEWSEARCH_H
#define VIEWSEARCH_H

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
    class ViewSearch : public Wt::WContainerWidget
    {
    public:
        ViewSearch();
        //functions
        void showView(const std::string &view);

    private:
        //variables
        /*******************--Search--********************/
        Wt::WVBoxLayout *m_layMain;//main layout
        Wt::WStackedWidget *m_stkSearch; //for switching Search
        Wt::WNavigationBar *m_navBarSearch;//Search navigation bar
        Wt::WContainerWidget *m_cntNavBarSearch;//container for navBarSearch
        Wt::WMenu *m_mnuSearch;//Search menu (holding, Projects, Users..etc)
        Wt::WMenuItem *m_mnuSearchProjectsItem;
        Wt::WMenuItem *m_mnuSearchUsersItem;

        //Projects
        Wt::WVBoxLayout *m_laySearchProjects;
        Wt::WContainerWidget *m_cntSearchProjects;//container for projects Search view

        //users
        Wt::WVBoxLayout *m_laySearchUsers;
        Wt::WContainerWidget *m_cntSearchUsers;//container for users Search view

        //functions
        //slots
        //general
        void mnuSearchProjectsItemTriggered();
        void mnuSearchUsersItemTriggered();
        void prepareView();
    };
}

#endif // VIEWSEARCH_H
