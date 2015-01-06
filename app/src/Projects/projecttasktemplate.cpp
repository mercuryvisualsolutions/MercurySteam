#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectTaskTemplate::ProjectTaskTemplate() :
    Ms::Dbo::MDboBase()
{
    init();
}

Projects::ProjectTaskTemplate::ProjectTaskTemplate(const std::string &name) :
    ProjectTaskTemplate()
{
    m_name = name;
}

Projects::ProjectTaskTemplate *Projects::ProjectTaskTemplate::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

const std::string Projects::ProjectTaskTemplate::name() const
{
    return m_name;
}

void Projects::ProjectTaskTemplate::setName(const std::string &name)
{
    m_name = name;
}

bool Projects::ProjectTaskTemplate::hasItem(Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem> taskItem) const
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

bool Projects::ProjectTaskTemplate::addItem(Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem> taskItem)
{
    if(!hasItem(taskItem))
    {
        m_items.insert(taskItem);

        return true;
    }

    return false;
}

bool Projects::ProjectTaskTemplate::removeItem(Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem> taskItem)
{
    if(hasItem(taskItem))
    {
        m_items.erase(taskItem);

        return true;
    }

    return false;
}

bool Projects::ProjectTaskTemplate::createTasksForProjectAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    std::vector<Projects::ProjectTask*> tasksVec;

    for(auto iter = m_items.begin(); iter != m_items.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        Projects::ProjectTask *task = new Projects::ProjectTask();
        task->setAsset(asset);
        task->setStatus((*iter)->status());
        task->setType((*iter)->type());
        task->setDescription((*iter)->description());

        tasksVec.push_back(task);
    }

    for(auto task : tasksVec)
    {
        if(!Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTask>(task))
            delete task;
    }

    return true;
}

bool Projects::ProjectTaskTemplate::createTasksForProjectShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    std::vector<Projects::ProjectTask*> tasksVec;

    for(auto iter = m_items.begin(); iter != m_items.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        Projects::ProjectTask *task = new Projects::ProjectTask();
        task->setShot(shot);
        task->setStatus((*iter)->status());
        task->setType((*iter)->type());
        task->setDescription((*iter)->description());

        tasksVec.push_back(task);
    }

    for(auto task : tasksVec)
    {
        if(!Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTask>(task))
            delete task;
    }

    return true;
}

bool Projects::ProjectTaskTemplate::createTasksForProjectSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    std::vector<Projects::ProjectTask*> tasksVec;

    for(auto iter = m_items.begin(); iter != m_items.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        Projects::ProjectTask *task = new Projects::ProjectTask();
        task->setSequence(sequence);
        task->setStatus((*iter)->status());
        task->setType((*iter)->type());
        task->setDescription((*iter)->description());

        tasksVec.push_back(task);
    }

    for(auto task : tasksVec)
    {
        if(!Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTask>(task))
            delete task;
    }

    return true;
}

bool Projects::ProjectTaskTemplate::createTasksForProject(Wt::Dbo::ptr<Projects::Project> project)
{
    std::vector<Projects::ProjectTask*> tasksVec;

    for(auto iter = m_items.begin(); iter != m_items.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        Projects::ProjectTask *task = new Projects::ProjectTask();
        task->setProject(project);
        task->setStatus((*iter)->status());
        task->setType((*iter)->type());
        task->setDescription((*iter)->description());

        tasksVec.push_back(task);
    }

    for(auto task : tasksVec)
    {
        if(!Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTask>(task))
            delete task;
    }

    return true;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem> > Projects::ProjectTaskTemplate::items() const
{
    return m_items;
}

bool Projects::ProjectTaskTemplate::operator ==(const Projects::ProjectTaskTemplate &other) const
{
    return m_name == other.name();
}

bool Projects::ProjectTaskTemplate::operator !=(const ProjectTaskTemplate &other) const
{
    return !operator ==(other);
}

void Projects::ProjectTaskTemplate::init()
{
    m_name = "New Template";
}
