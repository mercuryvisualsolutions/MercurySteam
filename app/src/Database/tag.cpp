#include "Database/dbtables.h"

Database::Tag::Tag() :
    Ms::Dbo::MDboBase()
{
    _init();
}

Database::Tag::Tag(const std::string &name, const std::string &content) :
    Tag()
{
    _name = name;
    _content = content;
}

Database::Tag *Database::Tag::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Database::Tag::name() const
{
    return _name;
}

void Database::Tag::setName(const std::string &name)
{
    _name = name;
}

std::string Database::Tag::content() const
{
    return _content;
}

void Database::Tag::setContent(const std::string &content)
{
    _content = content;
}

std::string Database::Tag::type() const
{
    return _type;
}

void Database::Tag::setType(const std::string &type)
{
    _type = type;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> Database::Tag::users() const
{
    return _users;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group> > Database::Tag::groups() const
{
    return _groups;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project>> Database::Tag::projects() const
{
    return _projects;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>> Database::Tag::sequences() const
{
    return _sequences;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>> Database::Tag::shots() const
{
    return _shots;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset> > Database::Tag::assets() const
{
    return _assets;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask> > Database::Tag::tasks() const
{
    return _tasks;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User> > Database::Tag::usersAssignedTo() const
{
    return _usersAssignedTo;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group> > Database::Tag::groupsAssignedTo() const
{
    return _groupsAssignedTo;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project> > Database::Tag::projectsAssignedTo() const
{
    return _projectsAssignedTo;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence> > Database::Tag::sequencesAssignedTo() const
{
    return _sequencesAssignedTo;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot> > Database::Tag::shotsAssignedTo() const
{
    return _shotsAssignedTo;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset> > Database::Tag::assetsAssignedTo() const
{
    return _assetsAssignedTo;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask> > Database::Tag::tasksAssignedTo() const
{
    return _tasksAssignedTo;
}

bool Database::Tag::operator ==(const Database::Tag &other) const
{
    return (_name == other.name()) && (_content == other.content()) && (_type == other.type());
}

bool Database::Tag::operator !=(const Database::Tag &other) const
{
    return !operator ==(other);
}

void Database::Tag::_init()
{
    thumbnail_ = "pics/NoPreview.png";
    _name = "New Tag";
    _content = "";
    _type = "Global";
}
