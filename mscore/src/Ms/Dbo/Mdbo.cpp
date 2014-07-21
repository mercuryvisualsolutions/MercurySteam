#include "MDbo.h"

Ms::Dbo::MDbo::MDbo()
{
    _init();
}

Ms::Dbo::MDbo::MDbo(bool active, int viewRank, int editRank, int removeRank) :
    MDbo()
{
    active_ = active;
    viewRank_ = viewRank;
    editRank_ = editRank;
    removeRank_ = removeRank;
}

Ms::Dbo::MDbo::~MDbo()
{
}

const std::string Ms::Dbo::MDbo::thumbnail() const
{
    return thumbnail_;
}

void Ms::Dbo::MDbo::setThumbnail(const std::string &path)
{
    thumbnail_ = path;
}

const Wt::WDateTime Ms::Dbo::MDbo::dateCreated() const
{
    return dateCreated_;
}

const Wt::WDateTime Ms::Dbo::MDbo::lastModifiedDate() const
{
    return lastModifiedDate_;
}

const std::string Ms::Dbo::MDbo::createdBy() const
{
    return createdBy_;
}

const std::string Ms::Dbo::MDbo::lastModifiedBy() const
{
    return lastModifiedBy_;
}

int Ms::Dbo::MDbo::viewRank() const
{
    return viewRank_;
}

void Ms::Dbo::MDbo::setViewRank(int rank)
{
    viewRank_ = rank;
}

int Ms::Dbo::MDbo::editRank() const
{
    return editRank_;
}

void Ms::Dbo::MDbo::setEditRank(int rank)
{
    editRank_ = rank;
}

int Ms::Dbo::MDbo::removeRank() const
{
    return removeRank_;
}

void Ms::Dbo::MDbo::setRemoveRank(int rank)
{
    removeRank_ = rank;
}

bool Ms::Dbo::MDbo::active() const
{
    return active_;
}

void Ms::Dbo::MDbo::setActive(bool active)
{
    active_ = active;
}

Ms::Dbo::MDbo *Ms::Dbo::MDbo::modify()
{
    return this;
}

Ms::Core::Dbo::MDboManagerBase *Ms::Dbo::MDbo::dboManager() const
{
    return dboManager_;
}

void Ms::Dbo::MDbo::setDboManager(Core::Dbo::MDboManagerBase *dboManager)
{
    dboManager_ = dboManager;
}

void Ms::Dbo::MDbo::_init()
{
    thumbnail_ = "pics/NoPreview.png";
    active_ = true;
    viewRank_ = 0;
    editRank_ = 0;
    removeRank_ = 0;
}
