#define BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>

#include <iostream>

#include <Wt/WServer>

#include "Settings/appsettings.h"
#include "Database/dbosession.h"
#include "Database/dbschemaattributes.h"
#include "Views/Main/viewmain.h"
#include "Auth/authmanager.h"
#include "Log/logmanager.h"
#include "Application/msapplication.h"
#include "Users/usersio.h"

void addDefaultUser(Database::DboSession &session)
{
    try
    {
        Wt::Dbo::Transaction transaction(session);

        Users::User *user = new Users::User("Admin", "admin@mercuryvs.com");
        user->setCreateRank(INT_MAX);
        user->setViewRank(INT_MAX);
        user->setEditRank(INT_MAX);
        user->setRemoveRank(INT_MAX);

        Wt::Dbo::ptr<Users::Group> group = session.getDbo<Users::Group>("Admin");
        user->setGroup(group);

        Wt::Dbo::ptr<Users::User> userPtr = session.createDbo<Users::User>(user);

        Wt::Auth::User authUser = session.users().findWithIdentity(Wt::Auth::Identity::LoginName, userPtr->name());

        Auth::AuthManager::instance().passwordService().updatePassword(authUser, "admin");

        transaction.commit();

        //create user directory structure
        Users::UsersIO::createUserDirectoryStructure(user->name());
    }
    catch(Wt::Dbo::Exception ex)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to add default users ") + ex.what() , Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log("Error occured while trying to add default work users", Ms::Log::LogMessageType::Error);
    }
}

bool initDatabase(Wt::Dbo::SqlConnectionPool &sqlConnectionPool)
{
    Database::DboSession session(sqlConnectionPool);

    //try to create schema if it doesn't already exist
    try
    {
        Wt::Dbo::Transaction transaction(session);

        int numTables = session.query<int>("SELECT COUNT(*) FROM information_schema.tables WHERE table_schema = '" + AppSettings::instance().databaseSchema() + "';");

        transaction.commit();

        if(numTables == 0)
        {
            Log::LogManager::instance().getGlobalLogger()->log("Server running for first time, creating database tables..", Ms::Log::LogMessageType::Info);

            Wt::Dbo::Transaction createTableTransaction(session);

            //create DB tables
            session.createTables();

            createTableTransaction.commit();

            //execute firstrun.sql file to add default data
            std::vector<std::string> sql;

            std::ifstream sqlFile;
            sqlFile.open("approot/firstrun.sql");
            if(sqlFile.is_open())
            {
                std::string line;
                while(std::getline(sqlFile, line))
                {
                    if((line != "") && (line.at(0) != '#') && (line.at(0) != ' '))
                        sql.push_back(line);
                }

                sqlFile.close();

                Wt::Dbo::Transaction firstRunTransaction(session);

                Log::LogManager::instance().getGlobalLogger()->log("Executing \"firstrun.sql\"..", Ms::Log::LogMessageType::Info);

                for( auto &line : sql)
                {
                    session.execute(line);
                }

                firstRunTransaction.commit();
            }

            Log::LogManager::instance().getGlobalLogger()->log("Creating default user..", Ms::Log::LogMessageType::Info);
            //add default user
            addDefaultUser(session);

            //set default schema attributes
            Log::LogManager::instance().getGlobalLogger()->log("Settings schema attributes..", Ms::Log::LogMessageType::Info);

            Wt::Dbo::Transaction schemaAttributesTransaction(session);

            for(const std::string &command : Database::sqlAttributesCommands)
            {
                try
                {
                    session.execute(command);
                }
                catch(Wt::Dbo::Exception e)
                {
                    Log::LogManager::instance().getGlobalLogger()->log(std::string("Error setting DB Schema Attributes") + e.what(), Ms::Log::LogMessageType::Error);
                }
            }

            schemaAttributesTransaction.commit();
        }
    }
    catch(Wt::Dbo::Exception e)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Can't Create DB Schema ") + e.what(), Ms::Log::LogMessageType::Fatal);

        return false;
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Can't Create DB Schema because of unknown error"), Ms::Log::LogMessageType::Fatal);

        return false;
    }

    return true;
}

Wt::WApplication *createApplication(const Wt::WEnvironment &env, Wt::Dbo::SqlConnectionPool* sqlConnectionPool)
{
    return new App::MSApplication(env, *sqlConnectionPool);
}

int main(int argc, char *argv[])
{
    //settings
    Log::LogManager::instance().getGlobalLogger()->log("Loading settings..", Ms::Log::LogMessageType::Info);

    AppSettings::instance().setSettingsFileName("Settings.xml");
    if(!boost::filesystem::exists(AppSettings::instance().settingsFileName()))
        AppSettings::instance().initDefaultSettings();
    else
        AppSettings::instance().loadAppSettings();

    if(argc < 0)//check args count
    {

    }

    //parse args to set approot and docroot settings before other initializations
    for(int i = 1; i < argc; ++i)//argv[0] = apppath
    {
        if(i + 1 != argc)//if we don't finish parsing all arguments already
        {
            if(std::string(argv[i]) == "--docroot")
                AppSettings::instance().setDocRoot(argv[i+1]);
            else if(std::string(argv[i]) == "--approot")
                AppSettings::instance().setAppRoot(argv[i+1]);
        }
    }

    if(AppSettings::instance().docRoot() == "")
    {
        Log::LogManager::instance().getGlobalLogger()->log("docroot can't be empty, please run the program with argument --docroot <path>", Ms::Log::LogMessageType::Fatal);
        exit(-1);
    }
    if(AppSettings::instance().appRoot() == "")
    {
        Log::LogManager::instance().getGlobalLogger()->log("approot can't be empty, please run the program with argument --approot <path>", Ms::Log::LogMessageType::Fatal);
        exit(-1);
    }

    //Auth
    Log::LogManager::instance().getGlobalLogger()->log("Initializing auth service..", Ms::Log::LogMessageType::Info);
    //configure auth service
    Auth::AuthManager::instance().configureAuth();

    //Database
    Log::LogManager::instance().getGlobalLogger()->log("Initializing database..", Ms::Log::LogMessageType::Info);
    //initialize database
    Wt::Dbo::SqlConnectionPool *sqlConnectionPool = Database::DboSession::createConnectionPool();

    initDatabase(*sqlConnectionPool);

    //run the application
    try
    {
        Wt::WServer server(argv[0]);

        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

        server.addEntryPoint(Wt::Application, boost::bind(&createApplication, _1, sqlConnectionPool));

        server.run();

//        if (server.start())
//        {
//              Wt::WServer::waitForShutdown();
//              server.stop();
//        }
    }
    catch (Wt::WServer::Exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }

    delete sqlConnectionPool;
}
