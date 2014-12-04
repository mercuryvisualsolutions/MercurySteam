#ifndef USERDATABASE_H
#define USERDATABASE_H

#include "userdbospecialization.h"

#include <Ms/Core/Dbo/MDboSession.h>

#include <Wt/Auth/AbstractUserDatabase>

namespace Database
{
    class InvalidUser : public std::runtime_error
    {
    public:
        InvalidUser(const std::string &id) :
            std::runtime_error("Invalid user: " + id)
        {
        }
    };

    class Transaction : public Wt::Auth::AbstractUserDatabase::Transaction, public Wt::Dbo::Transaction
    {
    public:
        Transaction(Wt::Dbo::Session &session) :
            Wt::Dbo::Transaction(session)
        {
        }

        virtual ~Transaction()
        {
        }

        virtual void commit()
        {
            Wt::Dbo::Transaction::commit();
        }

        virtual void rollback()
        {
            Wt::Dbo::Transaction::rollback();
        }
    };

    class UserDatabase : public Wt::Auth::AbstractUserDatabase
    {
    public:
        UserDatabase(Ms::Core::Dbo::MDboSession &session);

        virtual Database::Transaction *startTransaction();

        Wt::Dbo::ptr<Users::User> find(const Wt::Auth::User &user) const;
        Wt::Auth::User find(Wt::Dbo::ptr<Users::User> user) const;

        virtual Wt::Auth::User findWithId(const std::string &id) const;

        virtual Wt::Auth::User findWithIdentity(const std::string &provider, const Wt::WString &identity) const;

        virtual void addIdentity(const Wt::Auth::User &user, const std::string &provider, const Wt::WString &id);

        virtual Wt::WString identity(const Wt::Auth::User &user, const std::string &provider) const;

        virtual void removeIdentity(const Wt::Auth::User &user, const std::string &provider);

        virtual Wt::Auth::PasswordHash password(const Wt::Auth::User &user) const;
        virtual void setPassword(const Wt::Auth::User &user, const Wt::Auth::PasswordHash &password);

        virtual Wt::Auth::User::Status status(const Wt::Auth::User &user) const;
        virtual void setStatus(const Wt::Auth::User &user, Wt::Auth::User::Status status);

        virtual Wt::Auth::User registerNew();

        virtual void addAuthToken(const Wt::Auth::User &user, const Wt::Auth::Token &token);
        virtual int updateAuthToken(const Wt::Auth::User &user, const std::string &oldhash, const std::string &newhash);
        virtual void removeAuthToken(const Wt::Auth::User &user, const std::string &hash);

        virtual Wt::Auth::User findWithAuthToken(const std::string &hash) const;

        virtual int failedLoginAttempts(const Wt::Auth::User &user) const;
        virtual void setFailedLoginAttempts(const Wt::Auth::User &user, int count);

        virtual Wt::WDateTime lastLoginAttempt(const Wt::Auth::User &user) const;
        virtual void setLastLoginAttempt(const Wt::Auth::User &user, const Wt::WDateTime &t);

    private:
        Ms::Core::Dbo::MDboSession &_session;
        mutable Wt::Dbo::ptr<Users::User> _user;

        void getUser(const std::string &id) const;

        struct WithUser
        {
            WithUser(const Database::UserDatabase &self, const Wt::Auth::User &user);
            ~WithUser();

            Wt::Dbo::Transaction _transaction;
        };
    };
}

#endif // USERDATABASE_H
