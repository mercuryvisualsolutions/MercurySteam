#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectAsset::ProjectAsset() :
    ProjectDbo()
{
    init();
}

Projects::ProjectAsset::ProjectAsset(const std::string &assetName) :
    ProjectAsset()
{
    m_id.name = assetName;
}

Projects::ProjectAsset *Projects::ProjectAsset::modify()
{
    ProjectDbo::modify();

    return this;
}

const Projects::ProjectAssetId Projects::ProjectAsset::id() const
{
    return m_id;
}

const std::string Projects::ProjectAsset::name() const
{
    return m_id.name;
}

void Projects::ProjectAsset::setName(const std::string &name)
{
    m_id.name = name;
}

std::string Projects::ProjectAsset::projectName() const
{
    std::string name = m_id.project->name();

    return name;
}

Wt::Dbo::ptr<Projects::Project> Projects::ProjectAsset::project() const
{
    return m_id.project;
}

void Projects::ProjectAsset::setProject(Wt::Dbo::ptr<Projects::Project> project)
{
    m_id.project = project;
}

bool Projects::ProjectAsset::hasTask(Wt::Dbo::ptr<Projects::ProjectTask> task) const
{
    bool result = false;

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    {
        if((*iter).id() == task.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Projects::ProjectAsset::addTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!hasTask(task))
    {
        m_tasks.insert(task);

        return true;
    }

    return false;
}

bool Projects::ProjectAsset::removeTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(hasTask(task))
    {
        m_tasks.erase(task);

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

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalHours += (*iter)->totalHours();
    }

    return totalHours;
}

int Projects::ProjectAsset::doneHours() const
{
    int finishedHours = 0;

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        finishedHours += (*iter)->doneHours();
    }

    return finishedHours;
}

int Projects::ProjectAsset::totalTasks() const
{
    int totalTasks = 0;

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalTasks++;
    }

    return totalTasks;
}

int Projects::ProjectAsset::doneTasks() const
{
    int doneTasks = 0;

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")
        {
            doneTasks++;
        }
    }

    return doneTasks;
}

int Projects::ProjectAsset::totalActivities() const
{
    int totalActivities = 0;

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalActivities+= (*iter)->totalActivities();
    }

    return totalActivities;
}

int Projects::ProjectAsset::doneActivities() const
{
    int doneActivities = 0;

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        doneActivities+= (*iter)->doneActivities();
    }

    return doneActivities;
}

Wt::Dbo::ptr<Projects::ProjectAssetType> Projects::ProjectAsset::type() const
{
    return m_type;
}

void Projects::ProjectAsset::setType(const Wt::Dbo::ptr<Projects::ProjectAssetType> type)
{
    m_type = type;
}

bool Projects::ProjectAsset::operator ==(const Projects::ProjectAsset &other) const
{
    return m_id == other.id();
}

bool Projects::ProjectAsset::operator !=(const Projects::ProjectAsset &other) const
{
    return !operator==(other);
}

void Projects::ProjectAsset::init()
{
    m_thumbnail = "pics/NoPreviewBig.png";
    m_id.name = "New Project Asset";
}
