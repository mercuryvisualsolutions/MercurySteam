#include "../Database/dbtables.h"
#include "../Database/databasemanager.h"
#include "../Log/logmanager.h"

Users::Privilege::Privilege() :
    Ms::Dbo::MDbo()
{
    _init();
}

Users::Privilege::Privilege(const std::string &privilegeName) :
    Privilege()
{
    _name = privilegeName;
}

Users::Privilege *Users::Privilege::modify()
{
    Ms::Dbo::MDbo::modify();

    return this;
}

std::string Users::Privilege::name() const
{
    return _name;
}

void Users::Privilege::setName(const std::string &name)
{
    _name = name;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group>> Users::Privilege::groups() const
{
    return _groups;
}

bool Users::Privilege::operator ==(const Users::Privilege &other) const
{
    return _name == other.name();
}

bool Users::Privilege::operator !=(const Users::Privilege &other) const
{
    return !operator ==(other);
}

void Users::Privilege::_init()
{
    dboManager_ = &Database::DatabaseManager::instance();

    _name = "New Privilege";
}
