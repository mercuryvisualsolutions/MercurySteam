#include "viewtasks.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"
#include "../../Projects/projectsio.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Log/logmanager.h"
#include "dlgtaskselectdbo.h"
#include "dlgcreateandedittask.h"

#include <Wt/WApplication>

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewTasks::ViewTasks()
{
    _logger = Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId());

    _prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *Views::ViewTasks::qtvTasks() const
{
    return _qtvTasks;
}

void Views::ViewTasks::updateView(const std::vector<Wt::Dbo::ptr<Projects::Project>> &prjVec,
                                  const std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>> &seqVec,
                                  const std::vector<Wt::Dbo::ptr<Projects::ProjectShot>> &shotVec,
                                  const std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>> &assetVec) const
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTask>> query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTask>();
        bool multiDbo = false;
        bool update = false;

        std::string strQuery = "";

        if(prjVec.size() > 0)
        {
            update = true;

            std::vector<std::string> projectsIdValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::Project>(prjVec);

            strQuery = "(Task_Project_Project_Name IN (" + projectsIdValues.at(0) + "))";
        }

        if(seqVec.size() > 0)
        {
            if(update)
                multiDbo = true;
            else
                update = true;

            std::vector<std::string> sequencesIdValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectSequence>(seqVec);;

            if(multiDbo)
                strQuery += " OR ";

            strQuery += "(Task_Sequence_Sequence_Name IN (" + sequencesIdValues.at(0) + ") AND Task_Sequence_Sequence_Project_Project_Name IN (" + sequencesIdValues.at(1) + "))";
        }

        if(shotVec.size() > 0)
        {
            if(update)
                multiDbo = true;
            else
                update = true;

            std::vector<std::string> shotsIdValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectShot>(shotVec);

            if(multiDbo)
                strQuery += " OR ";

            strQuery += "(Task_Shot_Shot_Name IN (" + shotsIdValues.at(0) + ") AND Task_Shot_Shot_Sequence_Sequence_Name IN (" + shotsIdValues.at(1) + ") AND Task_Shot_Shot_Sequence_Sequence_Project_Project_Name IN (" + shotsIdValues.at(2) + "))";
        }

        if(assetVec.size() > 0)
        {
            if(update)
                multiDbo = true;
            else
                update = true;

            std::vector<std::string> assetsIdValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectAsset>(assetVec);

            if(multiDbo)
                strQuery += " OR ";

            strQuery += "(Task_Asset_Asset_Name IN (" + assetsIdValues.at(0) + ") AND Task_Asset_Asset_Project_Project_Name IN (" + assetsIdValues.at(1) + "))";
        }

        if(update == true)
        {
            query.where("(" + strQuery + ")");

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
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Progress_Share_Name", "Progress Share", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectProgressShare>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectProgressShare>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectProgressShare>().where("Active = ?").bind(true),
         "Name", editRank)));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("End_Date", "End Date",flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(seqVec.size() > 0)
        {
            _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Sequence_Sequence_Project_Project_Name", "Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        }

        if(shotVec.size() > 0)
        {
            _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Shot_Shot_Sequence_Sequence_Name", "Shot Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Shot Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        }

        if(assetVec.size() > 0)
        {
            _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Task_Asset_Asset_Project_Project_Name", "Asset Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        }
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Priority", "Priority", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));
        _qtvTasks->addColumn(Ms::Widgets::MTableViewColumn("Accepted_By_User", "Accepted By User", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));

        if(AppSettings::instance().isShowExtraColumns())
            _qtvTasks->addBaseColumns(flags, editRank);

        _qtvTasks->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating tasks view" << std::endl;
    }
}

bool Views::ViewTasks::isCreateOptionHidden()
{
    return _btnCreateTask->isHidden();
}

void Views::ViewTasks::setCreateOptionHidden(bool hidden) const
{
    _btnCreateTask->setHidden(hidden);
}

bool Views::ViewTasks::isEditOptionHidden()
{
    return _btnEditTasks->isHidden();
}

void Views::ViewTasks::setEditOptionHidden(bool hidden) const
{
    _btnEditTasks->setHidden(hidden);
}

//bool Views::ViewTasks::isRemoveOptionHidden()
//{
//    return _btnRemoveTasks->isHidden();
//}

//void Views::ViewTasks::setRemoveOptionHidden(bool hidden) const
//{
//    _btnRemoveTasks->setHidden(hidden);
//}

bool Views::ViewTasks::isOpenFilesOptionHidden()
{
    return _btnOpenFilesView->isHidden();
}

void Views::ViewTasks::setOpenFilesOptionHidden(bool hidden) const
{
    _btnOpenFilesView->setHidden(hidden);
}

Wt::Signal<> &Views::ViewTasks::createTaskRequested()
{
    return _createTaskRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> &Views::ViewTasks::removeTasksRequested()
{
    return _removeTasksRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> &Views::ViewTasks::openfilesViewRequested()
{
    return _openfilesViewRequested;
}

void Views::ViewTasks::_btnCreateTaskClicked()
{
    _createTaskRequested();
}

void Views::ViewTasks::_btnRemoveTasksClicked()
{
    _removeTasksRequested(_qtvTasks->selectedItems());
}

void Views::ViewTasks::_btnEditTasksClicked()
{
    if(_qtvTasks->table()->selectedIndexes().size() == 0)
    {
        _logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditTask *dlg = new Views::DlgCreateAndEditTask(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            for(auto taskPtr : _qtvTasks->selectedItems())
            {
                if(dlg->editedStartDate())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTask>(taskPtr)->setStartDate(dlg->startDate());

                if(dlg->editedEndDate())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTask>(taskPtr)->setEndDate(dlg->endDate());

                if(dlg->editedType())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTask>(taskPtr)->setType(dlg->type());

                if(dlg->editedPriority())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTask>(taskPtr)->setPriority(dlg->priority());

                if(dlg->editedStatus())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTask>(taskPtr)->setStatus(dlg->status());

                if(dlg->editedStatus())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTask>(taskPtr)->setUser(dlg->user());

                if(dlg->editedDescription())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTask>(taskPtr)->setDescription(dlg->description());

                if(dlg->editedActive())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTask>(taskPtr)->setActive(dlg->isActive());
            }

            _qtvTasks->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewTasks::_btnOpenFilesViewClicked()
{
    if(_qtvTasks->table()->selectedIndexes().size() != 1)
    {
        _logger->log("Please select only one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgTaskSelectDbo *dlgSelectDbo = new Views::DlgTaskSelectDbo();

    dlgSelectDbo->finished().connect(std::bind([=]()
    {
        if(dlgSelectDbo->result() == Wt::WDialog::Accepted)
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Wt::Dbo::ptr<Projects::ProjectTask> taskPtr =  _qtvTasks->selectedItems().at(0);

                std::string path = "";

                if(dlgSelectDbo->type() == "Project")
                {
                    if(taskPtr->project().get())
                        path = Projects::ProjectsIO::getRelativeProjectTaskDir(taskPtr->project()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                    else
                        _logger->log("Task does not belong to a project.", Ms::Log::LogMessageType::Warning);
                }
                else if(dlgSelectDbo->type() == "Sequence")
                {
                    if(taskPtr->sequence().get())
                        path = Projects::ProjectsIO::getRelativeSequenceTaskDir(taskPtr->sequence()->projectName(), taskPtr->sequence()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                    else
                        _logger->log("Task does not belong to a sequence.", Ms::Log::LogMessageType::Warning);
                }
                else if(dlgSelectDbo->type() == "Shot")
                {
                    if(taskPtr->shot().get())
                        path = Projects::ProjectsIO::getRelativeShotTaskDir(taskPtr->shot()->projectName(), taskPtr->shot()->sequenceName(), taskPtr->shot()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                    else
                        _logger->log("Task does not belong to a shot.", Ms::Log::LogMessageType::Warning);
                }
                else if(dlgSelectDbo->type() == "Asset")
                {
                    if(taskPtr->asset().get())
                        path = Projects::ProjectsIO::getRelativeAssetTaskDir(taskPtr->asset()->projectName(), taskPtr->asset()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                    else
                        _logger->log("Task does not belong to an asset.", Ms::Log::LogMessageType::Warning);
                }

                if(path != "")
                {
                    DlgFilesManager *dlgFiles = new DlgFilesManager(path);
                    dlgFiles->finished().connect(std::bind([=]()
                    {
                        delete dlgFiles;
                    }));

                    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
                        dlgFiles->setCreateDisabled(true);
                    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In"))
                        dlgFiles->setCheckInDisabled(true);
                    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out"))
                        dlgFiles->setCheckOutDisabled(true);

                    dlgFiles->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));

                    _openfilesViewRequested(_qtvTasks->selectedItems());
                }
            }
        }

        delete dlgSelectDbo;
    }));

    dlgSelectDbo->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewTasks::_createTasksTableView()
{
    _qtvTasks = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTask>(&Database::DatabaseManager::instance());

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        _btnCreateTask = _qtvTasks->createToolButton("", "icons/Add.png", "Create A New Task");
        _btnCreateTask->clicked().connect(this, &Views::ViewTasks::_btnCreateTaskClicked);

        _qtvTasks->setImportCSVFeatureEnabled(true);
    }
    else
        _qtvTasks->setImportCSVFeatureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //_btnRemoveTasks = _qtvTasks->createToolButton("", "icons/Remove.png", "Remove Selected Tasks");
        //_btnRemoveTasks->clicked().connect(this, &Views::ViewTasks::_btnRemoveTasksClicked);
    }

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        _btnEditTasks = _qtvTasks->createToolButton("", "icons/Edit.png", "Edit Selected Tasks");
        _btnEditTasks->clicked().connect(this, &Views::ViewTasks::_btnEditTasksClicked);
    }

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
    {
        _btnOpenFilesView = _qtvTasks->createToolButton("", "icons/Files.png", "Files Manager");
        _btnOpenFilesView->clicked().connect(this, &Views::ViewTasks::_btnOpenFilesViewClicked);
    }
}

void Views::ViewTasks::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _createTasksTableView();

    _layMain->addWidget(_qtvTasks);
}
