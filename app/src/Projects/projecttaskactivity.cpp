#include "../Database/dbtables.h"
#include "Database/databasemanager.h"

Projects::ProjectTaskPipelineActivity::ProjectTaskPipelineActivity() :
    Ms::Dbo::MDboBase()
{
    _init();
}

Projects::ProjectTaskPipelineActivity::ProjectTaskPipelineActivity(const std::string &name) :
    ProjectTaskPipelineActivity()
{
    _name = name;
}

Projects::ProjectTaskPipelineActivity *Projects::ProjectTaskPipelineActivity::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

const std::string Projects::ProjectTaskPipelineActivity::name() const
{
    return _name;
}

void Projects::ProjectTaskPipelineActivity::setName(const std::string &name)
{
    _name = name;
}

int Projects::ProjectTaskPipelineActivity::progress()
{

}

const Wt::Dbo::ptr<Projects::ProjectProgressShare> Projects::ProjectTaskPipelineActivity::progressShare() const
{
    return _progressShare;
}

void Projects::ProjectTaskPipelineActivity::setProgressShare(Wt::Dbo::ptr<Projects::ProjectProgressShare> progressShare)
{
    _progressShare = progressShare;
}

const Wt::Dbo::ptr<Projects::ProjectTaskPipeline> Projects::ProjectTaskPipelineActivity::pipeline() const
{
    return _pipeline;
}

void Projects::ProjectTaskPipelineActivity::setPipeline(Wt::Dbo::ptr<Projects::ProjectTaskPipeline> pipeline)
{
    _pipeline = pipeline;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectTaskPipelineActivity::status() const
{
    return _status;
}

void Projects::ProjectTaskPipelineActivity::setStatus(const Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    _status = status;
}

bool Projects::ProjectTaskPipelineActivity::operator ==(const Projects::ProjectTaskPipelineActivity &other) const
{
    return _name == other.name();
}

bool Projects::ProjectTaskPipelineActivity::operator !=(const ProjectTaskPipelineActivity &other) const
{
    return !operator==(other);
}

void Projects::ProjectTaskPipelineActivity::_init()
{
    _name = "New Activity";
    _percentage = 0;
}
