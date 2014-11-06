#ifndef VIEWAPP_H
#define VIEWAPP_H

#include <Wt/WNavigationBar>
#include <Wt/WMenu>
#include <Wt/WPopupMenu>
#include <Wt/WPopupMenuItem>
#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WText>
#include <Wt/WStackedWidget>
#include <Wt/Auth/Login>

#include "../Projects/viewprojects.h"
#include "../Users/viewusers.h"
#include "../Reports/viewreports.h"
#include "../Settings/viewsettings.h"
#include "../Search/viewsearch.h"
#include "../MyDashboard/viewmydashboard.h"
#include "../Log/viewlog.h"

#include <Ms/Widgets/MPropertiesPanel.h>

namespace Views
{
class ViewApp : public Wt::WContainerWidget
{
    public:
        ViewApp();
        //functions
        void setLoginObject(Wt::Auth::Login *login);

        //views
        void showAuthView();
        void showProjectsView();
        void showUsersView();
        void showReportsView(const std::string &subView);
        void showSearchView(const std::string &subView);
        void showMyDashboardView();
        void showSettingsView();

    private:
        //variables
        Log::Logger *_logger;
        //Ms
        Ms::Widgets::MPropertiesPanel *_propertiesPanel;

        /*******************--Main--********************/
        Wt::WVBoxLayout *_layMain;//main layout
        Wt::WHBoxLayout *_layMainH;//horizontal layout child of layMain
        Wt::WContainerWidget *_cntLeftMenu;//mnuleft container
        Wt::WVBoxLayout *_layCntLeftMenu;//vertical layout child of layMainH

        /*******************--Main Navigation Bar--********************/
        Wt::WContainerWidget *_cntNavBarMain; //main navigation bar container
        Wt::WNavigationBar *_navBarMain; //main navigation bar
        Wt::WMenu *_mnuMainLeft;//main menu in main navigation bar
        Wt::WMenuItem *_mnuMainLeftHelpItem;//help menu item
        Wt::WPopupMenu *_mnuMainLeftHelpSub;//help menu sub menu
        Wt::WMenuItem *_mnuMainLeftHelpAbout;//about menu item

        Wt::WMenuItem *_mnuMainLeftViewItem;//view menu item
        Wt::WPopupMenu *_mnuMainLeftViewSub;//view menu sub menu
        Wt::WMenuItem *_mnuMainLeftViewPanelsItem;//panels menu item
        Wt::WPopupMenu *_mnuMainLeftViewPanelsSub;//view panels menu sub menu
        Wt::WMenuItem *_mnuMainLeftViewShowMenuPanel;//show menu panel menu item
        Wt::WMenuItem *_mnuMainLeftViewShowPropertiesPanel;//show properties Panel menu item
        Wt::WMenuItem *_mnuMainLeftViewShowLogPanel;//show log Panel menu item

        Wt::WMenu *_mnuMainRight;//main menu in main navigation bar
        Wt::WMenuItem *_mnuMainRightCurrentUserItem;//CurrentUser menu item
        Wt::WPopupMenu *_mnuMainRightCurrentUserSub;//CurrentUser menu sub menu
        Wt::WMenuItem *_mnuMainRightCurrentUserSignOut;//sign out menu item

        /*******************--Main Left Menu--********************/
        Wt::WContainerWidget *_cntMnuSideMain; //main side menu container
        Wt::WMenu *_mnuSideMain;//main side menu
        Wt::WMenuItem *_mnuSideMainProjectsItem;//projects menu item
        Wt::WMenuItem *_mnuSideMainUsersAndGroupsItem;//users and groups menu item
        Wt::WMenuItem *_mnuSideMainReportsItem;//reports menu item
        Wt::WMenuItem *_mnuSideMainSearchItem;// search menu item
        Wt::WMenuItem *_mnuSideMainMyDashboardItem;// MyDashboard menu item
        Wt::WMenuItem *_mnuSideMainSettingsItem;//settings menu item

        /*******************--Child View (Projects, Accounts, Reports..etc)--********************/
        Wt::WContainerWidget *_cntChildViewsAndProperties;
        Wt::WHBoxLayout *_layCntChildViewsAndProperties;
        Wt::WContainerWidget *_cntChildViewsAndPropertiesAndLog;
        Wt::WVBoxLayout *_layCntChildViewsAndPropertiesAndLog;

        Wt::WStackedWidget *_stkMainView;

        /*******************--Child Views--********************/
        ViewProjects *_viwProjects;//Projects View
        ViewUsers *_viwUsers;//Accounts View
        ViewReports *_viwReports;//Reports View
        ViewSearch *_viwSearch;//Search View
        ViewMyDashboard *_viwMyDashboard;//MyDashboard View
        ViewSettings *_viwSettings;//Settings View
        ViewLog *_viwLog;//Log View

        /*******************--Properties View--********************/
        Wt::WContainerWidget *_cntPropertiesEmpty;

        /*******************--Footer--********************/
        Wt::WText *_txtFooter;//footer text
        Wt::WContainerWidget *_cntTxtFooter;//container for txtFooter

        //functions
        //slots
        //app
        void _globalAppKeyWentDown(Wt::WKeyEvent key);
        void _globalAppKeyWentUp(Wt::WKeyEvent key);
        //left menu
        void _mnuSideMainProjectsItemTriggered();
        void _mnuSideMainUsersAndGroupsItemTriggered();
        void _mnuSideMainReportsItemTriggered();
        void _mnuSideMainSearchItemTriggered();
        void _mnuSideMainMyDashboardItemTriggered();
        void _mnuSideMainSettingsItemTriggered();

        //main application menu
        void _mnuMainLeftHelpAboutTriggered();
        void _toggleLeftMenuView();
        void _togglePropertiesPanel();
        void _toggleLogPanel();
        void _mnuMainRightCurrentUserSignOutTriggered();

        //functions
        void _prepareView();
        void _prepareChildViews(Wt::WStackedWidget *widget);

        //auth
        Wt::Auth::Login *_login;
    };
}

#endif // VIEWAPP_H
