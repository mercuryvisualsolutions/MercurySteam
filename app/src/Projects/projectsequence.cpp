#include "../Database/dbtables.h"
#include "projectsmanager.h"
#include "Database/databasemanager.h"

Projects::ProjectSequence::ProjectSequence() :
    Database::Dbo()
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
    Database::Dbo::modify();

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

Wt::WDate Projects::ProjectSequence::startDate() const
{
    return _startDate;
}

void Projects::ProjectSequence::setStartDate(const Wt::WDate &startDate)
{
    _startDate = startDate;
}

Wt::WDate Projects::ProjectSequence::endDate() const
{
    return _endDate;
}

void Projects::ProjectSequence::setEndDate(const Wt::WDate &endDate)
{
    _endDate = endDate;
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

std::string Projects::ProjectSequence::description() const
{
    return _description;
}
void Projects::ProjectSequence::setDescription(const std::string &description)
{
    _description = description;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectSequence::status() const
{
    return _status;
}

void Projects::ProjectSequence::setStatus(const Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    _status = status;
}

int Projects::ProjectSequence::priority() const
{
    return _priority;
}

void Projects::ProjectSequence::setPriority(int priority)
{
    _priority = priority;
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
    _startDate = Wt::WDate::currentDate();
    _endDate = Wt::WDate::currentDate();
    _durationInFrames = 1000;
    _fps = 24.0f;
    _frameWidth = 1920;
    _frameHeight = 1080;
    _description = "";
    _priority = 0;
}
