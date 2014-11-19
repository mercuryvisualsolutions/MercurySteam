#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

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
    _workStatusType = "New Work Status Type";
}
