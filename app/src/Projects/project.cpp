#include "../Database/dbtables.h"
#include "projectsmanager.h"

Projects::Project::Project() :
    Ms::Dbo::MDbo()
{
    _init();
}

Projects::Project::Project(const std::string &projectName) :
    Project()
{
    _name = projectName;
}

Projects::Project *Projects::Project::modify()
{
    Ms::Dbo::MDbo::modify();

    return this;
}

std::string Projects::Project::name() const
{
    return _name;
}

void Projects::Project::setName(const std::string &name)
{
    _name = name;
}

Wt::WDate Projects::Project::startDate() const
{
    return _startDate;
}

void Projects::Project::setStartDate(const Wt::WDate &startDate)
{
    _startDate = startDate;
}

Wt::WDate Projects::Project::endDate() const
{
    return _endDate;
}

void Projects::Project::setEndDate(const Wt::WDate &endDate)
{
    _endDate = endDate;
}

int Projects::Project::durationInFrames() const
{
    return _durationInFrames;
}

void Projects::Project::setDurationInFrames(int durationInFrames)
{
    _durationInFrames = durationInFrames;
}

float Projects::Project::fps() const
{
    return _fps;
}

void Projects::Project::setFps(float fps)
{
    _fps = fps;
}

int Projects::Project::frameWidth() const
{
    return _frameWidth;
}

void Projects::Project::setFrameWidth(int frameWidth)
{
    _frameWidth = frameWidth;
}

int Projects::Project::frameHeight() const
{
    return _frameHeight;
}

void Projects::Project::setFrameHeight(int frameHeight)
{
    _frameHeight = frameHeight;
}

std::string Projects::Project::description() const
{
    return _description;
}

void Projects::Project::setDescription(const std::string &description)
{
    _description = description;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::Project::status() const
{
    return _status;
}

void Projects::Project::setStatus(const Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    _status = status;
}

Wt::Dbo::ptr<Users::User> Projects::Project::manager() const
{
    return _projectManager;
}

void Projects::Project::setManager(const Wt::Dbo::ptr<Users::User> user)
{
    _projectManager = user;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>>::size_type Projects::Project::numSequences() const
{
    return _sequences.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>>::size_type Projects::Project::numAssets() const
{
    return _assets.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>>::size_type Projects::Project::numData() const
{
    return _data.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>>::size_type Projects::Project::numNotes() const
{
    return _notes.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>>::size_type Projects::Project::numTags() const
{
    return _tags.size();
}

bool Projects::Project::operator ==(const Projects::Project &other) const
{
    return _name == other.name();
}

bool Projects::Project::operator !=(const Projects::Project &other) const
{
    return !operator==(other);
}

void Projects::Project::_init()
{
    thumbnail_ = "pics/NoPreviewBig.png";
    _progress = 0;
}
