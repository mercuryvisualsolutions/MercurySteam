#include "authmanager.h"
#include "Database/databasemanager.h"

Auth::AuthManager::AuthManager()
{
}

Auth::AuthManager::~AuthManager()
{
    //clean up
    delete _passwordService;

    for(std::vector<const Wt::Auth::OAuthService*>::size_type i = 0; i < _OAuthService.size(); ++i)
    {
        delete _OAuthService[i];
    }
}

void Auth::AuthManager::configureAuth()
{
    _authService.setAuthTokensEnabled(true, "logincookie");
    _authService.setEmailVerificationEnabled(true);

    Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
    verifier->addHashFunction(new Wt::Auth::BCryptHashFunction(7));

    _passwordService = new Wt::Auth::PasswordService(_authService);
    _passwordService->setVerifier(verifier);
    _passwordService->setAttemptThrottlingEnabled(true);
    _passwordService->setStrengthValidator(new Wt::Auth::PasswordStrengthValidator());

//    if(Wt::Auth::GoogleService::configured())
//        _OAuthService.push_back(new Wt::Auth::GoogleService(_authService));

//    if(Wt::Auth::FacebookService::configured())
//        _OAuthService.push_back(new Wt::Auth::FacebookService(_authService));
}

const Wt::Auth::AuthService &Auth::AuthManager::authService()
{
    return _authService;
}

const Wt::Auth::PasswordService *Auth::AuthManager::passwordService()
{
    return _passwordService;
}

const std::vector<const Wt::Auth::OAuthService *> &Auth::AuthManager::oAuthService()
{
    return _OAuthService;
}

Wt::Auth::User Auth::AuthManager::registerUser(const std::string &name, const std::string &password, const std::string &email)
{
    try
    {
        Wt::Auth::User authUser;

        if(!Database::DatabaseManager::instance().openTransaction())
            return authUser;

        //identity/user exist ?
        authUser = Database::DatabaseManager::instance().users().findWithIdentity(Wt::Auth::Identity::LoginName, name);
        if(authUser.isValid())
        {
            std::cerr << "User: " << name << " Already Exist" << std::endl;
            return Wt::Auth::User();
        }

        authUser = Database::DatabaseManager::instance().users().registerNew();
        authUser.addIdentity(Wt::Auth::Identity::LoginName, name);
        _passwordService->updatePassword(authUser, password);

        authUser.setEmail(email);

        return authUser;
    }
    catch(...)
    {
        std::cerr << "Exception occured while trying to add a new auth user" << std::endl << std::endl;

        return Wt::Auth::User();
    }
}

Wt::Dbo::ptr<Auth::AuthInfo> Auth::AuthManager::getUserAuthInfo(Wt::Auth::User authUser)
{
    Wt::Dbo::ptr<Users::AuthInfo> authInfo;
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return authInfo;

        authInfo = Database::DatabaseManager::instance().users().find(authUser);
    }
    catch(...)
    {
        std::cerr << "Exception occured while trying to fetch user auth info" << std::endl << std::endl;
    }
    return authInfo;
}

Wt::Auth::User Auth::AuthManager::getAuthUser(const std::string &provider, const std::string &identity)
{
    Wt::Auth::User authUser;
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return authUser;

        authUser = Database::DatabaseManager::instance().users().findWithIdentity(provider, identity);
    }
    catch(...)
    {
        std::cerr << "Exception occured while trying to fetch user auth info" << std::endl << std::endl;
    }
    return authUser;
}

bool Auth::AuthManager::setUserEmail(const std::string &provider, const std::string &identity, const std::string &email)
{
    Wt::Auth::User user = getAuthUser(provider, identity);
    if(!user.isValid())
        return false;

    user.setEmail(email);

    return true;
}

bool Auth::AuthManager::setUserPassword(const std::string &provider, const std::string &identity, const std::string &password)
{
    Wt::Auth::User user = getAuthUser(provider, identity);
    if(!user.isValid())
        return false;

    _passwordService->updatePassword(user, password);

    return true;
}

Wt::Auth::Login &Auth::AuthManager::login()
{
    return _login;
}
