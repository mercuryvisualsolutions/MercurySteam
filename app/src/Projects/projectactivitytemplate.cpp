#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectActivityTemplate::ProjectActivityTemplate() :
    Ms::Dbo::MDboBase()
{
    init();
}

Projects::ProjectActivityTemplate::ProjectActivityTemplate(const std::string &name) :
    ProjectActivityTemplate()
{
    m_name = name;
}

Projects::ProjectActivityTemplate *Projects::ProjectActivityTemplate::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

const std::string Projects::ProjectActivityTemplate::name() const
{
    return m_name;
}

void Projects::ProjectActivityTemplate::setName(const std::string &name)
{
    m_name = name;
}

bool Projects::ProjectActivityTemplate::hasItem(Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem> taskItem) const
{
    bool result = false;

    for(auto iter = m_items.begin(); iter != m_items.end(); ++iter)
    {
        if((*iter).id() == taskItem.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Projects::ProjectActivityTemplate::addItem(Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem> taskItem)
{
    if(!hasItem(taskItem))
    {
        m_items.insert(taskItem);

        return true;
    }

    return false;
}

bool Projects::ProjectActivityTemplate::removeItem(Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem> taskItem)
{
    if(hasItem(taskItem))
    {
        m_items.erase(taskItem);

        return true;
    }

    return false;
}

bool Projects::ProjectActivityTemplate::createActivitiesForProjectTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    std::vector<Projects::ProjectTaskActivity*> activitiesVec;

    for(auto iter = m_items.begin(); iter != m_items.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        Projects::ProjectTaskActivity *activity = new Projects::ProjectTaskActivity();
        activity->setTask(task);
        activity->setStatus((*iter)->status());
        activity->setType((*iter)->type());
        activity->setHours((*iter)->hours());
        activity->setDescription((*iter)->description());

        activitiesVec.push_back(activity);
    }

    for(auto activity : activitiesVec)
    {
        if(!Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTaskActivity>(activity))
            delete activity;
    }

    return true;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem> > Projects::ProjectActivityTemplate::items() const
{
    return m_items;
}

bool Projects::ProjectActivityTemplate::operator ==(const Projects::ProjectActivityTemplate &other) const
{
    return m_name == other.name();
}

bool Projects::ProjectActivityTemplate::operator !=(const ProjectActivityTemplate &other) const
{
    return !operator ==(other);
}

void Projects::ProjectActivityTemplate::init()
{
    m_name = "New Template";
}
