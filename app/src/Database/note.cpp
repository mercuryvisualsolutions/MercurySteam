#include "Database/dbtables.h"

Database::Note::Note() :
    Ms::Dbo::MDboBase()
{
    _init();
}

Database::Note::Note(const std::string &content) :
    Note()
{
    _content = content;
}

Database::Note *Database::Note::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Database::Note::content() const
{
    return _content;
}

void Database::Note::setContent(const std::string &content)
{
    _content = content;
}

Wt::Dbo::ptr<Users::User> Database::Note::user() const
{
    return _user;
}

void Database::Note::setUser(Wt::Dbo::ptr<Users::User> user)
{
    _user = user;
}

Wt::Dbo::ptr<Users::Group> Database::Note::group() const
{
    return _group;
}

void Database::Note::setGroup(Wt::Dbo::ptr<Users::Group> group)
{
    _group = group;
}

Wt::Dbo::ptr<Projects::Project> Database::Note::project() const
{
    return _project;
}

void Database::Note::setProject(Wt::Dbo::ptr<Projects::Project> project)
{
    _project = project;
}

Wt::Dbo::ptr<Projects::ProjectSequence> Database::Note::sequence() const
{
    return _sequence;
}

void Database::Note::setSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    _sequence = _sequence;
}

Wt::Dbo::ptr<Projects::ProjectShot> Database::Note::shot() const
{
    return _shot;
}

void Database::Note::setShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    _shot = shot;
}

Wt::Dbo::ptr<Projects::ProjectAsset> Database::Note::asset() const
{
    return _asset;
}

void Database::Note::setAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    _asset = asset;
}

Wt::Dbo::ptr<Projects::ProjectTask> Database::Note::task() const
{
    return _task;
}

void Database::Note::setTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    _task = task;
}

bool Database::Note::operator ==(const Database::Note &other) const
{
    return _content == other.content() &&
            _user == other.user() &&
            _project == other.project() &&
            _sequence == other.sequence() &&
            _shot == other.shot() &&
            _asset == other.asset() &&
            _task == other.task();
}

bool Database::Note::operator !=(const Database::Note &other) const
{
    return !operator ==(other);
}

void Database::Note::_init()
{
    thumbnail_ = "pics/NoPreview.png";
    _content = "";
}
