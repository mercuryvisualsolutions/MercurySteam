#include "../Database/dbtables.h"
#include "Database/databasemanager.h"

Projects::ProjectAsset::ProjectAsset() :
    Ms::Dbo::MDbo()
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
    Ms::Dbo::MDbo::modify();

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

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>>::size_type Projects::ProjectAsset::numTasks() const
{
    return _tasks.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>>::size_type Projects::ProjectAsset::numData() const
{
    return _data.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>>::size_type Projects::ProjectAsset::numNotes() const
{
    return _notes.size();
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>>::size_type Projects::ProjectAsset::numTags() const
{
    return _tags.size();
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
    thumbnail_ = "pics/NoPreviewBig.png";
    _id.name = "New Project Asset";
}
