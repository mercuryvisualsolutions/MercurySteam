#include "../Database/dbtables.h"
#include "../Database/dbosession.h"

Projects::ProjectTaskTemplateTaskItem::ProjectTaskTemplateTaskItem() :
    Ms::Dbo::MDboBase()
{
    _init();
}

Projects::ProjectTaskTemplateTaskItem::~ProjectTaskTemplateTaskItem()
{
}

Projects::ProjectTaskTemplateTaskItem *Projects::ProjectTaskTemplateTaskItem::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

const Wt::Dbo::ptr<Projects::ProjectTaskType> Projects::ProjectTaskTemplateTaskItem::type() const
{
    return _type;
}

void Projects::ProjectTaskTemplateTaskItem::setType(Wt::Dbo::ptr<ProjectTaskType> type)
{
    _type = type;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectTaskTemplateTaskItem::status() const
{
    return _status;
}

void Projects::ProjectTaskTemplateTaskItem::setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    _status = status;
}

const Wt::Dbo::ptr<Projects::ProjectTaskTemplate> Projects::ProjectTaskTemplateTaskItem::taskTemplate() const
{
    return _taskTemplate;
}

void Projects::ProjectTaskTemplateTaskItem::setTaskTemplate(Wt::Dbo::ptr<Projects::ProjectTaskTemplate> taskTemplate)
{
    _taskTemplate = taskTemplate;
}

std::string Projects::ProjectTaskTemplateTaskItem::description() const
{
    return _description;
}

void Projects::ProjectTaskTemplateTaskItem::setDescription(const std::string &description)
{
    _description = description;
}

void Projects::ProjectTaskTemplateTaskItem::_init()
{
    _description = "";
}
