#ifndef USERSMANAGER_H
#define USERSMANAGER_H

//#include <Wt/Dbo/ptr>

#include "../Database/dbtables.h"

#include <Ms/Core/MManagerBase.h>

namespace Users
{
    class UsersManager : public Ms::Core::MManagerBase
    {
    private:
        UsersManager();
        UsersManager(UsersManager const &copy) = delete;
        void operator=(UsersManager const &copy) = delete;

    public:
        static UsersManager &instance()
        {
            static UsersManager man;
            return man;
        }

        //functions
        //GROUP
        Wt::Dbo::ptr<Users::Group> getGroup(const std::string &name);
        bool groupExist(const std::string &name);
        bool groupHasPrivilege(Wt::Dbo::ptr<Users::Group> group, Wt::Dbo::ptr<Privilege> privilege);
        bool groupHasPrivilege(Wt::Dbo::ptr<Users::Group> group, Users::PrivilegeFlags flags);
        bool groupHasPrivilege(Wt::Dbo::ptr<Users::Group> group, const std::string &privilegeName);
        bool groupHasPrivilege(const std::string &groupName, Users::PrivilegeFlags flags);
        bool groupHasUser(Wt::Dbo::ptr<Users::Group> group, Wt::Dbo::ptr<Users::User> user);
        bool addPrivilegeToGroup(Wt::Dbo::ptr<Users::Group> group, Wt::Dbo::ptr<Privilege> privilege);
        bool removePrivilegeFromGroup(Wt::Dbo::ptr<Users::Group> group, Wt::Dbo::ptr<Privilege> privilege);
        bool addUserToGroup(Wt::Dbo::ptr<Users::Group> group, Wt::Dbo::ptr<Users::User> user);
        bool removeUserFromGroup(Wt::Dbo::ptr<Users::Group> group, Wt::Dbo::ptr<Users::User> user);

        //USER
        Wt::Dbo::ptr<Users::User> getUser(const std::string &provider, const std::string &identity);
        bool userExist(const std::string &provider, const std::string &identity);
        Wt::Dbo::ptr<Users::User> createUser(User *user, const std::string &password);
        Wt::Dbo::ptr<Users::Group> getUserGroup(const std::string &provider, const std::string &identity);
        bool userHasTask(Wt::Dbo::ptr<Users::User> user, Wt::Dbo::ptr<Projects::ProjectTask> task);
        bool assignTaskToUser(Wt::Dbo::ptr<Users::User> user, Wt::Dbo::ptr<Projects::ProjectTask> task);
        bool removeTaskFromUser(Wt::Dbo::ptr<Users::User> user, Wt::Dbo::ptr<Projects::ProjectTask> task);
        bool checkUserRank(Wt::Dbo::ptr<Users::User> userPtr, int rank, Database::RankFlag rankFlag);
        bool checkUserRank(const std::string &provider, const std::string &identity, int rank, Database::RankFlag rankFlag);
        const std::string getCurrentUserName() const;
        int getUserRank(Wt::Dbo::ptr<Users::User> userPtr, Database::RankFlag rankFlag);
        int getUserRank(const std::string &provider, const std::string &identity, Database::RankFlag rankFlag);
        int getCurrentUserRank(Database::RankFlag rankFlag);
        const std::string getCurrentUserDir();
        const std::string getCurrentUserTmpDir();
        bool checkUserPrivileges(Wt::Dbo::ptr<Users::User> userPtr, Users::PrivilegeFlags flags);
        bool checkUserPrivileges(const std::string &provider, const std::string &identity, Users::PrivilegeFlags flags);
        bool checkCurrentUserPrivileges(Users::PrivilegeFlags flags);

        //USER_TITLE
        Wt::Dbo::ptr<Users::UserTitle> getUserTitle(const std::string &name);
        bool userTitleExist(const std::string &name);

        //PRIVILEGE
        Wt::Dbo::ptr<Users::Privilege> getPrivilege(const std::string &name);
        bool privilegeExist(const std::string &name);

        //GENERAL
        void addDefaults();

    private:
        //variables
        Ms::Log::MLogger *_logger;

        //functions
        void _addDefaultPrivileges();
        void _addDefaultGroups();
        void _addDefaultUserTitles();
        void _addDefaultUsers();
    };
}

#endif // USERSMANAGER_H
