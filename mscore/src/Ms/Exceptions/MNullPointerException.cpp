#include "MNullPointerException.h"

Ms::Exceptions::MNullPointerException::MNullPointerException() throw() :
    std::exception()
{
}

Ms::Exceptions::MNullPointerException::MNullPointerException(const std::string &message) throw() :
    MNullPointerException()
{
    _message = message;
}

Ms::Exceptions::MNullPointerException::~MNullPointerException()
{
}

const char *Ms::Exceptions::MNullPointerException::what() throw()
{
    return _message.c_str();
}
