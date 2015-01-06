#include "authmanager.h"
#include "../Database/dbosession.h"
#include "../Session/sessionmanager.h"
#include "Log/logmanager.h"
#include "../Users/usersio.h"

#include <Wt/WApplication>

Auth::AuthManager::AuthManager() :
    Ms::Core::MManagerBase(),
    m_passwordService(m_authService)
{
}

Auth::AuthManager::~AuthManager()
{
    //clean up

    for(std::vector<const Wt::Auth::OAuthService*>::size_type i = 0; i < m_oAuthService.size(); ++i)
    {
        delete m_oAuthService[i];
    }
}

void Auth::AuthManager::configureAuth()
{
    m_authService.setAuthTokensEnabled(true, "logincookie");
    //m_authService.setEmailVerificationEnabled(true);

    Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
    verifier->addHashFunction(new Wt::Auth::BCryptHashFunction(7));

#ifdef WT_WITH_SSL
    verifier->addHashFunction(new Wt::Auth::SHA1HashFunction());
#endif

    m_passwordService.setVerifier(verifier);
    m_passwordService.setAttemptThrottlingEnabled(true);
    m_passwordService.setStrengthValidator(new Wt::Auth::PasswordStrengthValidator());

//    if(Wt::Auth::GoogleService::configured())
//        m_oAuthService.push_back(new Wt::Auth::GoogleService(m_authService));

//    if(Wt::Auth::FacebookService::configured())
//        m_oAuthService.push_back(new Wt::Auth::FacebookService(m_authService));
}

const Wt::Auth::AuthService &Auth::AuthManager::authService()
{
    return m_authService;
}

const Wt::Auth::PasswordService &Auth::AuthManager::passwordService()
{
    return m_passwordService;
}

const std::vector<const Wt::Auth::OAuthService*> &Auth::AuthManager::oAuthService()
{
    return m_oAuthService;
}
