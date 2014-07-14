#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

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

namespace Users
{
    class User;
}

namespace Auth
{
    //auth typedefs
    using AuthInfo = Wt::Auth::Dbo::AuthInfo<Users::User>;

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

        //functions
        void initSessionLogger();
        //auth stuff
        void configureAuth();
        const Wt::Auth::AuthService &authService();
        const Wt::Auth::PasswordService *passwordService();
        const std::vector<const Wt::Auth::OAuthService *> &oAuthService();
        Wt::Auth::User registerUser(const std::string &name, const std::string &password, const std::string &email = "");
        Wt::Dbo::ptr<Auth::AuthInfo> getUserAuthInfo(Wt::Auth::User authUser);
        Wt::Auth::User getAuthUser(const std::string &provider, const std::string &identity);

        bool setUserEmail(const std::string &provider, const std::string &identity, const std::string &email);
        bool setUserPassword(const std::string &provider, const std::string &identity, const std::string &password);

        //auth stuff
        Wt::Auth::Login &login();

        //variables
        //auth stuff
        Wt::Auth::Login _login;

    private:
        //variables
        Log::Logger *_logger;

        //auth stuff
        Wt::Auth::AuthService _authService;
        Wt::Auth::PasswordService* _passwordService;
        std::vector<const Wt::Auth::OAuthService*> _OAuthService;
    };
}

#endif // AUTHMANAGER_H
