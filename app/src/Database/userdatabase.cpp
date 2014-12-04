#include "userdatabase.h"

#include "dbtables.h"

#include <Wt/Auth/Identity>

Database::UserDatabase::UserDatabase(Ms::Core::Dbo::MDboSession &session) :
    _session(session)
{
}

Database::Transaction *Database::UserDatabase::startTransaction()
{
    return new Database::Transaction(_session);
}

Wt::Dbo::ptr<Users::User> Database::UserDatabase::find(const Wt::Auth::User &user) const
{
    getUser(user.id());

    return _user;
}

Wt::Auth::User Database::UserDatabase::find(Wt::Dbo::ptr<Users::User> user) const
{
    _user = user;

    return Wt::Auth::User(boost::lexical_cast<std::string>(user.id()), *this);
}

Wt::Auth::User Database::UserDatabase::findWithId(const std::string &id) const
{
    getUser(id);

    if(_user)
        return Wt::Auth::User(id, *this);
    else
        return Wt::Auth::User();
}

Wt::Auth::User Database::UserDatabase::findWithIdentity(const std::string &provider, const Wt::WString &identity) const
{
    Wt::Dbo::Transaction transaction(_session);

    if(provider == Wt::Auth::Identity::LoginName)
    {
        if(!_user || _user->name() != identity)
            _user = _session.find<Users::User>().where("Name = ?").bind(identity);
    }
    else
        _user = _session.find<Users::User>().where("OAuth_Id = ?").bind(identity.toUTF8())
                .where("OAuth_Provider = ?").bind(provider);

    transaction.commit();

    if(_user)
        return Wt::Auth::User(boost::lexical_cast<std::string>(_user.id()), *this);
    else
        return Wt::Auth::User();
}

void Database::UserDatabase::addIdentity(const Wt::Auth::User &user, const std::string &provider, const Wt::WString &id)
{
    Wt::Dbo::Transaction transaction(_session);

    WithUser find(*this, user);

    if(provider == Wt::Auth::Identity::LoginName)
        _session.modifyDbo<Users::User>(_user)->setName(id.toUTF8());
    else
    {
        _session.modifyDbo<Users::User>(_user)->setOAuthProvider(provider);
        _session.modifyDbo<Users::User>(_user)->setOAuthId(id.toUTF8());
    }

    transaction.commit();
}

Wt::WString Database::UserDatabase::identity(const Wt::Auth::User &user, const std::string &provider) const
{
    Wt::Dbo::Transaction transaction(_session);

    WithUser find(*this, user);

    if(provider == Wt::Auth::Identity::LoginName)
        return _user->name();
    else if(provider == _user->oAuthProvider())
        return Wt::WString::fromUTF8(_user->oAuthId());
    else
        return Wt::WString::Empty;

    transaction.commit();
}

void Database::UserDatabase::removeIdentity(const Wt::Auth::User &user, const std::string &provider)
{
    Wt::Dbo::Transaction transaction(_session);

    WithUser find(*this, user);

    if(provider == Wt::Auth::Identity::LoginName)
        std::cout << "Changing user identity login name is not supported" << std::endl;
    else
    {
        _session.modifyDbo<Users::User>(_user)->setOAuthProvider(std::string());
        _session.modifyDbo<Users::User>(_user)->setOAuthId(std::string());
    }

    transaction.commit();
}

Wt::Auth::PasswordHash Database::UserDatabase::password(const Wt::Auth::User &user) const
{
    WithUser find(*this, user);

    return Wt::Auth::PasswordHash(_user->passwordMethod(), _user->passwordSalt(), _user->password());
}

void Database::UserDatabase::setPassword(const Wt::Auth::User &user, const Wt::Auth::PasswordHash &password)
{
    Wt::Dbo::Transaction transaction(_session);

    WithUser find(*this, user);

    _session.modifyDbo<Users::User>(_user)->setPassword(password.value());
    _session.modifyDbo<Users::User>(_user)->setPasswordMethod(password.function());
    _session.modifyDbo<Users::User>(_user)->setPasswordSalt(password.salt());

    transaction.commit();
}

Wt::Auth::User::Status Database::UserDatabase::status(const Wt::Auth::User &user) const
{
    return Wt::Auth::User::Status::Normal;
}

void Database::UserDatabase::setStatus(const Wt::Auth::User &user, Wt::Auth::User::Status status)
{
    throw std::runtime_error("Changing status is not supported.");
}

Wt::Auth::User Database::UserDatabase::registerNew()
{
    Wt::Dbo::Transaction transaction(_session);

    Users::User *user = new Users::User();

    _user = _session.createDbo<Users::User>(user);

    _user.flush();

    transaction.commit();

    return Wt::Auth::User(boost::lexical_cast<std::string>(_user.id()), *this);
}

void Database::UserDatabase::addAuthToken(const Wt::Auth::User &user, const Wt::Auth::Token &token)
{
    Wt::Dbo::Transaction transaction(_session);

    WithUser find(*this, user);

    //very unlikely but big a security problem if we don't detect it
    if(_session.find<Database::Token>().where("Value = ?").bind(token.hash()).resultList().size() > 0)
        throw std::runtime_error("Token hash collision");

    //prevent a user from pilling up the database with tokens
    if(_user->authTokens().size() > 50)
        return;

    Wt::Dbo::ptr<Database::Token> tokenPtr = _session.createDbo<Database::Token>(new Database::Token(token.hash(), token.expirationTime()));

    if(tokenPtr)
        _session.modifyDbo<Users::User>(_user)->authTokens().insert(tokenPtr);

    transaction.commit();
}

int Database::UserDatabase::updateAuthToken(const Wt::Auth::User &user, const std::string &oldhash, const std::string &newhash)
{
    Wt::Dbo::Transaction transaction(_session);

    WithUser find(*this, user);

    for(auto iter = _user->authTokens().begin(); iter != _user->authTokens().end(); ++iter)
    {
        if((*iter)->value() == oldhash)
        {
            Wt::Dbo::ptr<Database::Token> tokenPtr = (*iter);
            _session.modifyDbo<Database::Token>(tokenPtr)->setValue(newhash);

            transaction.commit();

            return std::max(Wt::WDateTime::currentDateTime().secsTo(tokenPtr->expires()), 0);
        }
    }

    transaction.commit();

    return 0;
}

void Database::UserDatabase::removeAuthToken(const Wt::Auth::User &user, const std::string &hash)
{
    Wt::Dbo::Transaction transaction(_session);

    WithUser find(*this, user);

    for(auto iter = _user->authTokens().begin(); iter != _user->authTokens().end(); ++iter)
    {
        if((*iter)->value() == hash)
        {
            Wt::Dbo::ptr<Database::Token> tokenPtr = (*iter);
            tokenPtr.remove();

            break;
        }
    }

    transaction.commit();
}

Wt::Auth::User Database::UserDatabase::findWithAuthToken(const std::string &hash) const
{
    Wt::Dbo::Transaction transaction(_session);

    _user = _session.query<Wt::Dbo::ptr<Users::User>>(
                "SELECT u from \"user\" u join token t on u.Name = t.Token_User_Name").where(
                                                         "t.Value = ?").bind(hash).where(
                                                         "t.Expires > ?").bind(Wt::WDateTime::currentDateTime());

    transaction.commit();

    if(_user)
        return Wt::Auth::User(boost::lexical_cast<std::string>(_user.id()), *this);
    else
        return Wt::Auth::User();
}

int Database::UserDatabase::failedLoginAttempts(const Wt::Auth::User &user) const
{
    WithUser find(*this, user);

    return _user->failedLoginAttempts();
}

void Database::UserDatabase::setFailedLoginAttempts(const Wt::Auth::User &user, int count)
{
    Wt::Dbo::Transaction transaction(_session);

    WithUser find(*this, user);

    _session.modifyDbo<Users::User>(_user)->setFailedLoginAttempts(count);

    transaction.commit();
}

Wt::WDateTime Database::UserDatabase::lastLoginAttempt(const Wt::Auth::User &user) const
{
    WithUser find(*this, user);

    return _user->lastLoginAttempt();
}

void Database::UserDatabase::setLastLoginAttempt(const Wt::Auth::User &user, const Wt::WDateTime &t)
{
    Wt::Dbo::Transaction transaction(_session);

    WithUser find(*this, user);

    _session.modifyDbo<Users::User>(_user)->setLastLoginAttempt(t);

    transaction.commit();
}

void Database::UserDatabase::getUser(const std::string &id) const
{
    if(!_user || boost::lexical_cast<std::string>(_user.id()) != id)
    {
        Wt::Dbo::Transaction transaction(_session);

        _user = _session.find<Users::User>().where("Name = ?").bind(id);

        transaction.commit();
    }
}


Database::UserDatabase::WithUser::WithUser(const Database::UserDatabase &self, const Wt::Auth::User &user) :
    _transaction(self._session)
{
    self.getUser(user.id());

    if(!self._user)
        throw InvalidUser(user.id());
}

Database::UserDatabase::WithUser::~WithUser()
{
    _transaction.commit();
}
