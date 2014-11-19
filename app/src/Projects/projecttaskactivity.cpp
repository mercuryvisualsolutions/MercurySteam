#include "../Database/dbtables.h"
#include "Database/dbosession.h"

Projects::ProjectTaskActivity::ProjectTaskActivity() :
    Ms::Dbo::MDboBase()
{
    _init();
}

Projects::ProjectTaskActivity *Projects::ProjectTaskActivity::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

int Projects::ProjectTaskActivity::hours() const
{
    return _hours;
}

void Projects::ProjectTaskActivity::setHours(int hours)
{
    _hours = hours;
}

std::string Projects::ProjectTaskActivity::description() const
{
    return _description;
}

void Projects::ProjectTaskActivity::setDescription(const std::string &description)
{
    _description = description;
}

const Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectTaskActivity::status() const
{
    return _status;
}

void Projects::ProjectTaskActivity::setStatus(const Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    _status = status;
}

const Wt::Dbo::ptr<Projects::ProjectTaskActivityType> Projects::ProjectTaskActivity::type() const
{
    return _type;
}

void Projects::ProjectTaskActivity::setType(const Wt::Dbo::ptr<Projects::ProjectTaskActivityType> type)
{
    _type = type;
}

const Wt::Dbo::ptr<Projects::ProjectTask> Projects::ProjectTaskActivity::task() const
{
    return _task;
}

void Projects::ProjectTaskActivity::setTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    _task = task;
}

bool Projects::ProjectTaskActivity::operator ==(const Projects::ProjectTaskActivity &other) const
{
    return _type.id() == other.type().id();
}

bool Projects::ProjectTaskActivity::operator !=(const ProjectTaskActivity &other) const
{
    return !operator==(other);
}

void Projects::ProjectTaskActivity::_init()
{
    _hours = 0;
}
