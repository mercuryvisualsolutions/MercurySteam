#include "Database/dbtables.h"


Database::DboData::DboData() :
    Ms::Dbo::MDbo()
{
    _init();
}

Database::DboData::DboData(const std::string &key, const std::string &value) :
    DboData()
{
    _dboKey = key;
    _dboValue = value;
}

Database::DboData *Database::DboData::modify()
{
    Ms::Dbo::MDbo::modify();

    return this;
}

std::string Database::DboData::key() const
{
    return _dboKey;
}

void Database::DboData::setKey(const std::string &key)
{
    _dboKey = key;
}

std::string Database::DboData::value() const
{
    return _dboValue;
}

void Database::DboData::setValue(const std::string &value)
{
    _dboValue = value;
}

Wt::Dbo::ptr<Users::User> Database::DboData::user() const
{
    return _user;
}

void Database::DboData::setUser(Wt::Dbo::ptr<Users::User> user)
{
    _user = user;
}

Wt::Dbo::ptr<Users::Group> Database::DboData::group() const
{
    return _group;
}

void Database::DboData::setGroup(Wt::Dbo::ptr<Users::Group> group)
{
    _group = group;
}

Wt::Dbo::ptr<Projects::Project> Database::DboData::project() const
{
    return _project;
}

void Database::DboData::setProject(Wt::Dbo::ptr<Projects::Project> project)
{
    _project = project;
}

Wt::Dbo::ptr<Projects::ProjectSequence> Database::DboData::sequence() const
{
    return _sequence;
}

void Database::DboData::setSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    _sequence = _sequence;
}

Wt::Dbo::ptr<Projects::ProjectShot> Database::DboData::shot() const
{
    return _shot;
}

void Database::DboData::setShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    _shot = shot;
}

Wt::Dbo::ptr<Projects::ProjectAsset> Database::DboData::asset() const
{
    return _asset;
}

void Database::DboData::setAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    _asset = asset;
}

Wt::Dbo::ptr<Projects::ProjectTask> Database::DboData::task() const
{
    return _task;
}

void Database::DboData::setTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    _task = task;
}

bool Database::DboData::operator ==(const Database::DboData &other) const
{
    return _dboKey == other.key() &&
            _dboValue == other.value() &&
            _user == other.user() &&
            _project == other.project() &&
            _sequence == other.sequence() &&
            _shot == other.shot() &&
            _asset == other.asset() &&
            _task == other.task();
}

bool Database::DboData::operator !=(const Database::DboData &other) const
{
    return !operator ==(other);
}

void Database::DboData::_init()
{
    _dboKey = "";
    _dboValue = "";
}
