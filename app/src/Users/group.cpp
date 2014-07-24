#include "../Database/dbtables.h"
#include "../Database/databasemanager.h"
#include "../Log/logmanager.h"

#include <Ms/Exceptions/MNullPointerException.h>

Users::Group::Group() :
    Database::Dbo()
{
    _init();
}

Users::Group::Group(const std::string &groupName, int rank) :
    Group()
{
    _name = groupName;
    _rank = rank;
}

Users::Group *Users::Group::modify()
{
    Database::Dbo::modify();

    return this;
}

std::string Users::Group::name() const
{
    return _name;
}

void Users::Group::setName(const std::string &name)
{
    _name = name;
}

int Users::Group::rank() const
{
    return _rank;
}

bool Users::Group::hasUser(Wt::Dbo::ptr<Users::User> user) const
{
    if(dboManager_ && dboManager_->openTransaction())
    {
        for(auto iter = _users.begin(); iter != _users.end(); ++iter)
        {
            if((*iter).id() == user.id())
                return true;
        }
    }

    return false;
}

bool Users::Group::addUser(Wt::Dbo::ptr<Users::User> user)
{
    if(!hasUser(user))
    {
        _users.insert(user);
        return true;
    }

    return false;
}

bool Users::Group::removeUser(Wt::Dbo::ptr<User> user)
{
    if(hasUser(user))
    {
        _users.erase(user);
        return true;
    }

    return false;
}

bool Users::Group::hasPrivilege(Wt::Dbo::ptr<Users::Privilege> privilege) const
{
    if(dboManager_ && dboManager_->openTransaction())
    {
        for(auto iter = _privileges.begin(); iter != _privileges.end(); ++iter)
        {
            if((*iter).id() == privilege.id())
                return true;
        }
    }

    return false;
}

bool Users::Group::hasPrivilege(const char *privilegeName) const
{
    if(dboManager_ && dboManager_->openTransaction())
    {
        for(auto iter = _privileges.begin(); iter != _privileges.end(); ++iter)
        {
            if((*iter)->name() == privilegeName)
            {
                return true;
            }
        }
    }

    return false;
}

bool Users::Group::addPrivilege(Wt::Dbo::ptr<Users::Privilege> privilege)
{
    if(!hasPrivilege(privilege))
    {
        _privileges.insert(privilege);
        return true;
    }

    return false;
}

bool Users::Group::removePrivilege(Wt::Dbo::ptr<Users::Privilege> privilege)
{
    if(hasPrivilege(privilege))
    {
        _privileges.erase(privilege);
        return true;
    }

    return false;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Privilege> > Users::Group::privileges() const
{
    return _privileges;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User> > Users::Group::users() const
{
    return _users;
}

void Users::Group::setRank(int rank)
{
    _rank = rank;
}

bool Users::Group::operator ==(const Users::Group &other) const
{
    return _name == other.name();
}

bool Users::Group::operator !=(const Users::Group &other) const
{
    return !operator==(other);
}

void Users::Group::_init()
{
    dboManager_ = &Database::DatabaseManager::instance();

    _name = "New Group";
    _rank = 0;
}
