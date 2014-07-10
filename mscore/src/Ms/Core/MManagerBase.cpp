#include "MManagerBase.h"

Ms::Core::MManagerBase::MManagerBase()
{
    userName_ = "SYSTEM";
}

Ms::Core::MManagerBase::~MManagerBase()
{
}

std::string Ms::Core::MManagerBase::userName() const
{
    return userName_;
}

void Ms::Core::MManagerBase::setUserName(const std::string &userName)
{
    userName_ = userName;
}
