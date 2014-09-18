#include "databasemanager.h"
#include "Settings/appsettings.h"
#include "dbschemaattributes.h"
#include "Users/usersmanager.h"
#include "Projects/projectsmanager.h"
#include "Auth/authmanager.h"
#include "Log/logmanager.h"

#include <Ms/Exceptions/MNullPointerException.h>

#include <Wt/WApplication>
#include <Wt/Auth/Dbo/AuthInfo>

Database::DatabaseManager::DatabaseManager()
{
    _dbInitialized = false;
    _firstRun = false;
}

Database::DatabaseManager::~DatabaseManager()
{
    delete _users;
    delete session_;
}

void Database::DatabaseManager::initDatabase()
{
    Log::LogManager::instance().getGlobalLogger()->log("Establishing database connection..", Ms::Log::LogMessageType::Info);

    if(AppSettings::instance().databaseBackEnd() == "MySQL")
    {
        connection.reset(new Wt::Dbo::backend::MySQL(AppSettings::instance().databaseSchema(),
                                                      AppSettings::instance().databaseUsername(),
                                                      AppSettings::instance().databasePassword(),
                                                      AppSettings::instance().databaseHost(),
                                                     std::stoi(AppSettings::instance().databasePort())));
    }

    Log::LogManager::instance().getGlobalLogger()->log(
                std::string("Connected to \"") + AppSettings::instance().databaseBackEnd() + "\" database using schema \"" +
                AppSettings::instance().databaseSchema() + "\"", Ms::Log::LogMessageType::Info);

    session_ = new Wt::Dbo::Session();
    session_->setConnection(*connection);

    //setting up users database
    _users = new UserDatabase(*session_);

    _dbInitialized = _createSchema();
}

void Database::DatabaseManager::initSessionLogger()
{
    _logger = Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId());
}

void Database::DatabaseManager::discardChanges()
{
    session_->discardUnflushed();
}

Database::UserDatabase &Database::DatabaseManager::users()
{
    return *_users;
}

bool Database::DatabaseManager::firstRun()
{
    return _firstRun;
}

bool Database::DatabaseManager::dbInitialized()
{
    return _dbInitialized;
}

bool Database::DatabaseManager::_createSchema()
{
    //try to create schema if it doesn't already exist
    try
    {
        //general
        session_->mapClass<Database::DboData>("dbodata");
        session_->mapClass<Database::Note>("note");
        session_->mapClass<Database::Tag>("tag");

        //users
        session_->mapClass<Users::Privilege>("privilege");
        session_->mapClass<Users::Group>("group");
        session_->mapClass<Users::UserTitle>("user_title");
        session_->mapClass<Users::User>("user");
        //auth info
        session_->mapClass<Users::AuthInfo>("user_auth_info");
        session_->mapClass<Users::AuthInfo::AuthIdentityType>("user_auth_identity");
        session_->mapClass<Users::AuthInfo::AuthTokenType>("user_auth_token");
        //projects
        session_->mapClass<Projects::ProjectTaskActivity>("project_task_activity");
        session_->mapClass<Projects::ProjectTaskActivityType>("project_task_activity_type");
        session_->mapClass<Projects::ProjectTaskPipelineActivityItem>("project_task_pipeline_activity_item");
        session_->mapClass<Projects::ProjectTaskPipeline>("project_task_pipeline");
        session_->mapClass<Projects::ProjectWorkStatusType>("project_work_status_type");
        session_->mapClass<Projects::ProjectAssetType>("project_asset_type");
        session_->mapClass<Projects::ProjectWorkStatus>("project_work_status");
        session_->mapClass<Projects::ProjectSequence>("project_sequence");
        session_->mapClass<Projects::ProjectShot>("project_shot");
        session_->mapClass<Projects::ProjectTaskType>("project_task_type");
        session_->mapClass<Projects::ProjectTask>("project_task");
        session_->mapClass<Projects::ProjectAsset>("project_asset");
        session_->mapClass<Projects::Project>("project");

        Log::LogManager::instance().getGlobalLogger()->log("Creating database tables..", Ms::Log::LogMessageType::Info);

        if(!openTransaction())
            return false;

        int numTables = session_->query<int>("SELECT COUNT(*) FROM information_schema.tables WHERE table_schema = '" + AppSettings::instance().databaseSchema() + "';");

        if(numTables == 0)
        {
            _firstRun = true;

            //create DB tables
            session_->createTables();
            //add default rows
            _addDefaults();

            //set schema attributes
            _setSchemaAttributes();
        }

        commitTransaction();

        Log::LogManager::instance().getGlobalLogger()->log("Database tables creation done", Ms::Log::LogMessageType::Info);
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

void Database::DatabaseManager::_setSchemaAttributes()
{
    //settings some table attributes
    for(const std::string &command : Database::sqlAttributesCommands)
    {
        try
        {
            if(!openTransaction())
                return;

            session_->execute(command);

            commitTransaction();
        }
        catch(Wt::Dbo::Exception e)
        {
            Log::LogManager::instance().getGlobalLogger()->log(std::string("Error setting DB Schema Attributes") + e.what(), Ms::Log::LogMessageType::Error);
        }
    }
}

void Database::DatabaseManager::_addDefaults()
{
    if(!openTransaction())
        return;

    Users::UsersManager::instance().addDefaults();
    Projects::ProjectsManager::instance().addDefaults();

    commitTransaction();
}
