#include "../Database/dbtables.h"
#include "../Database/databasemanager.h"

Projects::ProjectWorkStatus::ProjectWorkStatus() :
    Ms::Dbo::MDbo()
{
    _init();
}

Projects::ProjectWorkStatus::ProjectWorkStatus(const std::string &status) :
    ProjectWorkStatus()
{
    _status = status;
}

Projects::ProjectWorkStatus *Projects::ProjectWorkStatus::modify()
{
    Ms::Dbo::MDbo::modify();

    return this;
}

std::string Projects::ProjectWorkStatus::status() const
{
    return _status;
}

void Projects::ProjectWorkStatus::setStatus(const std::string &status)
{
    _status = status;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatusType> Projects::ProjectWorkStatus::workStatusType() const
{
    return _workStatusType;
}

void Projects::ProjectWorkStatus::setWorkStatusType(const Wt::Dbo::ptr<Projects::ProjectWorkStatusType> &workStatusType)
{
    _workStatusType = workStatusType;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>>::size_type Projects::ProjectWorkStatus::numSequences() const
{
    return _sequences.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>>::size_type Projects::ProjectWorkStatus::numTasks() const
{
    return _tasks.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>>::size_type Projects::ProjectWorkStatus::numAssets() const
{
    return _assets.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>>::size_type Projects::ProjectWorkStatus::numShots() const
{
    return _shots.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project>>::size_type Projects::ProjectWorkStatus::numProjects() const
{
    return _projects.size();
}

bool Projects::ProjectWorkStatus::operator ==(const Projects::ProjectWorkStatus &other) const
{
    return _status == other.status();
}

bool Projects::ProjectWorkStatus::operator !=(const Projects::ProjectWorkStatus &other) const
{
    return !operator==(other);
}

void Projects::ProjectWorkStatus::_init()
{
    dboManager_ = &Database::DatabaseManager::instance();

    _status = "New Work Status";
}
