#include "MResource.h"

Ms::IO::MResource::MResource()
{
    m_isInUse = true;
}

bool Ms::IO::MResource::isInUse()
{
    return m_isInUse;
}
