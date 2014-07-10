#include "../Database/dbtables.h"

Projects::ProjectAssetType::ProjectAssetType() :
    Ms::Dbo::MDbo()
{
    _init();
}

Projects::ProjectAssetType::ProjectAssetType(const std::string &type) :
    ProjectAssetType()
{
    _type = type;
}

Projects::ProjectAssetType *Projects::ProjectAssetType::modify()
{
    Ms::Dbo::MDbo::modify();

    return this;
}

bool Projects::ProjectAssetType::operator ==(const Projects::ProjectAssetType &other) const
{
    return _type == other.type();
}

bool Projects::ProjectAssetType::operator !=(const Projects::ProjectAssetType &other) const
{
    return !operator==(other);
}

std::string Projects::ProjectAssetType::type() const
{
    return _type;
}

void Projects::ProjectAssetType::setType(const std::string &type)
{
    _type = type;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>>::size_type Projects::ProjectAssetType::numAssets() const
{
    return _assets.size();
}

void Projects::ProjectAssetType::_init()
{
    _type = "New Project Asset Type";
}
