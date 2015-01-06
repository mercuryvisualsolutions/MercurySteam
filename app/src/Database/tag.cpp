#include "Database/dbtables.h"

Database::Tag::Tag() :
    Ms::Dbo::MDboBase()
{
    init();
}

Database::Tag::Tag(const std::string &name, const std::string &content) :
    Tag()
{
    m_name = name;
    m_content = content;
}

Database::Tag *Database::Tag::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Database::Tag::name() const
{
    return m_name;
}

void Database::Tag::setName(const std::string &name)
{
    m_name = name;
}

std::string Database::Tag::content() const
{
    return m_content;
}

void Database::Tag::setContent(const std::string &content)
{
    m_content = content;
}

std::string Database::Tag::type() const
{
    return m_type;
}

void Database::Tag::setType(const std::string &type)
{
    m_type = type;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> Database::Tag::users() const
{
    return m_users;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group> > Database::Tag::groups() const
{
    return m_groups;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project>> Database::Tag::projects() const
{
    return m_projects;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>> Database::Tag::sequences() const
{
    return m_sequences;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>> Database::Tag::shots() const
{
    return m_shots;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset> > Database::Tag::assets() const
{
    return m_assets;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask> > Database::Tag::tasks() const
{
    return m_tasks;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User> > Database::Tag::usersAssignedTo() const
{
    return m_usersAssignedTo;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group> > Database::Tag::groupsAssignedTo() const
{
    return m_groupsAssignedTo;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project> > Database::Tag::projectsAssignedTo() const
{
    return m_projectsAssignedTo;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence> > Database::Tag::sequencesAssignedTo() const
{
    return m_sequencesAssignedTo;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot> > Database::Tag::shotsAssignedTo() const
{
    return m_shotsAssignedTo;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset> > Database::Tag::assetsAssignedTo() const
{
    return m_assetsAssignedTo;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask> > Database::Tag::tasksAssignedTo() const
{
    return m_tasksAssignedTo;
}

bool Database::Tag::operator ==(const Database::Tag &other) const
{
    return (m_name == other.name()) && (m_content == other.content()) && (m_type == other.type());
}

bool Database::Tag::operator !=(const Database::Tag &other) const
{
    return !operator ==(other);
}

void Database::Tag::init()
{
    m_thumbnail = "pics/NoPreview.png";
    m_name = "New Tag";
    m_content = "";
    m_type = "Global";
}
