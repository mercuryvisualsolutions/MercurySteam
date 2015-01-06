#include "Database/dbtables.h"


Database::DboData::DboData() :
    Ms::Dbo::MDboBase()
{
    init();
}

Database::DboData::DboData(const std::string &key, const std::string &value) :
    DboData()
{
    m_dboKey = key;
    m_dboValue = value;
}

Database::DboData *Database::DboData::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Database::DboData::key() const
{
    return m_dboKey;
}

void Database::DboData::setKey(const std::string &key)
{
    m_dboKey = key;
}

std::string Database::DboData::value() const
{
    return m_dboValue;
}

void Database::DboData::setValue(const std::string &value)
{
    m_dboValue = value;
}

Wt::Dbo::ptr<Users::User> Database::DboData::user() const
{
    return m_user;
}

void Database::DboData::setUser(Wt::Dbo::ptr<Users::User> user)
{
    m_user = user;
}

Wt::Dbo::ptr<Users::Group> Database::DboData::group() const
{
    return m_group;
}

void Database::DboData::setGroup(Wt::Dbo::ptr<Users::Group> group)
{
    m_group = group;
}

Wt::Dbo::ptr<Projects::Project> Database::DboData::project() const
{
    return m_project;
}

void Database::DboData::setProject(Wt::Dbo::ptr<Projects::Project> project)
{
    m_project = project;
}

Wt::Dbo::ptr<Projects::ProjectSequence> Database::DboData::sequence() const
{
    return m_sequence;
}

void Database::DboData::setSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    m_sequence = m_sequence;
}

Wt::Dbo::ptr<Projects::ProjectShot> Database::DboData::shot() const
{
    return m_shot;
}

void Database::DboData::setShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    m_shot = shot;
}

Wt::Dbo::ptr<Projects::ProjectAsset> Database::DboData::asset() const
{
    return m_asset;
}

void Database::DboData::setAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    m_asset = asset;
}

Wt::Dbo::ptr<Projects::ProjectTask> Database::DboData::task() const
{
    return m_task;
}

void Database::DboData::setTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    m_task = task;
}

bool Database::DboData::operator ==(const Database::DboData &other) const
{
    return m_dboKey == other.key() &&
            m_dboValue == other.value() &&
            m_user == other.user() &&
            m_project == other.project() &&
            m_sequence == other.sequence() &&
            m_shot == other.shot() &&
            m_asset == other.asset() &&
            m_task == other.task();
}

bool Database::DboData::operator !=(const Database::DboData &other) const
{
    return !operator ==(other);
}

void Database::DboData::init()
{
    m_thumbnail = "pics/NoPreview.png";
    m_dboKey = "";
    m_dboValue = "";
}
