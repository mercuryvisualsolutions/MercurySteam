#include "MResource.h"

Ms::IO::MResource::MResource()
{
    isInUse_ = true;
}

bool Ms::IO::MResource::isInUse()
{
    return isInUse_;
}
