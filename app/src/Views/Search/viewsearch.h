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
        Wt::WVBoxLayout *_layMain;//main layout
        Wt::WStackedWidget *_stkSearch; //for switching Search
        Wt::WNavigationBar *_navBarSearch;//Search navigation bar
        Wt::WContainerWidget *_cntNavBarSearch;//container for navBarSearch
        Wt::WMenu *_mnuSearch;//Search menu (holding, Projects, Users..etc)
        Wt::WMenuItem *_mnuSearchProjectsItem;
        Wt::WMenuItem *_mnuSearchUsersItem;

        //Projects
        Wt::WVBoxLayout *_laySearchProjects;
        Wt::WContainerWidget *_cntSearchProjects;//container for projects Search view

        //users
        Wt::WVBoxLayout *_laySearchUsers;
        Wt::WContainerWidget *_cntSearchUsers;//container for users Search view

        //functions
        //slots
        //general
        void _mnuSearchProjectsItemTriggered();
        void _mnuSearchUsersItemTriggered();
        void _prepareView();
    };
}

#endif // VIEWSEARCH_H
