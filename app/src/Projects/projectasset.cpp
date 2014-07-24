#include "../Database/dbtables.h"
#include "Database/databasemanager.h"

Projects::ProjectAsset::ProjectAsset() :
    Database::Dbo()
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
    Database::Dbo::modify();

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

Wt::WDate Projects::ProjectAsset::startDate() const
{
    return _startDate;
}

void Projects::ProjectAsset::setStartDate(const Wt::WDate &startDate)
{
    _startDate = startDate;
}

Wt::WDate Projects::ProjectAsset::endDate() const
{
    return _startDate;
}

void Projects::ProjectAsset::setEndDate(const Wt::WDate &endDate)
{
    _endDate = endDate;
}

std::string Projects::ProjectAsset::description() const
{
    return _description;
}
void Projects::ProjectAsset::setDescription(const std::string &description)
{
    _description = description;
}

Wt::Dbo::ptr<Projects::ProjectAssetType> Projects::ProjectAsset::type() const
{
    return _type;
}

void Projects::ProjectAsset::setType(const Wt::Dbo::ptr<Projects::ProjectAssetType> type)
{
    _type = type;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectAsset::status() const
{
    return _status;
}

void Projects::ProjectAsset::setStatus(const Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    _status = status;
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
    _startDate = Wt::WDate::currentDate();
    _endDate = Wt::WDate::currentDate();
    _description = "";
}
