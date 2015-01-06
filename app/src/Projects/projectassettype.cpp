#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectAssetType::ProjectAssetType() :
    Ms::Dbo::MDboBase()
{
    init();
}

Projects::ProjectAssetType::ProjectAssetType(const std::string &type) :
    ProjectAssetType()
{
    m_type = type;
}

Projects::ProjectAssetType *Projects::ProjectAssetType::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

bool Projects::ProjectAssetType::operator ==(const Projects::ProjectAssetType &other) const
{
    return m_type == other.type();
}

bool Projects::ProjectAssetType::operator !=(const Projects::ProjectAssetType &other) const
{
    return !operator==(other);
}

std::string Projects::ProjectAssetType::type() const
{
    return m_type;
}

void Projects::ProjectAssetType::setType(const std::string &type)
{
    m_type = type;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>>::size_type Projects::ProjectAssetType::numAssets() const
{
    return m_assets.size();
}

void Projects::ProjectAssetType::init()
{
    m_type = "New Project Asset Type";
}
