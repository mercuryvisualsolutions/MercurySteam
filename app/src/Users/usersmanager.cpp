#include "usersmanager.h"
#include "usersio.h"
#include "../Auth/authmanager.h"
#include "Database/databasemanager.h"
#include "../Log/logmanager.h"

#include <Ms/Exceptions/MNullPointerException.h>
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

void Users::UsersManager::addDefaults()
{
    _addDefaultPrivileges();
    _addDefaultGroups();
    _addDefaultUserTitles();
    _addDefaultUsers();
}

void Users::UsersManager::_addDefaultPrivileges()
{
    std::vector<Users::Privilege*> vec;

    vec.push_back(new Users::Privilege("View"));
    vec.push_back(new Users::Privilege("Edit"));
    vec.push_back(new Users::Privilege("Create"));
    vec.push_back(new Users::Privilege("Remove"));
    vec.push_back(new Users::Privilege("Check In"));
    vec.push_back(new Users::Privilege("Check Out"));
    vec.push_back(new Users::Privilege("Create Repositories"));

    try
    {
        for(std::vector<Users::Privilege*>::size_type i = 0; i < vec.size(); ++i)
        {
            if(!Database::DatabaseManager::instance().createDbo<Users::Privilege>(vec.at(i)))
            {
                delete vec.at(i);
            }
        }
    }
    catch(Wt::Dbo::Exception ex)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to add default privileges ") + ex.what() , Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log("Error occured while trying to add default work privileges", Ms::Log::LogMessageType::Error);
    }
}

void Users::UsersManager::_addDefaultGroups()
{
    try
    {
        Wt::Dbo::ptr<Users::Privilege> viewPrvPtr = Database::DatabaseManager::instance().getDbo<Users::Privilege>("View");
        Wt::Dbo::ptr<Users::Privilege> editPrvPtr = Database::DatabaseManager::instance().getDbo<Users::Privilege>("Edit");
        Wt::Dbo::ptr<Users::Privilege> createPrvPtr = Database::DatabaseManager::instance().getDbo<Users::Privilege>("Create");
        Wt::Dbo::ptr<Users::Privilege> removePrvPtr = Database::DatabaseManager::instance().getDbo<Users::Privilege>("Remove");
        Wt::Dbo::ptr<Users::Privilege> checkInPrvPtr = Database::DatabaseManager::instance().getDbo<Users::Privilege>("Check In");
        Wt::Dbo::ptr<Users::Privilege> checkOutPrvPtr = Database::DatabaseManager::instance().getDbo<Users::Privilege>("Check Out");
        Wt::Dbo::ptr<Users::Privilege> createReposPrvPtr = Database::DatabaseManager::instance().getDbo<Users::Privilege>("Create Repositories");

        Users::Group *groupAD = new Users::Group("Admin");
        Wt::Dbo::ptr<Users::Group> groupADPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(groupAD);
        if(groupADPtr.get())
        {
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupADPtr)->addPrivilege(viewPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupADPtr)->addPrivilege(editPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupADPtr)->addPrivilege(createPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupADPtr)->addPrivilege(removePrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupADPtr)->addPrivilege(checkInPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupADPtr)->addPrivilege(checkOutPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupADPtr)->addPrivilege(createReposPrvPtr);
        }
        else
            delete groupAD;

        Users::Group *groupGU = new Users::Group("Guest");
        Wt::Dbo::ptr<Users::Group> groupGUPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(groupGU);
        if(groupGUPtr.get())
        {
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupGUPtr)->addPrivilege(viewPrvPtr);
        }
        else
            delete groupGU;

        Users::Group *groupCL = new Users::Group("Client");
        Wt::Dbo::ptr<Users::Group> groupCLPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(groupCL);
        if(groupCLPtr.get())
        {
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupCLPtr)->addPrivilege(viewPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupCLPtr)->addPrivilege(editPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupCLPtr)->addPrivilege(checkInPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupCLPtr)->addPrivilege(checkOutPrvPtr);
        }
        else
            delete groupCL;

        Users::Group *groupFL = new Users::Group("FreeLancer");
        Wt::Dbo::ptr<Users::Group> groupFLPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(groupFL);
        if(groupFLPtr.get())
        {
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupFLPtr)->addPrivilege(viewPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupFLPtr)->addPrivilege(editPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupFLPtr)->addPrivilege(checkInPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupFLPtr)->addPrivilege(checkOutPrvPtr);
        }
        else
            delete groupFL;

        Users::Group *groupEM = new Users::Group("Employee");
        Wt::Dbo::ptr<Users::Group> groupEMPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(groupEM);
        if(groupEMPtr.get())
        {
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupEMPtr)->addPrivilege(viewPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupEMPtr)->addPrivilege(editPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupEMPtr)->addPrivilege(checkInPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupEMPtr)->addPrivilege(checkOutPrvPtr);
        }
        else
            delete groupEM;

        Users::Group *groupSU = new Users::Group("SuperVisor");
        Wt::Dbo::ptr<Users::Group> groupSUPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(groupSU);
        if(groupSUPtr.get())
        {
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupSUPtr)->addPrivilege(viewPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupSUPtr)->addPrivilege(editPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupSUPtr)->addPrivilege(createPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupSUPtr)->addPrivilege(checkInPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupSUPtr)->addPrivilege(checkOutPrvPtr);
        }
        else
            delete groupSU;

        Users::Group *groupPM = new Users::Group("ProjectManager");
        Wt::Dbo::ptr<Users::Group> groupPMPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(groupPM);
        if(groupPMPtr.get())
        {
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupPMPtr)->addPrivilege(viewPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupPMPtr)->addPrivilege(editPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupPMPtr)->addPrivilege(createPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupPMPtr)->addPrivilege(removePrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupPMPtr)->addPrivilege(checkInPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupPMPtr)->addPrivilege(checkOutPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupPMPtr)->addPrivilege(createReposPrvPtr);
        }
        else
            delete groupPM;

        Users::Group *groupGM = new Users::Group("GeneralManager");
        Wt::Dbo::ptr<Users::Group> groupGMPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(groupGM);
        if(groupGMPtr.get())
        {
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupGMPtr)->addPrivilege(viewPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupGMPtr)->addPrivilege(editPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupGMPtr)->addPrivilege(createPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupGMPtr)->addPrivilege(removePrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupGMPtr)->addPrivilege(checkInPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupGMPtr)->addPrivilege(checkOutPrvPtr);
            Database::DatabaseManager::instance().modifyDbo<Users::Group>(groupGMPtr)->addPrivilege(createReposPrvPtr);
        }
        else
            delete groupGM;

        Database::DatabaseManager::instance().commitTransaction();
    }
    catch(Wt::Dbo::Exception ex)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to add default groups ") + ex.what() , Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log("Error occured while trying to add default groups", Ms::Log::LogMessageType::Error);
    }
}

void Users::UsersManager::_addDefaultUserTitles()
{
    std::vector<Users::UserTitle*> vec;

    vec.push_back(new Users::UserTitle("Line Producer"));
    vec.push_back(new Users::UserTitle("3D-Artist"));
    vec.push_back(new Users::UserTitle("3D-Animator"));
    vec.push_back(new Users::UserTitle("3D-Technical Director"));
    vec.push_back(new Users::UserTitle("Project Manager"));
    vec.push_back(new Users::UserTitle("Art Director"));
    vec.push_back(new Users::UserTitle("Editor"));
    vec.push_back(new Users::UserTitle("Senior Editor"));
    vec.push_back(new Users::UserTitle("Compositor"));
    vec.push_back(new Users::UserTitle("Senior Compositor"));
    vec.push_back(new Users::UserTitle("3D Senior Artist"));
    vec.push_back(new Users::UserTitle("General Manager"));
    vec.push_back(new Users::UserTitle("Photoshop Artist"));
    vec.push_back(new Users::UserTitle("2D-Artist"));
    vec.push_back(new Users::UserTitle("Matte Painter"));
    vec.push_back(new Users::UserTitle("Environment Artist"));
    vec.push_back(new Users::UserTitle("Owner"));

    try
    {
        for(std::vector<Users::UserTitle*>::size_type i = 0; i < vec.size(); ++i)
        {
            if(!Database::DatabaseManager::instance().createDbo<Users::UserTitle>(vec.at(i)))
            {
                delete vec.at(i);
            }
        }
    }
    catch(Wt::Dbo::Exception ex)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to add default users titles ") + ex.what() , Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log("Error occured while trying to add default work users titles", Ms::Log::LogMessageType::Error);
    }
}

void Users::UsersManager::_addDefaultUsers()
{
    try
    {
        Users::User *user = new Users::User("admin", "admin@mercuryvs.com");
        user->setCreateRank(INT_MAX);
        user->setViewRank(INT_MAX);
        user->setEditRank(INT_MAX);
        user->setRemoveRank(INT_MAX);

        Wt::Dbo::ptr<Users::Group> group = Database::DatabaseManager::instance().getDbo<Users::Group>("Admin");
        user->setGroup(group);

        Wt::Auth::User authUser = Auth::AuthManager::instance().registerUser(user->name(), "admin");
        if(authUser.isValid())//added the auth user successfully ?
        {
            Wt::Dbo::ptr<Users::User> userPtr = Database::DatabaseManager::instance().createDbo<Users::User>(user);

            if(userPtr)
            {
                Wt::Dbo::ptr<Auth::AuthInfo> authInfo = Auth::AuthManager::instance().getUserAuthInfo(authUser);
                if(authInfo)
                    authInfo.modify()->setUser(userPtr);

                //create user directory structure
                Users::UsersIO::createUserDirectoryStructure(user->name());
            }
        }

        Database::DatabaseManager::instance().commitTransaction();
    }
    catch(Wt::Dbo::Exception ex)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to add default users ") + ex.what() , Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log("Error occured while trying to add default work users", Ms::Log::LogMessageType::Error);
    }
}


