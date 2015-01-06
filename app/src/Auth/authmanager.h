#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include "../Database/dbosession.h"

#include <Log/logger.h>

#include <Wt/Auth/Login>
#include <Wt/Auth/AuthService>
#include <Wt/Auth/HashFunction>
#include <Wt/Auth/PasswordService>
#include <Wt/Auth/PasswordStrengthValidator>
#include <Wt/Auth/PasswordVerifier>
#include <Wt/Auth/GoogleService>
#include <Wt/Auth/FacebookService>

#include <Ms/Core/MManagerBase.h>

namespace Auth
{
    class AuthManager : public Ms::Core::MManagerBase
    {
    private:
        AuthManager();
        AuthManager(AuthManager const &copy) = delete;
        void operator =(AuthManager const &copy) = delete;

    public:
        static AuthManager &instance()
        {
            static AuthManager man;
            return man;
        }

        ~AuthManager();

        //functions;
        //auth stuff
        void configureAuth();
        const Wt::Auth::AuthService &authService();
        const Wt::Auth::PasswordService &passwordService();
        const std::vector<const Wt::Auth::OAuthService*> &oAuthService();

    private:
        //variables

        //auth stuff
        Wt::Auth::AuthService m_authService;
        Wt::Auth::PasswordService m_passwordService;
        std::vector<const Wt::Auth::OAuthService*> m_oAuthService;
    };
}

#endif // AUTHMANAGER_H
