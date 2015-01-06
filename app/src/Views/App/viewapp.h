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
        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

    private:
        //variables
        Log::Logger *m_logger;
        //Ms
        Ms::Widgets::MPropertiesPanel *m_propertiesPanel;

        /*******************--Main--********************/
        Wt::WVBoxLayout *m_layMain;//main layout
        Wt::WHBoxLayout *m_layMainH;//horizontal layout child of layMain
        Wt::WContainerWidget *m_cntLeftMenu;//mnuleft container
        Wt::WVBoxLayout *m_layCntLeftMenu;//vertical layout child of layMainH

        /*******************--Main Navigation Bar--********************/
        Wt::WContainerWidget *m_cntNavBarMain; //main navigation bar container
        Wt::WNavigationBar *m_navBarMain; //main navigation bar
        Wt::WMenu *m_mnuMainLeft;//main menu in main navigation bar
        Wt::WMenuItem *m_mnuMainLeftHelpItem;//help menu item
        Wt::WPopupMenu *m_mnuMainLeftHelpSub;//help menu sub menu
        Wt::WMenuItem *m_mnuMainLeftHelpAbout;//about menu item

        Wt::WMenuItem *m_mnuMainLeftViewItem;//view menu item
        Wt::WPopupMenu *m_mnuMainLeftViewSub;//view menu sub menu
        Wt::WMenuItem *m_mnuMainLeftViewPanelsItem;//panels menu item
        Wt::WPopupMenu *m_mnuMainLeftViewPanelsSub;//view panels menu sub menu
        Wt::WMenuItem *m_mnuMainLeftViewShowMenuPanel;//show menu panel menu item
        Wt::WMenuItem *m_mnuMainLeftViewShowPropertiesPanel;//show properties Panel menu item
        Wt::WMenuItem *m_mnuMainLeftViewShowLogPanel;//show log Panel menu item

        Wt::WImage *m_imgUserAvatar;//shows current user avatar

        Wt::WMenu *m_mnuMainRight;//main menu in main navigation bar
        Wt::WMenuItem *m_mnuMainRightCurrentUserItem;//CurrentUser menu item
        Wt::WPopupMenu *m_mnuMainRightCurrentUserSub;//CurrentUser menu sub menu
        Wt::WMenuItem *m_mnuMainRightCurrentUserSignOut;//sign out menu item

        /*******************--Main Left Menu--********************/
        Wt::WContainerWidget *m_cntMnuSideMain; //main side menu container
        Wt::WMenu *m_mnuSideMain;//main side menu
        Wt::WMenuItem *m_mnuSideMainProjectsItem;//projects menu item
        Wt::WMenuItem *m_mnuSideMainUsersAndGroupsItem;//users and groups menu item
        Wt::WMenuItem *m_mnuSideMainReportsItem;//reports menu item
        Wt::WMenuItem *m_mnuSideMainSearchItem;// search menu item
        Wt::WMenuItem *m_mnuSideMainMyDashboardItem;// MyDashboard menu item
        Wt::WMenuItem *m_mnuSideMainSettingsItem;//settings menu item

        /*******************--Child View (Projects, Accounts, Reports..etc)--********************/
        Wt::WContainerWidget *m_cntChildViewsAndProperties;
        Wt::WHBoxLayout *m_layCntChildViewsAndProperties;
        Wt::WContainerWidget *m_cntChildViewsAndPropertiesAndLog;
        Wt::WVBoxLayout *m_layCntChildViewsAndPropertiesAndLog;

        Wt::WStackedWidget *m_stkMainView;

        /*******************--Child Views--********************/
        ViewProjects *m_viewProjects;//Projects View
        ViewUsers *m_viewUsers;//Accounts View
        ViewReports *m_viewReports;//Reports View
        ViewSearch *m_viewSearch;//Search View
        ViewMyDashboard *m_viewMyDashboard;//MyDashboard View
        ViewSettings *m_viewSettings;//Settings View
        ViewLog *m_viewLog;//Log View

        /*******************--Properties View--********************/
        Wt::WContainerWidget *m_cntPropertiesEmpty;

        /*******************--Footer--********************/
        Wt::WText *m_txtFooter;//footer text
        Wt::WContainerWidget *m_cntTxtFooter;//container for txtFooter

        //functions
        //slots
        //app
        void globalAppKeyWentDown(Wt::WKeyEvent key);
        void globalAppKeyWentUp(Wt::WKeyEvent key);
        //left menu
        void mnuSideMainProjectsItemTriggered();
        void mnuSideMainUsersAndGroupsItemTriggered();
        void mnuSideMainReportsItemTriggered();
        void mnuSideMainSearchItemTriggered();
        void mnuSideMainMyDashboardItemTriggered();
        void mnuSideMainSettingsItemTriggered();

        //main application menu
        void mnuMainLeftHelpAboutTriggered();
        void toggleLeftMenuView();
        void togglePropertiesPanel();
        void toggleLogPanel();
        void mnuMainRightCurrentUserSignOutTriggered();

        //functions
        void prepareView();
        void prepareChildViews(Wt::WStackedWidget *widget);

        //session handler
        
    };
}

#endif // VIEWAPP_H
