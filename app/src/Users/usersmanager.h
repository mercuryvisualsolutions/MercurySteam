#ifndef USERSMANAGER_H
#define USERSMANAGER_H

#include "../Database/dbtables.h"
#include "../Log/logger.h"

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
        void initSessionLogger();
        void addDefaults();

    private:
        //variables
        Log::Logger *_logger;

        //functions
        void _addDefaultPrivileges();
        void _addDefaultGroups();
        void _addDefaultUserTitles();
        void _addDefaultUsers();
    };
}

#endif // USERSMANAGER_H
