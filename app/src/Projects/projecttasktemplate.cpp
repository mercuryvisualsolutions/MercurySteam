#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectTaskTemplate::ProjectTaskTemplate() :
    Ms::Dbo::MDboBase()
{
    _init();
}

Projects::ProjectTaskTemplate::ProjectTaskTemplate(const std::string &name) :
    ProjectTaskTemplate()
{
    _name = name;
}

Projects::ProjectTaskTemplate *Projects::ProjectTaskTemplate::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

const std::string Projects::ProjectTaskTemplate::name() const
{
    return _name;
}

void Projects::ProjectTaskTemplate::setName(const std::string &name)
{
    _name = name;
}

bool Projects::ProjectTaskTemplate::hasItem(Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem> taskItem) const
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool result = false;

    for(auto iter = _items.begin(); iter != _items.end(); ++iter)
    {
        if((*iter).id() == taskItem.id())
        {
            result = true;

            break;
        }
    }

    transaction.commit();

    return result;
}

bool Projects::ProjectTaskTemplate::addItem(Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem> taskItem)
{
    if(!hasItem(taskItem))
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        _items.insert(taskItem);

        transaction.commit();

        return true;
    }

    return false;
}

bool Projects::ProjectTaskTemplate::removeItem(Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem> taskItem)
{
    if(hasItem(taskItem))
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        _items.erase(taskItem);

        transaction.commit();

        return true;
    }

    return false;
}

bool Projects::ProjectTaskTemplate::createTasksForProjectAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::vector<Projects::ProjectTask*> tasksVec;

    for(auto iter = _items.begin(); iter != _items.end(); ++iter)
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

    transaction.commit();

    return true;
}

bool Projects::ProjectTaskTemplate::createTasksForProjectShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::vector<Projects::ProjectTask*> tasksVec;

    for(auto iter = _items.begin(); iter != _items.end(); ++iter)
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

    transaction.commit();

    return true;
}

bool Projects::ProjectTaskTemplate::createTasksForProjectSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::vector<Projects::ProjectTask*> tasksVec;

    for(auto iter = _items.begin(); iter != _items.end(); ++iter)
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

    transaction.commit();

    return true;
}

bool Projects::ProjectTaskTemplate::createTasksForProject(Wt::Dbo::ptr<Projects::Project> project)
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::vector<Projects::ProjectTask*> tasksVec;

    for(auto iter = _items.begin(); iter != _items.end(); ++iter)
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

    transaction.commit();

    return true;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem> > Projects::ProjectTaskTemplate::items() const
{
    return _items;
}

bool Projects::ProjectTaskTemplate::operator ==(const Projects::ProjectTaskTemplate &other) const
{
    return _name == other.name();
}

bool Projects::ProjectTaskTemplate::operator !=(const ProjectTaskTemplate &other) const
{
    return !operator ==(other);
}

void Projects::ProjectTaskTemplate::_init()
{
    _name = "New Template";
}
