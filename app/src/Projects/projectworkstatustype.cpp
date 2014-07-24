#include "../Database/dbtables.h"
#include "../Database/databasemanager.h"

Projects::ProjectWorkStatusType::ProjectWorkStatusType() :
    Ms::Dbo::MDboBase()
{
    _init();
}

Projects::ProjectWorkStatusType::ProjectWorkStatusType(const std::string &workStatusType) :
    ProjectWorkStatusType()
{
    _workStatusType = workStatusType;
}

Projects::ProjectWorkStatusType *Projects::ProjectWorkStatusType::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Projects::ProjectWorkStatusType::workStatusType() const
{
    return _workStatusType;
}

void Projects::ProjectWorkStatusType::setWorkStatusType(const std::string &type)
{
    _workStatusType = type;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectWorkStatus>>::size_type Projects::ProjectWorkStatusType::numWorkStatus() const
{
    return _projectWorkStatus.size();
}

bool Projects::ProjectWorkStatusType::operator ==(const Projects::ProjectWorkStatusType &other) const
{
    return _workStatusType == other.workStatusType();
}

bool Projects::ProjectWorkStatusType::operator !=(const Projects::ProjectWorkStatusType &other) const
{
    return !operator==(other);
}

void Projects::ProjectWorkStatusType::_init()
{
    dboManager_ = &Database::DatabaseManager::instance();

    _workStatusType = "New Work Status Type";
}
