#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectShot::ProjectShot() :
    ProjectDbo()
{
    _init();
}

Projects::ProjectShot::ProjectShot(const std::string &shotName) :
    ProjectShot()
{
    _id.name = shotName;
}

Projects::ProjectShot *Projects::ProjectShot::modify()
{
    ProjectDbo::modify();

    return this;
}

const Projects::ProjectShotId Projects::ProjectShot::id() const
{
    return _id;
}

std::string Projects::ProjectShot::name() const
{
    return _id.name;
}

void Projects::ProjectShot::setName(const std::string &name)
{
    _id.name = name;
}

std::string Projects::ProjectShot::sequenceName() const
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string name = _id.sequence->name();

    transaction.commit();

    return name;
}

Wt::Dbo::ptr<Projects::ProjectSequence> Projects::ProjectShot::sequence() const
{
    return _id.sequence;
}

void Projects::ProjectShot::setSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    _id.sequence = sequence;
}

std::string Projects::ProjectShot::projectName() const
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string name = _id.sequence->projectName();

    transaction.commit();

    return name;
}

Wt::Dbo::ptr<Projects::Project> Projects::ProjectShot::project() const
{
    return _id.sequence->project();
}

bool Projects::ProjectShot::hasTask(Wt::Dbo::ptr<Projects::ProjectTask> task) const
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

bool Projects::ProjectShot::addTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
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

bool Projects::ProjectShot::removeTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
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

int Projects::ProjectShot::durationInFrames() const
{
    return _durationInFrames;
}

void Projects::ProjectShot::setDurationInFrames(int durationInFrames)
{
    _durationInFrames = durationInFrames;
}

float Projects::ProjectShot::fps() const
{
    return _fps;
}

void Projects::ProjectShot::setFps(float fps)
{
    _fps = fps;
}

int Projects::ProjectShot::frameWidth() const
{
    return _frameWidth;
}

void Projects::ProjectShot::setFrameWidth(int frameWidth)
{
    _frameWidth = frameWidth;
}

int Projects::ProjectShot::frameHeight() const
{
    return _frameHeight;
}

void Projects::ProjectShot::setFrameHeight(int frameHeight)
{
    _frameHeight = frameHeight;
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

int Projects::ProjectShot::doneHours() const
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

int Projects::ProjectShot::totalTasks() const
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

int Projects::ProjectShot::doneTasks() const
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

int Projects::ProjectShot::totalActivities() const
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

int Projects::ProjectShot::doneActivities() const
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

bool Projects::ProjectShot::operator ==(const Projects::ProjectShot &other) const
{
    return _id == other.id();
}

bool Projects::ProjectShot::operator !=(const Projects::ProjectShot &other) const
{
    return !operator==(other);
}

void Projects::ProjectShot::_init()
{
    thumbnail_ = "pics/NoPreviewBig.png";
    _id.name = "New Project Shot";
    _durationInFrames = 0;
    _fps = 24.0f;
    _frameWidth = 1920;
    _frameHeight = 1080;
}
