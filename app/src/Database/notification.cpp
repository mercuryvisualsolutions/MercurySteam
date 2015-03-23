#include "dbtables.h"

Database::Notification::Notification() :
    m_message("")
{
}

Database::Notification::Notification(const std::string &message) :
    m_message(message)
{
}

std::string Database::Notification::message() const
{
    return m_message;
}

void Database::Notification::setMessage(const std::string &message)
{
    m_message = message;
}

bool Database::Notification::hasUser(Wt::Dbo::ptr<Users::User> user) const
{
    bool result = false;

    for(auto iter = m_users.begin(); iter != m_users.end(); ++iter)
    {
        if((*iter).id() == user.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Database::Notification::addUser(Wt::Dbo::ptr<Users::User> user)
{
    if(!hasUser(user))
    {
        m_users.insert(user);

        return true;
    }

    return false;
}

bool Database::Notification::removeUser(Wt::Dbo::ptr<Users::User> user)
{
    if(hasUser(user))
    {
        m_users.erase(user);

        return true;
    }

    return false;
}

Database::Notification *Database::Notification::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}
