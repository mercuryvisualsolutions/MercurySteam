#ifndef VIEWPROPERTIES_IMPL_H
#define VIEWPROPERTIES_IMPL_H

#include "propertiesdialogs.h"

#include "../../Auth/authmanager.h"

namespace Views
{
    template<typename T>
    void Views::ViewProperties::updateDboPropertiesView(const std::vector<Wt::Dbo::ptr<T>> &dboVec) const
    {
        if(_stkProperties->currentWidget() == _qtvData)
        {
            updateDboDataView<T>(dboVec);
        }
        else if(_stkProperties->currentWidget() == _cntTags)
        {
            updateTagsView();
            updateDboTagsView<T>(dboVec);
        }
        else if(_stkProperties->currentWidget() == _qtvNotes)
        {
            updateDboNotesView<T>(dboVec);
        }
    }

    template<typename T>
    void Views::ViewProperties::updateDboDataView(const std::vector<Wt::Dbo::ptr<T>> &dboVec) const
    {
        if(_qtvData->isHidden())
            return;

        try
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

            _qtvData->clearColumns();

            //add columns
            _qtvData->addColumn(Ms::Widgets::MTableViewColumn("DBOKey", "Key", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            _qtvData->addColumn(Ms::Widgets::MTableViewColumn("DBOValue", "Value", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

            Wt::Dbo::Query<Wt::Dbo::ptr<Database::DboData>> query = Database::DatabaseManager::instance().session()->find<Database::DboData>();

            if(typeid(T) == typeid(Projects::Project))
            {
                if(dboVec.size() > 0)
                {
                    //generate the where clause
                    query.where("Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")");
                }

                _qtvData->addColumn(Ms::Widgets::MTableViewColumn("Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            }
            else if(typeid(T) == typeid(Projects::ProjectSequence))
            {
                if(dboVec.size() > 0)
                {
                    //generate the where clause for selected sequences
                    query.where("project_sequence_Sequence_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")"
                                " AND Project_Sequence_Sequence_Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(1) + ")");
                }

                _qtvData->addColumn(Ms::Widgets::MTableViewColumn("Project_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
                _qtvData->addColumn(Ms::Widgets::MTableViewColumn("Project_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            }
            else if(typeid(T) == typeid(Projects::ProjectShot))
            {
                if(dboVec.size() > 0)
                {
                    //generate the where clause for selected shots
                    query.where("Project_Shot_Shot_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")"
                                " AND Project_Shot_Shot_Sequence_Sequence_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(1) + ")"
                                " AND Project_Shot_Shot_Sequence_Sequence_Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(2) + ")");
                }

                _qtvData->addColumn(Ms::Widgets::MTableViewColumn("Project_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
                _qtvData->addColumn(Ms::Widgets::MTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
                _qtvData->addColumn(Ms::Widgets::MTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            }
            else if(typeid(T) == typeid(Projects::ProjectAsset))
            {
                if(dboVec.size() > 0)
                {
                    //generate the where clause for selected assets
                    query.where("Project_Asset_Asset_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")"
                                " AND Project_Asset_Asset_Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(1) + ")");
                }

                _qtvData->addColumn(Ms::Widgets::MTableViewColumn("Project_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
                _qtvData->addColumn(Ms::Widgets::MTableViewColumn("Project_Asset_Asset_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            }
            else if(typeid(T) == typeid(Projects::ProjectTask))
            {
                if(dboVec.size() > 0)
                {
                    //generate the where clause for selected assets
                    query.where("Project_Task_id IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")");
                }

                _qtvData->addColumn(Ms::Widgets::MTableViewColumn("Project_Task_id", "Task ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            }
            else if(typeid(T) == typeid(Users::User))
            {
                if(dboVec.size() > 0)
                {
                    //generate the where clause
                    query.where("User_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")");
                }

                _qtvData->addColumn(Ms::Widgets::MTableViewColumn("User_Name", "User Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            }
            else if(typeid(T) == typeid(Users::Group))
            {
                if(dboVec.size() > 0)
                {
                    //generate the where clause
                    query.where("Group_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")");
                }

                _qtvData->addColumn(Ms::Widgets::MTableViewColumn("Group_Name", "Group Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            }

            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);

            if(dboVec.size() > 0)
            {
                if(!AppSettings::instance().isLoadInactiveData())
                    query.where("Active = ?").bind(true);

                query.where("View_Rank <= ?").bind(viewRank);
            }
            else
                query.where("id = ?").bind(-1);//get DboData with id = -1 to clear the view

            _qtvData->setQuery(query);

            if(AppSettings::instance().isShowExtraColumns())
                _addExtraColumns<Database::DboData>(_qtvData, flags, editRank);

            _qtvData->updateView();
        }
        catch(...)
        {
            std::cerr << "Exception occured while updating data table view" << std::endl;
        }
    }

    template<typename T>
    void Views::ViewProperties::updateDboTagsView(const std::vector<Wt::Dbo::ptr<T>> &dboVec) const
    {
        if(_qtvAssignedTags->isHidden())
            return;

        try
        {
            if(!Database::DatabaseManager::instance().openTransaction())
                return;

            int editRank = Auth::AuthManager::instance().currentUser()->editRank();

            _qtvAssignedTags->clearColumns();

            //add columns
            _qtvAssignedTags->addColumn(Ms::Widgets::MTableViewColumn("Tag_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            _qtvAssignedTags->addColumn(Ms::Widgets::MTableViewColumn("Tag_Content", "Content", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

            Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query = Database::DatabaseManager::instance().session()->find<Database::Tag>();

            if(typeid(T) == typeid(Projects::Project))
            {
                if(dboVec.size() > 0)
                {
                    std::string projectsSelectSql = "pt.project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")";
                    query.where("Tag_Name IN (SELECT pt.tag_Tag_Name FROM rel_project_tags pt WHERE " +
                            projectsSelectSql + ") AND Tag_Content IN (SELECT pt.tag_Tag_Content FROM rel_project_tags pt WHERE " + projectsSelectSql  + ")");
                }
            }
            else if(typeid(T) == typeid(Projects::ProjectSequence))
            {
                if(dboVec.size() > 0)
                {
                    std::string sequencesSelectSql = "pt.project_sequence_Sequence_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ") AND "
                            "pt.project_sequence_Sequence_Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(1) + ")";

                    query.where("Tag_Name IN (SELECT pt.tag_Tag_Name FROM rel_project_sequence_tags pt WHERE " +
                            sequencesSelectSql + ") AND Tag_Content IN (SELECT pt.tag_Tag_Content FROM rel_project_sequence_tags pt WHERE " + sequencesSelectSql  + ")");
                }
            }
            else if(typeid(T) == typeid(Projects::ProjectShot))
            {
                if(dboVec.size() > 0)
                {
                    std::string shotsSelectSql = "pt.project_shot_Shot_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ") AND "
                            "pt.project_shot_Shot_Sequence_Sequence_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(1) + ") AND "
                            "pt.project_shot_Shot_Sequence_Sequence_Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(2) + ")";

                    query.where("Tag_Name IN (SELECT pt.tag_Tag_Name FROM rel_project_shot_tags pt WHERE " +
                            shotsSelectSql + ") AND Tag_Content IN (SELECT pt.tag_Tag_Content FROM rel_project_shot_tags pt WHERE " + shotsSelectSql  + ")");
                }
            }
            else if(typeid(T) == typeid(Projects::ProjectAsset))
            {
                if(dboVec.size() > 0)
                {
                    std::string assetsSelectSql = "pt.project_asset_Asset_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ") AND "
                            "pt.project_asset_Asset_Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(1) + ")";

                    query.where("Tag_Name IN (SELECT pt.tag_Tag_Name FROM rel_project_asset_tags pt WHERE " +
                            assetsSelectSql + ") AND Tag_Content IN (SELECT pt.tag_Tag_Content FROM rel_project_asset_tags pt WHERE " + assetsSelectSql  + ")");
                }
            }
            else if(typeid(T) == typeid(Projects::ProjectTask))
            {
                if(dboVec.size() > 0)
                {
                    std::string tasksSelectSql = "pt.project_task_id IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")";

                    query.where("Tag_Name IN (SELECT pt.tag_Tag_Name FROM rel_project_task_tags pt WHERE " +
                            tasksSelectSql + ") AND Tag_Content IN (SELECT pt.tag_Tag_Content FROM rel_project_task_tags pt WHERE " + tasksSelectSql  + ")");
                }
            }
            else if(typeid(T) == typeid(Users::User))
            {
                if(dboVec.size() > 0)
                {
                    std::string usersSelectSql = "pt.user_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")";

                    query.where("Tag_Name IN (SELECT pt.tag_Tag_Name FROM rel_user_tags pt WHERE " +
                            usersSelectSql + ") AND Tag_Content IN (SELECT pt.tag_Tag_Content FROM rel_user_tags pt WHERE " + usersSelectSql  + ")");
                }
            }
            else if(typeid(T) == typeid(Users::Group))
            {
                if(dboVec.size() > 0)
                {
                    std::string groupsSelectSql = "pt.group_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")";

                    query.where("Tag_Name IN (SELECT pt.tag_Tag_Name FROM rel_group_tags pt WHERE " +
                            groupsSelectSql + ") AND Tag_Content IN (SELECT pt.tag_Tag_Content FROM rel_group_tags pt WHERE " + groupsSelectSql  + ")");
                }
            }

            if(dboVec.size() > 0)
            {
                if(!AppSettings::instance().isLoadInactiveData())
                    query.where("Active = ?").bind(true);

                int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();
                query.where("View_Rank <= ?").bind(viewRank);
            }
            else
                query.where("Tag_Name = ? AND Tag_Content = ?").bind("").bind("");//get tag with name and content = * to clear the view

            _qtvAssignedTags->setQuery(query);

            if(AppSettings::instance().isShowExtraColumns())
                _addExtraColumns<Database::Tag>(_qtvAssignedTags, Wt::ItemIsSelectable, editRank);

            _qtvAssignedTags->removeColumn(Ms::Widgets::MTableViewColumn("Active"));

            _qtvAssignedTags->updateView();
        }
        catch(...)
        {
            std::cerr << "Exception occured while updating tags table view" << std::endl;
        }
    }

    template<typename T>
    void Views::ViewProperties::updateDboNotesView(const std::vector<Wt::Dbo::ptr<T>> &dboVec) const
    {
        if(_qtvNotes->isHidden())
            return;

        try
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

            _qtvNotes->clearColumns();

            //add columns
            _qtvNotes->addColumn(Ms::Widgets::MTableViewColumn("id", "Id", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            _qtvNotes->addColumn(Ms::Widgets::MTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

            Wt::Dbo::Query<Wt::Dbo::ptr<Database::Note>> query = Database::DatabaseManager::instance().session()->find<Database::Note>();

            if(typeid(T) == typeid(Projects::Project))
            {
                if(dboVec.size() > 0)
                {
                    //generate the where clause
                    query.where("Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")");
                }

                _qtvNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            }
            else if(typeid(T) == typeid(Projects::ProjectSequence))
            {
                if(dboVec.size() > 0)
                {
                    //generate the where clause for selected sequences
                    query.where("project_sequence_Sequence_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")"
                                " AND Project_Sequence_Sequence_Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(1) + ")");
                }

                _qtvNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
                _qtvNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            }
            else if(typeid(T) == typeid(Projects::ProjectShot))
            {
                if(dboVec.size() > 0)
                {
                    //generate the where clause for selected shots
                    query.where("Project_Shot_Shot_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")"
                                " AND Project_Shot_Shot_Sequence_Sequence_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(1) + ")"
                                " AND Project_Shot_Shot_Sequence_Sequence_Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(2) + ")");
                }

                _qtvNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
                _qtvNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
                _qtvNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            }
            else if(typeid(T) == typeid(Projects::ProjectAsset))
            {
                if(dboVec.size() > 0)
                {
                    //generate the where clause for selected assets
                    query.where("Project_Asset_Asset_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")"
                                " AND Project_Asset_Asset_Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(1) + ")");
                }

                _qtvNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
                _qtvNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Asset_Asset_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            }
            else if(typeid(T) == typeid(Projects::ProjectTask))
            {
                if(dboVec.size() > 0)
                {
                    //generate the where clause for selected assets
                    query.where("Project_Task_id IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")");
                }

                _qtvNotes->addColumn(Ms::Widgets::MTableViewColumn("Project_Task_id", "Task ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            }
            else if(typeid(T) == typeid(Users::User))
            {
                if(dboVec.size() > 0)
                {
                    //generate the where clause
                    query.where("User_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")");
                }

                _qtvNotes->addColumn(Ms::Widgets::MTableViewColumn("User_Name", "User Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            }
            else if(typeid(T) == typeid(Users::Group))
            {
                if(dboVec.size() > 0)
                {
                    //generate the where clause
                    query.where("Group_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<T>(dboVec).at(0) + ")");
                }

                _qtvNotes->addColumn(Ms::Widgets::MTableViewColumn("Group_Name", "Group Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            }

            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);

            if(dboVec.size() > 0)
            {
                if(!AppSettings::instance().isLoadInactiveData())
                    query.where("Active = ?").bind(true);

                query.where("View_Rank <= ?").bind(viewRank);
            }
            else
                query.where("id = ?").bind(-1);//get DboData with id = -1 to clear the view

            _qtvNotes->setQuery(query);

            if(AppSettings::instance().isShowExtraColumns())
                _addExtraColumns<Database::Note>(_qtvNotes, flags, editRank);

            _qtvNotes->updateView();
        }
        catch(...)
        {
            std::cerr << "Exception occured while updating notes table view" << std::endl;
        }
    }

    template<typename T>
    void Views::ViewProperties::addDataToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec) const
    {
        try
        {
            Views::DlgCreateDBOData *dlg = new Views::DlgCreateDBOData();
            dlg->finished().connect(std::bind([=]()
            {
               if(dlg->result() == Wt::WDialog::Accepted)
               {
                   for(auto &ptr : dboVec)
                   {
                       Wt::Dbo::ptr<Database::DboData> dataPtr =
                               Database::DatabaseManager::instance().createDbo<Database::DboData>(new Database::DboData(dlg->key(), dlg->value()));

                       if(dataPtr)
                           Database::DatabaseManager::instance().addDataToDbo<T>(ptr, dataPtr);
                   }

                   updateDboDataView<T>(dboVec);
               }

               delete dlg;
            }));

            dlg->show();
        }
        catch(...)
        {
            std::cerr << "Exception occured while adding data to dbo" << std::endl;
        }
    }

    template<typename T>
    void Views::ViewProperties::addNoteToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec) const
    {
        try
        {
            Views::DlgCreateNote *dlg = new Views::DlgCreateNote();
            dlg->finished().connect(std::bind([=]()
            {
               if(dlg->result() == Wt::WDialog::Accepted)
               {
                   for(auto &ptr : dboVec)
                   {
                       Wt::Dbo::ptr<Database::Note> notePtr =
                               Database::DatabaseManager::instance().createDbo<Database::Note>(new Database::Note(dlg->content()));

                       if(notePtr)
                           Database::DatabaseManager::instance().addNoteToDbo<T>(ptr, notePtr);
                   }

                   updateDboNotesView<T>(dboVec);
               }

               delete dlg;
            }));

            dlg->show();
        }
        catch(...)
        {
            std::cerr << "Exception occured while adding note to dbo" << std::endl;
        }
    }

    template<typename T>
    void Views::ViewProperties::addTagsToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec) const
    {
        if(dboVec.size() > 0)
        {
            for(auto &dboPtr : dboVec)
            {
                for(auto &tagPtr : tagVec)
                {
                    Database::DatabaseManager::instance().addTagToDbo<T>(dboPtr, tagPtr);
                }
            }

            updateDboPropertiesView<T>(dboVec);
        }
    }

    template<typename T>
    void Views::ViewProperties::removeTagsFromDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec) const
    {
        if(dboVec.size() > 0)
        {
            for(auto &dboPtr : dboVec)
            {
                for(auto &tagPtr : tagVec)
                {
                    Database::DatabaseManager::instance().removeTagFromDbo<T>(dboPtr, tagPtr);
                }
            }

            updateDboPropertiesView<T>(dboVec);
        }
    }

    template<typename T>
    void Views::ViewProperties::_addExtraColumns(Ms::Widgets::MQueryTableViewWidget<T> *widget, Wt::WFlags<Wt::ItemFlag> flags, int editRank) const
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
}

#endif // VIEWPROPERTIES_IMPL_H
