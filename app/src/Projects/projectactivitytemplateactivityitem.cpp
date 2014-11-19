#include "../Database/dbtables.h"
#include "../Database/dbosession.h"

Projects::ProjectActivityTemplateActivityItem::ProjectActivityTemplateActivityItem() :
    Ms::Dbo::MDboBase()
{
    _init();
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
    return _type;
}

void Projects::ProjectActivityTemplateActivityItem::setType(Wt::Dbo::ptr<ProjectTaskActivityType> type)
{
    _type = type;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectActivityTemplateActivityItem::status() const
{
    return _status;
}

void Projects::ProjectActivityTemplateActivityItem::setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    _status = status;
}

const Wt::Dbo::ptr<Projects::ProjectActivityTemplate> Projects::ProjectActivityTemplateActivityItem::activityTemplate() const
{
    return _activityTemplate;
}

void Projects::ProjectActivityTemplateActivityItem::setActivityTemplate(Wt::Dbo::ptr<Projects::ProjectActivityTemplate> activityTemplate)
{
    _activityTemplate = activityTemplate;
}

std::string Projects::ProjectActivityTemplateActivityItem::description() const
{
    return _description;
}

void Projects::ProjectActivityTemplateActivityItem::setDescription(const std::string &description)
{
    _description = description;
}

int Projects::ProjectActivityTemplateActivityItem::hours() const
{
    return _hours;
}

void Projects::ProjectActivityTemplateActivityItem::setHours(int hours)
{
    _hours = hours;
}

void Projects::ProjectActivityTemplateActivityItem::_init()
{
    _hours = 0;
    _description = "";
}
