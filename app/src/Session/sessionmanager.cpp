#include "sessionmanager.h"

#include <Wt/Http/Request>

#include <map>

Session::SessionManager::SessionManager()
{
}

Database::DboSession &Session::SessionManager::dboSession()
{
    return getCurrentAppInstance()->dboSession();
}

Wt::Auth::Login &Session::SessionManager::login()
{
    return getCurrentAppInstance()->dboSession().login();
}

Wt::Dbo::ptr<Users::User> Session::SessionManager::user()
{
    return getCurrentAppInstance()->dboSession().user();
}

Ms::Widgets::MPropertiesPanel *Session::SessionManager::propertiesPanel()
{
    return getCurrentAppInstance()->propertiesPanel();
}

Log::Logger *Session::SessionManager::logger()
{
    return getCurrentAppInstance()->logger();
}

App::MSApplication *Session::SessionManager::getCurrentAppInstance()
{
    return dynamic_cast<App::MSApplication*>(Wt::WApplication::instance());
}
