#ifndef DBOSESSION_H
#define DBOSESSION_H

#include "dbtables.h"
#include "Log/logger.h"
#include "userdatabase.h"

#include <Ms/Core/Dbo/MDboSession.h>

#include <Wt/Auth/Login>

#include <iostream>
#include <memory>

namespace Database
{
    class DboSession : public Ms::Core::Dbo::MDboSession
    {
    public:
        DboSession(Wt::Dbo::SqlConnectionPool& connectionPool);
        ~DboSession();

    //functions

    //GLOBAL types
    template<typename T>
    bool dboHasData(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::DboData> dboData);
    template<typename T>
    bool addDataToDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::DboData> dboData);
    template<typename T>
    bool removeDataFromDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::DboData> dboData);
    template<typename T>
    bool dboHasNote(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Note> note);
    template<typename T>
    bool addNoteToDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Note> note);
    template<typename T>
    bool removeNoteFromDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Note> note);
    template<typename T>
    bool dboHasTag(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Tag> tag);
    template<typename T>
    bool addTagToDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Tag> tag);
    template<typename T>
    bool removeTagFromDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Tag> tag);

    template<typename T>
    std::vector<std::string> getDboQueryIdValue(const Wt::Dbo::ptr<T> &dbo) const;

    template<typename T>
    std::vector<std::string> getDboQueryIdValues(const std::vector<Wt::Dbo::ptr<T>> &dboVec) const;

    Database::UserDatabase &users();
    Wt::Auth::Login &login();
    Wt::Dbo::ptr<Users::User> user() const;

    //statics
    static Wt::Dbo::SqlConnectionPool *createConnectionPool();

    private:
        //variables
        Database::UserDatabase _users;
        Wt::Auth::Login _login;

        //functions
        void mapClasses();
    };
}

#include "dbosession_impl.h"

#endif // DBOSESSION_H
