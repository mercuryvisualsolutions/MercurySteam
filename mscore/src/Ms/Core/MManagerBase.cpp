#include "MManagerBase.h"

Ms::Core::MManagerBase::MManagerBase()
{
    _init();
}

Ms::Core::MManagerBase::~MManagerBase()
{
}

std::string Ms::Core::MManagerBase::userName() const
{
    return userName_;
}

void Ms::Core::MManagerBase::setUserName(const std::string &userName) const
{
    userName_ = userName;
}

void Ms::Core::MManagerBase::_init()
{
    userName_ = "SYSTEM";
}
