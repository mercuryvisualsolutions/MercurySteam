#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"
#include "../Log/logmanager.h"

Users::UserTitle::UserTitle() :
    Ms::Dbo::MDboBase()
{
    init();
}

Users::UserTitle::UserTitle(const std::string &titleName) :
    UserTitle()
{
    m_name = titleName;
}

Users::UserTitle *Users::UserTitle::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Users::UserTitle::name() const
{
    return m_name;
}

void Users::UserTitle::setName(const std::string &name)
{
    m_name = name;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User> > Users::UserTitle::users() const
{
    return m_users;
}

bool Users::UserTitle::operator ==(const Users::UserTitle &other) const
{
    return m_name == other.name();
}

bool Users::UserTitle::operator !=(const Users::UserTitle &other) const
{
    return !operator==(other);
}

void Users::UserTitle::init()
{
    m_name = "New User Title";
}
