#include "viewprojects.h"

#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"
#include "../../Projects/projectsmanager.h"
#include "../../Projects/projectsio.h"
#include "Users/usersmanager.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Log/logmanager.h"
#include "../../Auth/authmanager.h"
#include "../Dialogs/dlgcreatedbodata.h"
#include "../Dialogs/dlgcreatenote.h"

#include <Ms/Widgets/MTableViewColumn.h>
#include <Ms/Widgets/Delegates/MDelegates>

#include <iostream>
#include <stdio.h>

#include <Wt/Dbo/Query>
#include <Wt/WMessageBox>
#include <Wt/WApplication>
#include <Wt/WEnvironment>

Views::ViewProjects::ViewProjects()
: WContainerWidget()
{
    _logger = Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId());
    _propertiesPanel = Session::SessionManager::instance().getSessionPropertiesPanel(Wt::WApplication::instance()->sessionId());

    _prepareView();

    _mnuMain->select(_mnuMainProjectsItem);

    updateProjectsView();
    _stkMain->setCurrentWidget(_cntProjects);
}

void Views::ViewProjects::updateView()
{
    if(_mnuMain->currentItem() == _mnuMainProjectsItem)
        updateProjectsView();
    else if(_mnuMain->currentItem() == _mnuMainSequencesItem)
        updateSequencesView();
    else if(_mnuMain->currentItem() == _mnuMainShotsItem)
        updateShotsView();
    else if(_mnuMain->currentItem() == _mnuMainAssetsItem)
        updateAssetsView();
    else if(_mnuMain->currentItem() == _mnuMainTasksItem)
        updateTasksView();
}

void Views::ViewProjects::updateProjectsView()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::Project>> query;

        int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

        if(AppSettings::instance().isLoadInactiveData())
            query = Database::DatabaseManager::instance().session()->find<Projects::Project>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Database::DatabaseManager::instance().session()->find<Projects::Project>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        _qtvProjects->setQuery(query);

        bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Auth::AuthManager::instance().currentUser()->editRank();

        _qtvProjects->clearColumns();

        //add columns
        _qtvProjects->addColumn(Ms::Widgets::MTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        _qtvProjects->addColumn(Ms::Widgets::MTableViewColumn("Project_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvProjects->addColumn(Ms::Widgets::MTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MTableViewColumn("End_Date", "End Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MTableViewColumn("Duration_In_Frames", "Duration In Frames", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MTableViewColumn("FPS", "FPS", flags, new Ms::Widgets::Delegates::MFloatFieldDelegate(editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MTableViewColumn("Frame_Width", "Frame Width", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MTableViewColumn("Frame_Height", "Frame Height", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true),
         "Status", editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MTableViewColumn("Project_Manager_Name", "Manager", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Users::User>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Users::User>() :
         Database::DatabaseManager::instance().session()->find<Users::User>().where("Active = ?").bind(true),
         "Name", editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));

        if(AppSettings::instance().isShowExtraColumns())
            _addExtraColumns<Projects::Project>(_qtvProjects, flags, editRank);

        _qtvProjects->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating table view" << std::endl;
    }
}

void Views::ViewProjects::updateSequencesView()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectSequence>> query = Database::DatabaseManager::instance().session()->find<Projects::ProjectSequence>();

        if(_qtvProjects->table()->selectedIndexes().size() > 0)//if there is a project or more selected
        {
            std::vector<std::string> selProjects;
            std::set<Wt::WModelIndex>::size_type i = 0;
            std::string queryParam = "";
            //get the name of the selected projects
            for(auto prjPtr : _qtvProjects->selectedItems())
            {
                selProjects.push_back(prjPtr->name());
                queryParam += "?";
                if(++i < _qtvProjects->table()->selectedIndexes().size())
                    queryParam += ",";
            }

            //generate the where clause
            query.where("Sequence_Project_Project_Name IN (" + queryParam + ")");

            //bind the selected projects
            for(std::vector<std::string>::size_type i = 0; i < selProjects.size(); ++i)
                query.bind(selProjects.at(i));

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("Sequence_Name = ?").bind("");//clear the view

        _qtvSequences->setQuery(query);

        bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Auth::AuthManager::instance().currentUser()->editRank();

        _qtvSequences->clearColumns();

        //add columns
        _qtvSequences->addColumn(Ms::Widgets::MTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        _qtvSequences->addColumn(Ms::Widgets::MTableViewColumn("Sequence_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvSequences->addColumn(Ms::Widgets::MTableViewColumn("Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvSequences->addColumn(Ms::Widgets::MTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MTableViewColumn("End_Date", "End Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MTableViewColumn("Duration_In_Frames", "Duration_In_Frames", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MTableViewColumn("FPS", "FPS", flags, new Ms::Widgets::Delegates::MFloatFieldDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MTableViewColumn("Frame_Width", "Frame Width", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MTableViewColumn("Frame_Height", "Frame Height", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true),
         "Status", editRank)));
        _qtvSequences->addColumn(Ms::Widgets::MTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));


        if(AppSettings::instance().isShowExtraColumns())
            _addExtraColumns<Projects::ProjectSequence>(_qtvSequences, flags, editRank);

        _qtvSequences->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating sequences view" << std::endl;
    }
}

void Views::ViewProjects::updateShotsView()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectShot>> query = Database::DatabaseManager::instance().session()->find<Projects::ProjectShot>();

        if(_qtvProjects->table()->selectedIndexes().size() > 0)//if there is a project or more selected
        {
            std::vector<std::string> projects;
            std::vector<std::string> sequences;
            std::set<Wt::WModelIndex>::size_type i = 0;
            std::string prjQueryParam = "";
            std::string seqQueryParam = "";

            //get the name of the selected projects
            for(const Wt::WModelIndex &index : _qtvProjects->table()->selectedIndexes())
            {
                projects.push_back(_qtvProjects->model()->resultRow(_qtvProjects->proxyModel()->mapToSource(index).row())->name());
                prjQueryParam += "?";
                if(++i < _qtvProjects->table()->selectedIndexes().size())
                    prjQueryParam += ",";
            }

            i = 0;

            //get the name of the selected sequences
            for(const Wt::WModelIndex &index : _qtvSequences->table()->selectedIndexes())
            {
                sequences.push_back(_qtvSequences->model()->resultRow(_qtvSequences->proxyModel()->mapToSource(index).row())->name());
                seqQueryParam += "?";
                if(++i < _qtvSequences->table()->selectedIndexes().size())
                    seqQueryParam += ",";
            }

            //generate a where clause for projects
            query.where("Shot_Sequence_Sequence_Project_Project_Name IN (" + prjQueryParam + ")");
            //bind the selected projects
            for(std::vector<std::string>::size_type i = 0; i < projects.size(); ++i)
                query.bind(projects.at(i));

            if(_qtvSequences->table()->selectedIndexes().size() > 0)
            {
                //generate a where clause for selected sequences
                query.where("Shot_Sequence_Sequence_Name IN (" + seqQueryParam + ")");
                //bind the selected sequences
                for(std::vector<std::string>::size_type i = 0; i < sequences.size(); ++i)
                    query.bind(sequences.at(i));
            }

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("Shot_Name = ?").bind("");//clear the view

        _qtvShots->setQuery(query);

        bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Auth::AuthManager::instance().currentUser()->editRank();

        _qtvShots->clearColumns();

        //add columns
        _qtvShots->addColumn(Ms::Widgets::MTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        _qtvShots->addColumn(Ms::Widgets::MTableViewColumn("Shot_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvShots->addColumn(Ms::Widgets::MTableViewColumn("Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvShots->addColumn(Ms::Widgets::MTableViewColumn("Shot_Sequence_Sequence_Project_Project_Name", "Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvShots->addColumn(Ms::Widgets::MTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvShots->addColumn(Ms::Widgets::MTableViewColumn("End_Date", "End Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvShots->addColumn(Ms::Widgets::MTableViewColumn("Duration_In_Frames", "Duration_In_Frames", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvShots->addColumn(Ms::Widgets::MTableViewColumn("FPS", "FPS", flags, new Ms::Widgets::Delegates::MFloatFieldDelegate(editRank)));
        _qtvShots->addColumn(Ms::Widgets::MTableViewColumn("Frame_Width", "Frame Width", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvShots->addColumn(Ms::Widgets::MTableViewColumn("Frame_Height", "Frame Height", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvShots->addColumn(Ms::Widgets::MTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true),
         "Status", editRank)));
        _qtvShots->addColumn(Ms::Widgets::MTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));

        if(AppSettings::instance().isShowExtraColumns())
            _addExtraColumns<Projects::ProjectShot>(_qtvShots, flags, editRank);

        _qtvShots->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating shots view" << std::endl;
    }
}

void Views::ViewProjects::updateAssetsView()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectAsset>> query = Database::DatabaseManager::instance().session()->find<Projects::ProjectAsset>();

        if(_qtvProjects->table()->selectedIndexes().size() > 0)//if there is a project or more selected
        {
            std::vector<std::string> selProjects;
            std::set<Wt::WModelIndex>::size_type i = 0;
            std::string queryParam = "";
            //get the name of the selected projects
            for(const Wt::WModelIndex &index : _qtvProjects->table()->selectedIndexes())
            {
                selProjects.push_back(_qtvProjects->model()->resultRow(_qtvProjects->proxyModel()->mapToSource(index).row())->name());
                queryParam += "?";
                if(++i < _qtvProjects->table()->selectedIndexes().size())
                    queryParam += ",";
            }

            //generate the where clause
            query.where("Asset_Project_Project_Name IN (" + queryParam + ")");

            //bind the selected projects
            for(std::vector<std::string>::size_type i = 0; i < selProjects.size(); ++i)
                query.bind(selProjects.at(i));

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("Asset_Name = ?").bind("");//clear the view

        _qtvAssets->setQuery(query);

        bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Auth::AuthManager::instance().currentUser()->editRank();

        _qtvAssets->clearColumns();

        //add columns
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Asset_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Asset_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Asset_Type", "Type", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectAssetType>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectAssetType>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectAssetType>().where("Active = ?").bind(true),
         "Type", editRank)));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("End_Date", "End Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
        Database::DatabaseManager::instance().session(),AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true), "Status", editRank)));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));

        if(AppSettings::instance().isShowExtraColumns())
            _addExtraColumns<Projects::ProjectAsset>(_qtvAssets, flags, editRank);

        _qtvAssets->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating assets view" << std::endl;
    }
}

void Views::ViewProjects::updateTasksView()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;
        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTask>> query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTask>();

        if(_qtvProjects->table()->selectedIndexes().size() > 0)//if there is a project or more selected
        {
            std::vector<std::string> projects;
            std::vector<std::string> sequences;
            std::vector<std::string> shots;
            std::vector<std::string> assets;
            std::set<Wt::WModelIndex>::size_type i = 0;
            std::string prjQueryParam = "";
            std::string seqQueryParam = "";
            std::string shtQueryParam = "";
            std::string astQueryParam = "";
            std::string strWhereQuery = "";

            //get name of selected projects
            for(const Wt::WModelIndex &index : _qtvProjects->table()->selectedIndexes())
            {
                projects.push_back(_qtvProjects->model()->resultRow(_qtvProjects->proxyModel()->mapToSource(index).row())->name());
                prjQueryParam += "?";
                if(++i < _qtvProjects->table()->selectedIndexes().size())
                    prjQueryParam += ",";
            }

            i = 0;

            //get name of selected sequences
            for(const Wt::WModelIndex &index : _qtvSequences->table()->selectedIndexes())
            {
                sequences.push_back(_qtvSequences->model()->resultRow(_qtvSequences->proxyModel()->mapToSource(index).row())->name());
                seqQueryParam += "?";
                if(++i < _qtvSequences->table()->selectedIndexes().size())
                    seqQueryParam += ",";
            }

            i = 0;

            //get name of selected shots
            for(const Wt::WModelIndex &index : _qtvShots->table()->selectedIndexes())
            {
                shots.push_back(_qtvShots->model()->resultRow(_qtvShots->proxyModel()->mapToSource(index).row())->name());
                shtQueryParam += "?";
                if(++i < _qtvShots->table()->selectedIndexes().size())
                    shtQueryParam += ",";
            }

            i = 0;

            //get name of selected assets
            for(const Wt::WModelIndex &index : _qtvAssets->table()->selectedIndexes())
            {
                assets.push_back(_qtvAssets->model()->resultRow(_qtvAssets->proxyModel()->mapToSource(index).row())->name());
                astQueryParam += "?";
                if(++i < _qtvAssets->table()->selectedIndexes().size())
                    astQueryParam += ",";
            }

            strWhereQuery = "(Task_Shot_Shot_Sequence_Sequence_Project_Project_Name IN (" + prjQueryParam + ")" +
                    (_qtvSequences->table()->selectedIndexes().size() > 0 ? " AND Task_Shot_Shot_Sequence_Sequence_Name IN (" + seqQueryParam + ")" : "") +
                    (_qtvShots->table()->selectedIndexes().size() > 0 ? " AND Task_Shot_Shot_Name IN (" + shtQueryParam + ")" : "") +
                    ")" +
                    " OR (Task_Asset_Asset_Project_Project_Name IN (" + prjQueryParam + ")" +
                    (_qtvAssets->table()->selectedIndexes().size() > 0 ? " AND Task_Asset_Asset_Name IN (" + astQueryParam + ")" : "") +
                    ")";

            //generate a where clause
            query.where(strWhereQuery);

            //bind projects for sequences
            for(std::vector<std::string>::size_type i = 0; i < projects.size(); ++i)
                query.bind(projects.at(i));

            //if there are sequences selected bind them
            for(std::vector<std::string>::size_type i = 0; i < sequences.size(); ++i)
                query.bind(sequences.at(i));

            //if there are shots selected bind them
            for(std::vector<std::string>::size_type i = 0; i < shots.size(); ++i)
                query.bind(shots.at(i));

            //bind projects for assets
            for(std::vector<std::string>::size_type i = 0; i < projects.size(); ++i)
                query.bind(projects.at(i));

            //if there are assets selected bind them
            for(std::vector<std::string>::size_type i = 0; i < assets.size(); ++i)
                query.bind(assets.at(i));

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);

        }
        else
            query.where("id = ?").bind(-1);//clear the view

        _qtvTasks->setQuery(query);

        bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Auth::AuthManager::instance().currentUser()->editRank();

        _qtvTasks->clearColumns();

        //add columns
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("id", "ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Type", "Type", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectTaskType>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskType>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskType>().where("Active = ?").bind(true),
         "Type", editRank), true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_User_Name", "Asignee", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Users::User>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Users::User>() :
         Database::DatabaseManager::instance().session()->find<Users::User>().where("Active = ?").bind(true),
         "Name", editRank), true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true),
         "Status", editRank)));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("End_Date", "End Date",flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Shot_Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Asset_Asset_Project_Project_Name", "Asset Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));

        if(AppSettings::instance().isShowExtraColumns())
            _addExtraColumns<Projects::ProjectTask>(_qtvTasks, flags, editRank);

        _qtvTasks->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating tasks view" << std::endl;
    }
}

void Views::ViewProjects::updatePropertiesView()
{
    if(_stkProperties->currentWidget() == _qtvPropertiesData)
    {
        _updatePropertiesDataView();
    }
    else if(_stkProperties->currentWidget() == _cntPropertiesTags)
    {
        _updatePropertiesTagsView();
        _updatePropertiesAssignedTagsView();
    }
    else if(_stkProperties->currentWidget() == _qtvPropertiesNotes)
    {
        _updatePropertiesNotesView();
    }
}

void Views::ViewProjects::showPropertiesView()
{
    _propertiesPanel->showView("Projects");
}

bool Views::ViewProjects::isProjectsViewShown()
{
    return _mnuMain->currentItem() == _mnuMainProjectsItem;
}

bool Views::ViewProjects::isSequencesViewShown()
{
    return _mnuMain->currentItem() == _mnuMainSequencesItem;
}

bool Views::ViewProjects::isShotsViewShown()
{
    return _mnuMain->currentItem() == _mnuMainShotsItem;
}

bool Views::ViewProjects::isAssetsViewShown()
{
    return _mnuMain->currentItem() == _mnuMainAssetsItem;
}

bool Views::ViewProjects::isTasksViewShown()
{
    return _mnuMain->currentItem() == _mnuMainTasksItem;
}

Ms::Widgets::MQueryTableViewWidget<Projects::Project> *Views::ViewProjects::projectsQueryTableView() const
{
    return _qtvProjects;
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectSequence> *Views::ViewProjects::sequencesQueryTableView() const
{
    return _qtvSequences;
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectShot> *Views::ViewProjects::shotsQueryTableView() const
{
    return _qtvShots;
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAsset> *Views::ViewProjects::assetsQueryTableView() const
{
    return _qtvAssets;
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *Views::ViewProjects::tasksQueryTableView() const
{
    return _qtvTasks;
}

Wt::Signal<> &Views::ViewProjects::onTabProjectsSelected()
{
    return _onTabProjectsSelected;
}

Wt::Signal<> &Views::ViewProjects::onTabSequencesSelected()
{
    return _onTabSequencesSelected;
}

Wt::Signal<> &Views::ViewProjects::onTabShotsSelected()
{
    return _onTabShotsSelected;
}

Wt::Signal<> &Views::ViewProjects::onTabAssetsSelected()
{
    return _onTabAssetsSelected;
}

Wt::Signal<> &Views::ViewProjects::onTabTasksSelected()
{
    return _onTabTasksSelected;
}

template<typename T>
void Views::ViewProjects::_addExtraColumns(Ms::Widgets::MQueryTableViewWidget<T> *widget, Wt::WFlags<Wt::ItemFlag> flags, int editRank)
{
    widget->addColumn(Ms::Widgets::MTableViewColumn("View_Rank", "View Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Edit_Rank", "Edit Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Remove_Rank", "Remove Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Date_Created", "Date Created", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Created_By", "Created By", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Last_Modified_Date", "Last Modified Date", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Last_Modified_By", "Last Modified By", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Active", "Active", Wt::ItemIsSelectable | Wt::ItemIsUserCheckable, new Ms::Widgets::Delegates::MCheckBoxDelegate(editRank)));
}

template<typename T>
void Views::ViewProjects::_addDataToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec)
{
    Views::Dialogs::DlgCreateDBOData *dlg = new Views::Dialogs::DlgCreateDBOData();
    dlg->finished().connect(std::bind([=]()
    {
       if(dlg->result() == Wt::WDialog::Accepted)
       {
           for(auto &ptr : dboVec)
           {
                Database::DboData *data = new Database::DboData(dlg->key(), dlg->value());
                Wt::Dbo::ptr<Database::DboData> dataPtr = Database::DatabaseManager::instance().createDbo<Database::DboData>(data);

                if(dataPtr.get())
                    Database::DatabaseManager::instance().modifyDbo<T>(ptr)->addData(dataPtr);
                else
                    delete data;
           }

           _updatePropertiesDataView();
       }

       delete dlg;
    }));

    dlg->show();
}

template<typename T>
void Views::ViewProjects::_addNoteToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec)
{
    Views::Dialogs::DlgCreateNote *dlg = new Views::Dialogs::DlgCreateNote();
    dlg->finished().connect(std::bind([=]()
    {
       if(dlg->result() == Wt::WDialog::Accepted)
       {
           for(auto &ptr : dboVec)
           {
                Database::Note *note = new Database::Note(dlg->content());
                Wt::Dbo::ptr<Database::Note> notePtr = Database::DatabaseManager::instance().createDbo<Database::Note>(note);

                if(notePtr.get())
                    Database::DatabaseManager::instance().modifyDbo<T>(ptr)->addNote(notePtr);
                else
                    delete note;
           }

           _updatePropertiesNotesView();
       }

       delete dlg;
    }));

    dlg->show();
}

template<typename T>
void Views::ViewProjects::_addTagsToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    if(dboVec.size() > 0)
    {
        for(auto &dboPtr : dboVec)
        {
            for(auto &tagPtr : tagVec)
            {
                Database::DatabaseManager::instance().modifyDbo<T>(dboPtr)->addTag(tagPtr);
            }
        }

        _updatePropertiesAssignedTagsView();
    }
}

template<typename T>
void Views::ViewProjects::_removeTagsFromDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    if(dboVec.size() > 0)
    {
        for(auto &dboPtr : dboVec)
        {
            for(auto &tagPtr : tagVec)
            {
                Database::DatabaseManager::instance().modifyDbo<T>(dboPtr)->removeTag(tagPtr);
            }
        }

        _updatePropertiesAssignedTagsView();
    }
}

//Main
void Views::ViewProjects::_mnuMainProjectsItemTriggered()
{
    _stkMain->setCurrentWidget(_cntProjects);
    updateProjectsView();

    _onTabProjectsSelected();
}

void Views::ViewProjects::_mnuMainSequencesItemTriggered()
{
    _stkMain->setCurrentWidget(_cntSequences);
    updateSequencesView();

    _onTabSequencesSelected();
}

void Views::ViewProjects::_mnuMainShotsItemTriggered()
{
    _stkMain->setCurrentWidget(_cntShots);
    updateShotsView();

    _onTabShotsSelected();
}

void Views::ViewProjects::_mnuMainAssetsItemTriggered()
{
    _stkMain->setCurrentWidget(_cntAssets);
    updateAssetsView();

    _onTabAssetsSelected();
}

void Views::ViewProjects::_mnuMainTasksItemTriggered()
{
    _stkMain->setCurrentWidget(_cntTasks);
    updateTasksView();

    _onTabTasksSelected();
}

//Projects
void Views::ViewProjects::_btnProjectsCreateClicked()
{
    Views::DlgCreateProject *dlg = new Views::DlgCreateProject();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            if(!Database::DatabaseManager::instance().dboExists<Projects::Project>(dlg->projectName()))
            {

                Projects::Project *project = new Projects::Project(dlg->projectName());
                project->setStatus(dlg->status());
                project->setManager(dlg->manager());
                project->setStartDate(dlg->startDate());
                project->setEndDate(dlg->endDate());
                project->setDurationInFrames(dlg->duration());
                project->setFps(dlg->fps());
                project->setFrameWidth(dlg->frameWidth());
                project->setFrameHeight(dlg->frameHeight());
                project->setDescription(dlg->description());
                project->setActive(dlg->isActive());

                if(Database::DatabaseManager::instance().createDbo<Projects::Project>(project))
                {
                    Projects::ProjectsIO::createProjectDirectoryStructure(dlg->projectName());
                    updateProjectsView();

                    _logger->log(std::string("Created project ") + dlg->projectName(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete project;

                    _logger->log(std::string("Error creating project ") + dlg->projectName(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                _logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
            }
        }

        delete dlg;
    }));

    dlg->show();
}

void Views::ViewProjects::_btnProjectsRemoveClicked()
{

}

void Views::ViewProjects::_btnProjectsFilesClicked()
{
    if(_qtvProjects->table()->selectedIndexes().size() != 1)
        return;

    std::string prjName = _qtvProjects->model()->resultRow(_qtvProjects->proxyModel()->mapToSource(
                                                               *_qtvProjects->table()->selectedIndexes().begin()).row())->name();

    DlgFilesManager *dlg = new DlgFilesManager(Projects::ProjectsIO::getAbsoluteProjectDir(prjName) + Ms::IO::dirSeparator() + "files");
    dlg->finished().connect(std::bind([=]()
    {
        delete dlg;
    }));

    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
        dlg->setCreateDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In"))
        dlg->setCheckInDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out"))
        dlg->setCheckOutDisabled(true);

    dlg->show();
}

void Views::ViewProjects::_btnProjectsImportThumbnailsClicked()
{
    if(_qtvProjects->model()->rowCount() == 0)
        return;

    Ms::Widgets::Dialogs::MFilesUploadDialog *dlg = new Ms::Widgets::Dialogs::MFilesUploadDialog(true, true);
    dlg->finished().connect(std::bind([=]()
    {
        std::vector<std::string> delFiles;//holds thumbnails files for later deletion
        for(const std::pair<std::string,std::string> &pair : dlg->uploadedFilesMap())
        {
            try
            {
                //get the original raw file name without the extension
                int lastIndex = pair.second.find_last_of(".");
                std::string rawFileName = pair.second.substr(0, lastIndex);

                //match thumbnail by project name
                if(!Database::DatabaseManager::instance().openTransaction())
                    return;

                Wt::Dbo::ptr<Projects::Project> prjPtr;

                if(AppSettings::instance().isLoadInactiveData())
                    prjPtr = Database::DatabaseManager::instance().session()->find<Projects::Project>().where("Project_Name = ?").bind(rawFileName);
                else
                    prjPtr = Database::DatabaseManager::instance().session()->find<Projects::Project>().where("Project_Name = ? AND Active = ?").bind(rawFileName).bind(true);

                if(prjPtr)//found a project that has the same name as the thumbnail ?
                {
                    std::string prjDir = Projects::ProjectsIO::getAbsoluteProjectDir(prjPtr->name());
                    std::string localFile = prjDir + Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second;

                    if(Ms::IO::fileExists(localFile))//if thumbnail already exist
                        Ms::IO::removeFile(localFile);//delete it

                    if(Ms::IO::copyFile(pair.first, localFile))//copy and rename the file to the original name
                    {
                        prjPtr.modify()->setThumbnail(Projects::ProjectsIO::getRelativeProjectDir(prjPtr->name()) + Ms::IO::dirSeparator() +
                                                      "thumbnails" + Ms::IO::dirSeparator() + pair.second);
                    }
                }
            }
            catch(Wt::WException e)
            {
                _logger->log(std::string("Error occured while trying to import thumbnails to table projects") + e.what(),
                             Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
            }

            delFiles.push_back(pair.first);//cache it for later deletion
        }

        for(std::vector<std::string>::size_type i = 0; i <delFiles.size(); ++i)
        {
            Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails
            _logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info);
        }

        _qtvProjects->reload();

        delete dlg;
    }));

    dlg->show();
}

void Views::ViewProjects::_createProjectsTableView()
{
    _qtvProjects = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::Project>(&Database::DatabaseManager::instance());
    _qtvProjects->setRowHeight(160);
    _qtvProjects->setDefaultFilterColumnIndex(1);
    _qtvProjects->setIgnoreNumFilterColumns(1);
    _qtvProjects->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvProjects->createToolButton("", "icons/Add.png", "Create A New Project");
        btn->clicked().connect(this, &Views::ViewProjects::_btnProjectsCreateClicked);

        _qtvProjects->setImportCSVFetureEnabled(true);
    }
    else
        _qtvProjects->setImportCSVFetureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //Wt::WPushButton *btn = _qtvProjects->createToolButton("", "icons/Remove.png", "Remove Selected Project");
        //btn->clicked().connect(this, &Views::ViewProjects::_btnProjectsRemoveClicked);
    }

    //requires "edit" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvProjects->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
        btn->clicked().connect(this, &Views::ViewProjects::_btnProjectsImportThumbnailsClicked);
    }

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
    {
        Wt::WPushButton *btn = _qtvProjects->createToolButton("", "icons/Files.png", "Files Manager");
        btn->clicked().connect(this, &Views::ViewProjects::_btnProjectsFilesClicked);
    }

    updateProjectsView();
}

//Sequences
void Views::ViewProjects::_btnSequencesCreateClicked()
{
    if(_qtvProjects->table()->selectedIndexes().size() > 0)
    {
        Views::DlgCreateSequence *dlg = new Views::DlgCreateSequence();
        dlg->finished().connect(std::bind([=]()
        {
            if(dlg->result() == Wt::WDialog::Accepted)
            {
                for(auto prjPtr : _qtvProjects->selectedItems())
                {
                    Projects::ProjectSequenceId id;
                    id.name = (dlg->sequenceName());
                    id.project = prjPtr;

                    if(!Database::DatabaseManager::instance().dboExists<Projects::ProjectSequence>(id))
                    {
                        Projects::ProjectSequence *sequence = new Projects::ProjectSequence(dlg->sequenceName());
                        sequence->setProject(prjPtr);
                        sequence->setStatus(dlg->status());
                        sequence->setStartDate(dlg->startDate());
                        sequence->setEndDate(dlg->endDate());
                        sequence->setDurationInFrames(dlg->duration());
                        sequence->setFps(dlg->fps());
                        sequence->setFrameWidth(dlg->frameWidth());
                        sequence->setFrameHeight(dlg->frameHeight());
                        sequence->setDescription(dlg->description());
                        sequence->setActive(dlg->isActive());

                        Wt::Dbo::ptr<Projects::ProjectSequence> seqPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectSequence>(sequence);
                        if(seqPtr)
                        {
                            Projects::ProjectsIO::createSequenceDirectoryStructure(prjPtr->name(), seqPtr->name());
                            updateSequencesView();

                            _logger->log(std::string("Created sequence for project ") + prjPtr->name(), Ms::Log::LogMessageType::Info);
                        }
                        else
                        {
                            delete sequence;

                            _logger->log(std::string("Error creating sequence for project ") + prjPtr->name(), Ms::Log::LogMessageType::Error);
                        }
                    }
                    else
                    {
                        _logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
                    }
                }
            }

            delete dlg;
        }));

        dlg->show();
    }
}

void Views::ViewProjects::_btnSequencesRemoveClicked()
{

}

void Views::ViewProjects::_btnSequencesFilesClicked()
{
    if(_qtvSequences->table()->selectedIndexes().size() != 1)
        return;

    Wt::Dbo::ptr<Projects::ProjectSequence> seqPtr = _qtvSequences->model()->resultRow(_qtvSequences->proxyModel()->mapToSource(
                                                               *_qtvSequences->table()->selectedIndexes().begin()).row());

    DlgFilesManager *dlg = new DlgFilesManager(Projects::ProjectsIO::getAbsoluteSequenceDir(seqPtr->projectName(), seqPtr->name()) + Ms::IO::dirSeparator() + "files");
    dlg->finished().connect(std::bind([=]()
    {
        delete dlg;
    }));

    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
        dlg->setCreateDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In"))
        dlg->setCheckInDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out"))
        dlg->setCheckOutDisabled(true);

    dlg->show();
}

void Views::ViewProjects::_btnSequencesImportThumbnailsClicked()
{
    if(_qtvSequences->model()->rowCount() == 0)
        return;

    Ms::Widgets::Dialogs::MFilesUploadDialog *dlg = new Ms::Widgets::Dialogs::MFilesUploadDialog(true, true);
    dlg->finished().connect(std::bind([=]()
    {
        std::vector<std::string> delFiles;//holds thumbnails files for later deletion
        for(const std::pair<std::string,std::string> &pair : dlg->uploadedFilesMap())
        {
            try
            {
                //get the original raw file name without the extension
                int lastIndex = pair.second.find_last_of(".");
                std::string rawFileName = pair.second.substr(0, lastIndex);

                //match thumbnail by sequence name
                if(!Database::DatabaseManager::instance().openTransaction())
                    return;

                //loop for all sequences
                for(int i = 0; i < _qtvSequences->model()->rowCount(); ++i)
                {
                    Wt::Dbo::ptr<Projects::ProjectSequence> sequence = _qtvSequences->model()->resultRow(i);

                    if(sequence->name() == rawFileName)//sequence has the same name of the thumbnail ?
                    {
                        if(sequence->active() == false && (!AppSettings::instance().isLoadInactiveData()))//don't change thumbnail for inactive sequence
                            continue;

                        std::string seqDir = Projects::ProjectsIO::getAbsoluteSequenceDir(sequence->projectName(), sequence->name());
                        std::string localFile = seqDir + Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second;

                        if(Ms::IO::fileExists(localFile))//if thumbnail already exist
                            Ms::IO::removeFile(localFile);//delete it

                        if(Ms::IO::copyFile(pair.first, localFile))//copy and rename the file to the original name
                        {
                            sequence.modify()->setThumbnail(Projects::ProjectsIO::getRelativeSequenceDir(sequence->projectName(), sequence->name()) +
                                    Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second);
                        }

                        break;//sequence mtching thumbnail name found, exit loop
                    }
                }
            }
            catch(Wt::WException e)
            {
                _logger->log(std::string("Error occured while trying to import thumbnails to table sequences") + e.what(),
                             Ms::Log::LogMessageType::Error);
            }

            delFiles.push_back(pair.first);//cache it for later deletion
        }

        for(std::vector<std::string>::size_type i = 0; i < delFiles.size(); ++i)
        {
            Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails
            _logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info);
        }

        _qtvSequences->reload();

        delete dlg;
    }));

    dlg->show();
}

void Views::ViewProjects::_createSequencesTableView()
{
    _qtvSequences = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectSequence>(&Database::DatabaseManager::instance());
    _qtvSequences->setRowHeight(160);
    _qtvSequences->setDefaultFilterColumnIndex(1);
    _qtvSequences->setIgnoreNumFilterColumns(1);
    _qtvSequences->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvSequences->createToolButton("", "icons/Add.png", "Create A New Sequence");
        btn->clicked().connect(this, &Views::ViewProjects::_btnSequencesCreateClicked);

        _qtvSequences->setImportCSVFetureEnabled(true);
    }
    else
        _qtvSequences->setImportCSVFetureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //Wt::WPushButton *btn = _qtvSequences->createToolButton("", "icons/Remove.png", "Remove Selected Sequences");
        //btn->clicked().connect(this, &Views::ViewProjects::_btnSequencesRemoveClicked);
    }

    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvSequences->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
        btn->clicked().connect(this, &Views::ViewProjects::_btnSequencesImportThumbnailsClicked);
    }

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
    {
        Wt::WPushButton *btn = _qtvSequences->createToolButton("", "icons/Files.png", "Files Manager");
        btn->clicked().connect(this, &Views::ViewProjects::_btnSequencesFilesClicked);
    }

    updateSequencesView();
}

//Shots
void Views::ViewProjects::_btnShotsCreateClicked()
{
    if(_qtvSequences->table()->selectedIndexes().size() > 0)
    {
        Views::DlgCreateShot *dlg = new Views::DlgCreateShot();
        dlg->finished().connect(std::bind([=]()
        {
            if(dlg->result() == Wt::WDialog::Accepted)
            {
                for(auto seqPtr : _qtvSequences->selectedItems())
                {
                    Projects::ProjectShotId id;
                    id.name = (dlg->shotName());
                    id.sequence = seqPtr;

                    if(!Database::DatabaseManager::instance().dboExists<Projects::ProjectShot>(id))
                    {
                        Projects::ProjectShot *shot = new Projects::ProjectShot(dlg->shotName());
                        shot->setSequence(seqPtr);
                        shot->setStatus(dlg->status());
                        shot->setStartDate(dlg->startDate());
                        shot->setEndDate(dlg->endDate());
                        shot->setDurationInFrames(dlg->duration());
                        shot->setFps(dlg->fps());
                        shot->setFrameWidth(dlg->frameWidth());
                        shot->setFrameHeight(dlg->frameHeight());
                        shot->setDescription(dlg->description());
                        shot->setActive(dlg->isActive());

                        Wt::Dbo::ptr<Projects::ProjectShot> shotPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectShot>(shot);
                        if(shotPtr)
                        {
                            Projects::ProjectsIO::createShotDirectoryStructure(seqPtr->projectName(), seqPtr->name(), shotPtr->name());
                            updateShotsView();

                            _logger->log(std::string("Created shot for sequence ") + seqPtr->name(), Ms::Log::LogMessageType::Info);
                        }
                        else
                        {
                            delete shot;

                            _logger->log(std::string("error creating shot for sequence ") + seqPtr->name(), Ms::Log::LogMessageType::Error);
                        }
                    }
                    else
                    {
                        _logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
                    }
                }
            }

            delete dlg;
        }));

        dlg->show();
    }
}

void Views::ViewProjects::_btnShotsRemoveClicked()
{

}

void Views::ViewProjects::_btnShotsFilesClicked()
{
    if(_qtvShots->table()->selectedIndexes().size() != 1)
        return;

    Wt::Dbo::ptr<Projects::ProjectShot> shotPtr = _qtvShots->model()->resultRow(_qtvShots->proxyModel()->mapToSource(
                                                               *_qtvShots->table()->selectedIndexes().begin()).row());

    DlgFilesManager *dlg = new DlgFilesManager(Projects::ProjectsIO::getAbsoluteShotDir(shotPtr->projectName(), shotPtr->sequenceName(), shotPtr->name()) + Ms::IO::dirSeparator() + "files");
    dlg->finished().connect(std::bind([=]()
    {
        delete dlg;
    }));

    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
        dlg->setCreateDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In"))
        dlg->setCheckInDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out"))
        dlg->setCheckOutDisabled(true);

    dlg->show();
}

void Views::ViewProjects::_btnShotsImportThumbnailsClicked()
{
    if(_qtvShots->model()->rowCount() == 0)
        return;

    Ms::Widgets::Dialogs::MFilesUploadDialog *dlg = new Ms::Widgets::Dialogs::MFilesUploadDialog(true, true);
    dlg->finished().connect(std::bind([=]()
    {
        std::vector<std::string> delFiles;//holds thumbnails files for later deletion
        for(const std::pair<std::string,std::string> &pair : dlg->uploadedFilesMap())
        {
            try
            {
                //get the original raw file name without the extension
                int lastIndex = pair.second.find_last_of(".");
                std::string rawFileName = pair.second.substr(0, lastIndex);

                //match thumbnail by shot name
                if(!Database::DatabaseManager::instance().openTransaction())
                    return;

                //loop for all shots
                for(int i = 0; i < _qtvShots->model()->rowCount(); ++i)
                {
                    Wt::Dbo::ptr<Projects::ProjectShot> shot = _qtvShots->model()->resultRow(i);

                    if(shot->name() == rawFileName)//shot has the same name of the thumbnail ?
                    {
                        if(shot->active() == false && (!AppSettings::instance().isLoadInactiveData()))//don't change thumbnail for inactive shot
                            continue;

                        std::string shotDir = Projects::ProjectsIO::getAbsoluteShotDir(shot->projectName(), shot->sequenceName(), shot->name());
                        std::string localFile = shotDir + Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second;

                        if(Ms::IO::fileExists(localFile))//if thumbnail already exist
                            Ms::IO::removeFile(localFile);//delete it

                        if(Ms::IO::copyFile(pair.first, localFile))//copy and rename the file to the original name
                        {
                            shot.modify()->setThumbnail(Projects::ProjectsIO::getRelativeShotDir(shot->projectName(), shot->sequenceName(), shot->name()) +
                                    Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second);
                        }

                        break;//shot mtching thumbnail name found, exit loop
                    }
                }
            }
            catch(Wt::WException e)
            {
                _logger->log(std::string("Error occured while trying to import thumbnails to table shots") + e.what(),
                                         Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
            }

            delFiles.push_back(pair.first);//cache it for later deletion
        }

        for(std::vector<std::string>::size_type i = 0; i <delFiles.size(); ++i)
        {
            Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails

            _logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info);
        }

        _qtvShots->reload();

        delete dlg;
    }));

    dlg->show();
}

void Views::ViewProjects::_createShotsTableView()
{
    _qtvShots = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectShot>(&Database::DatabaseManager::instance());
    _qtvShots->setRowHeight(160);
    _qtvShots->setDefaultFilterColumnIndex(1);
    _qtvShots->setIgnoreNumFilterColumns(1);
    _qtvShots->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvShots->createToolButton("", "icons/Add.png", "Create A New Shot");
        btn->clicked().connect(this, &Views::ViewProjects::_btnShotsCreateClicked);

        _qtvShots->setImportCSVFetureEnabled(true);
    }
    else
        _qtvShots->setImportCSVFetureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //Wt::WPushButton *btn = _qtvShots->createToolButton("", "icons/Remove.png", "Remove Selected Shots");
        //btn->clicked().connect(this, &Views::ViewProjects::_btnShotsRemoveClicked);
    }

    //Wt::WPushButton *btnExport = _qtvShots->createToolButton("", "icons/Export.png", "Export Selected Shots");
    //btnExport->clicked().connect(this, &Views::ViewSequences::_btnShotsExportClicked);

    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvShots->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
        btn->clicked().connect(this, &Views::ViewProjects::_btnShotsImportThumbnailsClicked);
    }

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
       Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
    {
        Wt::WPushButton *btn = _qtvShots->createToolButton("", "icons/Files.png", "Files Manager");
        btn->clicked().connect(this, &Views::ViewProjects::_btnShotsFilesClicked);
    }

    updateShotsView();
}

//Assets
void Views::ViewProjects::_btnAssetsCreateClicked()
{
    if(_qtvProjects->table()->selectedIndexes().size() > 0)
    {
        Views::DlgCreateAsset *dlg = new Views::DlgCreateAsset();
        dlg->finished().connect(std::bind([=]()
        {
            if(dlg->result() == Wt::WDialog::Accepted)
            {
                for(auto prjPtr : _qtvProjects->selectedItems())
                {
                    Projects::ProjectAssetId id;
                    id.name = (dlg->assetName());
                    id.project = prjPtr;

                    if(!Database::DatabaseManager::instance().dboExists<Projects::ProjectAsset>(id))
                    {
                        Projects::ProjectAsset *asset = new Projects::ProjectAsset(dlg->assetName());
                        asset->setProject(prjPtr);
                        asset->setStatus(dlg->status());
                        asset->setType(dlg->assetType());
                        asset->setStartDate(dlg->startDate());
                        asset->setEndDate(dlg->endDate());
                        asset->setDescription(dlg->description());
                        asset->setActive(dlg->isActive());

                        Wt::Dbo::ptr<Projects::ProjectAsset> assetPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectAsset>(asset);
                        if(assetPtr)
                        {
                            Projects::ProjectsIO::createAssetDirectoryStructure(prjPtr->name(), assetPtr->name());
                            updateAssetsView();

                            _logger->log(std::string("Created asset ") + dlg->assetName(), Ms::Log::LogMessageType::Info);
                        }
                        else
                        {
                            delete asset;

                            _logger->log(std::string("error creating asset ") + dlg->assetName(), Ms::Log::LogMessageType::Error);
                        }
                    }
                    else
                    {
                        _logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
                    }
                }
            }

            delete dlg;
        }));

        dlg->show();
    }
}

void Views::ViewProjects::_btnAssetsRemoveClicked()
{

}

void Views::ViewProjects::_btnAssetsFilesClicked()
{
    if(_qtvAssets->table()->selectedIndexes().size() != 1)
        return;

    Wt::Dbo::ptr<Projects::ProjectAsset> assetPtr =  _qtvAssets->model()->resultRow(_qtvAssets->proxyModel()->mapToSource(
                                                               *_qtvAssets->table()->selectedIndexes().begin()).row());

    DlgFilesManager *dlg = new DlgFilesManager(Projects::ProjectsIO::getAbsoluteAssetDir(assetPtr->projectName(), assetPtr->name()) + Ms::IO::dirSeparator() + "files");
    dlg->finished().connect(std::bind([=]()
    {
        delete dlg;
    }));

    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
        dlg->setCreateDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In"))
        dlg->setCheckInDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out"))
        dlg->setCheckOutDisabled(true);

    dlg->show();
}

void Views::ViewProjects::_btnAssetsImportThumbnailsClicked()
{
    if(_qtvAssets->model()->rowCount() == 0)
        return;

    Ms::Widgets::Dialogs::MFilesUploadDialog *dlg = new Ms::Widgets::Dialogs::MFilesUploadDialog(true, true);
    dlg->finished().connect(std::bind([=]()
    {
        std::vector<std::string> delFiles;//holds thumbnails files for later deletion
        for(const std::pair<std::string,std::string> &pair : dlg->uploadedFilesMap())
        {
            try
            {
                //get the original raw file name without the extension
                int lastIndex = pair.second.find_last_of(".");
                std::string rawFileName = pair.second.substr(0, lastIndex);

                //match thumbnail by asset name
                if(!Database::DatabaseManager::instance().openTransaction())
                    return;

                //loop for all assets
                for(int i = 0; i < _qtvAssets->model()->rowCount(); ++i)
                {
                    Wt::Dbo::ptr<Projects::ProjectAsset> asset = _qtvAssets->model()->resultRow(i);

                    if(asset->name() == rawFileName)//asset has the same name of the thumbnail ?
                    {
                        if(asset->active() == false && (!AppSettings::instance().isLoadInactiveData()))//don't change thumbnail for inactive asset
                            continue;

                        std::string assetDir = Projects::ProjectsIO::getAbsoluteAssetDir(asset->projectName(), asset->name());
                        std::string localFile = assetDir + Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second;

                        if(Ms::IO::fileExists(localFile))//if thumbnail already exist
                            Ms::IO::removeFile(localFile);//delete it

                        if(Ms::IO::copyFile(pair.first, localFile))//copy and rename the file to the original name
                        {
                            asset.modify()->setThumbnail(Projects::ProjectsIO::getRelativeAssetDir(asset->projectName(), asset->name()) +
                                    Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second);
                        }

                        break;//asset mtching thumbnail name found, exit loop
                    }
                }
            }
            catch(Wt::WException e)
            {
                _logger->log(std::string("Error occured while trying to import thumbnails to table assets") + e.what(),
                                         Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
            }

            delFiles.push_back(pair.first);//cache it for later deletion
        }

        for(std::vector<std::string>::size_type i = 0; i < delFiles.size(); ++i)
        {
            Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails
            _logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info);
        }

        _qtvAssets->reload();

        delete dlg;
    }));

    dlg->show();
}

void Views::ViewProjects::_createAssetsTableView()
{
    _qtvAssets = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectAsset>(&Database::DatabaseManager::instance());
    _qtvAssets->setRowHeight(160);
    _qtvAssets->setDefaultFilterColumnIndex(1);
    _qtvAssets->setIgnoreNumFilterColumns(1);
    _qtvAssets->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvAssets->createToolButton("", "icons/Add.png", "Create A New Asset");
        btn->clicked().connect(this, &Views::ViewProjects::_btnAssetsCreateClicked);

        _qtvAssets->setImportCSVFetureEnabled(true);
    }
    else
        _qtvAssets->setImportCSVFetureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //Wt::WPushButton *btn = _qtvAssets->createToolButton("", "icons/Remove.png", "Remove Selected Assets");
        //btn->clicked().connect(this, &Views::ViewProjects::_btnAssetsRemoveClicked);
    }

    //Wt::WPushButton *btnExport = _qtvAssets->createToolButton("", "icons/Export.png", "Export Selected Assets");
    //btnExport->clicked().connect(this, &Views::ViewSequences::_btnAssetsExportClicked);

    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvAssets->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
        btn->clicked().connect(this, &Views::ViewProjects::_btnAssetsImportThumbnailsClicked);
    }

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
    {
        Wt::WPushButton *btn = _qtvAssets->createToolButton("", "icons/Files.png", "Files Manager");
        btn->clicked().connect(this, &Views::ViewProjects::_btnAssetsFilesClicked);
    }

    updateAssetsView();
}

//Tasks
void Views::ViewProjects::_btnTasksCreateClicked()
{
    if((_qtvAssets->table()->selectedIndexes().size() > 0) || (_qtvShots->table()->selectedIndexes().size() > 0))
    {
        Views::DlgCreateTask *dlg = new Views::DlgCreateTask();
        dlg->finished().connect(std::bind([=]()
        {
            if(dlg->result() == Wt::WDialog::Accepted)
            {
                //add tasks for selected shots
                for(auto &shotPtr : _qtvShots->selectedItems())
                {
                    Projects::ProjectTask *task = new Projects::ProjectTask();
                    task->setShot(shotPtr);
                    task->setStatus(dlg->status());
                    task->setType(dlg->type());
                    task->setStartDate(dlg->startDate());
                    task->setEndDate(dlg->endDate());
                    task->setDescription(dlg->description());
                    task->setActive(dlg->isActive());

                    Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectTask>(task);
                    if(taskPtr)
                    {
                        Projects::ProjectsIO::createShotTaskDirectoryStructure(shotPtr->projectName(), shotPtr->sequenceName(), shotPtr->name(), taskPtr.id());

                        _logger->log(std::string("Created task for shot ") + shotPtr->name(), Ms::Log::LogMessageType::Info);
                    }
                    else
                    {
                        delete task;

                        _logger->log(std::string("Error creating task for shot") + shotPtr->name(), Ms::Log::LogMessageType::Error);
                    }
                }

                //add tasks for selected assets
                for(auto &assetPtr : _qtvAssets->selectedItems())
                {
                    Projects::ProjectTask *task = new Projects::ProjectTask();
                    task->setAsset(assetPtr);
                    task->setStatus(dlg->status());
                    task->setType(dlg->type());
                    task->setStartDate(dlg->startDate());
                    task->setEndDate(dlg->endDate());
                    task->setDescription(dlg->description());
                    task->setActive(dlg->isActive());

                    Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectTask>(task);
                    if(taskPtr)
                    {
                        Projects::ProjectsIO::createAssetTaskDirectoryStructure(assetPtr->projectName(), assetPtr->projectName(), taskPtr.id());

                        _logger->log(std::string("Created task for asset ") + assetPtr->name(), Ms::Log::LogMessageType::Info, Log::LogMessageContext::ServerAndClient);
                    }
                    else
                    {
                        delete task;

                        _logger->log(std::string("error creating task for asset ") + assetPtr->name(), Ms::Log::LogMessageType::Error);
                    }
                }

                updateTasksView();
            }

            delete dlg;
        }));

        dlg->show();
    }
}

void Views::ViewProjects::_btnTasksRemoveClicked()
{

}

void Views::ViewProjects::_btnTasksFilesClicked()
{
    if(_qtvTasks->table()->selectedIndexes().size() != 1)
        return;

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::ptr<Projects::ProjectTask> taskPtr =  _qtvTasks->model()->resultRow(_qtvTasks->proxyModel()->mapToSource(
                                                               *_qtvTasks->table()->selectedIndexes().begin()).row());


    if((!taskPtr->asset()) && (!taskPtr->shot()))
        return;

    std::string path = "";

    if(taskPtr->asset())
        path = Projects::ProjectsIO::getAbsoluteAssetTaskDir(taskPtr->asset()->projectName(), taskPtr->asset()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
    else if(taskPtr->shot())
        path = Projects::ProjectsIO::getAbsoluteShotTaskDir(taskPtr->shot()->projectName(), taskPtr->shot()->sequenceName(), taskPtr->shot()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";

    DlgFilesManager *dlg = new DlgFilesManager(path);
    dlg->finished().connect(std::bind([=]()
    {
        delete dlg;
    }));

    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
        dlg->setCreateDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In"))
        dlg->setCheckInDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out"))
        dlg->setCheckOutDisabled(true);

    dlg->show();
}

void Views::ViewProjects::_createTasksTableView()
{
    _qtvTasks = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTask>(&Database::DatabaseManager::instance());
    _qtvTasks->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvTasks->createToolButton("", "icons/Add.png", "Create A New Shot");
        btn->clicked().connect(this, &Views::ViewProjects::_btnTasksCreateClicked);

        _qtvTasks->setImportCSVFetureEnabled(true);
    }
    else
        _qtvTasks->setImportCSVFetureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //Wt::WPushButton *btn = _qtvTasks->createToolButton("", "icons/Remove.png", "Remove Selected Tasks");
        //btn->clicked().connect(this, &Views::ViewProjects::_btnTasksRemoveClicked);
    }

    //Wt::WPushButton *btnExport = _qtvTasks->createToolButton("", "icons/Export.png", "Export Selected Tasks");
    //btnExport->clicked().connect(this, &Views::ViewProjects::_btnTasksExportClicked);

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
    {
        Wt::WPushButton *btn = _qtvTasks->createToolButton("", "icons/Files.png", "Files Manager");
        btn->clicked().connect(this, &Views::ViewProjects::_btnTasksFilesClicked);
    }

    updateTasksView();
}

//Properties
void Views::ViewProjects::_mnuPropertiesNavBarDataItemTriggered()
{
    _stkProperties->setCurrentWidget(_qtvPropertiesData);

    _updatePropertiesDataView();
}

void Views::ViewProjects::_mnuPropertiesNavBarTagsItemTriggered()
{
    _stkProperties->setCurrentWidget(_cntPropertiesTags);

    _updatePropertiesTagsView();
    _updatePropertiesAssignedTagsView();
}

void Views::ViewProjects::_mnuPropertiesNavBarNotesItemTriggered()
{
    _stkProperties->setCurrentWidget(_qtvPropertiesNotes);

    _updatePropertiesNotesView();
}

void Views::ViewProjects::_btnAddPropertiesDataClicked()
{
    if(_stkMain->currentWidget() == _cntProjects)
    {
        if(_qtvProjects->table()->selectedIndexes().size() > 0)
            _addDataToDbo<Projects::Project>(_qtvProjects->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntSequences)
    {
        if(_qtvSequences->table()->selectedIndexes().size() > 0)
            _addDataToDbo<Projects::ProjectSequence>(_qtvSequences->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntShots)
    {
        if(_qtvShots->table()->selectedIndexes().size() > 0)
            _addDataToDbo<Projects::ProjectShot>(_qtvShots->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntAssets)
    {
        if(_qtvAssets->table()->selectedIndexes().size() > 0)
            _addDataToDbo<Projects::ProjectAsset>(_qtvAssets->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntTasks)
    {
        if(_qtvTasks->table()->selectedIndexes().size() > 0)
            _addDataToDbo<Projects::ProjectTask>(_qtvTasks->selectedItems());
    }
}

void Views::ViewProjects::_btnRemovePropertiesDataClicked()
{

}

void Views::ViewProjects::_btnAddPropertiesTagClicked()
{
    if(_stkMain->currentWidget() == _cntProjects)
    {
        if(_qtvProjects->table()->selectedIndexes().size() > 0)
            _addTagsToDbo<Projects::Project>(_qtvProjects->selectedItems(), _qtvPropertiesTags->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntSequences)
    {
        if(_qtvSequences->table()->selectedIndexes().size() > 0)
            _addTagsToDbo<Projects::ProjectSequence>(_qtvSequences->selectedItems(), _qtvPropertiesTags->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntShots)
    {
        if(_qtvShots->table()->selectedIndexes().size() > 0)
            _addTagsToDbo<Projects::ProjectShot>(_qtvShots->selectedItems(), _qtvPropertiesTags->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntAssets)
    {
        if(_qtvAssets->table()->selectedIndexes().size() > 0)
            _addTagsToDbo<Projects::ProjectAsset>(_qtvAssets->selectedItems(), _qtvPropertiesTags->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntTasks)
    {
        if(_qtvTasks->table()->selectedIndexes().size() > 0)
            _addTagsToDbo<Projects::ProjectTask>(_qtvTasks->selectedItems(), _qtvPropertiesTags->selectedItems());
    }
}

void Views::ViewProjects::_btnRemovePropertiesTagClicked()
{
    if(_stkMain->currentWidget() == _cntProjects)
    {
        if(_qtvProjects->table()->selectedIndexes().size() > 0)
            _removeTagsFromDbo<Projects::Project>(_qtvProjects->selectedItems(), _qtvPropertiesAssignedTags->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntSequences)
    {
        if(_qtvSequences->table()->selectedIndexes().size() > 0)
            _removeTagsFromDbo<Projects::ProjectSequence>(_qtvSequences->selectedItems(), _qtvPropertiesAssignedTags->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntShots)
    {
        if(_qtvShots->table()->selectedIndexes().size() > 0)
            _removeTagsFromDbo<Projects::ProjectShot>(_qtvShots->selectedItems(), _qtvPropertiesAssignedTags->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntAssets)
    {
        if(_qtvAssets->table()->selectedIndexes().size() > 0)
            _removeTagsFromDbo<Projects::ProjectAsset>(_qtvAssets->selectedItems(), _qtvPropertiesAssignedTags->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntTasks)
    {
        if(_qtvTasks->table()->selectedIndexes().size() > 0)
            _removeTagsFromDbo<Projects::ProjectTask>(_qtvTasks->selectedItems(), _qtvPropertiesAssignedTags->selectedItems());
    }
}

void Views::ViewProjects::_btnAddPropertiesNoteClicked()
{
    if(_stkMain->currentWidget() == _cntProjects)
    {
        if(_qtvProjects->table()->selectedIndexes().size() > 0)
            _addNoteToDbo<Projects::Project>(_qtvProjects->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntSequences)
    {
        if(_qtvSequences->table()->selectedIndexes().size() > 0)
            _addNoteToDbo<Projects::ProjectSequence>(_qtvSequences->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntShots)
    {
        if(_qtvShots->table()->selectedIndexes().size() > 0)
            _addNoteToDbo<Projects::ProjectShot>(_qtvShots->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntAssets)
    {
        if(_qtvAssets->table()->selectedIndexes().size() > 0)
            _addNoteToDbo<Projects::ProjectAsset>(_qtvAssets->selectedItems());
    }
    else if(_stkMain->currentWidget() == _cntTasks)
    {
        if(_qtvTasks->table()->selectedIndexes().size() > 0)
            _addNoteToDbo<Projects::ProjectTask>(_qtvTasks->selectedItems());
    }
}

void Views::ViewProjects::_btnRemovePropertiesNoteClicked()
{

}

void Views::ViewProjects::_createPropertiesView()
{
    _cntPropertiesMain = new Wt::WContainerWidget();

    _propertiesPanel->addPropertiesView("Projects", _cntPropertiesMain);

    _layCntPropertiesMain = new Wt::WVBoxLayout();
    _layCntPropertiesMain->setContentsMargins(0,0,0,0);
    _layCntPropertiesMain->setSpacing(0);

    _cntPropertiesMain->setLayout(_layCntPropertiesMain);

    _navBarPropertiesMain = new Wt::WNavigationBar();

    _layCntPropertiesMain->addWidget(_navBarPropertiesMain);

    _mnuPropertiesNavBar = new Wt::WMenu(Wt::Horizontal);

    _navBarPropertiesMain->addMenu(_mnuPropertiesNavBar);

    _mnuPropertiesNavBarDataItem = new Wt::WMenuItem("Data");
    _mnuPropertiesNavBarDataItem->triggered().connect(this, &Views::ViewProjects::_mnuPropertiesNavBarDataItemTriggered);
    _mnuPropertiesNavBar->addItem(_mnuPropertiesNavBarDataItem);

    _mnuPropertiesNavBarTagsItem = new Wt::WMenuItem("Tags");
    _mnuPropertiesNavBarTagsItem->triggered().connect(this, &Views::ViewProjects::_mnuPropertiesNavBarTagsItemTriggered);
    _mnuPropertiesNavBar->addItem(_mnuPropertiesNavBarTagsItem);

    _mnuPropertiesNavBarNotesItem = new Wt::WMenuItem("Notes");
    _mnuPropertiesNavBarNotesItem->triggered().connect(this, &Views::ViewProjects::_mnuPropertiesNavBarNotesItemTriggered);
    _mnuPropertiesNavBar->addItem(_mnuPropertiesNavBarNotesItem);

    _stkProperties = new Wt::WStackedWidget();

    _layCntPropertiesMain->addWidget(_stkProperties, 1);

    //Data Table View
    _createPropertiesDataTableView();
    _stkProperties->addWidget(_qtvPropertiesData);

    //Tags/AssignedTags Table Views
    _cntPropertiesTags = new Wt::WContainerWidget();
    _stkProperties->addWidget(_cntPropertiesTags);

    _layCntPropertiesTags = new Wt::WVBoxLayout();
    _layCntPropertiesTags->setContentsMargins(0,0,0,0);
    _layCntPropertiesTags->setSpacing(0);

    _cntPropertiesTags->setLayout(_layCntPropertiesTags);

    _cntPropertiesAssignedTags = new Wt::WContainerWidget();

    _layCntPropertiesTags->addWidget(_cntPropertiesAssignedTags);

    _layCntPropertiesAssignedTags = new Wt::WVBoxLayout();
    _layCntPropertiesAssignedTags->setContentsMargins(0,0,0,0);
    _layCntPropertiesAssignedTags->setSpacing(0);

    _cntPropertiesAssignedTags->setLayout(_layCntPropertiesAssignedTags);

    _cntTxtPropertiesAssignedTagsLabel = new Wt::WContainerWidget();
    _cntTxtPropertiesAssignedTagsLabel->setStyleClass("toolbar");
    _cntTxtPropertiesAssignedTagsLabel->setContentAlignment(Wt::AlignCenter);
    _cntTxtPropertiesAssignedTagsLabel->setMinimumSize(Wt::WLength::Auto, 25);

    _layCntPropertiesAssignedTags->addWidget(_cntTxtPropertiesAssignedTagsLabel);

    _txtPropertiesAssignedTagsLabel = new Wt::WText("<b><i>Assigned Tags</i></b>");
    _txtPropertiesAssignedTagsLabel->setStyleClass("soft-text");
    _cntTxtPropertiesAssignedTagsLabel->addWidget(_txtPropertiesAssignedTagsLabel);

    _createPropertiesAssignedTagsTableView();
    _layCntPropertiesAssignedTags->addWidget(_qtvPropertiesAssignedTags, 1);

    //Available Tags
    _cntPropertiesAvailableTags = new Wt::WContainerWidget();

    _layCntPropertiesTags->addWidget(_cntPropertiesAvailableTags);

    _layCntPropertiesAvailableTags = new Wt::WVBoxLayout();
    _layCntPropertiesAvailableTags->setContentsMargins(0,0,0,0);
    _layCntPropertiesAvailableTags->setSpacing(0);

    _cntPropertiesAvailableTags->setLayout(_layCntPropertiesAvailableTags);

    _cntTxtPropertiesAvailableTagsLabel = new Wt::WContainerWidget();
    _cntTxtPropertiesAvailableTagsLabel->setStyleClass("toolbar");
    _cntTxtPropertiesAvailableTagsLabel->setContentAlignment(Wt::AlignCenter);
    _cntTxtPropertiesAvailableTagsLabel->setMinimumSize(Wt::WLength::Auto, 25);

    _layCntPropertiesAvailableTags->addWidget(_cntTxtPropertiesAvailableTagsLabel);

    _txtPropertiesAvailableTagsLabel = new Wt::WText("<b><i>Available Tags</i></b>");
    _txtPropertiesAvailableTagsLabel->setStyleClass("soft-text");
    _cntTxtPropertiesAvailableTagsLabel->addWidget(_txtPropertiesAvailableTagsLabel);

    //Tags Table View
    _createPropertiesTagsTableView();
    _layCntPropertiesAvailableTags->addWidget(_qtvPropertiesTags, 1);

    //Notes Table View
    _createPropertiesNotesTableView();
    _stkProperties->addWidget(_qtvPropertiesNotes);

    //Default Selected Tab
    _mnuPropertiesNavBar->select(_mnuPropertiesNavBarDataItem);
}

void Views::ViewProjects::_createPropertiesDataTableView()
{
    _qtvPropertiesData = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::DboData>(&Database::DatabaseManager::instance());
    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvPropertiesData->createToolButton("", "icons/Add.png", "Add A New Field");
        btn->clicked().connect(this, &Views::ViewProjects::_btnAddPropertiesDataClicked);
    }
    else
        _qtvPropertiesData->setImportCSVFetureEnabled(false);

    //requires "remove" privilege
//    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
//    {
//        Wt::WPushButton *btn = _qtvPropertiesData->createToolButton("", "icons/Remove.png", "Remove Selected Fields");
//        btn->clicked().connect(this, &Views::ViewProjects::_btnRemovePropertiesDataClicked);
//    }

    _updatePropertiesDataView();
}

void Views::ViewProjects::_createPropertiesTagsTableView()
{
    _qtvPropertiesTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(&Database::DatabaseManager::instance());
    _qtvPropertiesTags->setImportCSVFetureEnabled(false);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvPropertiesTags->createToolButton("", "icons/AddTo.png", "Add selected tags to selected items");
        btn->clicked().connect(this, &Views::ViewProjects::_btnAddPropertiesTagClicked);
    }

    _updatePropertiesTagsView();
}

void Views::ViewProjects::_createPropertiesAssignedTagsTableView()
{
    _qtvPropertiesAssignedTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(&Database::DatabaseManager::instance());
    _qtvPropertiesAssignedTags->setImportCSVFetureEnabled(false);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvPropertiesAssignedTags->createToolButton("", "icons/RemoveFrom.png", "Remove selected tags from selected items");
        btn->clicked().connect(this, &Views::ViewProjects::_btnRemovePropertiesTagClicked);
    }

    _updatePropertiesAssignedTagsView();
}

void Views::ViewProjects::_createPropertiesNotesTableView()
{
    _qtvPropertiesNotes = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Note>(&Database::DatabaseManager::instance());
    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvPropertiesNotes->createToolButton("", "icons/Add.png", "Add A New Note");
        btn->clicked().connect(this, &Views::ViewProjects::_btnAddPropertiesNoteClicked);
    }
    else
        _qtvPropertiesNotes->setImportCSVFetureEnabled(false);

    //requires "remove" privilege
//    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
//    {
//        Wt::WPushButton *btn = _qtvPropertiesNotes->createToolButton("", "icons/Remove.png", "Remove Selected Notes");
//        btn->clicked().connect(this, &Views::ViewProjects::_btnRemovePropertiesNotesClicked);
//    }

    _updatePropertiesNotesView();
}

void Views::ViewProjects::_updatePropertiesDataView()
{
    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Auth::AuthManager::instance().currentUser()->editRank();

    _qtvPropertiesData->clearColumns();

    //add columns
    _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("DBOKey", "Key", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("DBOValue", "Value", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::DboData>> query = Database::DatabaseManager::instance().session()->find<Database::DboData>();

    bool update = false;

    if(_stkMain->currentWidget() == _cntProjects)
    {
        if(_qtvProjects->table()->selectedIndexes().size() > 0)
        {
            update = true;
            query.where("Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Projects::Project>(_qtvProjects->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntSequences)
    {
        if(_qtvSequences->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectSequence>(_qtvSequences->selectedItems());

            query.where("project_sequence_Sequence_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(1) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("Project_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("Project_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntShots)
    {
        if(_qtvShots->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectShot>(_qtvShots->selectedItems());

            query.where("Project_Shot_Shot_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Shot_Shot_Sequence_Sequence_Name IN (" + idValues.at(1) + ")"
                        " AND Project_Shot_Shot_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(2) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("Project_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntAssets)
    {
        if(_qtvAssets->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectAsset>(_qtvAssets->selectedItems());

            query.where("Project_Asset_Asset_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Asset_Asset_Project_Project_Name IN (" + idValues.at(1) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("Project_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("Project_Asset_Asset_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntTasks)
    {
        if(_qtvTasks->table()->selectedIndexes().size() > 0)
        {
            update = true;

            query.where("Project_Task_id IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectTask>(_qtvTasks->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("Project_Task_id", "Task ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }

    if(update)
    {
        int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

        if(!AppSettings::instance().isLoadInactiveData())
            query.where("Active = ?").bind(true);

        query.where("View_Rank <= ?").bind(viewRank);
    }
    else
        query.where("id = ?").bind(-1);

    _qtvPropertiesData->setQuery(query);

    if(AppSettings::instance().isShowExtraColumns())
        _addExtraColumns<Database::DboData>(_qtvPropertiesData, flags, editRank);

    _qtvPropertiesData->updateView();
}

void Views::ViewProjects::_updatePropertiesTagsView()
{
    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query;

    int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

    if(AppSettings::instance().isLoadInactiveData())
        query = Database::DatabaseManager::instance().session()->find<Database::Tag>().where("View_Rank <= ?").bind(viewRank);
    else
        query = Database::DatabaseManager::instance().session()->find<Database::Tag>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

    _qtvPropertiesTags->setQuery(query);

    _qtvPropertiesTags->clearColumns();

    //add columns
    _qtvPropertiesTags->addColumn(Ms::Widgets::MTableViewColumn("Tag_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), true));
    _qtvPropertiesTags->addColumn(Ms::Widgets::MTableViewColumn("Tag_Content", "Content", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), true));

    if(AppSettings::instance().isShowExtraColumns())
        _addExtraColumns<Database::Tag>(_qtvPropertiesTags, Wt::ItemIsSelectable, 0);

    _qtvPropertiesTags->removeColumn(Ms::Widgets::MTableViewColumn("Active"));

    _qtvPropertiesTags->updateView();
}

void Views::ViewProjects::_updatePropertiesAssignedTagsView()
{
    if(!Database::DatabaseManager::instance().openTransaction())
            return;

    bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Auth::AuthManager::instance().currentUser()->editRank();

    _qtvPropertiesAssignedTags->clearColumns();

    //add columns
    _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MTableViewColumn("Tag_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MTableViewColumn("Tag_Content", "Content", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query = Database::DatabaseManager::instance().session()->find<Database::Tag>();

    bool update = false;

    if(_stkMain->currentWidget() == _cntProjects)
    {
        if(_qtvProjects->table()->selectedIndexes().size() > 0)
        {
            update = true;
            std::string projectsSelectSql = "pt.project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Projects::Project>(_qtvProjects->selectedItems()).at(0) + ")";
            query.where("Tag_Name IN (SELECT pt.tag_Tag_Name FROM rel_project_tags pt WHERE " +
                    projectsSelectSql + ") AND Tag_Content IN (SELECT pt.tag_Tag_Content FROM rel_project_tags pt WHERE " + projectsSelectSql  + ")");
        }
    }
    else if(_stkMain->currentWidget() == _cntSequences)
    {
        if(_qtvSequences->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectSequence>(_qtvSequences->selectedItems());

            std::string sequencesSelectSql = "pt.project_sequence_Sequence_Name IN (" + idValues.at(0) + ") AND "
                    "pt.project_sequence_Sequence_Project_Project_Name IN (" + idValues.at(1) + ")";

            query.where("Tag_Name IN (SELECT pt.tag_Tag_Name FROM rel_project_sequence_tags pt WHERE " +
                    sequencesSelectSql + ") AND Tag_Content IN (SELECT pt.tag_Tag_Content FROM rel_project_sequence_tags pt WHERE " + sequencesSelectSql  + ")");
        }
    }
    else if(_stkMain->currentWidget() == _cntShots)
    {
        if(_qtvShots->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectShot>(_qtvShots->selectedItems());

            std::string shotsSelectSql = "pt.project_shot_Shot_Name IN (" + idValues.at(0) + ") AND "
                    "pt.project_shot_Shot_Sequence_Sequence_Name IN (" + idValues.at(1) + ") AND "
                    "pt.project_shot_Shot_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(2) + ")";

            query.where("Tag_Name IN (SELECT pt.tag_Tag_Name FROM rel_project_shot_tags pt WHERE " +
                    shotsSelectSql + ") AND Tag_Content IN (SELECT pt.tag_Tag_Content FROM rel_project_shot_tags pt WHERE " + shotsSelectSql  + ")");
        }
    }
    else if(_stkMain->currentWidget() == _cntAssets)
    {
        if(_qtvAssets->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectAsset>(_qtvAssets->selectedItems());

            std::string assetsSelectSql = "pt.project_asset_Asset_Name IN (" + idValues.at(0) + ") AND "
                    "pt.project_asset_Asset_Project_Project_Name IN (" + idValues.at(1) + ")";

            query.where("Tag_Name IN (SELECT pt.tag_Tag_Name FROM rel_project_asset_tags pt WHERE " +
                    assetsSelectSql + ") AND Tag_Content IN (SELECT pt.tag_Tag_Content FROM rel_project_asset_tags pt WHERE " + assetsSelectSql  + ")");
        }
    }
    else if(_stkMain->currentWidget() == _cntTasks)
    {
        if(_qtvTasks->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::string tasksSelectSql = "pt.project_task_id IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectTask>(_qtvTasks->selectedItems()).at(0) + ")";

            query.where("Tag_Name IN (SELECT pt.tag_Tag_Name FROM rel_project_task_tags pt WHERE " +
                    tasksSelectSql + ") AND Tag_Content IN (SELECT pt.tag_Tag_Content FROM rel_project_task_tags pt WHERE " + tasksSelectSql  + ")");
        }
    }

    if(update)
    {
        int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

        if(!AppSettings::instance().isLoadInactiveData())
            query.where("Active = ?").bind(true);

        query.where("View_Rank <= ?").bind(viewRank);
    }
    else
        query.where("Tag_Name = ? AND Tag_Content = ?").bind("").bind("");

    _qtvPropertiesAssignedTags->setQuery(query);

    if(AppSettings::instance().isShowExtraColumns())
        _addExtraColumns<Database::Tag>(_qtvPropertiesAssignedTags, flags, editRank);

    _qtvPropertiesAssignedTags->updateView();
}

void Views::ViewProjects::_updatePropertiesNotesView()
{
    if(!Database::DatabaseManager::instance().openTransaction())
            return;

    bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Auth::AuthManager::instance().currentUser()->editRank();

    _qtvPropertiesNotes->clearColumns();

    //add columns
    _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("id", "Id", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Note>> query = Database::DatabaseManager::instance().session()->find<Database::Note>();

    bool update = false;

    if(_stkMain->currentWidget() == _cntProjects)
    {
        if(_qtvProjects->table()->selectedIndexes().size() > 0)
        {
            update = true;
            query.where("Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Projects::Project>(_qtvProjects->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntSequences)
    {
        if(_qtvSequences->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectSequence>(_qtvSequences->selectedItems());

            query.where("project_sequence_Sequence_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(1) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntShots)
    {
        if(_qtvShots->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectShot>(_qtvShots->selectedItems());

            query.where("Project_Shot_Shot_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Shot_Shot_Sequence_Sequence_Name IN (" + idValues.at(1) + ")"
                        " AND Project_Shot_Shot_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(2) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntAssets)
    {
        if(_qtvAssets->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectAsset>(_qtvAssets->selectedItems());

            query.where("Project_Asset_Asset_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Asset_Asset_Project_Project_Name IN (" + idValues.at(1) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Asset_Asset_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntTasks)
    {
        if(_qtvTasks->table()->selectedIndexes().size() > 0)
        {
            update = true;

            query.where("Project_Task_id IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectTask>(_qtvTasks->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Task_id", "Task ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }

    if(update)
    {
        int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

        if(!AppSettings::instance().isLoadInactiveData())
            query.where("Active = ?").bind(true);

        query.where("View_Rank <= ?").bind(viewRank);
    }
    else
        query.where("id = ?").bind(-1);

    _qtvPropertiesNotes->setQuery(query);

    if(AppSettings::instance().isShowExtraColumns())
        _addExtraColumns<Database::Note>(_qtvPropertiesNotes, flags, editRank);

    _qtvPropertiesNotes->updateView();
}

void Views::ViewProjects::_prepareView()
{
    /*******************--Project--********************/
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _cntTxtProjectsPanelTitle = new Wt::WContainerWidget();
    _cntTxtProjectsPanelTitle->setStyleClass("toolbar");
    _cntTxtProjectsPanelTitle->setContentAlignment(Wt::AlignCenter);
    _cntTxtProjectsPanelTitle->setMinimumSize(Wt::WLength::Auto, 25);

    _layMain->addWidget(_cntTxtProjectsPanelTitle);

    _txtProjectsPanelTitle = new Wt::WText("<b><i>Projects</i></b>");
    _txtProjectsPanelTitle->setStyleClass("soft-text");

    _cntTxtProjectsPanelTitle->addWidget(_txtProjectsPanelTitle);

    _navBarMain= new Wt::WNavigationBar();

    _cntNavBarMain= new Wt::WContainerWidget();
    _cntNavBarMain->addWidget(_navBarMain);

    //add our navigation bar to the view
    _layMain->addWidget(_cntNavBarMain);

    _mnuMain = new Wt::WMenu(Wt::Horizontal);
    _navBarMain->addMenu(_mnuMain);

    _mnuMainProjectsItem = new Wt::WMenuItem("Projects");
    _mnuMainProjectsItem->triggered().connect(this, &Views::ViewProjects::_mnuMainProjectsItemTriggered);
    _mnuMain->addItem(_mnuMainProjectsItem);

    _mnuMainSequencesItem = new Wt::WMenuItem("Sequences");
    _mnuMainSequencesItem->triggered().connect(this, &Views::ViewProjects::_mnuMainSequencesItemTriggered);
    _mnuMain->addItem(_mnuMainSequencesItem);

    _mnuMainShotsItem = new Wt::WMenuItem("Shots");
    _mnuMainShotsItem->triggered().connect(this, &Views::ViewProjects::_mnuMainShotsItemTriggered);
    _mnuMain->addItem(_mnuMainShotsItem);

    _mnuMainAssetsItem = new Wt::WMenuItem("Assets");
    _mnuMainAssetsItem->triggered().connect(this, &Views::ViewProjects::_mnuMainAssetsItemTriggered);
    _mnuMain->addItem(_mnuMainAssetsItem);

    _mnuMainTasksItem = new Wt::WMenuItem("Tasks");
    _mnuMainTasksItem->triggered().connect(this, &Views::ViewProjects::_mnuMainTasksItemTriggered);
    _mnuMain->addItem(_mnuMainTasksItem);

    _stkMain = new Wt::WStackedWidget();

    _layMain->addWidget(_stkMain, 1);

    //Projects//////////////////////////////////////
    _layProjects = new Wt::WVBoxLayout();
    _layProjects->setContentsMargins(0,0,0,0);
    _layProjects->setSpacing(0);

    _cntProjects = new Wt::WContainerWidget();
    _cntProjects->setLayout(_layProjects);

    //add our projects view to the project view
    _stkMain->addWidget(_cntProjects);

    _createProjectsTableView();

    _layProjects->addWidget(_qtvProjects, 1);

    //Sequences//////////////////////////////////////
    _laySequences = new Wt::WVBoxLayout();
    _laySequences->setContentsMargins(0,0,0,0);
    _laySequences->setSpacing(0);

    _cntSequences = new Wt::WContainerWidget();
    _cntSequences->setStyleClass("toolbar");
    _cntSequences->setLayout(_laySequences);

    //add our sequences view to the project view
    _stkMain->addWidget(_cntSequences);

    _createSequencesTableView();

    _laySequences->addWidget(_qtvSequences, 1);

    //Shots///////////////////////////
    _layShots = new Wt::WVBoxLayout();
    _layShots->setContentsMargins(0,0,0,0);
    _layShots->setSpacing(0);

    _cntShots = new Wt::WContainerWidget();
    _cntShots->setLayout(_layShots);

    //add our shots view to the project view
    _stkMain->addWidget(_cntShots);

    _createShotsTableView();

    _layShots->addWidget(_qtvShots, 1);

    //Assets//////////////////////////////////////
    _layAssets = new Wt::WVBoxLayout();
    _layAssets->setContentsMargins(0,0,0,0);
    _layAssets->setSpacing(0);

    _cntAssets = new Wt::WContainerWidget();
    _cntAssets->setLayout(_layAssets);

    //add our Assets view to the project view
    _stkMain->addWidget(_cntAssets);

    _createAssetsTableView();

    _layAssets->addWidget(_qtvAssets, 1);

    //Tasks///////////////////////////
    _layTasks = new Wt::WVBoxLayout();
    _layTasks->setContentsMargins(0,0,0,0);
    _layTasks->setSpacing(0);

    _cntTasks = new Wt::WContainerWidget();
    _cntTasks->setLayout(_layTasks);

    //add our tasks view to the project view
    _stkMain->addWidget(_cntTasks);

    _createTasksTableView();

    _layTasks->addWidget(_qtvTasks, 1);

    //Properties View
    _createPropertiesView();
}
