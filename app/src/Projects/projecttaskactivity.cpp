#include "../Database/dbtables.h"
#include "Database/dbosession.h"

Projects::ProjectTaskActivity::ProjectTaskActivity() :
    Ms::Dbo::MDboBase()
{
    init();
}

Projects::ProjectTaskActivity *Projects::ProjectTaskActivity::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

int Projects::ProjectTaskActivity::hours() const
{
    return m_hours;
}

void Projects::ProjectTaskActivity::setHours(int hours)
{
    m_hours = hours;
}

std::string Projects::ProjectTaskActivity::description() const
{
    return m_description;
}

void Projects::ProjectTaskActivity::setDescription(const std::string &description)
{
    m_description = description;
}

const Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectTaskActivity::status() const
{
    return m_status;
}

void Projects::ProjectTaskActivity::setStatus(const Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    m_status = status;
}

const Wt::Dbo::ptr<Projects::ProjectTaskActivityType> Projects::ProjectTaskActivity::type() const
{
    return m_type;
}

void Projects::ProjectTaskActivity::setType(const Wt::Dbo::ptr<Projects::ProjectTaskActivityType> type)
{
    m_type = type;
}

const Wt::Dbo::ptr<Projects::ProjectTask> Projects::ProjectTaskActivity::task() const
{
    return m_task;
}

void Projects::ProjectTaskActivity::setTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    m_task = task;
}

bool Projects::ProjectTaskActivity::operator ==(const Projects::ProjectTaskActivity &other) const
{
    return m_type.id() == other.type().id();
}

bool Projects::ProjectTaskActivity::operator !=(const ProjectTaskActivity &other) const
{
    return !operator==(other);
}

void Projects::ProjectTaskActivity::init()
{
    m_hours = 0;
}
