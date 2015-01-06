#include "MDboBase.h"

Ms::Dbo::MDboBase::MDboBase()
{
    init();
}

Ms::Dbo::MDboBase::MDboBase(bool active, int viewRank, int editRank, int removeRank) :
    MDboBase()
{
    m_active = active;
    m_viewRank = viewRank;
    m_editRank = editRank;
    m_removeRank = removeRank;
}

Ms::Dbo::MDboBase::~MDboBase()
{
}

const std::string Ms::Dbo::MDboBase::thumbnail() const
{
    return m_thumbnail;
}

void Ms::Dbo::MDboBase::setThumbnail(const std::string &path)
{
    m_thumbnail = path;
}

const Wt::WDateTime Ms::Dbo::MDboBase::dateCreated() const
{
    return m_dateCreated;
}

const Wt::WDateTime Ms::Dbo::MDboBase::lastModifiedDate() const
{
    return m_lastModifiedDate;
}

const std::string Ms::Dbo::MDboBase::createdBy() const
{
    return m_createdBy;
}

const std::string Ms::Dbo::MDboBase::lastModifiedBy() const
{
    return m_lastModifiedBy;
}

int Ms::Dbo::MDboBase::viewRank() const
{
    return m_viewRank;
}

void Ms::Dbo::MDboBase::setViewRank(int rank)
{
    m_viewRank = rank;
}

int Ms::Dbo::MDboBase::editRank() const
{
    return m_editRank;
}

void Ms::Dbo::MDboBase::setEditRank(int rank)
{
    m_editRank = rank;
}

int Ms::Dbo::MDboBase::removeRank() const
{
    return m_removeRank;
}

void Ms::Dbo::MDboBase::setRemoveRank(int rank)
{
    m_removeRank = rank;
}

bool Ms::Dbo::MDboBase::active() const
{
    return m_active;
}

void Ms::Dbo::MDboBase::setActive(bool active)
{
    m_active = active;
}

Ms::Dbo::MDboBase *Ms::Dbo::MDboBase::modify()
{
    return this;
}

const Ms::Core::Dbo::MDboSession *Ms::Dbo::MDboBase::dboSession() const
{
    return m_dboSession;
}

void Ms::Dbo::MDboBase::setDboManager(const Core::Dbo::MDboSession *dboSession)
{
    m_dboSession = dboSession;
}

void Ms::Dbo::MDboBase::init()
{
    m_thumbnail = "pics/NoPreview.png";
    m_active = true;
    m_viewRank = 0;
    m_editRank = 0;
    m_removeRank = 0;
}
