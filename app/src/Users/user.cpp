#include "../Database/dbtables.h"
#include "usersmanager.h"

Users::User::User() :
    Ms::Dbo::MDbo()
{
    _init();
}

Users::User::User(const std::string &name) :
    User()
{
    _name = name;
}

Users::User::User(const std::string &name, const std::string &emailAddress) :
    User(name)
{
    _emailAddress = emailAddress;
}

Users::User *Users::User::modify()
{
    Ms::Dbo::MDbo::modify();

    return this;
}

std::string Users::User::name() const
{
    return _name;
}

void Users::User::setName(const std::string &name)
{
    _name = name;
}

std::string Users::User::idNumber() const
{
    return _idNumber;
}

void Users::User::setIdNumber(const std::string &idNumber)
{
    _idNumber = idNumber;
}

std::string Users::User::phoneNumber() const
{
    return _phoneNumber;
}

void Users::User::setPhoneNumber(const std::string &phoneNumber)
{
    _phoneNumber = phoneNumber;
}

std::string Users::User::emailAddress() const
{
    return _emailAddress;
}

void Users::User::setEmailAddress(const std::string &emailAddress)
{
    _emailAddress = emailAddress;
}

Wt::WDateTime Users::User::lastLogin() const
{
    return _lastLogin;
}

std::string Users::User::address() const
{
    return _address;
}

void Users::User::setAddress(const std::string &address)
{
    _address = address;
}

Wt::Dbo::ptr<Users::Group> Users::User::group() const
{
    return _group;
}

void Users::User::setGroup(Wt::Dbo::ptr<Group> group)
{
    _group = group;
}

Wt::Dbo::ptr<Users::UserTitle> Users::User::title() const
{
    return _title;
}

void Users::User::setTitle(Wt::Dbo::ptr<Users::UserTitle> title)
{
    _title = title;
}

Wt::Dbo::weak_ptr<Projects::Project> Users::User::project() const
{
    return _project;
}

void Users::User::setProject(Wt::Dbo::weak_ptr<Projects::Project> project)
{
    _project = project;
}

int Users::User::createRank() const
{
    return _createRank;
}

void Users::User::setCreateRank(int rank)
{
    _createRank = rank;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask> > Users::User::tasks()
{
    return _tasks;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData> > Users::User::data()
{
    return _data;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note> > Users::User::notes()
{
    return _notes;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>>::size_type Users::User::numTasks() const
{
    return _tasks.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>>::size_type Users::User::numData() const
{
    return _data.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>>::size_type Users::User::numNotes() const
{
    return _notes.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>>::size_type Users::User::numTags() const
{
    return _tags.size();
}

bool Users::User::operator ==(const Users::User &other) const
{
    return _name == other.name();
}

bool Users::User::operator !=(const Users::User &other) const
{
    return !operator==(other);
}

void Users::User::_init()
{
    thumbnail_ = "pics/NoPreview.png";
    _name = "New User";
    _idNumber = "";
    _phoneNumber = "";
    _emailAddress = "";
    _address = "";
}
