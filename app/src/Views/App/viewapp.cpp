#include "viewapp.h"

#include "Auth/authmanager.h"
#include "Database/dbosession.h"
#include "Settings/appsettings.h"
#include "../../Log/logmanager.h"
#include "../../Session/sessionmanager.h"

#include <Wt/WMessageBox>
#include <Wt/WApplication>
#include <Wt/WImage>

Views::ViewApp::ViewApp():
    WContainerWidget()
{
    m_logger = Session::SessionManager::instance().logger();
    m_propertiesPanel = Session::SessionManager::instance().propertiesPanel();

    prepareView();

    m_mnuSideMain->select(m_mnuSideMainMyDashboardItem);//default start to the main projects page

    //hide log view by default
    m_viewLog->hide();
    m_layCntChildViewsAndPropertiesAndLog->setResizable(0, false);
}

void Views::ViewApp::showAuthView()
{
    if(Session::SessionManager::instance().login().loggedIn())
        Session::SessionManager::instance().login().logout();
}

void Views::ViewApp::showProjectsView()
{
    m_propertiesPanel->showView(m_viewProjects->id());
    m_stkMainView->setCurrentWidget(m_viewProjects);
    m_viewProjects->updateView();
}

void Views::ViewApp::showUsersView()
{
    m_propertiesPanel->showView(m_viewUsers->id());
    m_stkMainView->setCurrentWidget(m_viewUsers);
    m_viewUsers->updateView();
}

void Views::ViewApp::showReportsView(const std::string &subView)
{
    m_propertiesPanel->showView(m_viewReports->id());
    m_stkMainView->setCurrentWidget(m_viewReports);
}

void Views::ViewApp::showSearchView(const std::string &subView)
{
    m_propertiesPanel->showView(m_viewSearch->id());
    m_stkMainView->setCurrentWidget(m_viewSearch);
}

void Views::ViewApp::showMyDashboardView()
{
    m_propertiesPanel->showView(m_viewMyDashboard->id());
    m_stkMainView->setCurrentWidget(m_viewMyDashboard);
    m_viewMyDashboard->updateView();
}

void Views::ViewApp::showSettingsView()
{
    m_propertiesPanel->showView(m_viewSettings->id());
    m_stkMainView->setCurrentWidget(m_viewSettings);
    m_viewSettings->updateView();
}

void Views::ViewApp::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool hasViewProjectsPriv = user->hasPrivilege("View Projects");
    bool hasViewUsersAndGroupsPriv = user->hasPrivilege("View Users And Groups");
    bool hasViewSettingsPriv = user->hasPrivilege("View Settings");

    m_mnuSideMainProjectsItem->setHidden(!hasViewProjectsPriv);
    m_mnuSideMainUsersAndGroupsItem->setHidden(!hasViewUsersAndGroupsPriv);
    m_mnuSideMainSettingsItem->setHidden(!hasViewSettingsPriv);

    m_mnuMainRightCurrentUserItem->setText(user->name());
    m_imgUserAvatar->setImageLink(Wt::WLink(user->thumbnail()));

    m_viewUsers->adjustUIPrivileges(user);
    m_viewProjects->adjustUIPrivileges(user);
    m_viewSettings->adjustUIPrivileges(user);
    m_viewMyDashboard->adjustUIPrivileges(user);

    transaction.commit();
}

void Views::ViewApp::globalAppKeyWentDown(Wt::WKeyEvent key)
{
    if(((key.modifiers() & Wt::KeyboardModifier::ShiftModifier) == Wt::KeyboardModifier::ShiftModifier) && key.key() == Wt::Key::Key_M)
    {
        m_mnuMainLeftViewShowMenuPanel->setChecked(!m_mnuMainLeftViewShowMenuPanel->isChecked());
        toggleLeftMenuView();
    }
    else if(((key.modifiers() & Wt::KeyboardModifier::ShiftModifier) == Wt::KeyboardModifier::ShiftModifier) && key.key() == Wt::Key::Key_P)
    {
        m_mnuMainLeftViewShowPropertiesPanel->setChecked(!m_mnuMainLeftViewShowPropertiesPanel->isChecked());
        togglePropertiesPanel();
    }
    else if(((key.modifiers() & Wt::KeyboardModifier::ShiftModifier) == Wt::KeyboardModifier::ShiftModifier) && key.key() == Wt::Key::Key_L)
    {
        m_mnuMainLeftViewShowLogPanel->setChecked(!m_mnuMainLeftViewShowLogPanel->isChecked());
        toggleLogPanel();
    }
}

void Views::ViewApp::globalAppKeyWentUp(Wt::WKeyEvent key)
{

}

void Views::ViewApp::mnuMainLeftHelpAboutTriggered()
{
    Wt::WMessageBox *msg = new Wt::WMessageBox("About", "MercurySteam (c) Ver 1.0<p>Coder: Mohamed Samir</p>", Wt::Information, Wt::Ok);
    msg->buttonClicked().connect(std::bind([=]()
    {
        delete msg;
    }));
    msg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewApp::toggleLeftMenuView()
{
    if(m_mnuMainLeftViewShowMenuPanel->isChecked())
        m_cntLeftMenu->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::SlideInFromLeft, Wt::WAnimation::TimingFunction::EaseInOut));
    else
        m_cntLeftMenu->animateHide(Wt::WAnimation(Wt::WAnimation::AnimationEffect::SlideInFromLeft, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewApp::togglePropertiesPanel()
{
    if(m_mnuMainLeftViewShowPropertiesPanel->isChecked())
    {
        m_propertiesPanel->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::SlideInFromRight, Wt::WAnimation::TimingFunction::EaseInOut));
        m_layCntChildViewsAndProperties->setResizable(0, true);
    }
    else
    {
        m_propertiesPanel->animateHide(Wt::WAnimation(Wt::WAnimation::AnimationEffect::SlideInFromRight, Wt::WAnimation::TimingFunction::EaseInOut));
        m_layCntChildViewsAndProperties->setResizable(0, false);
    }
}

void Views::ViewApp::toggleLogPanel()
{
    if(m_mnuMainLeftViewShowLogPanel->isChecked())
    {
        m_viewLog->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::SlideInFromBottom, Wt::WAnimation::TimingFunction::EaseInOut));
        m_layCntChildViewsAndPropertiesAndLog->setResizable(0, true);
    }
    else
    {
        m_viewLog->animateHide(Wt::WAnimation(Wt::WAnimation::AnimationEffect::SlideInFromBottom, Wt::WAnimation::TimingFunction::EaseInOut));
        m_layCntChildViewsAndPropertiesAndLog->setResizable(0, false);
    }
}

void Views::ViewApp::mnuMainRightCurrentUserSignOutTriggered()
{
    showAuthView();
}

void Views::ViewApp::mnuSideMainProjectsItemTriggered()
{
    showProjectsView();    
}

void Views::ViewApp::mnuSideMainUsersAndGroupsItemTriggered()
{
    showUsersView();
}

void Views::ViewApp::mnuSideMainReportsItemTriggered()
{
    showReportsView("projects");
}

void Views::ViewApp::mnuSideMainSearchItemTriggered()
{
    showSearchView("projects");
}

void Views::ViewApp::mnuSideMainMyDashboardItemTriggered()
{
    showMyDashboardView();
}

void Views::ViewApp::mnuSideMainSettingsItemTriggered()
{
    showSettingsView();
}

void Views::ViewApp::prepareView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    /*******************--Application--********************/
    Wt::WApplication::instance()->globalKeyWentDown().connect(this, &Views::ViewApp::globalAppKeyWentDown);
    Wt::WApplication::instance()->globalKeyWentUp().connect(this, &Views::ViewApp::globalAppKeyWentUp);

    /*******************--Layouts--********************/
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);
    setLayout(m_layMain);

    m_layMainH = new Wt::WHBoxLayout();

    m_cntLeftMenu = new Wt::WContainerWidget();

    m_layCntLeftMenu = new Wt::WVBoxLayout();
    m_layCntLeftMenu->setContentsMargins(0,0,0,0);
    m_layCntLeftMenu->setSpacing(0);

    m_cntLeftMenu->setLayout(m_layCntLeftMenu);
    m_layMainH->addWidget(m_cntLeftMenu);

    Wt::WContainerWidget *cntVLine = new Wt::WContainerWidget();
    cntVLine->setStyleClass("vertical-line");

    m_layMainH->addWidget(cntVLine);

    /*******************--Main Navigation Bar--********************/

    m_navBarMain = new Wt::WNavigationBar();
    m_navBarMain->setMinimumSize(500, 42);
    m_navBarMain->setTitle("MercurySteam", "http://www.mercuryvfx.com");
    //m_navBarMain->setResponsive(true);
    m_layMain->addWidget(m_navBarMain);

    //add m_layMainH to m_layMain
    m_layMain->addLayout(m_layMainH, 1);

    //main menu
    m_mnuMainLeft = new Wt::WMenu();
    m_navBarMain->addMenu(m_mnuMainLeft, Wt::AlignLeft);
    //mnuMain->setInternalPathEnabled();

    //main menu view menuItem
    m_mnuMainLeftViewItem = new Wt::WMenuItem("VIEW");
    m_mnuMainLeft->addItem(m_mnuMainLeftViewItem);//add the mnuMainViewItem to mnuMain

    //main menu view menuItem sub menu
    m_mnuMainLeftViewSub = new Wt::WPopupMenu();
    m_mnuMainLeftViewItem->setMenu(m_mnuMainLeftViewSub);//add mnuMainViewSub to mnuMainViewItem

    m_mnuMainLeftViewPanelsItem = new Wt::WMenuItem("Panels");
    m_mnuMainLeftViewSub->addItem(m_mnuMainLeftViewPanelsItem);

    m_mnuMainLeftViewPanelsSub = new Wt::WPopupMenu();
    m_mnuMainLeftViewPanelsItem->setSubMenu(m_mnuMainLeftViewPanelsSub);

    m_mnuMainLeftViewShowMenuPanel= new Wt::WMenuItem("Main Menu - Shift+M");
    m_mnuMainLeftViewShowMenuPanel->setCheckable(true);
    m_mnuMainLeftViewShowMenuPanel->setChecked(true);
    m_mnuMainLeftViewShowMenuPanel->triggered().connect(this, &Views::ViewApp::toggleLeftMenuView);

    m_mnuMainLeftViewPanelsSub->addItem(m_mnuMainLeftViewShowMenuPanel);//add "Show menu panel" item to mnuMainViewSub

    m_mnuMainLeftViewShowPropertiesPanel= new Wt::WMenuItem("Properties - Shift+P");
    m_mnuMainLeftViewShowPropertiesPanel->setCheckable(true);
    m_mnuMainLeftViewShowPropertiesPanel->setChecked(true);
    m_mnuMainLeftViewShowPropertiesPanel->triggered().connect(this, &Views::ViewApp::togglePropertiesPanel);

    m_mnuMainLeftViewPanelsSub->addItem(m_mnuMainLeftViewShowPropertiesPanel);//add "Show properties panel" item to mnuMainViewSub

    m_mnuMainLeftViewShowLogPanel = new Wt::WMenuItem("Log - Shift+L");
    m_mnuMainLeftViewShowLogPanel->setCheckable(true);
    m_mnuMainLeftViewShowLogPanel->setChecked(false);
    m_mnuMainLeftViewShowLogPanel->triggered().connect(this, &Views::ViewApp::toggleLogPanel);

    m_mnuMainLeftViewPanelsSub->addItem(m_mnuMainLeftViewShowLogPanel);//add "Show log panel" item to mnuMainViewSub

    //main menu help menuItem
    m_mnuMainLeftHelpItem = new Wt::WMenuItem("HELP");
    m_mnuMainLeft->addItem(m_mnuMainLeftHelpItem);//add the mnuMainHelpItem to mnuMain

    //main menu help menuItem sub menu
    m_mnuMainLeftHelpSub = new Wt::WPopupMenu();
    m_mnuMainLeftHelpItem->setMenu(m_mnuMainLeftHelpSub);//add mnuMainHelpSub to mnuMainHelpItem

    m_mnuMainLeftHelpAbout= new Wt::WMenuItem("About");
    m_mnuMainLeftHelpAbout->setIcon("icons/About.png");
    m_mnuMainLeftHelpAbout->triggered().connect(this, &Views::ViewApp::mnuMainLeftHelpAboutTriggered);

    m_mnuMainLeftHelpSub->addItem(m_mnuMainLeftHelpAbout);//add "About" item to mnuMainHelpSub

    //User menu
    Wt::Dbo::ptr<Users::User> user = Session::SessionManager::instance().user();

    //user avatar
    m_imgUserAvatar = new Wt::WImage();
    m_imgUserAvatar->setMaximumSize(64, 48);
    m_imgUserAvatar->setImageLink(Wt::WLink(user->thumbnail()));

    m_navBarMain->addWidget(m_imgUserAvatar, Wt::AlignRight);

    //mnuRight
    m_mnuMainRight = new Wt::WMenu();
    m_navBarMain->addMenu(m_mnuMainRight, Wt::AlignRight);

    m_mnuMainRightCurrentUserItem = new Wt::WMenuItem(user->name());
    m_mnuMainRight->addItem(m_mnuMainRightCurrentUserItem);//add the mnuMainCurrentUserItem to mnuMain

    m_mnuMainRightCurrentUserSub = new Wt::WPopupMenu();
    m_mnuMainRightCurrentUserItem->setMenu(m_mnuMainRightCurrentUserSub);//add mnuMainCurrentUserSub to mnuMainCurrentUserItem

    m_mnuMainRightCurrentUserSignOut= new Wt::WMenuItem("Sign Out");
    m_mnuMainRightCurrentUserSignOut->setIcon("icons/SignOut.png");
    m_mnuMainRightCurrentUserSignOut->triggered().connect(this, &Views::ViewApp::mnuMainRightCurrentUserSignOutTriggered);

    m_mnuMainRightCurrentUserSub->addItem(m_mnuMainRightCurrentUserSignOut);//add "Aign out" item to mnuMainCurrentUserSub

    /*******************--Main Left Menu--********************/
    m_cntMnuSideMain = new Wt::WContainerWidget();
    m_cntMnuSideMain->setMinimumSize(250, 500);
    m_cntMnuSideMain->addStyleClass("panel-border-right");

    m_mnuSideMain = new Wt::WMenu(Wt::Vertical);//main side menu
    m_mnuSideMain->addStyleClass("nav-pills nav-stacked");
    m_mnuSideMain->setWidth(250);
    //m_mnuSideMain->setInternalBasePath("/");
    //m_mnuSideMain->setInternalPathEnabled();
    m_cntMnuSideMain->addWidget(m_mnuSideMain);

    //m_mnuSideMain->addSectionHeader("Management");

    Wt::WContainerWidget *cntProjectMnuItem = new Wt::WContainerWidget();
    cntProjectMnuItem->setLayout(new Wt::WHBoxLayout());

    m_mnuSideMainProjectsItem = new Wt::WMenuItem("Projects");//projects menu item
    m_mnuSideMainProjectsItem->triggered().connect(this, &Views::ViewApp::mnuSideMainProjectsItemTriggered);
    m_mnuSideMain->addItem(m_mnuSideMainProjectsItem);

    //m_mnuSideMain->addSeparator();

    //m_mnuSideMain->addSectionHeader("Security");

    m_mnuSideMainUsersAndGroupsItem = new Wt::WMenuItem("Users And Groups");//Users menu item
    m_mnuSideMainUsersAndGroupsItem->triggered().connect(this, &Views::ViewApp::mnuSideMainUsersAndGroupsItemTriggered);
    m_mnuSideMain->addItem(m_mnuSideMainUsersAndGroupsItem);

    //m_mnuSideMain->addSeparator();

    //m_mnuSideMain->addSectionHeader("Reports");

    //m_mnuSideMainReportsItem = new Wt::WMenuItem("Reports");//reports menu item
    //m_mnuSideMainReportsItem->triggered().connect(this, &Views::ViewApp::mnuSideMainReportsItemTriggered);
    //m_mnuSideMain->addItem(m_mnuSideMainReportsItem);

    //m_mnuSideMain->addSectionHeader("Search");

    //m_mnuSideMainSearchItem = new Wt::WMenuItem("Search");//search menu item
    //m_mnuSideMainSearchItem->triggered().connect(this, &Views::ViewApp::mnuSideMainSearchItemTriggered);
    //m_mnuSideMain->addItem(m_mnuSideMainSearchItem);

    //m_mnuSideMain->addSectionHeader("Settings");

    m_mnuSideMainSettingsItem = new Wt::WMenuItem("Settings");//settings menu item
    //m_mnuSideMainSettingsItem->setStyleClass("side-menu-item");
    m_mnuSideMainSettingsItem->triggered().connect(this, &Views::ViewApp::mnuSideMainSettingsItemTriggered);
    m_mnuSideMain->addItem(m_mnuSideMainSettingsItem);
    //m_mnuSideMain->addSectionHeader("My Steam");

    m_mnuSideMainMyDashboardItem = new Wt::WMenuItem("My Dashboard");//my tasks menu item
    //m_mnuSideMainMyDashboardItem->setStyleClass("side-menu-item");
    m_mnuSideMainMyDashboardItem->triggered().connect(this, &Views::ViewApp::mnuSideMainMyDashboardItemTriggered);
    m_mnuSideMain->addItem(m_mnuSideMainMyDashboardItem);

    //add m_cntMnuSideMain to _layMainHSideV
    m_layCntLeftMenu->addWidget(m_cntMnuSideMain);

    //Prepare child view
    m_stkMainView = new Wt::WStackedWidget();
    m_stkMainView->setMinimumSize(535, 500);

    m_stkMainView->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);

    prepareChildViews(m_stkMainView);

    m_layCntChildViewsAndPropertiesAndLog = new Wt::WVBoxLayout();
    m_layCntChildViewsAndPropertiesAndLog->setContentsMargins(0,0,0,0);
    m_layCntChildViewsAndPropertiesAndLog->setSpacing(6);

    m_cntChildViewsAndPropertiesAndLog = new Wt::WContainerWidget();
    m_cntChildViewsAndPropertiesAndLog->setLayout(m_layCntChildViewsAndPropertiesAndLog);

    m_layCntChildViewsAndProperties = new Wt::WHBoxLayout();
    m_layCntChildViewsAndProperties->setContentsMargins(0,0,0,0);
    m_layCntChildViewsAndProperties->setSpacing(6);

    m_cntChildViewsAndProperties = new Wt::WContainerWidget();
    m_cntChildViewsAndProperties->setLayout(m_layCntChildViewsAndProperties);

    m_layCntChildViewsAndProperties->addWidget(m_stkMainView, 1);
    m_layCntChildViewsAndProperties->setResizable(0);

    m_layCntChildViewsAndPropertiesAndLog->addWidget(m_cntChildViewsAndProperties, 1);

    m_layCntChildViewsAndPropertiesAndLog->setResizable(0);

    m_layMainH->addWidget(m_cntChildViewsAndPropertiesAndLog, 1);

    /**************Properties View********************/
    m_layCntChildViewsAndProperties->addWidget(m_propertiesPanel);

    m_cntPropertiesEmpty = new Wt::WContainerWidget();
    m_propertiesPanel->addPropertiesView("NoProperties", m_cntPropertiesEmpty);

    /**************Log Panel********************/
    m_viewLog = new ViewLog();

    m_layCntChildViewsAndPropertiesAndLog->addWidget(m_viewLog);

    /*******************--Footer--********************/
    m_cntTxtFooter = new Wt::WContainerWidget();
    m_cntTxtFooter->setContentAlignment(Wt::AlignCenter);
    //m_cntTxtFooter->setStyleClass("footer");

    m_txtFooter = new Wt::WText("MercurySteam (c) Mercury Visual Solutions.");//footer text
    //m_txtFooter->setStyleClass("black-text");
    m_cntTxtFooter->addWidget(m_txtFooter);

    m_layMain->addWidget(m_cntTxtFooter);

    transaction.commit();
}

void Views::ViewApp::prepareChildViews(Wt::WStackedWidget *widget)
{
    //requires "view" privilege

    m_viewUsers = new ViewUsers();

    m_viewProjects = new ViewProjects();
    //m_viewReports = new ViewReports();
    //m_viewSearch = new ViewSearch();
    m_viewSettings = new ViewSettings();

    widget->addWidget(m_viewProjects);
    widget->addWidget(m_viewUsers);
    //widget->addWidget(m_viewReports);
    //widget->addWidget(m_viewSearch);
    widget->addWidget(m_viewSettings);

    m_viewMyDashboard = new ViewMyDashboard();

    widget->addWidget(m_viewMyDashboard);
}
