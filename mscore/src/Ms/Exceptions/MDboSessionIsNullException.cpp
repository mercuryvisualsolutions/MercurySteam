#include "MDboSessionIsNullException.h"

Ms::Exceptions::MDboSessionIsNullException::MDboSessionIsNullException() throw() :
    std::exception()
{
}

Ms::Exceptions::MDboSessionIsNullException::MDboSessionIsNullException(const std::string &message) throw() :
    MDboSessionIsNullException()
{
    _message = message;
}

Ms::Exceptions::MDboSessionIsNullException::~MDboSessionIsNullException()
{
}

const char *Ms::Exceptions::MDboSessionIsNullException::what() throw()
{
    return _message.c_str();
}
