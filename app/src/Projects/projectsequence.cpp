#include "../Database/dbtables.h"
#include "projectsmanager.h"
#include "Database/databasemanager.h"

Projects::ProjectSequence::ProjectSequence() :
    Ms::Dbo::MDbo()
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
    Ms::Dbo::MDbo::modify();

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

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>>::size_type Projects::ProjectSequence::numShots() const
{
    return _shots.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>>::size_type Projects::ProjectSequence::numData() const
{
    return _data.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>>::size_type Projects::ProjectSequence::numNotes() const
{
    return _notes.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>>::size_type Projects::ProjectSequence::numTags() const
{
    return _tags.size();
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
}
