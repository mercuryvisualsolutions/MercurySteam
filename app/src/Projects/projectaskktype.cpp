#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectTaskType::ProjectTaskType() :
    Ms::Dbo::MDboBase()
{
    init();
}

Projects::ProjectTaskType::ProjectTaskType(const std::string &type) :
    ProjectTaskType()
{
    m_type = type;
}

Projects::ProjectTaskType *Projects::ProjectTaskType::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Projects::ProjectTaskType::type() const
{
    return m_type;
}

void Projects::ProjectTaskType::setType(const std::string &type)
{
    m_type = type;
}

Wt::Dbo::collection<Projects::ProjectTask>::size_type Projects::ProjectTaskType::numTasks() const
{
    return m_tasks.size();
}

bool Projects::ProjectTaskType::operator ==(const Projects::ProjectTaskType &other) const
{
    return m_type == other.type();
}

bool Projects::ProjectTaskType::operator !=(const Projects::ProjectTaskType &other) const
{
    return !operator==(other);
}

void Projects::ProjectTaskType::init()
{
    m_type = "New Task Type";
}
