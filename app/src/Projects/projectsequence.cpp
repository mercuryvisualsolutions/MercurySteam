#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectSequence::ProjectSequence() :
    ProjectDbo()
{
    init();
}

Projects::ProjectSequence::ProjectSequence(const std::string &sequenceName) :
    ProjectSequence()
{
    m_id.name = sequenceName;
}

Projects::ProjectSequence *Projects::ProjectSequence::modify()
{
    ProjectDbo::modify();

    return this;
}

const Projects::ProjectSequenceId Projects::ProjectSequence::id() const
{
    return m_id;
}

std::string Projects::ProjectSequence::name() const
{
    return m_id.name;
}

void Projects::ProjectSequence::setName(const std::string &name)
{
    m_id.name = name;
}

std::string Projects::ProjectSequence::projectName() const
{
    std::string name = m_id.project->name();

    return name;
}

Wt::Dbo::ptr<Projects::Project> Projects::ProjectSequence::project() const
{
    return m_id.project;
}

void Projects::ProjectSequence::setProject(Wt::Dbo::ptr<Projects::Project> project)
{
    m_id.project = project;
}

bool Projects::ProjectSequence::hasShot(Wt::Dbo::ptr<Projects::ProjectShot> shot) const
{
    bool result = false;

    for(auto iter = m_shots.begin(); iter != m_shots.end(); ++iter)
    {
        if((*iter).id() == shot.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Projects::ProjectSequence::addShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    if(!hasShot(shot))
    {
        m_shots.insert(shot);

        return true;
    }

    return false;
}

bool Projects::ProjectSequence::removeShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    if(hasShot(shot))
    {
        m_shots.erase(shot);

        return true;
    }

    return false;
}

int Projects::ProjectSequence::durationInFrames() const
{
    return m_durationInFrames;
}

void Projects::ProjectSequence::setDurationInFrames(int durationInFrames)
{
    m_durationInFrames = durationInFrames;
}

float Projects::ProjectSequence::fps() const
{
    return m_fps;
}

void Projects::ProjectSequence::setFps(float fps)
{
    m_fps = fps;
}

int Projects::ProjectSequence::frameWidth() const
{
    return m_frameWidth;
}

void Projects::ProjectSequence::setFrameWidth(int frameWidth)
{
    m_frameWidth = frameWidth;
}

int Projects::ProjectSequence::frameHeight() const
{
    return m_frameHeight;
}

void Projects::ProjectSequence::setFrameHeight(int frameHeight)
{
    m_frameHeight = frameHeight;
}

int Projects::ProjectSequence::progress() const
{
    int dHours = doneHours();
    int tHours = totalHours();

    return tHours > 0 ? static_cast<float>(static_cast<float>(dHours) / static_cast<float>(tHours)) * 100.0f : 0;
}

int Projects::ProjectSequence::totalHours() const
{
    int totalHours = 0;

    for(auto iter = m_shots.begin(); iter != m_shots.end(); ++iter)//count shots hours
    {
        if(!(*iter)->active())
            continue;

        totalHours += (*iter)->totalHours();
    }

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)//count tasks hours
    {
        if(!(*iter)->active())
            continue;

        totalHours += (*iter)->totalHours();
    }

    return totalHours;
}

int Projects::ProjectSequence::doneHours() const
{
    int finishedHours = 0;

    for(auto iter = m_shots.begin(); iter != m_shots.end(); ++iter)//shots tasks hours
    {
        if(!(*iter)->active())
            continue;

        finishedHours += (*iter)->doneHours();
    }

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)//sequences tasks hours
    {
        if(!(*iter)->active())
            continue;

        finishedHours += (*iter)->doneHours();
    }

    return finishedHours;
}

int Projects::ProjectSequence::totalShots() const
{
    int totalShots = 0;

    for(auto iter = m_shots.begin(); iter != m_shots.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalShots++;
    }

    return totalShots;
}

int Projects::ProjectSequence::doneShots() const
{
    int doneShots = 0;

    for(auto iter = m_shots.begin(); iter != m_shots.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")
        {
            doneShots++;
        }
    }

    return doneShots;
}

int Projects::ProjectSequence::totalTasks() const
{
    int totalTasks = 0;

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalTasks++;
    }

    for(auto iter = m_shots.begin(); iter != m_shots.end(); ++iter)//count shots tasks
    {
        if(!(*iter)->active())
            continue;

        totalTasks += (*iter)->totalTasks();
    }

    return totalTasks;
}

int Projects::ProjectSequence::doneTasks() const
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

    for(auto iter = m_shots.begin(); iter != m_shots.end(); ++iter)//shots tasks
    {
        if(!(*iter)->active())
            continue;

        doneTasks += (*iter)->doneTasks();
    }

    return doneTasks;
}

int Projects::ProjectSequence::totalActivities() const
{
    int totalActivities = 0;

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalActivities+= (*iter)->totalActivities();
    }

    for(auto iter = m_shots.begin(); iter != m_shots.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalActivities += (*iter)->totalActivities();
    }

    return totalActivities;
}

int Projects::ProjectSequence::doneActivities() const
{
    int doneActivities = 0;

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        doneActivities+= (*iter)->doneActivities();
    }

    for(auto iter = m_shots.begin(); iter != m_shots.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        doneActivities += (*iter)->doneActivities();
    }

    return doneActivities;
}

bool Projects::ProjectSequence::operator ==(const Projects::ProjectSequence &other) const
{
    return m_id.name == other.name() &&
        m_id.project == other.project();
}

bool Projects::ProjectSequence::operator !=(const Projects::ProjectSequence &other) const
{
    return !operator==(other);
}

void Projects::ProjectSequence::init()
{
    m_thumbnail = "pics/NoPreviewBig.png";
    m_id.name = "New Project Sequence";
    m_durationInFrames = 0;
    m_fps = 24.0f;
    m_frameWidth = 1920;
    m_frameHeight = 1080;
}
