#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"
#include "../Log/logmanager.h"
#include "Users/usersio.h"

Users::User::User() :
    Database::Dbo()
{
    _init();
}

Users::User::User(const std::string &name) :
    User()
{
    _name = name;
}

Users::User::User(const std::string &name, const std::string &emailAddress) :
    User(name)
{
    _emailAddress = emailAddress;
}

Users::User *Users::User::modify()
{
    Database::Dbo::modify();

    return this;
}

std::string Users::User::name() const
{
    return _name;
}

void Users::User::setName(const std::string &name)
{
    _name = name;
}

std::string Users::User::password() const
{
    return _password;
}

void Users::User::setPassword(const std::string &password)
{
    _password = password;
}

std::string Users::User::passwordMethod() const
{
    return _passwordMethod;
}

void Users::User::setPasswordMethod(const std::string &passwordMethod)
{
    _passwordMethod = passwordMethod;
}

std::string Users::User::passwordSalt() const
{
    return _passwordSalt;
}

void Users::User::setPasswordSalt(const std::string &passwordSalt)
{
    _passwordSalt = passwordSalt;
}

int Users::User::failedLoginAttempts() const
{
    return _failedLoginAttempts;
}

void Users::User::setFailedLoginAttempts(int failedLoginAttempts)
{
    _failedLoginAttempts = failedLoginAttempts;
}

Wt::WDateTime Users::User::lastLoginAttempt() const
{
    return _lastLoginAttempt;
}

void Users::User::setLastLoginAttempt(const Wt::WDateTime &lastLoginAttempt)
{
    _lastLoginAttempt = lastLoginAttempt;
}

std::string Users::User::oAuthId() const
{
    return _oAuthId;
}

void Users::User::setOAuthId(const std::string &oAuthId)
{
    _oAuthId = oAuthId;
}

std::string Users::User::oAuthProvider() const
{
    return _oAuthProvider;
}

void Users::User::setOAuthProvider(const std::string &oAuthProvider)
{
    _oAuthProvider = oAuthProvider;
}

bool Users::User::hasPrivilege(Wt::Dbo::ptr<Users::Privilege> privilege) const
{
    return _group->hasPrivilege(privilege);
}

bool Users::User::hasPrivilege(const char *privilegeName) const
{
    return _group->hasPrivilege(privilegeName);
}

bool Users::User::hasTask(Wt::Dbo::ptr<Projects::ProjectTask> task) const
{
    bool result = false;

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    {
        if((*iter).id() == task.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Users::User::addTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!hasTask(task))
    {
        _tasks.insert(task);

        return true;
    }

    return false;
}

bool Users::User::removeTask(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(hasTask(task))
    {
        _tasks.erase(task);

        return true;
    }

    return false;
}

int Users::User::createRank() const
{
    return _createRank;
}

void Users::User::setCreateRank(int rank)
{
    _createRank = rank;
}

std::string Users::User::idNumber() const
{
    return _idNumber;
}

void Users::User::setIdNumber(const std::string &idNumber)
{
    _idNumber = idNumber;
}

std::string Users::User::phoneNumber() const
{
    return _phoneNumber;
}

void Users::User::setPhoneNumber(const std::string &phoneNumber)
{
    _phoneNumber = phoneNumber;
}

std::string Users::User::emailAddress() const
{
    return _emailAddress;
}

void Users::User::setEmailAddress(const std::string &emailAddress)
{
    _emailAddress = emailAddress;
}

std::string Users::User::address() const
{
    return _address;
}

void Users::User::setAddress(const std::string &address)
{
    _address = address;
}

Wt::Dbo::ptr<Users::Group> Users::User::group() const
{
    return _group;
}

void Users::User::setGroup(Wt::Dbo::ptr<Group> group)
{
    _group = group;
}

Wt::Dbo::ptr<Users::UserTitle> Users::User::title() const
{
    return _title;
}

void Users::User::setTitle(Wt::Dbo::ptr<Users::UserTitle> title)
{
    _title = title;
}

Wt::Dbo::weak_ptr<Projects::Project> Users::User::project() const
{
    return _project;
}

void Users::User::setProject(Wt::Dbo::weak_ptr<Projects::Project> project)
{
    _project = project;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask> > Users::User::tasks()
{
    return _tasks;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Token> > Users::User::authTokens() const
{
    return _authTokens;
}

bool Users::User::operator ==(const Users::User &other) const
{
    return _name == other.name();
}

bool Users::User::operator !=(const Users::User &other) const
{
    return !operator==(other);
}

void Users::User::_init()
{
    thumbnail_ = "pics/NoPreview.png";
    _name = "New User";
    _password = "";
    _passwordMethod = "";
    _passwordSalt = "";
    _failedLoginAttempts = 0;
    _lastLoginAttempt = Wt::WDateTime::currentDateTime();
    _oAuthId = "";
    _oAuthProvider = "";
    _idNumber = "";
    _phoneNumber = "";
    _emailAddress = "";
    _address = "";
    _createRank = 0;
}
