#include "dbtables.h"

Database::Token::Token()
{
}

Database::Token::Token(const std::string &value, const Wt::WDateTime &expires) :
    m_value(value), m_expires(expires)
{
}

Database::Token *Database::Token::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Database::Token::value() const
{
    return m_value;
}

void Database::Token::setValue(const std::string &value)
{
    m_value = value;
}

Wt::WDateTime Database::Token::expires() const
{
    return m_expires;
}

void Database::Token::setExpires(const Wt::WDateTime &expires)
{
    m_expires = expires;
}

const Wt::Dbo::ptr<Users::User> Database::Token::user() const
{
    return m_user;
}
