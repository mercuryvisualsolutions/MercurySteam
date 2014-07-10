#include "projectsmanager.h"
#include "projectsio.h"

Projects::ProjectsManager::ProjectsManager()
{
}

//PROJECT
Wt::Dbo::ptr<Projects::Project> Projects::ProjectsManager::getProject(const std::string &name)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't get project: " + name + ", DbSession object is null");

    Wt::Dbo::ptr<Projects::Project> prjPtr;
    if(Database::DatabaseManager::instance().openTransaction())
    {
        prjPtr = Database::DatabaseManager::instance().session()->find<Projects::Project>().where("Project_Name = ?").bind(name);

        Database::DatabaseManager::instance().commitTransaction();
    }
    return prjPtr;
}

bool Projects::ProjectsManager::projectExist(const std::string &name)
{
    return getProject(name).get() != nullptr;
}

bool Projects::ProjectsManager::addSequenceToProject(Wt::Dbo::ptr<Projects::Project> project, Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    if(!project)
        throw std::invalid_argument("Can't add sequence to project!, project object is null");

    if(!sequence)
        throw std::invalid_argument("Can't add sequence to project: " + project->name() + ", sequence object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't add sequence: " + sequence->name() + " to project: " + project->name() + ", DbSession object is null");

    try
    {
        if(!projectHasSequence(project, sequence))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Projects::Project>(project)->_sequences.insert(sequence);
                return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while trying to add sequence: " + sequence->name() + " to project: " + project->name() << std::endl;
    }

    return false;
}

bool Projects::ProjectsManager::removeSequenceFromProject(Wt::Dbo::ptr<Projects::Project> project, Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    if(!project)
        throw std::invalid_argument("Can't remove sequence from project!, project object is null");

    if(!sequence)
        throw std::invalid_argument("Can't remove sequence from project: " + project->name() + ", sequence object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't remove sequence: " + sequence->name() + " from project: " + project->name() + ", DbSession object is null");

    try
    {
        if(projectHasSequence(project, sequence))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Projects::Project>(project)->_sequences.erase(sequence);

                return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while trying to remove sequence: " + sequence->name() + " from project: " + project->name() << std::endl;
    }

    return false;
}

bool Projects::ProjectsManager::projectHasSequence(Wt::Dbo::ptr<Projects::Project> project, Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    if(!project)
        throw std::invalid_argument("Can't check sequence existence in project!, project object is null");

    if(!sequence)
        throw std::invalid_argument("Can't check sequence existence in project: " + project->name() + ", sequence object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't check sequence: " + sequence->name() + " existence in project: " + project->name() + ", DbSession object is null");

    try
    {
        if(Database::DatabaseManager::instance().openTransaction())
        {
            for(auto iter = project->_sequences.begin(); iter != project->_sequences.end(); ++iter)
            {
                if((*iter).id() == sequence.id())
                    return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while enumerating through project: " + project->name() + " sequences"<< std::endl;
    }

    return false;
}

bool Projects::ProjectsManager::addAssetToProject(Wt::Dbo::ptr<Projects::Project> project, Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    if(!project)
        throw std::invalid_argument("Can't add asset to project!, project object is null");

    if(!asset)
        throw std::invalid_argument("Can't add asset to project: " + project->name() + ", asset object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't add asset: " + asset->name() + " to project: " + project->name() + ", DbSession object is null");

    try
    {
        if(!projectHasAsset(project, asset))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Projects::Project>(project)->_assets.insert(asset);
                return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while trying to add asset: " + asset->name() + " to project: " + project->name() << std::endl;
    }

    return false;
}

bool Projects::ProjectsManager::removeAssetFromProject(Wt::Dbo::ptr<Projects::Project> project, Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    if(!project)
        throw std::invalid_argument("Can't remove asset from project!, project object is null");

    if(!asset)
        throw std::invalid_argument("Can't remove asset from project: " + project->name() + ", asset object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't remove asset: " + asset->name() + " from project: " + project->name() + ", DbSession object is null");

    try
    {
        if(projectHasAsset(project, asset))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Projects::Project>(project)->_assets.erase(asset);

                return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while trying to remove asset: " + asset->name() + " from project: " + project->name() << std::endl;
    }

    return false;
}

bool Projects::ProjectsManager::projectHasAsset(Wt::Dbo::ptr<Projects::Project> project, Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    if(!project)
        throw std::invalid_argument("Can't check asset existence in project!, project object is null");

    if(!asset)
        throw std::invalid_argument("Can't check asset existence in project: " + project->name() + ", asset object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't check asset: " + asset->name() + " existence in project: " + project->name() + ", DbSession object is null");

    try
    {
        if(Database::DatabaseManager::instance().openTransaction())
        {
            for(auto iter = project->_assets.begin(); iter != project->_assets.end(); ++iter)
            {
                if((*iter).id() == asset.id())
                    return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while enumerating through project: " + project->name() + " assets"<< std::endl;
    }

    return false;
}

//PROJECT_ASSET
Wt::Dbo::ptr<Projects::ProjectAsset> Projects::ProjectsManager::getAsset(const std::string &projectName, const std::string &assetName)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't get asset: " + projectName + ", DbSession object is null");

    Wt::Dbo::ptr<Projects::ProjectAsset> assetPtr;
    if(Database::DatabaseManager::instance().openTransaction())
    {
        assetPtr = Database::DatabaseManager::instance().session()->find<Projects::ProjectAsset>().where("Asset_Project_Project_Name = ? AND Asset_Name = ?").bind(projectName).bind(assetName);

        Database::DatabaseManager::instance().commitTransaction();
    }
    return assetPtr;
}

bool Projects::ProjectsManager::assetExist(const std::string &projectName, const std::string &assetName)
{
    return getAsset(projectName, assetName).get() != nullptr;
}

bool Projects::ProjectsManager::addTaskToAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset, Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!asset)
        throw std::invalid_argument("Can't add task to asset!, asset object is null");

    if(!task)
        throw std::invalid_argument("Can't add task to asset: " + asset->name() + ", task object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't add task to project: " + asset->name() + ", DbSession object is null");

    try
    {
        if(!assetHasTask(asset, task))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Projects::ProjectAsset>(asset)->_tasks.insert(task);
                return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while trying to add task to asset: " + asset->name() << std::endl;
    }

    return false;
}

bool Projects::ProjectsManager::removeTaskFromAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset, Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!asset)
        throw std::invalid_argument("Can't remove task from asset!, asset object is null");

    if(!task)
        throw std::invalid_argument("Can't remove task from asset: " + asset->name() + ", task object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't remove task from asset: " + asset->name() + ", DbSession object is null");

    try
    {
        if(assetHasTask(asset, task))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Projects::ProjectAsset>(asset)->_tasks.erase(task);
                return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while trying to remove task from asset: " + asset->name() << std::endl;
    }

    return false;
}

bool Projects::ProjectsManager::assetHasTask(Wt::Dbo::ptr<Projects::ProjectAsset> asset, Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!asset)
        throw std::invalid_argument("Can't check task existence in asset!, asset object is null");

    if(!task)
        throw std::invalid_argument("Can't check asset existence in asset: " + asset->name() + ", asset object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't check task existence in asset: " + asset->name() + ", DbSession object is null");

    try
    {
        if(Database::DatabaseManager::instance().openTransaction())
        {
            for(auto iter = asset->_tasks.begin(); iter != asset->_tasks.end(); ++iter)
            {
                if((*iter).id() == task.id())
                    return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while enumerating through asset: " + asset->name() + " tasks"<< std::endl;
    }

    return false;
}

//PROJECT_SEQUENCE
Wt::Dbo::ptr<Projects::ProjectSequence> Projects::ProjectsManager::getSequence(const std::string &projectName, const std::string &sequenceName)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't get sequence: " + sequenceName + ", DbSession object is null");

    Wt::Dbo::ptr<Projects::ProjectSequence> seqPtr;
    if(Database::DatabaseManager::instance().openTransaction())
    {
        seqPtr = Database::DatabaseManager::instance().session()->find<Projects::ProjectSequence>().where("Sequence_Project_Project_Name AND Sequence_Name = ?").bind(projectName).bind(sequenceName);

        Database::DatabaseManager::instance().commitTransaction();
    }
    return seqPtr;
}

bool Projects::ProjectsManager::sequenceExist(const std::string &projectName, const std::string &sequenceName)
{
    return getSequence(projectName, sequenceName).get() != nullptr;
}

bool Projects::ProjectsManager::addShotToSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence, Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    if(!sequence)
        throw std::invalid_argument("Can't add shot to sequence!, sequence object is null");

    if(!shot)
        throw std::invalid_argument("Can't add shot to sequence: " + sequence->name() + ", shot object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't add shot: " + shot->name() + " to sequence: " + sequence->name() + ", DbSession object is null");

    try
    {
        if(!sequenceHasShot(sequence, shot))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Projects::ProjectSequence>(sequence)->_shots.insert(shot);
                return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while trying to add shot: " + shot->name() + " to sequence: " + sequence->name() << std::endl;
    }

    return false;
}

bool Projects::ProjectsManager::removeShotFromSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence, Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    if(!sequence)
        throw std::invalid_argument("Can't remove shot from sequence!, sequence object is null");

    if(!shot)
        throw std::invalid_argument("Can't remove shot from sequence: " + sequence->name() + ", shot object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't remove shot: " + shot->name() + " from sequence: " + sequence->name() + ", DbSession object is null");

    try
    {
        if(sequenceHasShot(sequence, shot))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Projects::ProjectSequence>(sequence)->_shots.erase(shot);
                return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while trying to remove shot: " + shot->name() + " from sequence: " + sequence->name() << std::endl;
    }

    return false;
}

bool Projects::ProjectsManager::sequenceHasShot(Wt::Dbo::ptr<Projects::ProjectSequence> sequence, Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    if(!sequence)
        throw std::invalid_argument("Can't check shot existence in sequence!, sequence object is null");

    if(!shot)
        throw std::invalid_argument("Can't check shot existence in sequence: " + sequence->name() + ", shot object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't check shot: " + shot->name() + " existence in sequence: " + sequence->name() + ", DbSession object is null");

    try
    {
        if(Database::DatabaseManager::instance().openTransaction())
        {
            for(auto iter = sequence->_shots.begin(); iter != sequence->_shots.end(); ++iter)
            {
                if((*iter).id() == shot.id())
                    return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while enumerating through sequence: " + sequence->name() + " shots" << std::endl;
    }

    return false;
}

//PROJECT_SHOT
Wt::Dbo::ptr<Projects::ProjectShot> Projects::ProjectsManager::getShot(const std::string &projectName, const std::string &sequenceName, const std::string &shotName)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't get shot: " + shotName + ", DbSession object is null");

    Wt::Dbo::ptr<Projects::ProjectShot> shotPtr;
    if(Database::DatabaseManager::instance().openTransaction())
    {
        shotPtr = Database::DatabaseManager::instance().session()->find<Projects::ProjectShot>().where(
                    "Shot_Sequence_Sequence_Project_Project_Name = ? AND Shot_Sequence_Sequence_Name = ? AND Shot_Name = ?").bind(projectName).bind(sequenceName).bind(shotName);

        Database::DatabaseManager::instance().commitTransaction();
    }
    return shotPtr;
}

bool Projects::ProjectsManager::shotExist(const std::string &projectName, const std::string &sequenceName, const std::string &shotName)
{
    return getShot(projectName, sequenceName, shotName).get() != nullptr;
}

bool Projects::ProjectsManager::addTaskToShot(Wt::Dbo::ptr<Projects::ProjectShot> shot, Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!shot)
        throw std::invalid_argument("Can't add task to shot!, shot object is null");

    if(!task)
        throw std::invalid_argument("Can't add task to shot: " + shot->name() + ", task object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't add task to shot: " + shot->name() + ", DbSession object is null");

    try
    {
        if(!shotHasTask(shot, task))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Projects::ProjectShot>(shot)->_tasks.insert(task);
                return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while trying to add task to shot: " + shot->name() << std::endl;
    }

    return false;
}

bool Projects::ProjectsManager::removeTaskFromShot(Wt::Dbo::ptr<Projects::ProjectShot> shot, Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!shot)
        throw std::invalid_argument("Can't remove task from shot!, shot object is null");

    if(!task)
        throw std::invalid_argument("Can't remove task from shot: " + shot->name() + ", task object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't remove task from shot: " + shot->name() + ", DbSession object is null");

    try
    {
        if(shotHasTask(shot, task))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<Projects::ProjectShot>(shot)->_tasks.erase(task);
                return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while trying to remove task from shot: " + shot->name() << std::endl;
    }

    return false;
}

bool Projects::ProjectsManager::shotHasTask(Wt::Dbo::ptr<Projects::ProjectShot> shot, Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(!shot)
        throw std::invalid_argument("Can't check task existence in shot!, shot object is null");

    if(!task)
        throw std::invalid_argument("Can't check task existence in shot: " + shot->name() + ", task object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't check task existence in shot: " + shot->name() + ", DbSession object is null");

    try
    {
        if(Database::DatabaseManager::instance().openTransaction())
        {
            for(auto iter = shot->_tasks.begin(); iter != shot->_tasks.end(); ++iter)
            {
                if((*iter).id() == task.id())
                    return true;
            }
        }
    }
    catch(...)
    {
        std::cout << "Error occured while enumerating through shot: " + shot->name() + " tasks" << std::endl;
    }

    return false;
}

//PROJECT_TASK
Wt::Dbo::ptr<Projects::ProjectTask> Projects::ProjectsManager::getShotTask(
        const std::string &projectName, const std::string &sequenceName, const std::string &shotName, Wt::Dbo::dbo_traits<Projects::ProjectTask>::IdType id)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't get shot task, DbSession object is null");

    Wt::Dbo::ptr<Projects::ProjectTask> taskPtr;
    if(Database::DatabaseManager::instance().openTransaction())
    {
        taskPtr = Database::DatabaseManager::instance().session()->find<Projects::ProjectTask>().where(
                    "Task_Shot_Shot_Sequence_Sequence_Project_Project_Name = ? AND Task_Shot_Shot_Sequence_Sequence_Name = ? AND Task_Shot_Shot_Name = ? AND id = ?").bind(
                    projectName).bind(sequenceName).bind(shotName).bind(id);


        Database::DatabaseManager::instance().commitTransaction();
    }
    return taskPtr;
}

Wt::Dbo::ptr<Projects::ProjectTask> Projects::ProjectsManager::getAssetTask(const std::string &projectName, const std::string &assetName, Wt::Dbo::dbo_default_traits::IdType id)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't get shot task, DbSession object is null");

    Wt::Dbo::ptr<Projects::ProjectTask> taskPtr;
    if(Database::DatabaseManager::instance().openTransaction())
    {
        taskPtr = Database::DatabaseManager::instance().session()->find<Projects::ProjectTask>().where(
                    "Task_Asset_Asset_Project_Project_Name = ? AND Task_Asset_Asset_Name = ? AND id = ?").bind(projectName).bind(assetName).bind(id);

        Database::DatabaseManager::instance().commitTransaction();
    }
    return taskPtr;
}

bool Projects::ProjectsManager::shotTaskExist(const std::string &projectName, const std::string &sequenceName, const std::string &shotName, u_int64_t id)
{
    return getShotTask(projectName, sequenceName, shotName, id).get() != nullptr;
}

bool Projects::ProjectsManager::assetTaskExist(const std::string &projectName, const std::string &assetName, Wt::Dbo::dbo_default_traits::IdType id)
{
    return getAssetTask(projectName, assetName, id).get() != nullptr;
}

//PROJECT_WORK_STATUS_TYPE
Wt::Dbo::ptr<Projects::ProjectWorkStatusType> Projects::ProjectsManager::getProjectWorkStatusType(const std::string &name)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't get ProjectWorkStatusType, DbSession object is null");

    Wt::Dbo::ptr<Projects::ProjectWorkStatusType> typePtr;
    if(Database::DatabaseManager::instance().openTransaction())
    {
        typePtr = Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatusType>().where("Work_Status_Type = ?").bind(name);

        Database::DatabaseManager::instance().commitTransaction();
    }
    return typePtr;
}

bool Projects::ProjectsManager::projectWorkStatusTypeExist(const std::string &name)
{
    return getProjectWorkStatusType(name).get() != nullptr;
}

//PROJECT_ASSET_TYPE
Wt::Dbo::ptr<Projects::ProjectAssetType> Projects::ProjectsManager::getProjectAssetType(const std::string &name)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't get ProjectAssetType, DbSession object is null");

    Wt::Dbo::ptr<Projects::ProjectAssetType> typePtr;
    if(Database::DatabaseManager::instance().openTransaction())
    {
        typePtr = Database::DatabaseManager::instance().session()->find<Projects::ProjectAssetType>().where("Type = ?").bind(name);

        Database::DatabaseManager::instance().commitTransaction();
    }
    return typePtr;
}

bool Projects::ProjectsManager::projectAssetTypeExist(const std::string &name)
{
    return getProjectAssetType(name).get() != nullptr;
}

//PROJECT_WORK_STATUS
Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectsManager::getProjectWorkStatus(const std::string &status)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't get ProjectWorkStatus, DbSession object is null");

    Wt::Dbo::ptr<Projects::ProjectWorkStatus> statusPtr;
    if(Database::DatabaseManager::instance().openTransaction())
    {
        statusPtr = Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("Status = ?").bind(status);

        Database::DatabaseManager::instance().commitTransaction();
    }
    return statusPtr;
}

bool Projects::ProjectsManager::projectWorkStatusExist(const std::string &status)
{
    return getProjectWorkStatus(status).get() != nullptr;
}

//PROJECT_TASK_TYPE
Wt::Dbo::ptr<Projects::ProjectTaskType> Projects::ProjectsManager::getProjectTaskType(const std::string &name)
{
    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MDboSessionIsNullException("Can't get ProjectTaskType, DbSession object is null");

    Wt::Dbo::ptr<Projects::ProjectTaskType> typePtr;
    if(Database::DatabaseManager::instance().openTransaction())
    {
        typePtr = Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskType>().where("Type = ?").bind(name);

        Database::DatabaseManager::instance().commitTransaction();
    }
    return typePtr;
}

bool Projects::ProjectsManager::projectTaskExist(const std::string &name)
{
    return getProjectTaskType(name).get() != nullptr;
}

//GENERAL
void Projects::ProjectsManager::addDefaults()
{
    _addDefaultProjectWorkStatusTypes();
    _addDefaultProjectWorkStatus();
    _addDefaultProjectAssetTypes();
    _addDefaultProjectTaskTypes();
}

void Projects::ProjectsManager::_addDefaultProjectWorkStatusTypes()
{
    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatusType>(new Projects::ProjectWorkStatusType("Blocked"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatusType>(new Projects::ProjectWorkStatusType("Not Started"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatusType>(new Projects::ProjectWorkStatusType("In Progress"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatusType>(new Projects::ProjectWorkStatusType("Done"));
}

void Projects::ProjectsManager::_addDefaultProjectWorkStatus()
{
    Wt::Dbo::ptr<Projects::ProjectWorkStatusType> pwsTypeNotStartedPtr = getProjectWorkStatusType("Not Started");
    Wt::Dbo::ptr<Projects::ProjectWorkStatusType> pwsTypeBlockedPtr = getProjectWorkStatusType("Blocked");
    Wt::Dbo::ptr<Projects::ProjectWorkStatusType> pwsTypeInProgressPtr = getProjectWorkStatusType("In Progress");
    Wt::Dbo::ptr<Projects::ProjectWorkStatusType> pwsTypeDonePtr = getProjectWorkStatusType("Done");

    Wt::Dbo::ptr<Projects::ProjectWorkStatus> pwsPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(new Projects::ProjectWorkStatus("Not Started"));
    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsPtr)->setWorkStatusType(pwsTypeNotStartedPtr);

    pwsPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(new Projects::ProjectWorkStatus("Normal"));
    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsPtr)->setWorkStatusType(pwsTypeInProgressPtr);

    pwsPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(new Projects::ProjectWorkStatus("In Progress"));
    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsPtr)->setWorkStatusType(pwsTypeInProgressPtr);

    pwsPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(new Projects::ProjectWorkStatus("WIP"));
    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsPtr)->setWorkStatusType(pwsTypeNotStartedPtr);

    pwsPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(new Projects::ProjectWorkStatus("Pending Review"));
    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsPtr)->setWorkStatusType(pwsTypeInProgressPtr);

    pwsPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(new Projects::ProjectWorkStatus("Approved"));
    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsPtr)->setWorkStatusType(pwsTypeDonePtr);

    pwsPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(new Projects::ProjectWorkStatus("Proposed Final"));
    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsPtr)->setWorkStatusType(pwsTypeInProgressPtr);

    pwsPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(new Projects::ProjectWorkStatus("Client Approved"));
    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsPtr)->setWorkStatusType(pwsTypeDonePtr);

    pwsPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(new Projects::ProjectWorkStatus("On Hold"));
    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsPtr)->setWorkStatusType(pwsTypeBlockedPtr);

    pwsPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(new Projects::ProjectWorkStatus("Omitted"));
    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsPtr)->setWorkStatusType(pwsTypeBlockedPtr);
}

void Projects::ProjectsManager::_addDefaultProjectAssetTypes()
{
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("3D-Model"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("3D-Rig"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("3D-Animation"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("3D-Simulation"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Maya-Scene"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("3DSmax-Scene"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("blender-Scene"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Houdini-Scene"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Renderman-Shader"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Arnold-Shader"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("OSL-Shader"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Vray-Shader"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Mentalray-Shader"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Maya-Mel-Script"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Maya-Python-Script"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("3DSMax-Script"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("blender-Script"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Mari-Scene"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Realflow-Scene"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Nuke-Composition"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Nuke-Script"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Hiero-Scene"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("After Effects-Composition"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("After Effects-Script"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Photoshop File"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Gimp File"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Krita File"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Word Document"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("PDF Document"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(new Projects::ProjectAssetType("Other"));
}

void Projects::ProjectsManager::_addDefaultProjectTaskTypes()
{
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Generic"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Modeling"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Animation"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Lookdev"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Composting"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Editing"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Lighting"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Roroscoping"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Deliverable"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Environment"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Previz"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Tracking"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Rigging"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Skinning"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Hair"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("FX"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Character"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Production"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Flame"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Smoke"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Color Correction"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Stereoscopic Conversion"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Coding"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Database Design"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Reverse Engeneering"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Debugging"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("UI Design"));
    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(new Projects::ProjectTaskType("Web Design"));
}
