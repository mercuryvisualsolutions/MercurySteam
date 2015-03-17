#include "dbtables.h"

Database::Notification::Notification() :
    m_message("")
{
}

Database::Notification::Notification(const std::string &message) :
    m_message(message)
{
}

std::string Database::Notification::message() const
{
    return m_message;
}

void Database::Notification::setMessage(const std::string &message)
{
    m_message = message;
}

Database::Notification *Database::Notification::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}
