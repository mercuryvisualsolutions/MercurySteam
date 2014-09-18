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

bool Projects::ProjectTaskPipeline::hasItem(Wt::Dbo::ptr<Projects::ProjectTaskPipelineActivityItem> taskItem) const
{
    if(dboManager_ && dboManager_->openTransaction())
    {
        for(auto iter = _taskItems.begin(); iter != _taskItems.end(); ++iter)
        {
            if((*iter).id() == taskItem.id())
            {
                return true;
            }
        }
    }

    return false;
}

bool Projects::ProjectTaskPipeline::addItem(Wt::Dbo::ptr<Projects::ProjectTaskPipelineActivityItem> taskItem)
{
    if(!hasItem(taskItem))
    {
        _taskItems.insert(taskItem);
        return true;
    }

    return false;
}

bool Projects::ProjectTaskPipeline::removeItem(Wt::Dbo::ptr<Projects::ProjectTaskPipelineActivityItem> taskItem)
{
    if(hasItem(taskItem))
    {
        _taskItems.erase(taskItem);
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
