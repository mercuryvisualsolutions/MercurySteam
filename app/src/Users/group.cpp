#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"
#include "../Log/logmanager.h"

#include <Ms/Exceptions/MNullPointerException.h>

Users::Group::Group() :
    Database::Dbo()
{
    init();
}

Users::Group::Group(const std::string &groupName, int rank) :
    Group()
{
    m_name = groupName;
    m_rank = rank;
}

Users::Group *Users::Group::modify()
{
    Database::Dbo::modify();

    return this;
}

std::string Users::Group::name() const
{
    return m_name;
}

void Users::Group::setName(const std::string &name)
{
    m_name = name;
}

int Users::Group::rank() const
{
    return m_rank;
}

bool Users::Group::hasUser(Wt::Dbo::ptr<Users::User> user) const
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

bool Users::Group::addUser(Wt::Dbo::ptr<Users::User> user)
{
    if(!hasUser(user))
    {
        m_users.insert(user);
        return true;
    }

    return false;
}

bool Users::Group::removeUser(Wt::Dbo::ptr<User> user)
{
    if(hasUser(user))
    {
        m_users.erase(user);
        return true;
    }

    return false;
}

bool Users::Group::hasPrivilege(Wt::Dbo::ptr<Users::Privilege> privilege) const
{
    bool result = false;

    for(auto iter = m_privileges.begin(); iter != m_privileges.end(); ++iter)
    {
        if((*iter).id() == privilege.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Users::Group::hasPrivilege(const char *privilegeName) const
{
    bool result = false;

    for(auto iter = m_privileges.begin(); iter != m_privileges.end(); ++iter)
    {
        if((*iter)->name() == privilegeName)
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Users::Group::addPrivilege(Wt::Dbo::ptr<Users::Privilege> privilege)
{
    if(!hasPrivilege(privilege))
    {
        m_privileges.insert(privilege);

        return true;
    }

    return false;
}

bool Users::Group::removePrivilege(Wt::Dbo::ptr<Users::Privilege> privilege)
{
    if(hasPrivilege(privilege))
    {
        m_privileges.erase(privilege);
        return true;
    }

    return false;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Privilege> > Users::Group::privileges() const
{
    return m_privileges;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User> > Users::Group::users() const
{
    return m_users;
}

void Users::Group::setRank(int rank)
{
    m_rank = rank;
}

bool Users::Group::operator ==(const Users::Group &other) const
{
    return m_name == other.name();
}

bool Users::Group::operator !=(const Users::Group &other) const
{
    return !operator==(other);
}

void Users::Group::init()
{
    m_name = "New Group";
    m_rank = 0;
}
