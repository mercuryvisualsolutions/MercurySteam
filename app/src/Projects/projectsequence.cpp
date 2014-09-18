#include "../Database/dbtables.h"
#include "projectsmanager.h"
#include "Database/databasemanager.h"

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
    if(Database::DatabaseManager::instance().openTransaction())
        return _id.project->name();

    return "";
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
    if(dboManager_ && dboManager_->openTransaction())
    {
        for(auto iter = _shots.begin(); iter != _shots.end(); ++iter)
        {
            if((*iter).id() == shot.id())
            {
                return true;
            }
        }
    }

    return false;
}

bool Projects::ProjectSequence::addShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    if(!hasShot(shot))
    {
        _shots.insert(shot);
        return true;
    }

    return false;
}

bool Projects::ProjectSequence::removeShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    if(hasShot(shot))
    {
        _shots.erase(shot);
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
    dboManager_ = &Database::DatabaseManager::instance();

    thumbnail_ = "pics/NoPreviewBig.png";
    _id.name = "New Project Sequence";
    _durationInFrames = 0;
    _fps = 24.0f;
    _frameWidth = 1920;
    _frameHeight = 1080;
}
