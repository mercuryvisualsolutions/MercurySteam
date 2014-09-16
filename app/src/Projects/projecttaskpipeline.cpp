#include "../Database/dbtables.h"
#include "Database/databasemanager.h"

Projects::ProjectTaskPipeline::ProjectTaskPipeline() :
    Ms::Dbo::MDboBase()
{
    _init();
}

Projects::ProjectTaskPipeline::ProjectTaskPipeline(const std::string &name) :
    ProjectTaskPipeline()
{
    _name = name;
}

Projects::ProjectTaskPipeline *Projects::ProjectTaskPipeline::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

const std::string Projects::ProjectTaskPipeline::name() const
{
    return _name;
}

void Projects::ProjectTaskPipeline::setName(const std::string &name)
{
    _name = name;
}

bool Projects::ProjectTaskPipeline::hasTask(Wt::Dbo::ptr<Projects::ProjectTask> task) const
{
    if(dboManager_ && dboManager_->openTransaction())
    {
        for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
        {
            if((*iter).id() == task.id())
            {
                return true;
            }
        }
    }

    return false;
}

bool Projects::ProjectTaskPipeline::addTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!hasTask(task))
    {
        _tasks.insert(task);
        return true;
    }

    return false;
}

bool Projects::ProjectTaskPipeline::removeTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(hasTask(task))
    {
        _tasks.erase(task);
        return true;
    }

    return false;
}

bool Projects::ProjectTaskPipeline::hasActivity(Wt::Dbo::ptr<Projects::ProjectTaskPipelineActivity> activity) const
{
    if(dboManager_ && dboManager_->openTransaction())
    {
        for(auto iter = _activities.begin(); iter != _activities.end(); ++iter)
        {
            if((*iter).id() == activity.id())
            {
                return true;
            }
        }
    }

    return false;
}

bool Projects::ProjectTaskPipeline::addActivity(Wt::Dbo::ptr<Projects::ProjectTaskPipelineActivity> activity)
{
    if(!hasActivity(activity))
    {
        _activities.insert(activity);
        return true;
    }

    return false;
}

bool Projects::ProjectTaskPipeline::removeActivity(Wt::Dbo::ptr<Projects::ProjectTaskPipelineActivity> activity)
{
    if(hasActivity(activity))
    {
        _activities.erase(activity);
        return true;
    }

    return false;
}

bool Projects::ProjectTaskPipeline::operator ==(const Projects::ProjectTaskPipeline &other) const
{
    return _name == other.name();
}

bool Projects::ProjectTaskPipeline::operator !=(const ProjectTaskPipeline &other) const
{
    return !operator ==(other);
}

void Projects::ProjectTaskPipeline::_init()
{
    _name = "New Pipeline";
}
