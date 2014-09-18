#ifndef PROJECTSMANAGER_H
#define PROJECTSMANAGER_H

#include "../Database/dbtables.h"
#include "../Database/databasemanager.h"

#include <Ms/Core/MManagerBase.h>

namespace Projects
{
    class ProjectsManager : public Ms::Core::MManagerBase
    {
    private:
        ProjectsManager();
        ProjectsManager(ProjectsManager const &copy) = delete;
        void operator=(ProjectsManager const &copy) = delete;

    public:
        static ProjectsManager &instance()
        {
            static ProjectsManager man;
            return man;
        }

        //functions
        void initSessionLogger();
        void addDefaults();

    private:
        //variables
        Log::Logger *_logger;

        //functions
        void _addDefaultProjectTaskActivityTypes();
        void _addDefaultProjectWorkStatusTypes();
        void _addDefaultProjectWorkStatus();
        void _addDefaultProjectAssetTypes();
        void _addDefaultProjectTaskTypes();
    };
}

#endif // PROJECTSMANAGER_H
