#include "MDboBase.h"

Ms::Dbo::MDboBase::MDboBase()
{
    _init();
}

Ms::Dbo::MDboBase::MDboBase(bool active, int viewRank, int editRank, int removeRank) :
    MDboBase()
{
    active_ = active;
    viewRank_ = viewRank;
    editRank_ = editRank;
    removeRank_ = removeRank;
}

Ms::Dbo::MDboBase::~MDboBase()
{
}

const std::string Ms::Dbo::MDboBase::thumbnail() const
{
    return thumbnail_;
}

void Ms::Dbo::MDboBase::setThumbnail(const std::string &path)
{
    thumbnail_ = path;
}

const Wt::WDateTime Ms::Dbo::MDboBase::dateCreated() const
{
    return dateCreated_;
}

const Wt::WDateTime Ms::Dbo::MDboBase::lastModifiedDate() const
{
    return lastModifiedDate_;
}

const std::string Ms::Dbo::MDboBase::createdBy() const
{
    return createdBy_;
}

const std::string Ms::Dbo::MDboBase::lastModifiedBy() const
{
    return lastModifiedBy_;
}

int Ms::Dbo::MDboBase::viewRank() const
{
    return viewRank_;
}

void Ms::Dbo::MDboBase::setViewRank(int rank)
{
    viewRank_ = rank;
}

int Ms::Dbo::MDboBase::editRank() const
{
    return editRank_;
}

void Ms::Dbo::MDboBase::setEditRank(int rank)
{
    editRank_ = rank;
}

int Ms::Dbo::MDboBase::removeRank() const
{
    return removeRank_;
}

void Ms::Dbo::MDboBase::setRemoveRank(int rank)
{
    removeRank_ = rank;
}

bool Ms::Dbo::MDboBase::active() const
{
    return active_;
}

void Ms::Dbo::MDboBase::setActive(bool active)
{
    active_ = active;
}

Ms::Dbo::MDboBase *Ms::Dbo::MDboBase::modify()
{
    return this;
}

Ms::Core::Dbo::MDboManagerBase *Ms::Dbo::MDboBase::dboManager() const
{
    return dboManager_;
}

void Ms::Dbo::MDboBase::setDboManager(Core::Dbo::MDboManagerBase *dboManager)
{
    dboManager_ = dboManager;
}

void Ms::Dbo::MDboBase::_init()
{
    thumbnail_ = "pics/NoPreview.png";
    active_ = true;
    viewRank_ = 0;
    editRank_ = 0;
    removeRank_ = 0;
}
