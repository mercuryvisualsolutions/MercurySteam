#ifndef PROJECTSMANAGER_H
#define PROJECTSMANAGER_H

#include "../Database/dbtables.h"
#include "../Database/databasemanager.h"

#include <Ms/Core/MManagerBase.h>

namespace Projects
{
//singleton respoinsible for managing projects
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
        //PROJECT
        Wt::Dbo::ptr<Projects::Project> getProject(const std::string &name);
        bool projectExist(const std::string &name);
        bool addSequenceToProject(Wt::Dbo::ptr<Projects::Project> project, Wt::Dbo::ptr<Projects::ProjectSequence> sequence);
        bool removeSequenceFromProject(Wt::Dbo::ptr<Projects::Project> project, Wt::Dbo::ptr<Projects::ProjectSequence> sequence);
        bool projectHasSequence(Wt::Dbo::ptr<Projects::Project> project, Wt::Dbo::ptr<Projects::ProjectSequence> sequence);
        bool addAssetToProject(Wt::Dbo::ptr<Projects::Project> project, Wt::Dbo::ptr<Projects::ProjectAsset> asset);
        bool removeAssetFromProject(Wt::Dbo::ptr<Projects::Project> project, Wt::Dbo::ptr<Projects::ProjectAsset> asset);
        bool projectHasAsset(Wt::Dbo::ptr<Projects::Project> project, Wt::Dbo::ptr<Projects::ProjectAsset> asset);

        //PROJECT_ASSET
        Wt::Dbo::ptr<Projects::ProjectAsset> getAsset(const std::string &projectName, const std::string &assetName);
        bool assetExist(const std::string &projectName, const std::string &assetName);
        bool addTaskToAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset, Wt::Dbo::ptr<Projects::ProjectTask> task);
        bool removeTaskFromAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset, Wt::Dbo::ptr<Projects::ProjectTask> task);
        bool assetHasTask(Wt::Dbo::ptr<Projects::ProjectAsset> asset, Wt::Dbo::ptr<Projects::ProjectTask> task);

        //PROJECT_SEQUENCE
        Wt::Dbo::ptr<Projects::ProjectSequence> getSequence(const std::string &projectName, const std::string &sequenceName);
        bool sequenceExist(const std::string &projectName, const std::string &sequenceName);
        bool addShotToSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence, Wt::Dbo::ptr<Projects::ProjectShot> shot);
        bool removeShotFromSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence, Wt::Dbo::ptr<Projects::ProjectShot> shot);
        bool sequenceHasShot(Wt::Dbo::ptr<Projects::ProjectSequence> sequence, Wt::Dbo::ptr<Projects::ProjectShot> shot);
        bool sequenceHasShot(Wt::Dbo::ptr<Projects::ProjectSequence> sequence, const Projects::ProjectShot &shot);

        //PROJECT_SHOT
        Wt::Dbo::ptr<Projects::ProjectShot> getShot(const std::string &projectName, const std::string &sequenceName, const std::string &shotName);
        bool shotExist(const std::string &projectName, const std::string &sequenceName, const std::string &shotName);
        bool addTaskToShot(Wt::Dbo::ptr<Projects::ProjectShot> shot, Wt::Dbo::ptr<Projects::ProjectTask> task);
        bool removeTaskFromShot(Wt::Dbo::ptr<Projects::ProjectShot> shot, Wt::Dbo::ptr<Projects::ProjectTask> task);
        bool shotHasTask(Wt::Dbo::ptr<Projects::ProjectShot> shot, Wt::Dbo::ptr<Projects::ProjectTask> task);

        //PROJECT_TASK
        Wt::Dbo::ptr<Projects::ProjectTask> getShotTask(const std::string &projectName, const std::string &sequenceName,
                                                        const std::string &shotName, Wt::Dbo::dbo_traits<Projects::ProjectTask>::IdType id);
        Wt::Dbo::ptr<Projects::ProjectTask> getAssetTask(const std::string &projectName, const std::string &assetName, Wt::Dbo::dbo_traits<Projects::ProjectTask>::IdType id);
        bool shotTaskExist(const std::string &projectName, const std::string &sequenceName, const std::string &shotName, u_int64_t id);
        bool assetTaskExist(const std::string &projectName, const std::string &assetName, Wt::Dbo::dbo_traits<Projects::ProjectTask>::IdType id);

        //PROJECT_WORK_STATUS_TYPE
        Wt::Dbo::ptr<Projects::ProjectWorkStatusType> getProjectWorkStatusType(const std::string &name);
        bool projectWorkStatusTypeExist(const std::string &name);

        //PROJECT_ASSET_TYPE
        Wt::Dbo::ptr<Projects::ProjectAssetType> getProjectAssetType(const std::string &name);
        bool projectAssetTypeExist(const std::string &name);

        //PROJECT_WORK_STATUS
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> getProjectWorkStatus(const std::string &status);
        bool projectWorkStatusExist(const std::string &status);

        //PROJECT_TASK_TYPE
        Wt::Dbo::ptr<Projects::ProjectTaskType> getProjectTaskType(const std::string &name);
        bool projectTaskExist(const std::string &name);

        //GENERAL
        void addDefaults();

    private:
        void _addDefaultProjectWorkStatusTypes();
        void _addDefaultProjectWorkStatus();
        void _addDefaultProjectAssetTypes();
        void _addDefaultProjectTaskTypes();
    };
}

#endif // PROJECTSMANAGER_H
