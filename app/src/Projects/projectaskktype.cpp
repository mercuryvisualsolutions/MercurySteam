#include "../Database/dbtables.h"

Projects::ProjectTaskType::ProjectTaskType() :
    Ms::Dbo::MDbo()
{
    _init();
}

Projects::ProjectTaskType::ProjectTaskType(const std::string &type) :
    ProjectTaskType()
{
    _type = type;
}

Projects::ProjectTaskType *Projects::ProjectTaskType::modify()
{
    Ms::Dbo::MDbo::modify();

    return this;
}

std::string Projects::ProjectTaskType::type() const
{
    return _type;
}

void Projects::ProjectTaskType::setType(const std::string &type)
{
    _type = type;
}

Wt::Dbo::collection<Projects::ProjectTask>::size_type Projects::ProjectTaskType::numTasks() const
{
    return _tasks.size();
}

bool Projects::ProjectTaskType::operator ==(const Projects::ProjectTaskType &other) const
{
    return _type == other.type();
}

bool Projects::ProjectTaskType::operator !=(const Projects::ProjectTaskType &other) const
{
    return !operator==(other);
}

void Projects::ProjectTaskType::_init()
{
    _type = "New Task Type";
}
