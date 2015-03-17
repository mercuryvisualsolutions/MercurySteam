#include "dbosession.h"
#include "Settings/appsettings.h"
#include "dbschemaattributes.h"
#include "Auth/authmanager.h"
#include "Log/logmanager.h"

#include <Ms/Exceptions/MNullPointerException.h>

#include <Wt/WApplication>
#include <Wt/Auth/Dbo/AuthInfo>
#include <Wt/Dbo/FixedSqlConnectionPool>
#include <Wt/Dbo/backend/MySQL>

Database::DboSession::DboSession(Wt::Dbo::SqlConnectionPool &connectionPool) :
    Ms::Core::Dbo::MDboSession(connectionPool),
    m_users(*this)
{
    mapClasses();
}

Database::UserDatabase &Database::DboSession::users()
{
    return m_users;
}

Wt::Auth::Login &Database::DboSession::login()
{
    return m_login;
}

Wt::Dbo::ptr<Users::User> Database::DboSession::user() const
{
    if(m_login.loggedIn())
        return m_users.find(m_login.user());
    else
        return Wt::Dbo::ptr<Users::User>();
}

void Database::DboSession::mapClasses()
{
    //general
    mapClass<Database::DboData>("dbodata");
    mapClass<Database::Note>("note");
    mapClass<Database::Tag>("tag");
    mapClass<Database::Token>("token");
    mapClass<Database::Notification>("notification");

    //users
    mapClass<Users::Privilege>("privilege");
    mapClass<Users::Group>("group");
    mapClass<Users::UserTitle>("user_title");
    mapClass<Users::User>("user");
    //projects
    mapClass<Projects::ProjectTaskActivity>("project_task_activity");
    mapClass<Projects::ProjectTaskActivityType>("project_task_activity_type");
    mapClass<Projects::ProjectActivityTemplateActivityItem>("project_activity_template_item");
    mapClass<Projects::ProjectActivityTemplate>("project_activity_template");
    mapClass<Projects::ProjectTaskTemplateTaskItem>("project_task_template_item");
    mapClass<Projects::ProjectTaskTemplate>("project_task_template");
    mapClass<Projects::ProjectWorkStatusType>("project_work_status_type");
    mapClass<Projects::ProjectAssetType>("project_asset_type");
    mapClass<Projects::ProjectWorkStatus>("project_work_status");
    mapClass<Projects::ProjectSequence>("project_sequence");
    mapClass<Projects::ProjectShot>("project_shot");
    mapClass<Projects::ProjectTaskType>("project_task_type");
    mapClass<Projects::ProjectTask>("project_task");
    mapClass<Projects::ProjectAsset>("project_asset");
    mapClass<Projects::Project>("project");
}

Wt::Dbo::SqlConnectionPool *Database::DboSession::createConnectionPool()
{
    if(AppSettings::instance().databaseBackEnd() == "MySQL")
    {
        Wt::Dbo::backend::MySQL *connection = new Wt::Dbo::backend::MySQL(AppSettings::instance().databaseSchema(),
                                                      AppSettings::instance().databaseUsername(),
                                                      AppSettings::instance().databasePassword(),
                                                      AppSettings::instance().databaseHost(),
                                                     std::stoi(AppSettings::instance().databasePort()));


        connection->setProperty("shot-queries", "true");

        return new Wt::Dbo::FixedSqlConnectionPool(connection, 10);
    }
    else
        return nullptr;
}
