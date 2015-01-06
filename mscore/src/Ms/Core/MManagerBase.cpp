#include "MManagerBase.h"

Ms::Core::MManagerBase::MManagerBase()
{
    init();
}

Ms::Core::MManagerBase::~MManagerBase()
{
}

std::string Ms::Core::MManagerBase::userName() const
{
    return m_userName;
}

void Ms::Core::MManagerBase::setUserName(const std::string &userName) const
{
    m_userName = userName;
}

void Ms::Core::MManagerBase::init()
{
    m_userName = "SYSTEM";
}
