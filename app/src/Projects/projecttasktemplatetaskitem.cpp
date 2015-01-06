#include "../Database/dbtables.h"
#include "../Database/dbosession.h"

Projects::ProjectTaskTemplateTaskItem::ProjectTaskTemplateTaskItem() :
    Ms::Dbo::MDboBase()
{
    init();
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
    return m_type;
}

void Projects::ProjectTaskTemplateTaskItem::setType(Wt::Dbo::ptr<ProjectTaskType> type)
{
    m_type = type;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectTaskTemplateTaskItem::status() const
{
    return m_status;
}

void Projects::ProjectTaskTemplateTaskItem::setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    m_status = status;
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
    return m_description;
}

void Projects::ProjectTaskTemplateTaskItem::setDescription(const std::string &description)
{
    m_description = description;
}

void Projects::ProjectTaskTemplateTaskItem::init()
{
    m_description = "";
}
