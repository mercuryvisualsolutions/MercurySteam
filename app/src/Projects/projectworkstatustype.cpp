#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectWorkStatusType::ProjectWorkStatusType() :
    Ms::Dbo::MDboBase()
{
    init();
}

Projects::ProjectWorkStatusType::ProjectWorkStatusType(const std::string &workStatusType) :
    ProjectWorkStatusType()
{
    m_workStatusType = workStatusType;
}

Projects::ProjectWorkStatusType *Projects::ProjectWorkStatusType::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Projects::ProjectWorkStatusType::workStatusType() const
{
    return m_workStatusType;
}

void Projects::ProjectWorkStatusType::setWorkStatusType(const std::string &type)
{
    m_workStatusType = type;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectWorkStatus>>::size_type Projects::ProjectWorkStatusType::numWorkStatus() const
{
    return m_projectWorkStatus.size();
}

bool Projects::ProjectWorkStatusType::operator ==(const Projects::ProjectWorkStatusType &other) const
{
    return m_workStatusType == other.workStatusType();
}

bool Projects::ProjectWorkStatusType::operator !=(const Projects::ProjectWorkStatusType &other) const
{
    return !operator==(other);
}

void Projects::ProjectWorkStatusType::init()
{
    m_workStatusType = "New Work Status Type";
}
