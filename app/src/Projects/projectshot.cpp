#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectShot::ProjectShot() :
    ProjectDbo()
{
    init();
}

Projects::ProjectShot::ProjectShot(const std::string &shotName) :
    ProjectShot()
{
    m_id.name = shotName;
}

Projects::ProjectShot *Projects::ProjectShot::modify()
{
    ProjectDbo::modify();

    return this;
}

const Projects::ProjectShotId Projects::ProjectShot::id() const
{
    return m_id;
}

std::string Projects::ProjectShot::name() const
{
    return m_id.name;
}

void Projects::ProjectShot::setName(const std::string &name)
{
    m_id.name = name;
}

std::string Projects::ProjectShot::sequenceName() const
{
    std::string name = m_id.sequence->name();

    return name;
}

Wt::Dbo::ptr<Projects::ProjectSequence> Projects::ProjectShot::sequence() const
{
    return m_id.sequence;
}

void Projects::ProjectShot::setSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    m_id.sequence = sequence;
}

std::string Projects::ProjectShot::projectName() const
{
    std::string name = m_id.sequence->projectName();

    return name;
}

Wt::Dbo::ptr<Projects::Project> Projects::ProjectShot::project() const
{
    return m_id.sequence->project();
}

bool Projects::ProjectShot::hasTask(Wt::Dbo::ptr<Projects::ProjectTask> task) const
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

bool Projects::ProjectShot::addTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!hasTask(task))
    {
        m_tasks.insert(task);

        return true;
    }

    return false;
}

bool Projects::ProjectShot::removeTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(hasTask(task))
    {
        m_tasks.erase(task);

        return true;
    }

    return false;
}

int Projects::ProjectShot::durationInFrames() const
{
    return m_durationInFrames;
}

void Projects::ProjectShot::setDurationInFrames(int durationInFrames)
{
    m_durationInFrames = durationInFrames;
}

float Projects::ProjectShot::fps() const
{
    return m_fps;
}

void Projects::ProjectShot::setFps(float fps)
{
    m_fps = fps;
}

int Projects::ProjectShot::frameWidth() const
{
    return m_frameWidth;
}

void Projects::ProjectShot::setFrameWidth(int frameWidth)
{
    m_frameWidth = frameWidth;
}

int Projects::ProjectShot::frameHeight() const
{
    return m_frameHeight;
}

void Projects::ProjectShot::setFrameHeight(int frameHeight)
{
    m_frameHeight = frameHeight;
}

int Projects::ProjectShot::progress() const
{
    int dHours = doneHours();
    int tHours = totalHours();

    return tHours > 0 ? static_cast<float>(static_cast<float>(dHours) / static_cast<float>(tHours)) * 100.0f : 0;
}

int Projects::ProjectShot::totalHours() const
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

int Projects::ProjectShot::doneHours() const
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

int Projects::ProjectShot::totalTasks() const
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

int Projects::ProjectShot::doneTasks() const
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

int Projects::ProjectShot::totalActivities() const
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

int Projects::ProjectShot::doneActivities() const
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

bool Projects::ProjectShot::operator ==(const Projects::ProjectShot &other) const
{
    return m_id == other.id();
}

bool Projects::ProjectShot::operator !=(const Projects::ProjectShot &other) const
{
    return !operator==(other);
}

void Projects::ProjectShot::init()
{
    m_thumbnail = "pics/NoPreviewBig.png";
    m_id.name = "New Project Shot";
    m_durationInFrames = 0;
    m_fps = 24.0f;
    m_frameWidth = 1920;
    m_frameHeight = 1080;
}
