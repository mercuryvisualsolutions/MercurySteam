#include "viewmain.h"
#include "../../Database/dbosession.h"
#include "../../Auth/authmanager.h"
#include "../../Log/logmanager.h"

#include <Wt/WApplication>

Views::ViewMain::ViewMain() :
    m_applicationCreated(false)
{
    Session::SessionManager::instance().login().changed().connect(this, &Views::ViewMain::authEvent);

    prepareView();
}

void Views::ViewMain::authEvent()
{
    if(Session::SessionManager::instance().login().loggedIn())
        loggedIn();
    else
        loggedOut();
}

void Views::ViewMain::loggedIn()
{
    App::MSApplication *app = dynamic_cast<App::MSApplication*>(Wt::WApplication::instance());

    if(!m_applicationCreated)
    {
        app->createGlobalWidgets();

        m_logger = app->logger();

        prepareAppView(m_stkMain);

        m_applicationCreated = true;
    }

    std::string userName = app->dboSession().login().user().identity(Wt::Auth::Identity::LoginName).toUTF8();

    m_logger->log(std::string("User: ") + userName + " Logged In", Ms::Log::LogMessageType::Info);

    //assign userName for current session
    app->dboSession().setUserName(userName);

    //adjust view based on current user's privileges
    m_viewApp->adjustUIPrivileges(Session::SessionManager::instance().user());

    showAppView();
}

void Views::ViewMain::loggedOut()
{
    m_stkMain->setCurrentWidget(m_cntAuth);
}

void Views::ViewMain::showAuthView()
{
    Session::SessionManager::instance().login().logout();//logout current user (if any)

    //show the auth view
    m_stkMain->setCurrentWidget(m_cntAuth);
}

void Views::ViewMain::showAppView()
{
    m_stkMain->setCurrentWidget(m_viewApp);
}

void Views::ViewMain::prepareView()
{
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    m_stkMain = new Wt::WStackedWidget();
    m_stkMain->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::SlideInFromTop, Wt::WAnimation::TimingFunction::EaseInOut, 500), true);

    m_layMain->addWidget(m_stkMain);

    m_layCntAuth = new Wt::WVBoxLayout();
    m_layCntAuth->setContentsMargins(0,0,0,0);
    m_layCntAuth->setSpacing(0);

    m_cntAuth = new Wt::WContainerWidget();
    m_cntAuth->setLayout(m_layCntAuth);

    m_stkMain->addWidget(m_cntAuth);

    m_authWidget = new Wt::Auth::AuthWidget(Auth::AuthManager::instance().authService(), Session::SessionManager::instance().dboSession().users(), Session::SessionManager::instance().login());
    m_authWidget->model()->addPasswordAuth(&Auth::AuthManager::instance().passwordService());
    m_authWidget->model()->addOAuth(Auth::AuthManager::instance().oAuthService());
    m_authWidget->setMinimumSize(400, 400);

    for(auto wid : m_authWidget->children())
    {
        wid->setStyleClass("form-inline");
    }

    //m_authWidget->setRegistrationEnabled(true);

    m_authWidget->processEnvironment();

    m_cntAuth->setContentAlignment(Wt::AlignCenter);

    m_layCntAuth->addWidget(m_authWidget);

    //if user is already logged in
    if(Session::SessionManager::instance().login().loggedIn())
        loggedIn();
}

void Views::ViewMain::prepareAppView(Wt::WStackedWidget *widget)
{
    m_viewApp = new Views::ViewApp();

    widget->addWidget(m_viewApp);
}
