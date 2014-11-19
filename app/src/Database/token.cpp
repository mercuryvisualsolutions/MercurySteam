#include "dbtables.h"

Database::Token::Token()
{
}

Database::Token::Token(const std::string &value, const Wt::WDateTime &expires) :
    _value(value), _expires(expires)
{
}

Database::Token *Database::Token::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

std::string Database::Token::value() const
{
    return _value;
}

void Database::Token::setValue(const std::string &value)
{
    _value = value;
}

Wt::WDateTime Database::Token::expires() const
{
    return _expires;
}

void Database::Token::setExpires(const Wt::WDateTime &expires)
{
    _expires = expires;
}

const Wt::Dbo::ptr<Users::User> Database::Token::user() const
{
    return _user;
}
