#include "viewmain.h"
#include "../../Database/databasemanager.h"
#include "../../Users/usersmanager.h"
#include "../../Projects/projectsmanager.h"
#include "../../Auth/authmanager.h"
#include "../../Log/logmanager.h"

#include <Wt/WApplication>

Views::ViewMain::ViewMain()
{
    _logger = Log::LogManager::instance().getAppSessionLogger(Wt::WApplication::instance()->sessionId());

    _prepareView();

    Auth::AuthManager::instance().login().changed().connect(this, &Views::ViewMain::authEvent);
}

void Views::ViewMain::authEvent()
{
    if(Auth::AuthManager::instance().login().loggedIn())
    {
        std::string userName = Auth::AuthManager::instance().login().user().identity(Wt::Auth::Identity::LoginName).toUTF8();

        _logger->log(std::string("User: ") + userName + " Logged In", Ms::Log::LogMessageType::Info);

        //assign userName for managers
        Database::DatabaseManager::instance().setUserName(userName);
        Users::UsersManager::instance().setUserName(userName);
        Projects::ProjectsManager::instance().setUserName(userName);

        _prepareAppView(_stkMain);
        _showAppView();
    }
    else
    {
        _logger->log("User Logged Out", Ms::Log::LogMessageType::Info);

        Wt::WApplication::instance()->refresh();
        showAuthView();
    }
}

void Views::ViewMain::showAuthView()
{
    Auth::AuthManager::instance().login().logout();//logout current user (if any)

    //show the auth view
    _stkMain->setCurrentWidget(_cntAuth);
}

void Views::ViewMain::_showAppView()
{
    viwApp->showProjectsView();
    _stkMain->setCurrentWidget(viwApp);
}

void Views::ViewMain::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _stkMain = new Wt::WStackedWidget();
    _layMain->addWidget(_stkMain);

    _layCntAuth = new Wt::WVBoxLayout();
    _layCntAuth->setContentsMargins(0,0,0,0);
    _layCntAuth->setSpacing(0);

    _cntAuth = new Wt::WContainerWidget();
    _cntAuth->setLayout(_layCntAuth);

    _stkMain->addWidget(_cntAuth);

    _authWidget = new Wt::Auth::AuthWidget(Auth::AuthManager::instance().authService(), Database::DatabaseManager::instance().users(), Auth::AuthManager::instance().login());
    _authWidget->model()->addPasswordAuth(Auth::AuthManager::instance().passwordService());
    _authWidget->model()->addOAuth(Auth::AuthManager::instance().oAuthService());
    //_authWidget->setRegistrationEnabled(true);

    _authWidget->processEnvironment();

    _cntAuth->setContentAlignment(Wt::AlignCenter);

    _layCntAuth->addWidget(_authWidget);

    if(Auth::AuthManager::instance().login().loggedIn())//if user is already logged in
    {
        _prepareAppView(_stkMain);
        _showAppView();
    }
}

void Views::ViewMain::_prepareAppView(Wt::WStackedWidget *widget)
{
    viwApp = new Views::ViewApp();    

    widget->addWidget(viwApp);
}
