#include "MNullPointerException.h"

Ms::Exceptions::MNullPointerException::MNullPointerException() throw() :
    std::exception()
{
}

Ms::Exceptions::MNullPointerException::MNullPointerException(const std::string &message) throw() :
    MNullPointerException()
{
    m_message = message;
}

Ms::Exceptions::MNullPointerException::~MNullPointerException()
{
}

const char *Ms::Exceptions::MNullPointerException::what() throw()
{
    return m_message.c_str();
}
