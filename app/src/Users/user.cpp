#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"
#include "../Log/logmanager.h"
#include "Users/usersio.h"

Users::User::User() :
    Database::Dbo()
{
    init();
}

Users::User::User(const std::string &name) :
    User()
{
    m_name = name;
}

Users::User::User(const std::string &name, const std::string &emailAddress) :
    User(name)
{
    m_emailAddress = emailAddress;
}

Users::User *Users::User::modify()
{
    Database::Dbo::modify();

    return this;
}

std::string Users::User::name() const
{
    return m_name;
}

void Users::User::setName(const std::string &name)
{
    m_name = name;
}

std::string Users::User::password() const
{
    return m_password;
}

void Users::User::setPassword(const std::string &password)
{
    m_password = password;
}

std::string Users::User::passwordMethod() const
{
    return m_passwordMethod;
}

void Users::User::setPasswordMethod(const std::string &passwordMethod)
{
    m_passwordMethod = passwordMethod;
}

std::string Users::User::passwordSalt() const
{
    return m_passwordSalt;
}

void Users::User::setPasswordSalt(const std::string &passwordSalt)
{
    m_passwordSalt = passwordSalt;
}

int Users::User::failedLoginAttempts() const
{
    return m_failedLoginAttempts;
}

void Users::User::setFailedLoginAttempts(int failedLoginAttempts)
{
    m_failedLoginAttempts = failedLoginAttempts;
}

Wt::WDateTime Users::User::lastLoginAttempt() const
{
    return m_lastLoginAttempt;
}

void Users::User::setLastLoginAttempt(const Wt::WDateTime &lastLoginAttempt)
{
    m_lastLoginAttempt = lastLoginAttempt;
}

std::string Users::User::oAuthId() const
{
    return m_oAuthId;
}

void Users::User::setOAuthId(const std::string &oAuthId)
{
    m_oAuthId = oAuthId;
}

std::string Users::User::oAuthProvider() const
{
    return m_oAuthProvider;
}

void Users::User::setOAuthProvider(const std::string &oAuthProvider)
{
    m_oAuthProvider = oAuthProvider;
}

bool Users::User::hasPrivilege(Wt::Dbo::ptr<Users::Privilege> privilege) const
{
    return m_group->hasPrivilege(privilege);
}

bool Users::User::hasPrivilege(const char *privilegeName) const
{
    return m_group->hasPrivilege(privilegeName);
}

bool Users::User::hasTask(Wt::Dbo::ptr<Projects::ProjectTask> task) const
{
    bool result = false;

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    {
        if((*iter).id() == task.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Users::User::addTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!hasTask(task))
    {
        m_tasks.insert(task);

        return true;
    }

    return false;
}

bool Users::User::removeTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(hasTask(task))
    {
        m_tasks.erase(task);

        return true;
    }

    return false;
}

bool Users::User::hasNotification(Wt::Dbo::ptr<Database::Notification> notification) const
{
    bool result = false;

    for(auto iter = m_notifications.begin(); iter != m_notifications.end(); ++iter)
    {
        if((*iter).id() == notification.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Users::User::addNotification(Wt::Dbo::ptr<Database::Notification> notification)
{
    if(!hasNotification(notification))
    {
        m_notifications.insert(notification);

        return true;
    }

    return false;
}

bool Users::User::removeNotification(Wt::Dbo::ptr<Database::Notification> notification)
{
    if(hasNotification(notification))
    {
        m_notifications.erase(notification);

        return true;
    }

    return false;
}

int Users::User::createRank() const
{
    return m_createRank;
}

void Users::User::setCreateRank(int rank)
{
    m_createRank = rank;
}

std::string Users::User::idNumber() const
{
    return m_idNumber;
}

void Users::User::setIdNumber(const std::string &idNumber)
{
    m_idNumber = idNumber;
}

bool Users::User::isAvailable() const
{
    return m_available;
}

void Users::User::setAvailable(bool available)
{
    m_available = available;
}

std::string Users::User::phoneNumber() const
{
    return m_phoneNumber;
}

void Users::User::setPhoneNumber(const std::string &phoneNumber)
{
    m_phoneNumber = phoneNumber;
}

std::string Users::User::emailAddress() const
{
    return m_emailAddress;
}

void Users::User::setEmailAddress(const std::string &emailAddress)
{
    m_emailAddress = emailAddress;
}

std::string Users::User::address() const
{
    return m_address;
}

void Users::User::setAddress(const std::string &address)
{
    m_address = address;
}

Wt::Dbo::ptr<Users::Group> Users::User::group() const
{
    return m_group;
}

void Users::User::setGroup(Wt::Dbo::ptr<Group> group)
{
    m_group = group;
}

Wt::Dbo::ptr<Users::UserTitle> Users::User::title() const
{
    return m_title;
}

void Users::User::setTitle(Wt::Dbo::ptr<Users::UserTitle> title)
{
    m_title = title;
}

Wt::Dbo::weak_ptr<Projects::Project> Users::User::project() const
{
    return m_project;
}

void Users::User::setProject(Wt::Dbo::weak_ptr<Projects::Project> project)
{
    m_project = project;
}

Wt::WDateTime Users::User::lastSeenNotificationsDate() const
{
    return m_lastSeenNotificationDate;
}

void Users::User::setLastSeenNotificationsDate(const Wt::WDateTime &date)
{
    m_lastSeenNotificationDate = date;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask> > Users::User::tasks()
{
    return m_tasks;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Token> > Users::User::authTokens() const
{
    return m_authTokens;
}

bool Users::User::operator ==(const Users::User &other) const
{
    return m_name == other.name();
}

bool Users::User::operator !=(const Users::User &other) const
{
    return !operator==(other);
}

void Users::User::init()
{
    m_thumbnail = "pics/NoPreview.png";
    m_name = "New User";
    m_password = "";
    m_passwordMethod = "";
    m_passwordSalt = "";
    m_failedLoginAttempts = 0;
    m_lastLoginAttempt = Wt::WDateTime::currentDateTime();
    m_oAuthId = "";
    m_oAuthProvider = "";
    m_idNumber = "";
    m_phoneNumber = "";
    m_emailAddress = "";
    m_address = "";
    m_createRank = 0;
    m_lastSeenNotificationDate = Wt::WDateTime::currentDateTime().addMonths(-1);//set last notifications seen date to a month old when creating new users
}
