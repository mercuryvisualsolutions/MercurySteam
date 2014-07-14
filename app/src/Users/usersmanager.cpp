#include "usersmanager.h"
#include "usersio.h"
#include "../Auth/authmanager.h"
#include "Database/databasemanager.h"
#include "../Log/logmanager.h"

#include <Ms/Exceptions/MDboSessionIsNullException.h>
#include <Ms/IO/IO.h>

#include <Wt/WApplication>

Users::UsersManager::UsersManager() :
    MManagerBase()
{
}

void Users::UsersManager::initSessionLogger()
{
    _logger = Log::LogManager::instance().getAppSessionLogger(Wt::WApplication::instance()->sessionId());
}

//GROUP
Wt::Dbo::ptr<Users::Group> Users::UsersManager::getGroup(const std::string &name)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't get group: " + name + ", DbSession object is null");

    Wt::Dbo::ptr<Users::Group> groupPtr;
    if(Database::DatabaseManager::instance().openTransaction())
    {
        groupPtr = Database::DatabaseManager::instance().session()->find<Users::Group>().where("Name = ?").bind(name);

        Database::DatabaseManager::instance().commitTransaction();
    }
    return groupPtr;
}

bool Users::UsersManager::groupExist(const std::string &name)
{
    return getGroup(name).get() != nullptr;
}

bool Users::UsersManager::groupHasPrivilege(Wt::Dbo::ptr<Group> group, Wt::Dbo::ptr<Users::Privilege> privilege)
{
    if(!group)
        throw std::invalid_argument("Can't check privilege object existence in group!, group object is null");

    if(!privilege)
        throw std::invalid_argument("Can't check privilege object existence in group: " + group->name() + ", privilege object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't enumerate through group: " + group->name() + "privileges, DbSession object is null");

    try
    {
        if(Database::DatabaseManager::instance().openTransaction())
        {
            for(auto iter = group->_privileges.begin(); iter != group->_privileges.end(); ++iter)
            {
                if((*iter).id() == privilege.id())
                    return true;
            }
        }
    }
    catch(...)
    {
        _logger->log("Error occured while enumerating through group privileges!", Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

bool Users::UsersManager::groupHasPrivilege(Wt::Dbo::ptr<Users::Group> group, Users::PrivilegeFlags flags)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't check group privileges!, DbSession object is null");

    if(!group)
        throw std::invalid_argument("Can't check group privileges!, group object is null");

    if(group->numPrivileges() == 0)
        return false;

    if(!Database::DatabaseManager::instance().openTransaction())
        return false;

    std::bitset<PrivilegeFlags::COUNT> hasPrivileges;

    for(unsigned int i = 0; i < Users::PrivilegeFlags::COUNT; ++i)
    {
        std::string privilegeName = "";
        if((flags & Users::PrivilegeFlags::View) != 0)
            privilegeName = "View";
        else if((flags & Users::PrivilegeFlags::Edit) != 0)
            privilegeName = "Edit";
        else if((flags & Users::PrivilegeFlags::Create) != 0)
            privilegeName = "Create";
        else if((flags & Users::PrivilegeFlags::Remove) != 0)
            privilegeName = "Remove";
        else if((flags & Users::PrivilegeFlags::CheckIn) != 0)
            privilegeName = "Check In";
        else if((flags & Users::PrivilegeFlags::CheckOut) != 0)
            privilegeName = "Check Out";
        else if((flags & Users::PrivilegeFlags::CreateRepos) != 0)
            privilegeName = "Create Repositories";

        for(auto iter = group->_privileges.begin(); iter != group->_privileges.end(); ++iter)
        {
            if((*iter)->name() == privilegeName)
            {
                hasPrivileges.set(i);
                break;
            }
        }
    }

    return hasPrivileges.count() == PrivilegeFlags::COUNT;
}

bool Users::UsersManager::groupHasPrivilege(Wt::Dbo::ptr<Users::Group> group, const std::string &privilegeName)
{
    if(!group)
        throw std::invalid_argument("Can't check privilege object: " + privilegeName + ", existence in group!, group object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't enumerate through group: " + group->name() + "privileges, DbSession object is null");

    try
    {
        if(Database::DatabaseManager::instance().openTransaction())
        {
            for(auto iter = group->_privileges.begin(); iter != group->_privileges.end(); ++iter)
            {
                if((*iter)->name() == privilegeName)
                    return true;
            }
        }
    }
    catch(...)
    {
        _logger->log("Error occured while enumerating through group privileges!", Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

bool Users::UsersManager::groupHasPrivilege(const std::string &groupName, Users::PrivilegeFlags flags)
{
    Wt::Dbo::ptr<Users::Group> groupPtr = getGroup(groupName);
    return groupHasPrivilege(groupPtr, flags);
}

bool Users::UsersManager::groupHasUser(Wt::Dbo::ptr<Users::Group> group, Wt::Dbo::ptr<Users::User> user)
{
    if(!group)
        throw std::invalid_argument("Can't check user object existence in group!, group object is null");

    if(!user)
        throw std::invalid_argument("Can't check user object existence in group: " + group->name() + ", user object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't enumerate through group: " + group->name() + "users, DbSession object is null");

    try
    {
        if(Database::DatabaseManager::instance().openTransaction())
        {
            for(auto iter = group->_users.begin(); iter != group->_users.end(); ++iter)
            {
                if((*iter).id() == user.id())
                    return true;
            }
        }
    }
    catch(...)
    {
        _logger->log("Error occured while enumerating through group users!", Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

bool Users::UsersManager::addPrivilegeToGroup(Wt::Dbo::ptr<Users::Group> group, Wt::Dbo::ptr<Users::Privilege> privilege)
{
    if(!group)
        throw std::invalid_argument("Can't add privilege to group!, group object is null");

    if(!privilege)
        throw std::invalid_argument("Can't add privilege to group: " + group->name() + ", privilege object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't add privilege: " + privilege->name() + " to group: " + group->name() + " privileges, DbSession object is null");

    try
    {
        if(!groupHasPrivilege(group, privilege))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Users::Group>(group)->_privileges.insert(privilege);
                return true;
            }
        }
    }
    catch(...)
    {
        _logger->log(std::string("Error occured while trying to add privilege: ") + privilege->name() + "to group: " + group->name(),
                     Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

bool Users::UsersManager::removePrivilegeFromGroup(Wt::Dbo::ptr<Users::Group> group, Wt::Dbo::ptr<Users::Privilege> privilege)
{
    if(!group)
        throw std::invalid_argument("Can't check privilege object existence in group!, group object is null");

    if(!privilege)
        throw std::invalid_argument("Can't check privilege object existence in group: " + group->name() + ", privilege object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't remove privilege: " + privilege->name() + " from group: " + group->name() + ", DbSession object is null");

    try
    {
        if(groupHasPrivilege(group, privilege))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Users::Group>(group)->_privileges.erase(privilege);

                return true;
            }
        }
    }
    catch(...)
    {
        _logger->log(std::string("Error occured while trying to remove privilege: ") + privilege->name() + " from group: " + group->name(),
                     Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

bool Users::UsersManager::addUserToGroup(Wt::Dbo::ptr<Users::Group> group, Wt::Dbo::ptr<User> user)
{
    if(!group)
        throw std::invalid_argument("Can't add user to group!, group object is null");

    if(!user)
        throw std::invalid_argument("Can't add user to group: " + group->name() + ", user object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't add user: " + user->name() + " to group: " + user->name() + "user, DbSession object is null");

    try
    {
        if(!groupHasUser(group, user))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Users::Group>(group)->_users.insert(user);
                return true;
            }
        }
    }
    catch(...)
    {
        _logger->log(std::string("Error occured while trying to add user: ") + user->name() + "to group: " + group->name(),
                     Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

bool Users::UsersManager::removeUserFromGroup(Wt::Dbo::ptr<Users::Group> group, Wt::Dbo::ptr<Users::User> user)
{
    if(!group)
        throw std::invalid_argument("Can't remove user from group!, group object is null");

    if(!user)
        throw std::invalid_argument("Can't remove user from group: " + group->name() + ", user object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't remove user:" + user->name() + " from group: " + group->name() + ", DbSession object is null");

    try
    {
        if(groupHasUser(group, user))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Users::Group>(group)->_users.erase(user);
                return true;
            }
        }
    }
    catch(...)
    {
        _logger->log(std::string("Error occured while trying to remove user: ") + user->name() + " from group: " + group->name(),
                     Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

//USER
Wt::Dbo::ptr<Users::User> Users::UsersManager::createUser(User *user, const std::string &password)
{
    Wt::Dbo::ptr<Users::User> userPtr;

    if(!user)
        throw std::invalid_argument("Can't create user, user object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't create user: " + user->name() + ", DbSession object is null");

    Wt::Auth::User authUser = Auth::AuthManager::instance().registerUser(user->name(), password, user->emailAddress());

    if(authUser.isValid())//added the auth user successfully ?
    {
        try
        {
            if(!Database::DatabaseManager::instance().openTransaction())
                return userPtr;

            userPtr = Database::DatabaseManager::instance().createDbo<Users::User>(user);

            if(userPtr)
            {
                Wt::Dbo::ptr<Auth::AuthInfo> authInfo = Auth::AuthManager::instance().getUserAuthInfo(authUser);
                if(authInfo)
                    authInfo.modify()->setUser(userPtr);
            }

            Database::DatabaseManager::instance().commitTransaction();

            //create user directory structure
            Users::UsersIO::createUserDirectoryStructure(user->name());
        }
        catch(Wt::WException e)
        {
            std::cerr << "Exception occured while trying to add a new user" << std::endl << e.what() << std::endl;
        }
    }

    return userPtr;
}

Wt::Dbo::ptr<Users::Group> Users::UsersManager::getUserGroup(const std::string &provider, const std::string &identity)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't get user: " + identity + " group, DbSession object is null");

    Wt::Dbo::ptr<Users::Group> groupPtr;
    Wt::Dbo::ptr<Users::User> userPtr = getUser(provider, identity);

    if(userPtr)
    {
        if(Database::DatabaseManager::instance().openTransaction())
            groupPtr = userPtr->group();
    }

    return groupPtr;
}

bool Users::UsersManager::userHasTask(Wt::Dbo::ptr<Users::User> user, Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!user)
        throw std::invalid_argument("Can't check task object existence in user!, user object is null");

    if(!task)
        throw std::invalid_argument("Can't check task object existence in user: " + user->name() + ", task object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't enumerate through user: " + user->name() + "tasks, DbSession object is null");

    try
    {
        if(Database::DatabaseManager::instance().openTransaction())
        {
            for(auto iter = user->_tasks.begin(); iter != user->_tasks.end(); ++iter)
            {
                if((*iter).id() == task.id())
                    return true;
            }
        }
    }
    catch(...)
    {
        _logger->log(std::string("Error occured while enumerating through user: ") + user->name() + "tasks!",
                     Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

bool Users::UsersManager::assignTaskToUser(Wt::Dbo::ptr<Users::User> user, Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!user)
        throw std::invalid_argument("Can't check task object existence in user!, user object is null");

    if(!task)
        throw std::invalid_argument("Can't check task object existence in user: " + user->name() + ", task object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't enumerate through user: " + user->name() + "tasks, DbSession object is null");

    try
    {
        if(!userHasTask(user, task))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Users::User>(user)->_tasks.insert(task);
                return true;
            }
        }
    }
    catch(...)
    {
        _logger->log(std::string("Error occured while enumerating through user: ") + user->name() + "tasks!",
                     Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

bool Users::UsersManager::removeTaskFromUser(Wt::Dbo::ptr<Users::User> user, Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!user)
        throw std::invalid_argument("Can't remove task from user!, user object is null");

    if(!task)
        throw std::invalid_argument("Can't remove task from user: " + user->name() + ", task object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't remove task from user: " + user->name() + "tasks, DbSession object is null");

    try
    {
        if(userHasTask(user, task))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Users::User>(user)->_tasks.erase(task);
                return true;
            }
        }
    }
    catch(...)
    {
        _logger->log(std::string("Error occured while enumerating through user: ") + user->name() + "tasks!",
                     Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

Wt::Dbo::ptr<Users::User> Users::UsersManager::getUser(const std::string &provider, const std::string &identity)
{
    Wt::Auth::User authUser = Auth::AuthManager::instance().getAuthUser(provider, identity);
    Wt::Dbo::ptr<Users::User> userPtr;

    if(authUser.isValid())
    {
        Wt::Dbo::ptr<Auth::AuthInfo> authInfo = Auth::AuthManager::instance().getUserAuthInfo(authUser);
        if(authInfo)
        {
            if(Database::DatabaseManager::instance().openTransaction())
                userPtr = authInfo->user();
        }
    }

    return userPtr;
}

bool Users::UsersManager::userExist(const std::string &provider, const std::string &identity)
{
    return getUser(provider, identity).get() != nullptr;
}

bool Users::UsersManager::checkUserRank(Wt::Dbo::ptr<Users::User> userPtr, int rank, Database::RankFlag rankFlag)
{
    if(!userPtr)
        throw std::invalid_argument("Can't check user rank, user object is null");

    if(!Database::DatabaseManager::instance().openTransaction())
        return false;

    switch(rankFlag)
    {
        case Database::ViewRank:
            return userPtr->viewRank() >= rank ? true : false;
        case Database::EditRank:
            return userPtr->editRank() >= rank ? true : false;
        case Database::CreateRank:
            return userPtr->createRank() >= rank ? true : false;
        case Database::RemoveRank:
            return userPtr->removeRank() >= rank ? true : false;
    }

    return false;
}

bool Users::UsersManager::checkUserRank(const std::string &provider, const std::string &identity, int rank, Database::RankFlag rankFlag)
{   
    Wt::Dbo::ptr<Users::User> userPtr = getUser(provider, identity);

    return checkUserRank(userPtr, rank, rankFlag);
}

const std::string Users::UsersManager::getCurrentUserName() const
{
    return Auth::AuthManager::instance().login().user().identity(Wt::Auth::Identity::LoginName).toUTF8();
}

int Users::UsersManager::getUserRank(Wt::Dbo::ptr<Users::User> userPtr, Database::RankFlag rankFlag)
{
    if(!userPtr)
        throw std::invalid_argument("Can't get user rank!, user object is null");

    if(!Database::DatabaseManager::instance().openTransaction())
        return -1;

    switch(rankFlag)
    {
        case Database::ViewRank:
            return userPtr->viewRank();
        case Database::EditRank:
            return userPtr->editRank();
        case Database::CreateRank:
            return userPtr->createRank();
        case Database::RemoveRank:
            return userPtr->removeRank();
    }

    return -1;
}

int Users::UsersManager::getUserRank(const std::string &provider, const std::string &identity, Database::RankFlag rankFlag)
{
    return getUserRank(getUser(provider, identity), rankFlag);
}

int Users::UsersManager::getCurrentUserRank(Database::RankFlag rankFlag)
{
    return getUserRank(Wt::Auth::Identity::LoginName, Auth::AuthManager::instance().login().user().identity(Wt::Auth::Identity::LoginName).toUTF8(), rankFlag);
}

const std::string Users::UsersManager::getCurrentUserDir()
{
    return Users::UsersIO::getAbsoluteUserDir(Auth::AuthManager::instance().login().user().identity(Wt::Auth::Identity::LoginName).toUTF8());
}

const std::string Users::UsersManager::getCurrentUserTmpDir()
{
    return getCurrentUserDir() + Ms::IO::dirSeparator() + "tmp";
}

bool Users::UsersManager::checkUserPrivileges(Wt::Dbo::ptr<Users::User> userPtr, Users::PrivilegeFlags flags)
{
    if(!userPtr)
        throw std::invalid_argument("Can't check user privilege!, user object is null");

    if(!userPtr->group())
        throw std::invalid_argument("Can't check user: " + userPtr->name() + " privilege!, user group is null");

    if(!Database::DatabaseManager::instance().openTransaction())
        return false;

    return groupHasPrivilege(userPtr->group(), flags);
}

bool Users::UsersManager::checkUserPrivileges(const std::string &provider, const std::string &identity, Users::PrivilegeFlags flags)
{
    Wt::Dbo::ptr<Users::User> userPtr = getUser(provider, identity);
    return checkUserPrivileges(userPtr, flags);
}

bool Users::UsersManager::checkCurrentUserPrivileges(Users::PrivilegeFlags flags)
{
    return checkUserPrivileges(Wt::Auth::Identity::LoginName, Auth::AuthManager::instance().login().user().identity(Wt::Auth::Identity::LoginName).toUTF8(), flags);
}

//USER_TITLE
Wt::Dbo::ptr<Users::UserTitle> Users::UsersManager::getUserTitle(const std::string &name)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't get user title: " + name + ", DbSession object is null");

    Wt::Dbo::ptr<Users::UserTitle> titlePtr;
    if(Database::DatabaseManager::instance().openTransaction())
    {
        titlePtr = Database::DatabaseManager::instance().session()->find<Users::UserTitle>().where("Name = ?").bind(name);

        Database::DatabaseManager::instance().commitTransaction();
    }
    return titlePtr;
}

bool Users::UsersManager::userTitleExist(const std::string &name)
{
    return getUserTitle(name).get() != nullptr;
}

//PRIVIELEG
Wt::Dbo::ptr<Users::Privilege> Users::UsersManager::getPrivilege(const std::string &name)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't get privilege: " + name + ", DbSession object is null");

    Wt::Dbo::ptr<Users::Privilege> prvPtr;
    if(Database::DatabaseManager::instance().openTransaction())
    {
        prvPtr = Database::DatabaseManager::instance().session()->find<Users::Privilege>().where("Name = ?").bind(name);

        Database::DatabaseManager::instance().commitTransaction();
    }
    return prvPtr;
}

bool Users::UsersManager::privilegeExist(const std::string &name)
{
    return getPrivilege(name).get() != nullptr;
}

//GENERAL

void Users::UsersManager::addDefaults()
{
    _addDefaultPrivileges();
    _addDefaultGroups();
    _addDefaultUserTitles();
    _addDefaultUsers();
}

void Users::UsersManager::_addDefaultPrivileges()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Database::DatabaseManager::instance().createDbo<Users::Privilege>(new Users::Privilege("View"));
        Database::DatabaseManager::instance().createDbo<Users::Privilege>(new Users::Privilege("Edit"));
        Database::DatabaseManager::instance().createDbo<Users::Privilege>(new Users::Privilege("Create"));
        Database::DatabaseManager::instance().createDbo<Users::Privilege>(new Users::Privilege("Remove"));
        Database::DatabaseManager::instance().createDbo<Users::Privilege>(new Users::Privilege("Check In"));
        Database::DatabaseManager::instance().createDbo<Users::Privilege>(new Users::Privilege("Check Out"));
        Database::DatabaseManager::instance().createDbo<Users::Privilege>(new Users::Privilege("Create Repositories"));

        Database::DatabaseManager::instance().commitTransaction();
    }
    catch(Wt::Dbo::Exception ex)
    {
        std::cerr << "Error occured while trying to add default privileges" << std::endl << ex.what() << std::endl;
    }
}

void Users::UsersManager::_addDefaultGroups()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::ptr<Users::Group> grpPtr;

        Wt::Dbo::ptr<Users::Privilege> viewPrvPtr = getPrivilege("View");
        Wt::Dbo::ptr<Users::Privilege> editPrvPtr = getPrivilege("Edit");
        Wt::Dbo::ptr<Users::Privilege> createPrvPtr = getPrivilege("Create");
        Wt::Dbo::ptr<Users::Privilege> removePrvPtr = getPrivilege("Remove");
        Wt::Dbo::ptr<Users::Privilege> checkInPrvPtr = getPrivilege("Check In");
        Wt::Dbo::ptr<Users::Privilege> checkOutPrvPtr = getPrivilege("Check Out");
        Wt::Dbo::ptr<Users::Privilege> createReposPrvPtr = getPrivilege("Create Repositories");

        grpPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(new Users::Group("Admin"));
        addPrivilegeToGroup(grpPtr, viewPrvPtr);
        addPrivilegeToGroup(grpPtr, editPrvPtr);
        addPrivilegeToGroup(grpPtr, createPrvPtr);
        addPrivilegeToGroup(grpPtr, removePrvPtr);
        addPrivilegeToGroup(grpPtr, checkInPrvPtr);
        addPrivilegeToGroup(grpPtr, checkOutPrvPtr);
        addPrivilegeToGroup(grpPtr, createReposPrvPtr);

        grpPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(new Users::Group("Guest"));
        addPrivilegeToGroup(grpPtr, viewPrvPtr);

        grpPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(new Users::Group("Client"));
        addPrivilegeToGroup(grpPtr, viewPrvPtr);
        addPrivilegeToGroup(grpPtr, editPrvPtr);
        addPrivilegeToGroup(grpPtr, checkInPrvPtr);
        addPrivilegeToGroup(grpPtr, checkOutPrvPtr);

        grpPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(new Users::Group("FreeLancer"));
        addPrivilegeToGroup(grpPtr, viewPrvPtr);
        addPrivilegeToGroup(grpPtr, editPrvPtr);
        addPrivilegeToGroup(grpPtr, checkInPrvPtr);
        addPrivilegeToGroup(grpPtr, checkOutPrvPtr);

        grpPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(new Users::Group("Employee"));
        addPrivilegeToGroup(grpPtr, viewPrvPtr);
        addPrivilegeToGroup(grpPtr, editPrvPtr);
        addPrivilegeToGroup(grpPtr, checkInPrvPtr);
        addPrivilegeToGroup(grpPtr, checkOutPrvPtr);

        grpPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(new Users::Group("SuperVisor"));
        addPrivilegeToGroup(grpPtr, viewPrvPtr);
        addPrivilegeToGroup(grpPtr, editPrvPtr);
        addPrivilegeToGroup(grpPtr, createPrvPtr);
        addPrivilegeToGroup(grpPtr, checkInPrvPtr);
        addPrivilegeToGroup(grpPtr, checkOutPrvPtr);

        grpPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(new Users::Group("ProjectManager"));
        addPrivilegeToGroup(grpPtr, viewPrvPtr);
        addPrivilegeToGroup(grpPtr, editPrvPtr);
        addPrivilegeToGroup(grpPtr, createPrvPtr);
        addPrivilegeToGroup(grpPtr, removePrvPtr);
        addPrivilegeToGroup(grpPtr, checkInPrvPtr);
        addPrivilegeToGroup(grpPtr, checkOutPrvPtr);
        addPrivilegeToGroup(grpPtr, createReposPrvPtr);

        grpPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(new Users::Group("GeneralManager"));
        addPrivilegeToGroup(grpPtr, viewPrvPtr);
        addPrivilegeToGroup(grpPtr, editPrvPtr);
        addPrivilegeToGroup(grpPtr, createPrvPtr);
        addPrivilegeToGroup(grpPtr, removePrvPtr);
        addPrivilegeToGroup(grpPtr, checkInPrvPtr);
        addPrivilegeToGroup(grpPtr, checkOutPrvPtr);
        addPrivilegeToGroup(grpPtr, createReposPrvPtr);

        Database::DatabaseManager::instance().commitTransaction();
    }
    catch(Wt::Dbo::Exception ex)
    {
        std::cerr << "Error occured while trying to add default groups" << std::endl << ex.what() << std::endl;
    }
}

void Users::UsersManager::_addDefaultUserTitles()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("Line Producer"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("3D-Artist"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("3D-Animator"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("3D-Technical Director"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("Project Manager"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("Art Director"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("Editor"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("Senior Editor"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("Compositor"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("Senior Compositor"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("3D Senior Artist"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("General Manager"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("Photoshop Artist"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("2D-Artist"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("Matte Painter"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("Environment Artist"));
        Database::DatabaseManager::instance().createDbo<Users::UserTitle>(new Users::UserTitle("Owner"));

        Database::DatabaseManager::instance().commitTransaction();
    }
    catch(Wt::Dbo::Exception ex)
    {
        std::cerr << "Error occured while trying to add default user titles" << std::endl << ex.what() << std::endl;;
    }
}

void Users::UsersManager::_addDefaultUsers()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Users::User *user = new Users::User("admin", "admin@mercuryvs.com");
        user->setCreateRank(INT_MAX);
        user->setViewRank(INT_MAX);
        user->setEditRank(INT_MAX);
        user->setRemoveRank(INT_MAX);

        Wt::Dbo::ptr<Users::Group> group = getGroup("Admin");
        user->setGroup(group);

        createUser(user, "admin");

        Database::DatabaseManager::instance().commitTransaction();
    }
    catch(Wt::Dbo::Exception ex)
    {
        std::cerr << "Error occured while trying to add default users" << std::endl << ex.what() << std::endl;
    }
}


