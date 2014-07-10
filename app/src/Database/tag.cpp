#include "Database/dbtables.h"

Database::Tag::Tag() :
    Ms::Dbo::MDbo()
{
    _init();
}

Database::Tag::Tag(const std::string &name, const std::string &content) :
    Tag()
{
    _tagId.name = name;
    _tagId.content = content;
}

Database::Tag *Database::Tag::modify()
{
    Ms::Dbo::MDbo::modify();

    return this;
}

const Database::TagId Database::Tag::id() const
{
    return _tagId;
}

std::string Database::Tag::name() const
{
    return _tagId.name;
}

void Database::Tag::setName(const std::string &name)
{
    _tagId.name = name;
}

std::string Database::Tag::content() const
{
    return _tagId.content;
}

void Database::Tag::setContent(const std::string &content)
{
    _tagId.content = content;
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

bool Database::Tag::operator ==(const Database::Tag &other) const
{
    return _tagId == other.id();
}

bool Database::Tag::operator !=(const Database::Tag &other) const
{
    return !operator ==(other);
}

void Database::Tag::_init()
{
    _tagId.name = "New Tag";
    _tagId.content = "";
}
