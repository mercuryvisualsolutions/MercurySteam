#include "../Database/dbtables.h"
#include "Database/databasemanager.h"

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
    if(Database::DatabaseManager::instance().openTransaction())
        return _id.sequence->name();

    return "";
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
    if(Database::DatabaseManager::instance().openTransaction())
        return _id.sequence->projectName();

    return "";
}

Wt::Dbo::ptr<Projects::Project> Projects::ProjectShot::project() const
{
    return _id.sequence->project();
}

bool Projects::ProjectShot::hasTask(Wt::Dbo::ptr<Projects::ProjectTask> task) const
{
    if(dboManager_ && dboManager_->openTransaction())
    {
        for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
        {
            if((*iter).id() == task.id())
            {
                return true;
            }
        }
    }

    return false;
}

bool Projects::ProjectShot::addTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!hasTask(task))
    {
        _tasks.insert(task);
        return true;
    }

    return false;
}

bool Projects::ProjectShot::removeTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(hasTask(task))
    {
        _tasks.erase(task);
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
    dboManager_ = &Database::DatabaseManager::instance();

    thumbnail_ = "pics/NoPreviewBig.png";
    _id.name = "New Project Shot";
    _durationInFrames = 0;
    _fps = 24.0f;
    _frameWidth = 1920;
    _frameHeight = 1080;
}
