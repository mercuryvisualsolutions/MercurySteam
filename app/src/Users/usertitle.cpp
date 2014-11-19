#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"
#include "../Log/logmanager.h"

Users::UserTitle::UserTitle() :
    Ms::Dbo::MDboBase()
{
    _init();
}

Users::UserTitle::UserTitle(const std::string &titleName) :
    UserTitle()
{
    _name = titleName;
}

Users::UserTitle *Users::UserTitle::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Users::UserTitle::name() const
{
    return _name;
}

void Users::UserTitle::setName(const std::string &name)
{
    _name = name;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User> > Users::UserTitle::users() const
{
    return _users;
}

bool Users::UserTitle::operator ==(const Users::UserTitle &other) const
{
    return _name == other.name();
}

bool Users::UserTitle::operator !=(const Users::UserTitle &other) const
{
    return !operator==(other);
}

void Users::UserTitle::_init()
{
    _name = "New User Title";
}
