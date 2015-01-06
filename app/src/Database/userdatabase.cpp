#include "userdatabase.h"

#include "dbtables.h"

#include <Wt/Auth/Identity>

Database::UserDatabase::UserDatabase(Ms::Core::Dbo::MDboSession &session) :
    m_session(session)
{
}

Database::Transaction *Database::UserDatabase::startTransaction()
{
    return new Database::Transaction(m_session);
}

Wt::Dbo::ptr<Users::User> Database::UserDatabase::find(const Wt::Auth::User &user) const
{
    getUser(user.id());

    return m_user;
}

Wt::Auth::User Database::UserDatabase::find(Wt::Dbo::ptr<Users::User> user) const
{
    m_user = user;

    return Wt::Auth::User(boost::lexical_cast<std::string>(user.id()), *this);
}

Wt::Auth::User Database::UserDatabase::findWithId(const std::string &id) const
{
    getUser(id);

    if(m_user)
        return Wt::Auth::User(id, *this);
    else
        return Wt::Auth::User();
}

Wt::Auth::User Database::UserDatabase::findWithIdentity(const std::string &provider, const Wt::WString &identity) const
{
    Wt::Dbo::Transaction transaction(m_session);

    if(provider == Wt::Auth::Identity::LoginName)
    {
        if(!m_user || m_user->name() != identity)
            m_user = m_session.find<Users::User>().where("Name = ?").bind(identity);
    }
    else
        m_user = m_session.find<Users::User>().where("OAuth_Id = ?").bind(identity.toUTF8())
                .where("OAuth_Provider = ?").bind(provider);

    transaction.commit();

    if(m_user)
        return Wt::Auth::User(boost::lexical_cast<std::string>(m_user.id()), *this);
    else
        return Wt::Auth::User();
}

void Database::UserDatabase::addIdentity(const Wt::Auth::User &user, const std::string &provider, const Wt::WString &id)
{
    Wt::Dbo::Transaction transaction(m_session);

    WithUser find(*this, user);

    if(provider == Wt::Auth::Identity::LoginName)
        m_session.modifyDbo<Users::User>(m_user)->setName(id.toUTF8());
    else
    {
        m_session.modifyDbo<Users::User>(m_user)->setOAuthProvider(provider);
        m_session.modifyDbo<Users::User>(m_user)->setOAuthId(id.toUTF8());
    }

    transaction.commit();
}

Wt::WString Database::UserDatabase::identity(const Wt::Auth::User &user, const std::string &provider) const
{
    Wt::Dbo::Transaction transaction(m_session);

    WithUser find(*this, user);

    if(provider == Wt::Auth::Identity::LoginName)
        return m_user->name();
    else if(provider == m_user->oAuthProvider())
        return Wt::WString::fromUTF8(m_user->oAuthId());
    else
        return Wt::WString::Empty;

    transaction.commit();

    return Wt::WString::Empty;
}

void Database::UserDatabase::removeIdentity(const Wt::Auth::User &user, const std::string &provider)
{
    Wt::Dbo::Transaction transaction(m_session);

    WithUser find(*this, user);

    if(provider == Wt::Auth::Identity::LoginName)
        std::cout << "Changing user identity login name is not supported" << std::endl;
    else
    {
        m_session.modifyDbo<Users::User>(m_user)->setOAuthProvider(std::string());
        m_session.modifyDbo<Users::User>(m_user)->setOAuthId(std::string());
    }

    transaction.commit();
}

Wt::Auth::PasswordHash Database::UserDatabase::password(const Wt::Auth::User &user) const
{
    WithUser find(*this, user);

    return Wt::Auth::PasswordHash(m_user->passwordMethod(), m_user->passwordSalt(), m_user->password());
}

void Database::UserDatabase::setPassword(const Wt::Auth::User &user, const Wt::Auth::PasswordHash &password)
{
    Wt::Dbo::Transaction transaction(m_session);

    WithUser find(*this, user);

    m_session.modifyDbo<Users::User>(m_user)->setPassword(password.value());
    m_session.modifyDbo<Users::User>(m_user)->setPasswordMethod(password.function());
    m_session.modifyDbo<Users::User>(m_user)->setPasswordSalt(password.salt());

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
    Wt::Dbo::Transaction transaction(m_session);

    Users::User *user = new Users::User();

    m_user = m_session.createDbo<Users::User>(user);

    m_user.flush();

    transaction.commit();

    return Wt::Auth::User(boost::lexical_cast<std::string>(m_user.id()), *this);
}

void Database::UserDatabase::addAuthToken(const Wt::Auth::User &user, const Wt::Auth::Token &token)
{
    Wt::Dbo::Transaction transaction(m_session);

    WithUser find(*this, user);

    //very unlikely but big a security problem if we don't detect it
    if(m_session.find<Database::Token>().where("Value = ?").bind(token.hash()).resultList().size() > 0)
        throw std::runtime_error("Token hash collision");

    //prevent a user from pilling up the database with tokens
    if(m_user->authTokens().size() > 50)
        return;

    Wt::Dbo::ptr<Database::Token> tokenPtr = m_session.createDbo<Database::Token>(new Database::Token(token.hash(), token.expirationTime()));

    if(tokenPtr)
        m_session.modifyDbo<Users::User>(m_user)->authTokens().insert(tokenPtr);

    transaction.commit();
}

int Database::UserDatabase::updateAuthToken(const Wt::Auth::User &user, const std::string &oldhash, const std::string &newhash)
{
    Wt::Dbo::Transaction transaction(m_session);

    WithUser find(*this, user);

    for(auto iter = m_user->authTokens().begin(); iter != m_user->authTokens().end(); ++iter)
    {
        if((*iter)->value() == oldhash)
        {
            Wt::Dbo::ptr<Database::Token> tokenPtr = (*iter);
            m_session.modifyDbo<Database::Token>(tokenPtr)->setValue(newhash);

            transaction.commit();

            return std::max(Wt::WDateTime::currentDateTime().secsTo(tokenPtr->expires()), 0);
        }
    }

    transaction.commit();

    return 0;
}

void Database::UserDatabase::removeAuthToken(const Wt::Auth::User &user, const std::string &hash)
{
    Wt::Dbo::Transaction transaction(m_session);

    WithUser find(*this, user);

    for(auto iter = m_user->authTokens().begin(); iter != m_user->authTokens().end(); ++iter)
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
    Wt::Dbo::Transaction transaction(m_session);

    m_user = m_session.query<Wt::Dbo::ptr<Users::User>>(
                "SELECT u from \"user\" u join token t on u.Name = t.Token_User_Name").where(
                                                         "t.Value = ?").bind(hash).where(
                                                         "t.Expires > ?").bind(Wt::WDateTime::currentDateTime());

    transaction.commit();

    if(m_user)
        return Wt::Auth::User(boost::lexical_cast<std::string>(m_user.id()), *this);
    else
        return Wt::Auth::User();
}

int Database::UserDatabase::failedLoginAttempts(const Wt::Auth::User &user) const
{
    WithUser find(*this, user);

    return m_user->failedLoginAttempts();
}

void Database::UserDatabase::setFailedLoginAttempts(const Wt::Auth::User &user, int count)
{
    Wt::Dbo::Transaction transaction(m_session);

    WithUser find(*this, user);

    m_session.modifyDbo<Users::User>(m_user)->setFailedLoginAttempts(count);

    transaction.commit();
}

Wt::WDateTime Database::UserDatabase::lastLoginAttempt(const Wt::Auth::User &user) const
{
    WithUser find(*this, user);

    return m_user->lastLoginAttempt();
}

void Database::UserDatabase::setLastLoginAttempt(const Wt::Auth::User &user, const Wt::WDateTime &t)
{
    Wt::Dbo::Transaction transaction(m_session);

    WithUser find(*this, user);

    m_session.modifyDbo<Users::User>(m_user)->setLastLoginAttempt(t);

    transaction.commit();
}

void Database::UserDatabase::getUser(const std::string &id) const
{
    if(!m_user || boost::lexical_cast<std::string>(m_user.id()) != id)
    {
        Wt::Dbo::Transaction transaction(m_session);

        m_user = m_session.find<Users::User>().where("Name = ?").bind(id);

        transaction.commit();
    }
}


Database::UserDatabase::WithUser::WithUser(const Database::UserDatabase &self, const Wt::Auth::User &user) :
    m_transaction(self.m_session)
{
    self.getUser(user.id());

    if(!self.m_user)
        throw InvalidUser(user.id());
}

Database::UserDatabase::WithUser::~WithUser()
{
    m_transaction.commit();
}
