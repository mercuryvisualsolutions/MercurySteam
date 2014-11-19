#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectAsset::ProjectAsset() :
    ProjectDbo()
{
    _init();
}

Projects::ProjectAsset::ProjectAsset(const std::string &assetName) :
    ProjectAsset()
{
    _id.name = assetName;
}

Projects::ProjectAsset *Projects::ProjectAsset::modify()
{
    ProjectDbo::modify();

    return this;
}

const Projects::ProjectAssetId Projects::ProjectAsset::id() const
{
    return _id;
}

const std::string Projects::ProjectAsset::name() const
{
    return _id.name;
}

void Projects::ProjectAsset::setName(const std::string &name)
{
    _id.name = name;
}

std::string Projects::ProjectAsset::projectName() const
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string name = _id.project->name();

    transaction.commit();

    return name;
}

Wt::Dbo::ptr<Projects::Project> Projects::ProjectAsset::project() const
{
    return _id.project;
}

void Projects::ProjectAsset::setProject(Wt::Dbo::ptr<Projects::Project> project)
{
    _id.project = project;
}

bool Projects::ProjectAsset::hasTask(Wt::Dbo::ptr<Projects::ProjectTask> task) const
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool result = false;

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    {
        if((*iter).id() == task.id())
        {
            result = true;

            break;
        }
    }

    transaction.commit();

    return result;
}

bool Projects::ProjectAsset::addTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!hasTask(task))
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        _tasks.insert(task);

        transaction.commit();

        return true;
    }

    return false;
}

bool Projects::ProjectAsset::removeTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(hasTask(task))
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        _tasks.erase(task);

        transaction.commit();

        return true;
    }

    return false;
}

int Projects::ProjectAsset::progress() const
{
    int dHours = doneHours();
    int tHours = totalHours();

    return tHours > 0 ? static_cast<float>(static_cast<float>(dHours) / static_cast<float>(tHours)) * 100.0f : 0;
}

int Projects::ProjectAsset::totalHours() const
{
    int totalHours = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalHours += (*iter)->totalHours();
    }

    transaction.commit();

    return totalHours;
}

int Projects::ProjectAsset::doneHours() const
{
    int finishedHours = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        finishedHours += (*iter)->doneHours();
    }

    transaction.commit();

    return finishedHours;
}

int Projects::ProjectAsset::totalTasks() const
{
    int totalTasks = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalTasks++;
    }

    transaction.commit();

    return totalTasks;
}

int Projects::ProjectAsset::doneTasks() const
{
    int doneTasks = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")
        {
            doneTasks++;
        }
    }

    transaction.commit();

    return doneTasks;
}

int Projects::ProjectAsset::totalActivities() const
{
    int totalActivities = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalActivities+= (*iter)->totalActivities();
    }

    transaction.commit();

    return totalActivities;
}

int Projects::ProjectAsset::doneActivities() const
{
    int doneActivities = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        doneActivities+= (*iter)->doneActivities();
    }

    transaction.commit();

    return doneActivities;
}

Wt::Dbo::ptr<Projects::ProjectAssetType> Projects::ProjectAsset::type() const
{
    return _type;
}

void Projects::ProjectAsset::setType(const Wt::Dbo::ptr<Projects::ProjectAssetType> type)
{
    _type = type;
}

bool Projects::ProjectAsset::operator ==(const Projects::ProjectAsset &other) const
{
    return _id == other.id();
}

bool Projects::ProjectAsset::operator !=(const Projects::ProjectAsset &other) const
{
    return !operator==(other);
}

void Projects::ProjectAsset::_init()
{
    thumbnail_ = "pics/NoPreviewBig.png";
    _id.name = "New Project Asset";
}
