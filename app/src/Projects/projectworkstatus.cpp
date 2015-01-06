#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectWorkStatus::ProjectWorkStatus() :
    Ms::Dbo::MDboBase()
{
    init();
}

Projects::ProjectWorkStatus::ProjectWorkStatus(const std::string &status) :
    ProjectWorkStatus()
{
    m_status = status;
}

Projects::ProjectWorkStatus *Projects::ProjectWorkStatus::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Projects::ProjectWorkStatus::status() const
{
    return m_status;
}

void Projects::ProjectWorkStatus::setStatus(const std::string &status)
{
    m_status = status;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatusType> Projects::ProjectWorkStatus::workStatusType() const
{
    return m_workStatusType;
}

void Projects::ProjectWorkStatus::setWorkStatusType(const Wt::Dbo::ptr<Projects::ProjectWorkStatusType> &workStatusType)
{
    m_workStatusType = workStatusType;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>>::size_type Projects::ProjectWorkStatus::numSequences() const
{
    return m_sequences.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>>::size_type Projects::ProjectWorkStatus::numTasks() const
{
    return m_tasks.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>>::size_type Projects::ProjectWorkStatus::numAssets() const
{
    return m_assets.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>>::size_type Projects::ProjectWorkStatus::numShots() const
{
    return m_shots.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project>>::size_type Projects::ProjectWorkStatus::numProjects() const
{
    return m_projects.size();
}

bool Projects::ProjectWorkStatus::operator ==(const Projects::ProjectWorkStatus &other) const
{
    return m_status == other.status();
}

bool Projects::ProjectWorkStatus::operator !=(const Projects::ProjectWorkStatus &other) const
{
    return !operator==(other);
}

void Projects::ProjectWorkStatus::init()
{
    m_status = "New Work Status";
}
