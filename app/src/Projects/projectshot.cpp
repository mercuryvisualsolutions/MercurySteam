#include "../Database/dbtables.h"
#include "Database/databasemanager.h"

Projects::ProjectShot::ProjectShot() :
    Ms::Dbo::MDbo()
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
    Ms::Dbo::MDbo::modify();

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

Wt::WDate Projects::ProjectShot::startDate() const
{
    return _startDate;
}

void Projects::ProjectShot::setStartDate(const Wt::WDate &startDate)
{
    _startDate = startDate;
}

Wt::WDate Projects::ProjectShot::endDate() const
{
    return _endDate;
}

void Projects::ProjectShot::setEndDate(const Wt::WDate &endDate)
{
    _endDate = endDate;
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

std::string Projects::ProjectShot::description() const
{
    return _description;
}

void Projects::ProjectShot::setDescription(const std::string &description)
{
    _description = description;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectShot::status() const
{
    return _status;
}

void Projects::ProjectShot::setStatus(const Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    _status = status;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>>::size_type Projects::ProjectShot::numTasks() const
{
    return _tasks.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>>::size_type Projects::ProjectShot::numData() const
{
    return _data.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>>::size_type Projects::ProjectShot::numNotes() const
{
    return _notes.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>>::size_type Projects::ProjectShot::numTags() const
{
    return _tags.size();
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
}
