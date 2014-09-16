#include "../Database/dbtables.h"
#include "Database/databasemanager.h"

Projects::ProjectAsset::ProjectAsset() :
    ProjectDbo()
{
    _init();
}

Projects::ProjectAsset::ProjectAsset(const std::string &assetName) :
    ProjectAsset()
{
    _id.name = assetName;
}

Projects::ProjectAsset *Projects::ProjectAsset::modify()
{
    ProjectDbo::modify();

    return this;
}

const Projects::ProjectAssetId Projects::ProjectAsset::id() const
{
    return _id;
}

const std::string Projects::ProjectAsset::name() const
{
    return _id.name;
}

void Projects::ProjectAsset::setName(const std::string &name)
{
    _id.name = name;
}

std::string Projects::ProjectAsset::projectName() const
{
    if(Database::DatabaseManager::instance().openTransaction())
        return _id.project->name();

    return "";
}

Wt::Dbo::ptr<Projects::Project> Projects::ProjectAsset::project() const
{
    return _id.project;
}

void Projects::ProjectAsset::setProject(Wt::Dbo::ptr<Projects::Project> project)
{
    _id.project = project;
}

bool Projects::ProjectAsset::hasTask(Wt::Dbo::ptr<Projects::ProjectTask> task) const
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

bool Projects::ProjectAsset::addTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!hasTask(task))
    {
        _tasks.insert(task);
        return true;
    }

    return false;
}

bool Projects::ProjectAsset::removeTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(hasTask(task))
    {
        _tasks.erase(task);
        return true;
    }

    return false;
}

int Projects::ProjectAsset::progress() const
{

}

const Wt::Dbo::ptr<Projects::ProjectProgressShare> Projects::ProjectAsset::progressShare() const
{
    return _progressShare;
}

void Projects::ProjectAsset::setProgressShare(Wt::Dbo::ptr<Projects::ProjectProgressShare> progressShare)
{
    _progressShare = progressShare;
}

Wt::Dbo::ptr<Projects::ProjectAssetType> Projects::ProjectAsset::type() const
{
    return _type;
}

void Projects::ProjectAsset::setType(const Wt::Dbo::ptr<Projects::ProjectAssetType> type)
{
    _type = type;
}

bool Projects::ProjectAsset::operator ==(const Projects::ProjectAsset &other) const
{
    return _id == other.id();
}

bool Projects::ProjectAsset::operator !=(const Projects::ProjectAsset &other) const
{
    return !operator==(other);
}

void Projects::ProjectAsset::_init()
{
    dboManager_ = &Database::DatabaseManager::instance();

    thumbnail_ = "pics/NoPreviewBig.png";
    _id.name = "New Project Asset";
}
