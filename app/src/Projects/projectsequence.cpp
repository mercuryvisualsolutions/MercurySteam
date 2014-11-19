#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectSequence::ProjectSequence() :
    ProjectDbo()
{
    _init();
}

Projects::ProjectSequence::ProjectSequence(const std::string &sequenceName) :
    ProjectSequence()
{
    _id.name = sequenceName;
}

Projects::ProjectSequence *Projects::ProjectSequence::modify()
{
    ProjectDbo::modify();

    return this;
}

const Projects::ProjectSequenceId Projects::ProjectSequence::id() const
{
    return _id;
}

std::string Projects::ProjectSequence::name() const
{
    return _id.name;
}

void Projects::ProjectSequence::setName(const std::string &name)
{
    _id.name = name;
}

std::string Projects::ProjectSequence::projectName() const
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string name = _id.project->name();

    transaction.commit();

    return name;
}

Wt::Dbo::ptr<Projects::Project> Projects::ProjectSequence::project() const
{
    return _id.project;
}

void Projects::ProjectSequence::setProject(Wt::Dbo::ptr<Projects::Project> project)
{
    _id.project = project;
}

bool Projects::ProjectSequence::hasShot(Wt::Dbo::ptr<Projects::ProjectShot> shot) const
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool result = false;

    for(auto iter = _shots.begin(); iter != _shots.end(); ++iter)
    {
        if((*iter).id() == shot.id())
        {
            result = true;

            break;
        }
    }

    transaction.commit();

    return result;
}

bool Projects::ProjectSequence::addShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    if(!hasShot(shot))
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        _shots.insert(shot);

        transaction.commit();

        return true;
    }

    return false;
}

bool Projects::ProjectSequence::removeShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    if(hasShot(shot))
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        _shots.erase(shot);

        transaction.commit();

        return true;
    }

    return false;
}

int Projects::ProjectSequence::durationInFrames() const
{
    return _durationInFrames;
}

void Projects::ProjectSequence::setDurationInFrames(int durationInFrames)
{
    _durationInFrames = durationInFrames;
}

float Projects::ProjectSequence::fps() const
{
    return _fps;
}

void Projects::ProjectSequence::setFps(float fps)
{
    _fps = fps;
}

int Projects::ProjectSequence::frameWidth() const
{
    return _frameWidth;
}

void Projects::ProjectSequence::setFrameWidth(int frameWidth)
{
    _frameWidth = frameWidth;
}

int Projects::ProjectSequence::frameHeight() const
{
    return _frameHeight;
}

void Projects::ProjectSequence::setFrameHeight(int frameHeight)
{
    _frameHeight = frameHeight;
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

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _shots.begin(); iter != _shots.end(); ++iter)//count shots hours
    {
        if(!(*iter)->active())
            continue;

        totalHours += (*iter)->totalHours();
    }

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)//count tasks hours
    {
        if(!(*iter)->active())
            continue;

        totalHours += (*iter)->totalHours();
    }

    transaction.commit();

    return totalHours;
}

int Projects::ProjectSequence::doneHours() const
{
    int finishedHours = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _shots.begin(); iter != _shots.end(); ++iter)//shots tasks hours
    {
        if(!(*iter)->active())
            continue;

        finishedHours += (*iter)->doneHours();
    }

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)//sequences tasks hours
    {
        if(!(*iter)->active())
            continue;

        finishedHours += (*iter)->doneHours();
    }

    transaction.commit();

    return finishedHours;
}

int Projects::ProjectSequence::totalShots() const
{
    int totalShots = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _shots.begin(); iter != _shots.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalShots++;
    }

    transaction.commit();

    return totalShots;
}

int Projects::ProjectSequence::doneShots() const
{
    int doneShots = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _shots.begin(); iter != _shots.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")
        {
            doneShots++;
        }
    }

    transaction.commit();

    return doneShots;
}

int Projects::ProjectSequence::totalTasks() const
{
    int totalTasks = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalTasks++;
    }

    for(auto iter = _shots.begin(); iter != _shots.end(); ++iter)//count shots tasks
    {
        if(!(*iter)->active())
            continue;

        totalTasks += (*iter)->totalTasks();
    }

    transaction.commit();

    return totalTasks;
}

int Projects::ProjectSequence::doneTasks() const
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

    for(auto iter = _shots.begin(); iter != _shots.end(); ++iter)//shots tasks
    {
        if(!(*iter)->active())
            continue;

        doneTasks += (*iter)->doneTasks();
    }

    transaction.commit();

    return doneTasks;
}

int Projects::ProjectSequence::totalActivities() const
{
    int totalActivities = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalActivities+= (*iter)->totalActivities();
    }

    for(auto iter = _shots.begin(); iter != _shots.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalActivities += (*iter)->totalActivities();
    }

    transaction.commit();

    return totalActivities;
}

int Projects::ProjectSequence::doneActivities() const
{
    int doneActivities = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        doneActivities+= (*iter)->doneActivities();
    }

    for(auto iter = _shots.begin(); iter != _shots.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        doneActivities += (*iter)->doneActivities();
    }

    transaction.commit();

    return doneActivities;
}

bool Projects::ProjectSequence::operator ==(const Projects::ProjectSequence &other) const
{
    return _id.name == other.name() &&
        _id.project == other.project();
}

bool Projects::ProjectSequence::operator !=(const Projects::ProjectSequence &other) const
{
    return !operator==(other);
}

void Projects::ProjectSequence::_init()
{
    thumbnail_ = "pics/NoPreviewBig.png";
    _id.name = "New Project Sequence";
    _durationInFrames = 0;
    _fps = 24.0f;
    _frameWidth = 1920;
    _frameHeight = 1080;
}
