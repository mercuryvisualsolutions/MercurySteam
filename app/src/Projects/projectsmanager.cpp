#include "projectsmanager.h"
#include "projectsio.h"
#include "../Log/logmanager.h"
#include "../Database/databasemanager.h"

#include <Wt/WApplication>

Projects::ProjectsManager::ProjectsManager()
{
}

void Projects::ProjectsManager::initSessionLogger()
{
    _logger = Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId());
}

void Projects::ProjectsManager::addDefaults()
{
    _addDefaultProjectWorkStatusTypes();
    _addDefaultProjectWorkStatus();
    _addDefaultProjectAssetTypes();
    _addDefaultProjectTaskTypes();
}

void Projects::ProjectsManager::_addDefaultProjectWorkStatusTypes()
{
    std::vector<Projects::ProjectWorkStatusType*> vec;

    vec.push_back(new Projects::ProjectWorkStatusType("Blocked"));
    vec.push_back(new Projects::ProjectWorkStatusType("Not Started"));
    vec.push_back(new Projects::ProjectWorkStatusType("In Progress"));
    vec.push_back(new Projects::ProjectWorkStatusType("Done"));

    try
    {
        for(std::vector<Projects::ProjectWorkStatusType*>::size_type i = 0; i < vec.size(); ++i)
        {
            if(!Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatusType>(vec.at(i)))
            {
                delete vec.at(i);
            }
        }
    }
    catch(Wt::Dbo::Exception ex)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to add default work status ") + ex.what() , Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log("Error occured while trying to add default work status", Ms::Log::LogMessageType::Error);
    }
}

void Projects::ProjectsManager::_addDefaultProjectWorkStatus()
{
    try
    {
        Wt::Dbo::ptr<Projects::ProjectWorkStatusType> pwsTypeNotStartedPtr = Database::DatabaseManager::instance().getDbo<Projects::ProjectWorkStatusType>("Not Started");
        Wt::Dbo::ptr<Projects::ProjectWorkStatusType> pwsTypeBlockedPtr = Database::DatabaseManager::instance().getDbo<Projects::ProjectWorkStatusType>("Blocked");
        Wt::Dbo::ptr<Projects::ProjectWorkStatusType> pwsTypeInProgressPtr = Database::DatabaseManager::instance().getDbo<Projects::ProjectWorkStatusType>("In Progress");
        Wt::Dbo::ptr<Projects::ProjectWorkStatusType> pwsTypeDonePtr = Database::DatabaseManager::instance().getDbo<Projects::ProjectWorkStatusType>("Done");

        Projects::ProjectWorkStatus *pwsNS = new Projects::ProjectWorkStatus("Not Started");
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> pwsNSPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(pwsNS);
        if(pwsNSPtr.get())
            Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsNSPtr)->setWorkStatusType(pwsTypeNotStartedPtr);
        else
            delete pwsNS;

        Projects::ProjectWorkStatus *pwsN = new Projects::ProjectWorkStatus("Normal");
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> pwsNPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(pwsN);
        if(pwsNPtr.get())
            Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsNPtr)->setWorkStatusType(pwsTypeInProgressPtr);
        else
            delete pwsN;

        Projects::ProjectWorkStatus *pwsIP = new Projects::ProjectWorkStatus("In Progress");
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> pwsIPPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(pwsIP);
        if(pwsIPPtr.get())
            Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsIPPtr)->setWorkStatusType(pwsTypeInProgressPtr);
        else
            delete pwsIP;

        Projects::ProjectWorkStatus *pwsWIP = new Projects::ProjectWorkStatus("WIP");
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> pwsWIPPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(pwsWIP);
        if(pwsWIPPtr.get())
            Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsWIPPtr)->setWorkStatusType(pwsTypeNotStartedPtr);
        else
            delete pwsWIP;

        Projects::ProjectWorkStatus *pwsPP = new Projects::ProjectWorkStatus("Pending Review");
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> pwsPPPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(pwsPP);
        if(pwsPPPtr.get())
            Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsPPPtr)->setWorkStatusType(pwsTypeInProgressPtr);
        else
            delete pwsPP;

        Projects::ProjectWorkStatus *pwsAP = new Projects::ProjectWorkStatus("Approved");
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> pwsAPPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(pwsAP);
        if(pwsAPPtr.get())
            Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsAPPtr)->setWorkStatusType(pwsTypeDonePtr);
        else
            delete pwsAP;

        Projects::ProjectWorkStatus *pwsPF = new Projects::ProjectWorkStatus("Proposed Final");
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> pwsPFPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(pwsPF);
        if(pwsPFPtr.get())
            Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsPFPtr)->setWorkStatusType(pwsTypeInProgressPtr);
        else
            delete pwsPF;

        Projects::ProjectWorkStatus *pwsCA = new Projects::ProjectWorkStatus("Client Approved");
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> pwsCAPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(pwsCA);
        if(pwsCAPtr.get())
            Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsCAPtr)->setWorkStatusType(pwsTypeDonePtr);
        else
            delete pwsCA;

        Projects::ProjectWorkStatus *pwsOH = new Projects::ProjectWorkStatus("On Hold");
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> pwsOHPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(pwsOH);
        if(pwsOHPtr.get())
            Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsOHPtr)->setWorkStatusType(pwsTypeBlockedPtr);
        else
            delete pwsOH;

        Projects::ProjectWorkStatus *pwsOM = new Projects::ProjectWorkStatus("Omitted");
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> pwsOMPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectWorkStatus>(pwsOM);
        if(pwsOMPtr.get())
            Database::DatabaseManager::instance().modifyDbo<Projects::ProjectWorkStatus>(pwsOMPtr)->setWorkStatusType(pwsTypeBlockedPtr);
        else
            delete pwsOM;
    }
    catch(Wt::Dbo::Exception ex)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to add default work status ") + ex.what() , Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log("Error occured while trying to add default work status", Ms::Log::LogMessageType::Error);
    }
}

void Projects::ProjectsManager::_addDefaultProjectAssetTypes()
{
    std::vector<Projects::ProjectAssetType*> vec;

    vec.push_back(new Projects::ProjectAssetType("3D-Model"));
    vec.push_back(new Projects::ProjectAssetType("3D-Rig"));
    vec.push_back(new Projects::ProjectAssetType("3D-Animation"));
    vec.push_back(new Projects::ProjectAssetType("3D-Simulation"));
    vec.push_back(new Projects::ProjectAssetType("Maya-Scene"));
    vec.push_back(new Projects::ProjectAssetType("3DSmax-Scene"));
    vec.push_back(new Projects::ProjectAssetType("blender-Scene"));
    vec.push_back(new Projects::ProjectAssetType("Houdini-Scene"));
    vec.push_back(new Projects::ProjectAssetType("Renderman-Shader"));
    vec.push_back(new Projects::ProjectAssetType("Arnold-Shader"));
    vec.push_back(new Projects::ProjectAssetType("OSL-Shader"));
    vec.push_back(new Projects::ProjectAssetType("Vray-Shader"));
    vec.push_back(new Projects::ProjectAssetType("Mentalray-Shader"));
    vec.push_back(new Projects::ProjectAssetType("Maya-Mel-Script"));
    vec.push_back(new Projects::ProjectAssetType("Maya-Python-Script"));
    vec.push_back(new Projects::ProjectAssetType("3DSMax-Script"));
    vec.push_back(new Projects::ProjectAssetType("blender-Script"));
    vec.push_back(new Projects::ProjectAssetType("Mari-Scene"));
    vec.push_back(new Projects::ProjectAssetType("Realflow-Scene"));
    vec.push_back(new Projects::ProjectAssetType("Nuke-Composition"));
    vec.push_back(new Projects::ProjectAssetType("Nuke-Script"));
    vec.push_back(new Projects::ProjectAssetType("Hiero-Scene"));
    vec.push_back(new Projects::ProjectAssetType("After Effects-Composition"));
    vec.push_back(new Projects::ProjectAssetType("After Effects-Script"));
    vec.push_back(new Projects::ProjectAssetType("Photoshop File"));
    vec.push_back(new Projects::ProjectAssetType("Gimp File"));
    vec.push_back(new Projects::ProjectAssetType("Krita File"));
    vec.push_back(new Projects::ProjectAssetType("Word Document"));
    vec.push_back(new Projects::ProjectAssetType("PDF Document"));
    vec.push_back(new Projects::ProjectAssetType("Other"));

    try
    {
        for(std::vector<Projects::ProjectAssetType*>::size_type i = 0; i < vec.size(); ++i)
        {
            if(!Database::DatabaseManager::instance().createDbo<Projects::ProjectAssetType>(vec.at(i)))
            {
                delete vec.at(i);
            }
        }
    }
    catch(Wt::Dbo::Exception ex)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to add default asset types ") + ex.what() , Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log("Error occured while trying to add default asset types", Ms::Log::LogMessageType::Error);
    }
}

void Projects::ProjectsManager::_addDefaultProjectTaskTypes()
{
    std::vector<Projects::ProjectTaskType*> vec;

    vec.push_back(new Projects::ProjectTaskType("Generic"));
    vec.push_back(new Projects::ProjectTaskType("Modeling"));
    vec.push_back(new Projects::ProjectTaskType("Animation"));
    vec.push_back(new Projects::ProjectTaskType("Lookdev"));
    vec.push_back(new Projects::ProjectTaskType("Composting"));
    vec.push_back(new Projects::ProjectTaskType("Editing"));
    vec.push_back(new Projects::ProjectTaskType("Lighting"));
    vec.push_back(new Projects::ProjectTaskType("Roroscoping"));
    vec.push_back(new Projects::ProjectTaskType("Deliverable"));
    vec.push_back(new Projects::ProjectTaskType("Environment"));
    vec.push_back(new Projects::ProjectTaskType("Previz"));
    vec.push_back(new Projects::ProjectTaskType("Tracking"));
    vec.push_back(new Projects::ProjectTaskType("Rigging"));
    vec.push_back(new Projects::ProjectTaskType("Skinning"));
    vec.push_back(new Projects::ProjectTaskType("Hair"));
    vec.push_back(new Projects::ProjectTaskType("FX"));
    vec.push_back(new Projects::ProjectTaskType("Character"));
    vec.push_back(new Projects::ProjectTaskType("Production"));
    vec.push_back(new Projects::ProjectTaskType("Flame"));
    vec.push_back(new Projects::ProjectTaskType("Smoke"));
    vec.push_back(new Projects::ProjectTaskType("Color Correction"));
    vec.push_back(new Projects::ProjectTaskType("Stereoscopic Conversion"));
    vec.push_back(new Projects::ProjectTaskType("Coding"));
    vec.push_back(new Projects::ProjectTaskType("Database Design"));
    vec.push_back(new Projects::ProjectTaskType("Reverse Engeneering"));
    vec.push_back(new Projects::ProjectTaskType("Debugging"));
    vec.push_back(new Projects::ProjectTaskType("UI Design"));
    vec.push_back(new Projects::ProjectTaskType("Web Design"));

    try
    {
        for(std::vector<Projects::ProjectTaskType*>::size_type i = 0; i < vec.size(); ++i)
        {
            if(!Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskType>(vec.at(i)))
            {
                delete vec.at(i);
            }
        }
    }
    catch(Wt::Dbo::Exception ex)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to add default task types ") + ex.what() , Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log("Error occured while trying to add default task types", Ms::Log::LogMessageType::Error);
    }
}
