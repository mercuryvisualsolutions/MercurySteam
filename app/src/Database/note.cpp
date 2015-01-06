#include "Database/dbtables.h"

Database::Note::Note() :
    Ms::Dbo::MDboBase()
{
    init();
}

Database::Note::Note(const std::string &content) :
    Note()
{
    m_content = content;
}

Database::Note *Database::Note::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Database::Note::content() const
{
    return m_content;
}

void Database::Note::setContent(const std::string &content)
{
    m_content = content;
}

Wt::Dbo::ptr<Users::User> Database::Note::user() const
{
    return m_user;
}

void Database::Note::setUser(Wt::Dbo::ptr<Users::User> user)
{
    m_user = user;
}

Wt::Dbo::ptr<Users::Group> Database::Note::group() const
{
    return m_group;
}

void Database::Note::setGroup(Wt::Dbo::ptr<Users::Group> group)
{
    m_group = group;
}

Wt::Dbo::ptr<Projects::Project> Database::Note::project() const
{
    return m_project;
}

void Database::Note::setProject(Wt::Dbo::ptr<Projects::Project> project)
{
    m_project = project;
}

Wt::Dbo::ptr<Projects::ProjectSequence> Database::Note::sequence() const
{
    return m_sequence;
}

void Database::Note::setSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    m_sequence = m_sequence;
}

Wt::Dbo::ptr<Projects::ProjectShot> Database::Note::shot() const
{
    return m_shot;
}

void Database::Note::setShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    m_shot = shot;
}

Wt::Dbo::ptr<Projects::ProjectAsset> Database::Note::asset() const
{
    return m_asset;
}

void Database::Note::setAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    m_asset = asset;
}

Wt::Dbo::ptr<Projects::ProjectTask> Database::Note::task() const
{
    return m_task;
}

void Database::Note::setTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    m_task = task;
}

bool Database::Note::operator ==(const Database::Note &other) const
{
    return m_content == other.content() &&
            m_user == other.user() &&
            m_project == other.project() &&
            m_sequence == other.sequence() &&
            m_shot == other.shot() &&
            m_asset == other.asset() &&
            m_task == other.task();
}

bool Database::Note::operator !=(const Database::Note &other) const
{
    return !operator ==(other);
}

void Database::Note::init()
{
    m_thumbnail = "pics/NoPreview.png";
    m_content = "";
}
