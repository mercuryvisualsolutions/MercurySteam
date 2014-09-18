#include "../Database/dbtables.h"
#include "../Database/databasemanager.h"

Projects::ProjectTaskPipelineActivityItem::ProjectTaskPipelineActivityItem() :
    Ms::Dbo::MDboBase()
{
}

Projects::ProjectTaskPipelineActivityItem::~ProjectTaskPipelineActivityItem()
{
}

Projects::ProjectTaskPipelineActivityItem *Projects::ProjectTaskPipelineActivityItem::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

const Wt::Dbo::ptr<Projects::ProjectTaskActivityType> Projects::ProjectTaskPipelineActivityItem::type() const
{
    return _type;
}

void Projects::ProjectTaskPipelineActivityItem::setType(Wt::Dbo::ptr<ProjectTaskActivityType> type)
{
    _type = type;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectTaskPipelineActivityItem::status() const
{
    return _status;
}

void Projects::ProjectTaskPipelineActivityItem::setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    _status = status;
}
