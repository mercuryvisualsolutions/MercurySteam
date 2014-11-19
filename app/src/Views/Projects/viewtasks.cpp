#include "viewtasks.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"
#include "../../Projects/projectsio.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Log/logmanager.h"
#include "dlgtaskselectdbo.h"
#include "dlgcreateandedittask.h"

#include <Wt/WApplication>

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewTasks::ViewTasks()
{
    _logger = Session::SessionManager::instance().logger();

    _prepareView();

    adjustUIPrivileges();
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *Views::ViewTasks::qtvTasks()
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
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTask>> query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTask>();
        bool multiDbo = false;
        bool update = false;

        std::string strQuery = "";

        if(prjVec.size() > 0)
        {
            update = true;

            std::vector<std::string> projectsIdValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::Project>(prjVec);

            strQuery = "(Task_Project_Project_Name IN (" + projectsIdValues.at(0) + "))";
        }

        if(seqVec.size() > 0)
        {
            if(update)
                multiDbo = true;
            else
                update = true;

            std::vector<std::string> sequencesIdValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectSequence>(seqVec);;

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

            std::vector<std::string> shotsIdValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectShot>(shotVec);

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

            std::vector<std::string> assetsIdValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectAsset>(assetVec);

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

            int viewRank = Session::SessionManager::instance().user()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("id = ?").bind(-1);//clear the view

        _qtvTasks->setQuery(query);

        transaction.commit();

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        _qtvTasks->clearColumns();

        //add columns
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Type", "Type", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectTaskType>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskType>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskType>().where("Active = ?").bind(true),
         "Type", editRank), true));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_User_Name", "Asignee", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Users::User>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Users::User>() :
         Session::SessionManager::instance().dboSession().find<Users::User>().where("Active = ?").bind(true),
         "Name", editRank), true));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true),
         "Status", editRank)));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("End_Date", "End Date",flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(seqVec.size() > 0)
        {
            _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Sequence_Sequence_Project_Project_Name", "Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        }

        if(shotVec.size() > 0)
        {
            _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Shot_Shot_Sequence_Sequence_Name", "Shot Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Shot Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        }

        if(assetVec.size() > 0)
        {
            _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Asset_Asset_Project_Project_Name", "Asset Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        }
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Priority", "Priority", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));
        _qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Accepted_By_User", "Accepted By User", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));

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
    if(_btnCreateTask)
        return _btnCreateTask->isHidden();

    return false;
}

void Views::ViewTasks::setCreateOptionHidden(bool hidden) const
{
    if(_btnCreateTask)
        _btnCreateTask->setHidden(hidden);
}

bool Views::ViewTasks::isCreateForTemplateOptionHidden()
{
    if(_btnCreateTasksForTemplate)
        return _btnCreateTasksForTemplate->isHidden();

    return false;
}

void Views::ViewTasks::setCreateForTemplateOptionHidden(bool hidden) const
{
    if(_btnCreateTasksForTemplate)
        _btnCreateTasksForTemplate->setHidden(hidden);
}

bool Views::ViewTasks::isEditOptionHidden()
{
    if(_btnEditTasks)
        return _btnEditTasks->isHidden();

    return false;
}

void Views::ViewTasks::setEditOptionHidden(bool hidden) const
{
    if(_btnEditTasks)
        _btnEditTasks->setHidden(hidden);
}

//bool Views::ViewTasks::isRemoveOptionHidden()
//{
//    if(_btnRemoveTasks)
//        return _btnRemoveTasks->isHidden();

//    return false;
//}

//void Views::ViewTasks::setRemoveOptionHidden(bool hidden) const
//{
//    if(_btnRemoveTasks)
//        _btnRemoveTasks->setHidden(hidden);
//}

bool Views::ViewTasks::isOpenFilesOptionHidden()
{
    if(_btnOpenFilesView)
        return _btnOpenFilesView->isHidden();

    return false;
}

void Views::ViewTasks::setOpenFilesOptionHidden(bool hidden) const
{
    if(_btnOpenFilesView)
        _btnOpenFilesView->setHidden(hidden);
}

void Views::ViewTasks::adjustUIPrivileges()
{
    Wt::Dbo::ptr<Users::User> user = Session::SessionManager::instance().user();

    bool hasViewFilesPriv = user->hasPrivilege("View Files");
    bool hasEditPriv = user->hasPrivilege("Edit");
    bool hasCreateProjectsPriv = user->hasPrivilege("Create Projects");
    bool hasCheckInPriv = user->hasPrivilege("Check In");
    bool hasCheckOutPriv = user->hasPrivilege("Check Out");
    bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

    _btnCreateTask->setHidden(!hasCreateProjectsPriv);
    _btnCreateTasksForTemplate->setHidden(!hasCreateProjectsPriv);
    _btnEditTasks->setHidden(!hasEditPriv);

    _qtvTasks->setImportCSVFeatureEnabled(hasCreateProjectsPriv);

    bool showTaskFilesButton = hasViewFilesPriv || hasCheckInPriv || hasCheckOutPriv || hasCreateRepoPriv;//if have any of the privileges
    _btnOpenFilesView->setHidden(!showTaskFilesButton);
}

Wt::Signal<> &Views::ViewTasks::createTaskRequested()
{
    return _createTaskRequested;
}

Wt::Signal<> &Views::ViewTasks::createTasksForTemplateRequested()
{
    return _createTasksForTemplateRequested;
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

void Views::ViewTasks::_btnCreateTasksForTemplateClicked()
{
    _createTasksForTemplateRequested();
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
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setStartDate(dlg->startDate());

                if(dlg->editedEndDate())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setEndDate(dlg->endDate());

                if(dlg->editedType())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setType(dlg->type());

                if(dlg->editedPriority())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setPriority(dlg->priority());

                if(dlg->editedStatus())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setStatus(dlg->status());

                if(dlg->editedUser())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setUser(dlg->user());

                if(dlg->editedDescription())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setDescription(dlg->description());

                if(dlg->editedActive())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setActive(dlg->isActive());
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
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

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

                Wt::Dbo::ptr<Users::User> user = Session::SessionManager::instance().user();

                bool hasViewFilesPriv = user->hasPrivilege("View Files");
                bool hasCheckInPriv = user->hasPrivilege("Check In");
                bool hasCheckOutPriv = user->hasPrivilege("Check Out");
                bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

                dlgFiles->setViewDisabled(!hasViewFilesPriv);
                dlgFiles->setCreateDisabled(!hasCreateRepoPriv);
                dlgFiles->setCheckInDisabled(!hasCheckInPriv);
                dlgFiles->setCheckOutDisabled(!hasCheckOutPriv);

                dlgFiles->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));

                _openfilesViewRequested(_qtvTasks->selectedItems());
            }

            transaction.commit();
        }

        delete dlgSelectDbo;
    }));

    dlgSelectDbo->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewTasks::_createTasksTableView()
{
    _qtvTasks = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTask>(Session::SessionManager::instance().dboSession());

    _btnCreateTask = _qtvTasks->createToolButton("", "icons/Add.png", "Create A New Task");
    _btnCreateTask->clicked().connect(this, &Views::ViewTasks::_btnCreateTaskClicked);

    _btnCreateTasksForTemplate = _qtvTasks->createToolButton("", "icons/Template.png", "Create Tasks Based On Template");
    _btnCreateTasksForTemplate->clicked().connect(this, &Views::ViewTasks::_btnCreateTasksForTemplateClicked);

    //_btnRemoveTasks = _qtvTasks->createToolButton("", "icons/Remove.png", "Remove Selected Tasks");
    //_btnRemoveTasks->clicked().connect(this, &Views::ViewTasks::_btnRemoveTasksClicked);

    _btnEditTasks = _qtvTasks->createToolButton("", "icons/Edit.png", "Edit Selected Tasks");
    _btnEditTasks->clicked().connect(this, &Views::ViewTasks::_btnEditTasksClicked);

    _btnOpenFilesView = _qtvTasks->createToolButton("", "icons/Files.png", "Files Manager");
    _btnOpenFilesView->clicked().connect(this, &Views::ViewTasks::_btnOpenFilesViewClicked);
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
