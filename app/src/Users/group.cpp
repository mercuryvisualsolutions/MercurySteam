#include "../Database/dbtables.h"

Users::Group::Group() :
    Ms::Dbo::MDbo()
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
    Ms::Dbo::MDbo::modify();

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

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Privilege> > Users::Group::privileges() const
{
    return _privileges;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User> > Users::Group::users() const
{
    return _users;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData> > Users::Group::data()
{
    return _data;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag> > Users::Group::tags()
{
    return _tags;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note> > Users::Group::notes()
{
    return _notes;
}

void Users::Group::setRank(int rank)
{
    _rank = rank;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Users::Privilege>>::size_type Users::Group::numPrivileges() const
{
    return _privileges.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>>::size_type Users::Group::numUsers() const
{
    return _users.size();
}

Wt::Dbo::collection<Database::DboData>::size_type Users::Group::numData() const
{
    return _data.size();
}

Wt::Dbo::collection<Database::Note>::size_type Users::Group::numNotes() const
{
    return _notes.size();
}

Wt::Dbo::collection<Database::Tag>::size_type Users::Group::numTags() const
{
    return _tags.size();
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
    _name = "New Group";
    _rank = 0;
}
