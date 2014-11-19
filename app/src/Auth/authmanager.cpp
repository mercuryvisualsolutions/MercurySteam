#include "authmanager.h"
#include "../Database/dbosession.h"
#include "../Session/sessionmanager.h"
#include "Log/logmanager.h"
#include "../Users/usersio.h"

#include <Wt/WApplication>

Auth::AuthManager::AuthManager() :
    Ms::Core::MManagerBase(),
    _passwordService(_authService)
{
}

Auth::AuthManager::~AuthManager()
{
    //clean up

    for(std::vector<const Wt::Auth::OAuthService*>::size_type i = 0; i < _oAuthService.size(); ++i)
    {
        delete _oAuthService[i];
    }
}

void Auth::AuthManager::configureAuth()
{
    _authService.setAuthTokensEnabled(true, "logincookie");
    //_authService.setEmailVerificationEnabled(true);

    Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
    verifier->addHashFunction(new Wt::Auth::BCryptHashFunction(7));

#ifdef WT_WITH_SSL
    verifier->addHashFunction(new Wt::Auth::SHA1HashFunction());
#endif

    _passwordService.setVerifier(verifier);
    _passwordService.setAttemptThrottlingEnabled(true);
    _passwordService.setStrengthValidator(new Wt::Auth::PasswordStrengthValidator());

//    if(Wt::Auth::GoogleService::configured())
//        _oAuthService.push_back(new Wt::Auth::GoogleService(_authService));

//    if(Wt::Auth::FacebookService::configured())
//        _oAuthService.push_back(new Wt::Auth::FacebookService(_authService));
}

const Wt::Auth::AuthService &Auth::AuthManager::authService()
{
    return _authService;
}

const Wt::Auth::PasswordService &Auth::AuthManager::passwordService()
{
    return _passwordService;
}

const std::vector<const Wt::Auth::OAuthService*> &Auth::AuthManager::oAuthService()
{
    return _oAuthService;
}
