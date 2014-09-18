#include "../Database/dbtables.h"
#include "../Database/databasemanager.h"

Projects::ProjectTaskActivityType::ProjectTaskActivityType() :
    Ms::Dbo::MDboBase()
{
    _init();
}

Projects::ProjectTaskActivityType::ProjectTaskActivityType(const std::string &type) :
    ProjectTaskActivityType()
{
    _type = type;
}

Projects::ProjectTaskActivityType *Projects::ProjectTaskActivityType::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Projects::ProjectTaskActivityType::type() const
{
    return _type;
}

void Projects::ProjectTaskActivityType::setType(const std::string &type)
{
    _type = type;
}

bool Projects::ProjectTaskActivityType::hasActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity) const
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

bool Projects::ProjectTaskActivityType::addActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity)
{
    if(!hasActivity(activity))
    {
        _activities.insert(activity);
        return true;
    }

    return false;
}

bool Projects::ProjectTaskActivityType::removeActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity)
{
    if(hasActivity(activity))
    {
        _activities.erase(activity);
        return true;
    }

    return false;
}

bool Projects::ProjectTaskActivityType::operator ==(const Projects::ProjectTaskActivityType &other) const
{
    return _type == other.type();
}

bool Projects::ProjectTaskActivityType::operator !=(const Projects::ProjectTaskActivityType &other) const
{
    return !operator ==(other);
}

void Projects::ProjectTaskActivityType::_init()
{
    _type = "";
}
