#include "viewmain.h"
#include "../../Database/dbosession.h"
#include "../../Auth/authmanager.h"
#include "../../Log/logmanager.h"

#include <Wt/WApplication>

Views::ViewMain::ViewMain() :
    _applicationCreated(false)
{
    Session::SessionManager::instance().login().changed().connect(this, &Views::ViewMain::authEvent);

    _prepareView();
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

    if(!_applicationCreated)
    {
        app->createGlobalWidgets();

        _logger = app->logger();

        _prepareAppView(_stkMain);

        _applicationCreated = true;
    }

    std::string userName = app->dboSession().login().user().identity(Wt::Auth::Identity::LoginName).toUTF8();

    _logger->log(std::string("User: ") + userName + " Logged In", Ms::Log::LogMessageType::Info);

    //assign userName for current session
    app->dboSession().setUserName(userName);

    //adjust view based on current user's privileges
    viwApp->adjustUIPrivileges(Session::SessionManager::instance().user());

    _showAppView();
}

void Views::ViewMain::loggedOut()
{
    _stkMain->setCurrentWidget(_cntAuth);
}

void Views::ViewMain::showAuthView()
{
    Session::SessionManager::instance().login().logout();//logout current user (if any)

    //show the auth view
    _stkMain->setCurrentWidget(_cntAuth);
}

void Views::ViewMain::_showAppView()
{
    _stkMain->setCurrentWidget(viwApp);
}

void Views::ViewMain::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _stkMain = new Wt::WStackedWidget();
    _stkMain->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::SlideInFromTop, Wt::WAnimation::TimingFunction::EaseInOut, 500), true);

    _layMain->addWidget(_stkMain);

    _layCntAuth = new Wt::WVBoxLayout();
    _layCntAuth->setContentsMargins(0,0,0,0);
    _layCntAuth->setSpacing(0);

    _cntAuth = new Wt::WContainerWidget();
    _cntAuth->setLayout(_layCntAuth);

    _stkMain->addWidget(_cntAuth);

    _authWidget = new Wt::Auth::AuthWidget(Auth::AuthManager::instance().authService(), Session::SessionManager::instance().dboSession().users(), Session::SessionManager::instance().login());
    _authWidget->model()->addPasswordAuth(&Auth::AuthManager::instance().passwordService());
    _authWidget->model()->addOAuth(Auth::AuthManager::instance().oAuthService());
    _authWidget->setMinimumSize(400, 400);

    for(auto wid : _authWidget->children())
    {
        wid->setStyleClass("form-inline");
    }

    //_authWidget->setRegistrationEnabled(true);

    _authWidget->processEnvironment();

    _cntAuth->setContentAlignment(Wt::AlignCenter);

    _layCntAuth->addWidget(_authWidget);

    //if user is already logged in
    if(Session::SessionManager::instance().login().loggedIn())
        loggedIn();
}

void Views::ViewMain::_prepareAppView(Wt::WStackedWidget *widget)
{
    viwApp = new Views::ViewApp();    

    widget->addWidget(viwApp);
}
