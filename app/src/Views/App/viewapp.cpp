#include "viewapp.h"

#include "Auth/authmanager.h"
#include "Users/usersmanager.h"
#include "Projects/projectsmanager.h"
#include "Database/databasemanager.h"
#include "Settings/appsettings.h"
#include "../../Log/logmanager.h"
#include "../../Session/sessionmanager.h"

#include <Wt/WMessageBox>
#include <Wt/WApplication>

Views::ViewApp::ViewApp()
: WContainerWidget()
{
    _logger = Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId());
    _propertiesPanel = Session::SessionManager::instance().getSessionPropertiesPanel(Wt::WApplication::instance()->sessionId());

    _prepareView();

    _mnuSideMain->select(_mnuSideMainProjectsItem);//default start to the main projects page
}

void Views::ViewApp::showAuthView()
{
    if(Auth::AuthManager::instance().login().loggedIn())
        Auth::AuthManager::instance().login().logout();
}

void Views::ViewApp::showProjectsView()
{
    //_mnuSideMain->select(_mnuSideMainProjectsItem);
    if(_stkMainView->currentWidget() != _viwProjects)
    {
        _stkMainView->setCurrentWidget(_viwProjects);
        _viwProjects->showPropertiesView();
        _viwProjects->updateView();
    }
}

void Views::ViewApp::showUsersView()
{
    //_mnuSideMain->select(_mnuSideMainUsersAndGroupsItem);
    if(_stkMainView->currentWidget() != _viwUsers)
    {
        _stkMainView->setCurrentWidget(_viwUsers);
        _viwUsers->showPropertiesView();
        _viwUsers->updateView();
    }
}

void Views::ViewApp::showReportsView(const std::string &subView)
{
    if(_stkMainView->currentWidget() != _viwReports)
    {
        //_mnuSideMain->select(_mnuSideMainReportsItem);
        _stkMainView->setCurrentWidget(_viwReports);
    }
}

void Views::ViewApp::showSearchView(const std::string &subView)
{
    if(_stkMainView->currentWidget() != _viwSearch)
    {
        //_mnuSideMain->select(_mnuSideMainSearchItem);
        _stkMainView->setCurrentWidget(_viwSearch);
    }
}

void Views::ViewApp::showMyDashboardView()
{
    if(_stkMainView->currentWidget() != _viwMyDashboard)
    {
        //_mnuSideMain->select(_mnuSideMainMyDashboardItem);
        _stkMainView->setCurrentWidget(_viwMyDashboard);
        _viwMyDashboard->showPropertiesView();
        _viwMyDashboard->updateView();
    }
}

void Views::ViewApp::showSettingsView()
{
    if(_stkMainView->currentWidget() != _viwSettings)
    {
        //_mnuSideMain->select(_mnuSideMainSettingsItem);
        _stkMainView->setCurrentWidget(_viwSettings);
        _viwSettings->showPropertiesView();
        _viwSettings->updateView();
    }
}

void Views::ViewApp::_mnuMainLeftHelpAboutTriggered()
{
    Wt::WMessageBox *msg = new Wt::WMessageBox("About", "MercurySteam (c) Ver 1.0<p>Coder: Mohamed Samir</p>", Wt::Information, Wt::Ok);
    msg->buttonClicked().connect(std::bind([=]()
    {
        delete msg;
    }));
    msg->show();
}

void Views::ViewApp::_mnuMainLeftViewShowMenuPanelTriggered()
{
    if(_mnuMainLeftViewShowMenuPanel->isChecked())
        _cntLeftMenu->show();
    else
        _cntLeftMenu->hide();
}

void Views::ViewApp::_mnuMainLeftViewShowPropertiesPanelTriggered()
{
    if(_mnuMainLeftViewShowPropertiesPanel->isChecked())
    {
        _propertiesPanel->show();
    }
    else
    {
        _propertiesPanel->hide();
    }
}

void Views::ViewApp::_mnuMainLeftViewShowLogPanelTriggered()
{
    if(_mnuMainLeftViewShowLogPanel->isChecked())
        _viwLog->show();
    else
        _viwLog->hide();
}

void Views::ViewApp::_mnuMainRightCurrentUserSignOutTriggered()
{
    showAuthView();
}

void Views::ViewApp::_mnuSideMainProjectsItemTriggered()
{
    showProjectsView();    
}

void Views::ViewApp::_mnuSideMainUsersAndGroupsItemTriggered()
{
    showUsersView();
}

void Views::ViewApp::_mnuSideMainReportsItemTriggered()
{
    showReportsView("projects");
}

void Views::ViewApp::_mnuSideMainSearchItemTriggered()
{
    showSearchView("projects");
}

void Views::ViewApp::_mnuSideMainMyDashboardItemTriggered()
{
    showMyDashboardView();
}

void Views::ViewApp::_mnuSideMainSettingsItemTriggered()
{
    showSettingsView();
}

void Views::ViewApp::_prepareView()
{
    /*******************--Layouts--********************/
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);
    setLayout(_layMain);

    _layMainH = new Wt::WHBoxLayout();
    _layMainH->setContentsMargins(0,0,0,0);
    _layMainH->setSpacing(0);

    _cntLeftMenu = new Wt::WContainerWidget();

    _layCntLeftMenu = new Wt::WVBoxLayout();
    _layCntLeftMenu->setContentsMargins(0,0,0,0);
    _layCntLeftMenu->setSpacing(0);

    _cntLeftMenu->setLayout(_layCntLeftMenu);
    _layMainH->addWidget(_cntLeftMenu);

    /*******************--Main Navigation Bar--********************/

    _navBarMain = new Wt::WNavigationBar();
    _navBarMain->setTitle("MercurySteam", "http://www.mercuryvfx.com");
    //_navBarMain->setResponsive(true);
    _layMain->addWidget(_navBarMain);

    //add _layMainH to _layMain
    _layMain->addLayout(_layMainH, 1);

    //main menu
    _mnuMainLeft = new Wt::WMenu();
    _navBarMain->addMenu(_mnuMainLeft, Wt::AlignLeft);
    //mnuMain->setInternalPathEnabled();

    //main menu view menuItem
    _mnuMainLeftViewItem = new Wt::WMenuItem("VIEW");
    _mnuMainLeft->addItem(_mnuMainLeftViewItem);//add the mnuMainViewItem to mnuMain

    //main menu view menuItem sub menu
    _mnuMainLeftViewSub = new Wt::WPopupMenu();
    _mnuMainLeftViewItem->setMenu(_mnuMainLeftViewSub);//add mnuMainViewSub to mnuMainViewItem

    _mnuMainLeftViewPanelsItem = new Wt::WMenuItem("Panels");
    _mnuMainLeftViewSub->addItem(_mnuMainLeftViewPanelsItem);

    _mnuMainLeftViewPanelsSub = new Wt::WPopupMenu();
    _mnuMainLeftViewPanelsItem->setSubMenu(_mnuMainLeftViewPanelsSub);

    _mnuMainLeftViewShowMenuPanel= new Wt::WMenuItem("Main Menu");
    _mnuMainLeftViewShowMenuPanel->setCheckable(true);
    _mnuMainLeftViewShowMenuPanel->setChecked(true);
    _mnuMainLeftViewShowMenuPanel->triggered().connect(this, &Views::ViewApp::_mnuMainLeftViewShowMenuPanelTriggered);

    _mnuMainLeftViewPanelsSub->addItem(_mnuMainLeftViewShowMenuPanel);//add "Show menu panel" item to mnuMainViewSub

    _mnuMainLeftViewShowPropertiesPanel= new Wt::WMenuItem("Properties");
    _mnuMainLeftViewShowPropertiesPanel->setCheckable(true);
    _mnuMainLeftViewShowPropertiesPanel->setChecked(true);
    _mnuMainLeftViewShowPropertiesPanel->triggered().connect(this, &Views::ViewApp::_mnuMainLeftViewShowPropertiesPanelTriggered);

    _mnuMainLeftViewPanelsSub->addItem(_mnuMainLeftViewShowPropertiesPanel);//add "Show properties panel" item to mnuMainViewSub

    _mnuMainLeftViewShowLogPanel = new Wt::WMenuItem("Log");
    _mnuMainLeftViewShowLogPanel->setCheckable(true);
    _mnuMainLeftViewShowLogPanel->setChecked(true);
    _mnuMainLeftViewShowLogPanel->triggered().connect(this, &Views::ViewApp::_mnuMainLeftViewShowLogPanelTriggered);

    _mnuMainLeftViewPanelsSub->addItem(_mnuMainLeftViewShowLogPanel);//add "Show log panel" item to mnuMainViewSub

    //main menu help menuItem
    _mnuMainLeftHelpItem = new Wt::WMenuItem("HELP");
    _mnuMainLeft->addItem(_mnuMainLeftHelpItem);//add the mnuMainHelpItem to mnuMain

    //main menu help menuItem sub menu
    _mnuMainLeftHelpSub = new Wt::WPopupMenu();
    _mnuMainLeftHelpItem->setMenu(_mnuMainLeftHelpSub);//add mnuMainHelpSub to mnuMainHelpItem

    _mnuMainLeftHelpAbout= new Wt::WMenuItem("About");
    _mnuMainLeftHelpAbout->triggered().connect(this, &Views::ViewApp::_mnuMainLeftHelpAboutTriggered);

    _mnuMainLeftHelpSub->addItem(_mnuMainLeftHelpAbout);//add "About" item to mnuMainHelpSub

    //mnuRight
    _mnuMainRight = new Wt::WMenu();
    _navBarMain->addMenu(_mnuMainRight, Wt::AlignRight);

    _mnuMainRightCurrentUserItem = new Wt::WMenuItem(Auth::AuthManager::instance().login().user().identity(Wt::Auth::Identity::LoginName));
    _mnuMainRight->addItem(_mnuMainRightCurrentUserItem);//add the mnuMainCurrentUserItem to mnuMain

    _mnuMainRightCurrentUserSub = new Wt::WPopupMenu();
    _mnuMainRightCurrentUserItem->setMenu(_mnuMainRightCurrentUserSub);//add mnuMainCurrentUserSub to mnuMainCurrentUserItem

    _mnuMainRightCurrentUserSignOut= new Wt::WMenuItem("Sign Out");
    _mnuMainRightCurrentUserSignOut->triggered().connect(this, &Views::ViewApp::_mnuMainRightCurrentUserSignOutTriggered);

    _mnuMainRightCurrentUserSub->addItem(_mnuMainRightCurrentUserSignOut);//add "Aign out" item to mnuMainCurrentUserSub

    /*******************--Main Left Menu--********************/
    _cntMnuSideMain = new Wt::WContainerWidget();
    _cntMnuSideMain->addStyleClass("panel-border-right");

    _mnuSideMain = new Wt::WMenu(Wt::Vertical);//main side menu
    _mnuSideMain->addStyleClass("nav-pills nav-stacked");
    _mnuSideMain->setWidth(250);
    //_mnuSideMain->setInternalBasePath("/");
    //_mnuSideMain->setInternalPathEnabled();
    _cntMnuSideMain->addWidget(_mnuSideMain);

    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("View"))
    {
        _mnuSideMain->addSectionHeader("Management");

        _mnuSideMainProjectsItem = new Wt::WMenuItem("Projects");//projects menu item
        _mnuSideMainProjectsItem->triggered().connect(this, &Views::ViewApp::_mnuSideMainProjectsItemTriggered);
        _mnuSideMain->addItem(_mnuSideMainProjectsItem);

        _mnuSideMain->addSeparator();

        //_mnuSideMain->addSectionHeader("Security");

        _mnuSideMainUsersAndGroupsItem = new Wt::WMenuItem("Users And Groups");//Users menu item
        _mnuSideMainUsersAndGroupsItem->triggered().connect(this, &Views::ViewApp::_mnuSideMainUsersAndGroupsItemTriggered);
        _mnuSideMain->addItem(_mnuSideMainUsersAndGroupsItem);

        _mnuSideMain->addSeparator();

        //_mnuSideMain->addSectionHeader("Reports");

        //_mnuSideMainReportsItem = new Wt::WMenuItem("Reports");//reports menu item
        //_mnuSideMainReportsItem->triggered().connect(this, &Views::ViewApp::_mnuSideMainReportsItemTriggered);
        //_mnuSideMain->addItem(_mnuSideMainReportsItem);

        //_mnuSideMain->addSectionHeader("Search");

        //_mnuSideMainSearchItem = new Wt::WMenuItem("Search");//search menu item
        //_mnuSideMainSearchItem->triggered().connect(this, &Views::ViewApp::_mnuSideMainSearchItemTriggered);
        //_mnuSideMain->addItem(_mnuSideMainSearchItem);

        _mnuSideMain->addSectionHeader("Settings");

        _mnuSideMainSettingsItem = new Wt::WMenuItem("Settings");//settings menu item
        _mnuSideMainSettingsItem->triggered().connect(this, &Views::ViewApp::_mnuSideMainSettingsItemTriggered);
        _mnuSideMain->addItem(_mnuSideMainSettingsItem);
    }

    _mnuSideMain->addSectionHeader("My Steam");

    _mnuSideMainMyDashboardItem = new Wt::WMenuItem("My Dashboard");//my tasks menu item
    _mnuSideMainMyDashboardItem->triggered().connect(this, &Views::ViewApp::_mnuSideMainMyDashboardItemTriggered);
    _mnuSideMain->addItem(_mnuSideMainMyDashboardItem);

    //add _cntMnuSideMain to _layMainHSideV
    _layCntLeftMenu->addWidget(_cntMnuSideMain);

    //Prepare child view
    _stkMainView = new Wt::WStackedWidget();

    _prepareChildViews(_stkMainView);

    _layMainH->addWidget(_stkMainView, 1);

    /**************Properties View********************/
    _layMainH->addWidget(_propertiesPanel);

    /**************Log Panel********************/
    _viwLog = new ViewLog();
    _layMain->addWidget(_viwLog);

    /*******************--Footer--********************/
    _cntTxtFooter = new Wt::WContainerWidget();
    _cntTxtFooter->setContentAlignment(Wt::AlignCenter);
    _cntTxtFooter->setStyleClass("footer");

    _txtFooter = new Wt::WText("MercurySteam (c) Mercury Visual Solutions.");//footer text
    _txtFooter->setStyleClass("black-text");
    _cntTxtFooter->addWidget(_txtFooter);

    _layMain->addWidget(_cntTxtFooter);
}

void Views::ViewApp::_prepareChildViews(Wt::WStackedWidget *widget)
{
    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("View"))
    {
        _viwUsers = new ViewUsers();

        _viwProjects = new ViewProjects();
        //_viwReports = new ViewReports();
        //_viwSearch = new ViewSearch();
        _viwSettings = new ViewSettings();

        widget->addWidget(_viwProjects);
        widget->addWidget(_viwUsers);
        //widget->addWidget(_viwReports);
        //widget->addWidget(_viwSearch);
        widget->addWidget(_viwSettings);
    }

    _viwMyDashboard = new ViewMyDashboard();

    widget->addWidget(_viwMyDashboard);
}
