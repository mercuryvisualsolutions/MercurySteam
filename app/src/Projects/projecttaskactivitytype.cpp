#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectTaskActivityType::ProjectTaskActivityType() :
    Ms::Dbo::MDboBase()
{
    init();
}

Projects::ProjectTaskActivityType::ProjectTaskActivityType(const std::string &type) :
    ProjectTaskActivityType()
{
    m_type = type;
}

Projects::ProjectTaskActivityType *Projects::ProjectTaskActivityType::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Projects::ProjectTaskActivityType::type() const
{
    return m_type;
}

void Projects::ProjectTaskActivityType::setType(const std::string &type)
{
    m_type = type;
}

bool Projects::ProjectTaskActivityType::hasActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity) const
{
    bool result = true;

    for(auto iter = m_activities.begin(); iter != m_activities.end(); ++iter)
    {
        if((*iter).id() == activity.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Projects::ProjectTaskActivityType::addActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity)
{
    if(!hasActivity(activity))
    {
        m_activities.insert(activity);

        return true;
    }

    return false;
}

bool Projects::ProjectTaskActivityType::removeActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity)
{
    if(hasActivity(activity))
    {
        m_activities.erase(activity);

        return true;
    }

    return false;
}

bool Projects::ProjectTaskActivityType::operator ==(const Projects::ProjectTaskActivityType &other) const
{
    return m_type == other.type();
}

bool Projects::ProjectTaskActivityType::operator !=(const Projects::ProjectTaskActivityType &other) const
{
    return !operator ==(other);
}

void Projects::ProjectTaskActivityType::init()
{
    m_type = "";
}
