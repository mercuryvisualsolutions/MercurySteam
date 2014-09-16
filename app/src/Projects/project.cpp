#include "../Database/dbtables.h"
#include "projectsmanager.h"

Projects::Project::Project() :
    ProjectDbo()
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
    ProjectDbo::modify();

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

bool Projects::Project::hasSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence) const
{
    if(dboManager_ && dboManager_->openTransaction())
    {
        for(auto iter = _sequences.begin(); iter != _sequences.end(); ++iter)
        {
            if((*iter).id() == sequence.id())
            {
                return true;
            }
        }
    }

    return false;
}

bool Projects::Project::addSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    if(!hasSequence(sequence))
    {
        _sequences.insert(sequence);
        return true;
    }

    return false;
}

bool Projects::Project::removeSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    if(hasSequence(sequence))
    {
        _sequences.erase(sequence);
        return true;
    }

    return false;
}

bool Projects::Project::hasAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset) const
{
    if(dboManager_ && dboManager_->openTransaction())
    {
        for(auto iter = _assets.begin(); iter != _assets.end(); ++iter)
        {
            if((*iter).id() == asset.id())
            {
                return true;
            }
        }
    }

    return false;
}

bool Projects::Project::addAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    if(!hasAsset(asset))
    {
        _assets.insert(asset);
        return true;
    }

    return false;
}

bool Projects::Project::removeAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    if(hasAsset(asset))
    {
        _assets.erase(asset);
        return true;
    }

    return false;
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

int Projects::Project::progress() const
{

}

Wt::Dbo::ptr<Users::User> Projects::Project::manager() const
{
    return _projectManager;
}

void Projects::Project::setManager(const Wt::Dbo::ptr<Users::User> user)
{
    _projectManager = user;
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
    dboManager_ = &Database::DatabaseManager::instance();

    thumbnail_ = "pics/NoPreviewBig.png";
    _name = "New Project";
    _durationInFrames = 0;
    _fps = 24.0f;
    _frameWidth = 1920;
    _frameHeight = 1080;
}
