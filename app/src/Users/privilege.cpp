#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"
#include "../Log/logmanager.h"

Users::Privilege::Privilege() :
    Ms::Dbo::MDboBase()
{
    init();
}

Users::Privilege::Privilege(const std::string &privilegeName) :
    Privilege()
{
    m_name = privilegeName;
}

Users::Privilege *Users::Privilege::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Users::Privilege::name() const
{
    return m_name;
}

void Users::Privilege::setName(const std::string &name)
{
    m_name = name;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group>> Users::Privilege::groups() const
{
    return m_groups;
}

bool Users::Privilege::operator ==(const Users::Privilege &other) const
{
    return m_name == other.name();
}

bool Users::Privilege::operator !=(const Users::Privilege &other) const
{
    return !operator ==(other);
}

void Users::Privilege::init()
{
    m_name = "New Privilege";
}
