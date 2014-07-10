#ifndef DATABASERESULT_H
#define DATABASERESULT_H

#include <Wt/Dbo/Session>
#include <Wt/Dbo/backend/MySQL>
#include <Wt/Auth/Dbo/UserDatabase>

#include <Ms/Core/Dbo/MDboManagerBase.h>
#include <Ms/Exceptions/MDboSessionIsNullException.h>

#include <iostream>
#include <memory>

#include "dbtables.h"

namespace Database
{
    using UserDatabase = Wt::Auth::Dbo::UserDatabase<Users::AuthInfo>;

    //implemented as a singleton
    class DatabaseManager : public Ms::Core::Dbo::MDboManagerBase
    {
    private:
        DatabaseManager();
        DatabaseManager(DatabaseManager const &copy) = delete;
        void operator=(DatabaseManager const &copy) = delete;

    public:
        static DatabaseManager &instance()
        {
            static DatabaseManager man;
            return man;
        }
        ~DatabaseManager();

    //functions
    void initDatabase();
    void discardChanges();

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
    std::vector<std::string> getDboQueryIdValue(const Wt::Dbo::ptr<T> &dbo);

    template<typename T>
    std::vector<std::string> getDboQueryIdValues(const std::vector<Wt::Dbo::ptr<T>> &dboVec);


    //Wt::Auth::AbstractUserDatabase &users();
    UserDatabase &users();
    bool firstRun();

    //variables
    std::shared_ptr<Wt::Dbo::backend::MySQL> connection;
    bool dbInitialized();

    //variables

    private:
        //functions
        bool _createSchema();
        void _setSchemaAttributes();
        void _addDefaults();

        //variables
        UserDatabase *_users;
        bool _firstRun;//whether this is the first time running the application and creating the database
        bool _dbInitialized;
    };
}

#include "databasemanager_impl.h"

#endif // DATABASERESULT_H
