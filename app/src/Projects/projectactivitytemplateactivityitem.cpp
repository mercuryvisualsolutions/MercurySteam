#include "../Database/dbtables.h"
#include "../Database/dbosession.h"

Projects::ProjectActivityTemplateActivityItem::ProjectActivityTemplateActivityItem() :
    Ms::Dbo::MDboBase()
{
    init();
}

Projects::ProjectActivityTemplateActivityItem::~ProjectActivityTemplateActivityItem()
{
}

Projects::ProjectActivityTemplateActivityItem *Projects::ProjectActivityTemplateActivityItem::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

const Wt::Dbo::ptr<Projects::ProjectTaskActivityType> Projects::ProjectActivityTemplateActivityItem::type() const
{
    return m_type;
}

void Projects::ProjectActivityTemplateActivityItem::setType(Wt::Dbo::ptr<ProjectTaskActivityType> type)
{
    m_type = type;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectActivityTemplateActivityItem::status() const
{
    return m_status;
}

void Projects::ProjectActivityTemplateActivityItem::setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    m_status = status;
}

const Wt::Dbo::ptr<Projects::ProjectActivityTemplate> Projects::ProjectActivityTemplateActivityItem::activityTemplate() const
{
    return m_activityTemplate;
}

void Projects::ProjectActivityTemplateActivityItem::setActivityTemplate(Wt::Dbo::ptr<Projects::ProjectActivityTemplate> activityTemplate)
{
    m_activityTemplate = activityTemplate;
}

std::string Projects::ProjectActivityTemplateActivityItem::description() const
{
    return m_description;
}

void Projects::ProjectActivityTemplateActivityItem::setDescription(const std::string &description)
{
    m_description = description;
}

int Projects::ProjectActivityTemplateActivityItem::hours() const
{
    return m_hours;
}

void Projects::ProjectActivityTemplateActivityItem::setHours(int hours)
{
    m_hours = hours;
}

void Projects::ProjectActivityTemplateActivityItem::init()
{
    m_hours = 0;
    m_description = "";
}
