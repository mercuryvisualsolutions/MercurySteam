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
    m_logger = Session::SessionManager::instance().logger();

    prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *Views::ViewTasks::qtvTasks()
{
    return m_qtvTasks;
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

        m_qtvTasks->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        m_qtvTasks->clearColumns();

        //add columns
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Type", "Type", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectTaskType>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskType>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskType>().where("Active = ?").bind(true),
         "Type", editRank), true));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_User_Name", "Asignee", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Users::User>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Users::User>() :
         Session::SessionManager::instance().dboSession().find<Users::User>().where("Active = ?").bind(true),
         "Name", editRank), true));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true),
         "Status", editRank)));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("End_Date", "End Date",flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(seqVec.size() > 0)
        {
            m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Sequence_Sequence_Project_Project_Name", "Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        }

        if(shotVec.size() > 0)
        {
            m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Shot_Shot_Sequence_Sequence_Name", "Shot Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Shot Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        }

        if(assetVec.size() > 0)
        {
            m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Task_Asset_Asset_Project_Project_Name", "Asset Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        }
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Priority", "Priority", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));
        m_qtvTasks->addColumn(Ms::Widgets::MQueryTableViewColumn("Accepted_By_User", "Accepted By User", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));

        if(AppSettings::instance().isShowExtraColumns())
            m_qtvTasks->addBaseColumns(flags, editRank);

        transaction.commit();

        m_qtvTasks->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating tasks view" << std::endl;
    }
}

bool Views::ViewTasks::isCreateOptionHidden()
{
    if(m_btnCreateTask)
        return m_btnCreateTask->isHidden();

    return false;
}

void Views::ViewTasks::setCreateOptionHidden(bool hidden) const
{
    if(m_btnCreateTask)
        m_btnCreateTask->setHidden(hidden);
}

bool Views::ViewTasks::isCreateForTemplateOptionHidden()
{
    if(m_btnCreateTasksForTemplate)
        return m_btnCreateTasksForTemplate->isHidden();

    return false;
}

void Views::ViewTasks::setCreateForTemplateOptionHidden(bool hidden) const
{
    if(m_btnCreateTasksForTemplate)
        m_btnCreateTasksForTemplate->setHidden(hidden);
}

bool Views::ViewTasks::isEditOptionHidden()
{
    if(m_btnEditTasks)
        return m_btnEditTasks->isHidden();

    return false;
}

void Views::ViewTasks::setEditOptionHidden(bool hidden) const
{
    if(m_btnEditTasks)
        m_btnEditTasks->setHidden(hidden);
}

//bool Views::ViewTasks::isRemoveOptionHidden()
//{
//    if(m_btnRemoveTasks)
//        return m_btnRemoveTasks->isHidden();

//    return false;
//}

//void Views::ViewTasks::setRemoveOptionHidden(bool hidden) const
//{
//    if(m_btnRemoveTasks)
//        m_btnRemoveTasks->setHidden(hidden);
//}

bool Views::ViewTasks::isOpenFilesOptionHidden()
{
    if(m_btnOpenFilesView)
        return m_btnOpenFilesView->isHidden();

    return false;
}

void Views::ViewTasks::setOpenFilesOptionHidden(bool hidden) const
{
    if(m_btnOpenFilesView)
        m_btnOpenFilesView->setHidden(hidden);
}

void Views::ViewTasks::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasViewFilesPriv = user->hasPrivilege("View Files");
    bool hasEditPriv = user->hasPrivilege("Edit");
    bool hasCreateProjectsPriv = user->hasPrivilege("Create Projects");
    bool hasCheckInPriv = user->hasPrivilege("Check In");
    bool hasCheckOutPriv = user->hasPrivilege("Check Out");
    bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

    m_btnCreateTask->setHidden(!hasCreateProjectsPriv);
    m_btnCreateTasksForTemplate->setHidden(!hasCreateProjectsPriv);
    m_btnEditTasks->setHidden(!hasEditPriv);

    m_qtvTasks->setImportCSVFeatureEnabled(hasCreateProjectsPriv);

    bool showTaskFilesButton = hasViewFilesPriv || hasCheckInPriv || hasCheckOutPriv || hasCreateRepoPriv;//if have any of the privileges
    m_btnOpenFilesView->setHidden(!showTaskFilesButton);
}

Wt::Signal<> &Views::ViewTasks::createTaskRequested()
{
    return m_createTaskRequested;
}

Wt::Signal<> &Views::ViewTasks::createTasksForTemplateRequested()
{
    return m_createTasksForTemplateRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> &Views::ViewTasks::removeTasksRequested()
{
    return m_removeTasksRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> &Views::ViewTasks::openfilesViewRequested()
{
    return m_openfilesViewRequested;
}

void Views::ViewTasks::btnCreateTaskClicked()
{
    m_createTaskRequested();
}

void Views::ViewTasks::btnCreateTasksForTemplateClicked()
{
    m_createTasksForTemplateRequested();
}

void Views::ViewTasks::btnRemoveTasksClicked()
{
    m_removeTasksRequested(m_qtvTasks->selectedItems());
}

void Views::ViewTasks::btnEditTasksClicked()
{
    if(m_qtvTasks->table()->selectedIndexes().size() == 0)
    {
        m_logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditTask *dlg = new Views::DlgCreateAndEditTask(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            for(auto taskPtr : m_qtvTasks->selectedItems())
            {
                if(dlg->editedStartDate())
                {
                    sendTaskNotification(taskPtr, "Start Date", Wt::asString(taskPtr->startDate()).toUTF8(), Wt::asString(dlg->startDate()).toUTF8());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setStartDate(dlg->startDate());
                }

                if(dlg->editedEndDate())
                {
                    sendTaskNotification(taskPtr, "End Date", Wt::asString(taskPtr->endDate()).toUTF8(), Wt::asString(dlg->endDate()).toUTF8());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setEndDate(dlg->endDate());
                }

                if(dlg->editedType())
                {
                    sendTaskNotification(taskPtr, "Type", taskPtr->type()->type(), dlg->type()->type());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setType(dlg->type());
                }

                if(dlg->editedPriority())
                {
                    sendTaskNotification(taskPtr, "Priority", Wt::asString(taskPtr->priority()).toUTF8(), Wt::asString(dlg->priority()).toUTF8());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setPriority(dlg->priority());
                }

                if(dlg->editedStatus())
                {
                    sendTaskNotification(taskPtr, "Status", taskPtr->status()->status(), dlg->status()->status());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setStatus(dlg->status());
                }

                if(dlg->editedThumbnail())
                {
                    std::string thumbnail = taskPtr->thumbnail();

                    if(dlg->thumbnail() == "Project")
                    {   
                        if(taskPtr->project())
                            thumbnail = taskPtr->project()->thumbnail();
                        else if(taskPtr->sequence())
                            thumbnail = taskPtr->sequence()->project()->thumbnail();
                        else if(taskPtr->asset())
                            thumbnail = taskPtr->asset()->project()->thumbnail();
                        else if(taskPtr->shot())
                            thumbnail = taskPtr->shot()->project()->thumbnail();
                    }
                    else if(dlg->thumbnail() == "Sequence")
                    {
                        if(taskPtr->sequence())
                            thumbnail = taskPtr->sequence()->thumbnail();
                        else if(taskPtr->shot())
                            thumbnail = taskPtr->shot()->sequence()->thumbnail();
                    }
                    else if(dlg->thumbnail() == "Asset")
                    {
                        if(taskPtr->asset())
                            thumbnail = taskPtr->asset()->thumbnail();
                    }
                    else if(dlg->thumbnail() == "Shot")
                    {
                        if(taskPtr->shot())
                            thumbnail = taskPtr->shot()->thumbnail();
                    }

                    sendTaskNotification(taskPtr, "Thumbnail", taskPtr->thumbnail(), dlg->thumbnail());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setThumbnail(thumbnail);
                }

                if(dlg->editedUser())
                {
                    sendTaskNotification(taskPtr, "User", taskPtr->user()->name(), dlg->user()->name());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setUser(dlg->user());
                }

                if(dlg->editedDescription())
                {
                    sendTaskNotification(taskPtr, "Description", taskPtr->description(), dlg->description());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setDescription(dlg->description());
                }

                if(dlg->editedActive())
                {
                    sendTaskNotification(taskPtr, "Active", Wt::asString(taskPtr->active()).toUTF8(), Wt::asString(dlg->isActive()).toUTF8());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTask>(taskPtr)->setActive(dlg->isActive());
                }
            }

            transaction.commit();

            m_qtvTasks->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewTasks::btnOpenFilesViewClicked()
{
    if(m_qtvTasks->table()->selectedIndexes().size() != 1)
    {
        m_logger->log("Please select only one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgTaskSelectDbo *dlgSelectDbo = new Views::DlgTaskSelectDbo();

    dlgSelectDbo->finished().connect(std::bind([=]()
    {
        if(dlgSelectDbo->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            Wt::Dbo::ptr<Projects::ProjectTask> taskPtr =  m_qtvTasks->selectedItems().at(0);

            std::string path = "";

            if(dlgSelectDbo->type() == "Project")
            {
                if(taskPtr->project().get())
                    path = Projects::ProjectsIO::getRelativeProjectTaskDir(taskPtr->project()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                else
                    m_logger->log("Task does not belong to a project.", Ms::Log::LogMessageType::Warning);
            }
            else if(dlgSelectDbo->type() == "Sequence")
            {
                if(taskPtr->sequence().get())
                    path = Projects::ProjectsIO::getRelativeSequenceTaskDir(taskPtr->sequence()->projectName(), taskPtr->sequence()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                else
                    m_logger->log("Task does not belong to a sequence.", Ms::Log::LogMessageType::Warning);
            }
            else if(dlgSelectDbo->type() == "Shot")
            {
                if(taskPtr->shot().get())
                    path = Projects::ProjectsIO::getRelativeShotTaskDir(taskPtr->shot()->projectName(), taskPtr->shot()->sequenceName(), taskPtr->shot()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                else
                    m_logger->log("Task does not belong to a shot.", Ms::Log::LogMessageType::Warning);
            }
            else if(dlgSelectDbo->type() == "Asset")
            {
                if(taskPtr->asset().get())
                    path = Projects::ProjectsIO::getRelativeAssetTaskDir(taskPtr->asset()->projectName(), taskPtr->asset()->name(), taskPtr.id()) + Ms::IO::dirSeparator() + "files";
                else
                    m_logger->log("Task does not belong to an asset.", Ms::Log::LogMessageType::Warning);
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

                m_openfilesViewRequested(m_qtvTasks->selectedItems());
            }

            transaction.commit();
        }

        delete dlgSelectDbo;
    }));

    dlgSelectDbo->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewTasks::taskDataAboutToBeChanged(const Wt::WModelIndex &index, const boost::any &value, int role)
{
    //get task
    Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = m_qtvTasks->itemForModelIndex(index);

    std::string headerName = Wt::asString(index.model()->headerData(index.column())).toUTF8();
    std::string orgValue = Wt::asString(index.data(role)).toUTF8();
    std::string newValue = Wt::asString(value).toUTF8();

    if(orgValue == newValue)
        return;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string message = "Task \"" + std::to_string(taskPtr.id()) + "\" \"" + headerName + "\" has changed from \"" + orgValue + "\" to \"" + newValue + "\"";

    Database::Notification *notification = new Database::Notification(message);

    try
    {
        Wt::Dbo::ptr<Database::Notification> notificationPtr = Session::SessionManager::instance().dboSession().createDbo<Database::Notification>(notification);

        if(notificationPtr.get())
        {
            if(taskPtr->project())
            {
                const Wt::Dbo::ptr<Projects::Project> projectPtr = taskPtr->project();

                //notify all users of tasks in parent project of the change
                for(auto iter = projectPtr->tasks().begin(); iter != projectPtr->tasks().end(); ++iter)
                {
                    (*iter)->user().modify()->addNotification(notificationPtr);
                }
            }
            else if(taskPtr->sequence())
            {
                const Wt::Dbo::ptr<Projects::ProjectSequence> sequencePtr = taskPtr->sequence();

                //notify all users of tasks in parent sequence of the change
                for(auto iter = sequencePtr->tasks().begin(); iter != sequencePtr->tasks().end(); ++iter)
                {
                    (*iter)->user().modify()->addNotification(notificationPtr);
                }
            }
            else if(taskPtr->shot())
            {
                const Wt::Dbo::ptr<Projects::ProjectShot> shotPtr = taskPtr->shot();

                //notify all users of tasks in parent shot of the change
                for(auto iter = shotPtr->tasks().begin(); iter != shotPtr->tasks().end(); ++iter)
                {
                    (*iter)->user().modify()->addNotification(notificationPtr);
                }
            }
            else if(taskPtr->asset())
            {
                const Wt::Dbo::ptr<Projects::ProjectAsset> assetPtr = taskPtr->asset();

                //notify all users of tasks in parent asset of the change
                for(auto iter = assetPtr->tasks().begin(); iter != assetPtr->tasks().end(); ++iter)
                {
                    (*iter)->user().modify()->addNotification(notificationPtr);
                }
            }
        }
        else
        {
            delete notification;

            m_logger->log("Error occured while trying to create new notification", Ms::Log::LogMessageType::Error);
        }
    }
    catch(Wt::Dbo::Exception ex)
    {
        delete notification;

        m_logger->log(ex.what(), Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        delete notification;

        m_logger->log("Error occured while trying to create new notification", Ms::Log::LogMessageType::Error);
    }
}

void Views::ViewTasks::createTasksTableView()
{
    m_qtvTasks = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTask>(Session::SessionManager::instance().dboSession());
    m_qtvTasks->setRowHeight(160);
    m_qtvTasks->setIgnoreNumFilterColumns(1);

    Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<Projects::ProjectTask>> *model = const_cast<Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<Projects::ProjectTask>>*>(m_qtvTasks->model());

    model->dataAboutToBeChanged().connect(this, &Views::ViewTasks::taskDataAboutToBeChanged);

    m_btnCreateTask = m_qtvTasks->createToolButton("", "icons/Add.png", "Create A New Task");
    m_btnCreateTask->clicked().connect(this, &Views::ViewTasks::btnCreateTaskClicked);

    m_btnCreateTasksForTemplate = m_qtvTasks->createToolButton("", "icons/Template.png", "Create Tasks Based On Template");
    m_btnCreateTasksForTemplate->clicked().connect(this, &Views::ViewTasks::btnCreateTasksForTemplateClicked);

    //m_btnRemoveTasks = m_qtvTasks->createToolButton("", "icons/Remove.png", "Remove Selected Tasks");
    //m_btnRemoveTasks->clicked().connect(this, &Views::ViewTasks::btnRemoveTasksClicked);

    m_btnEditTasks = m_qtvTasks->createToolButton("", "icons/Edit.png", "Edit Selected Tasks");
    m_btnEditTasks->clicked().connect(this, &Views::ViewTasks::btnEditTasksClicked);

    m_btnOpenFilesView = m_qtvTasks->createToolButton("", "icons/Files.png", "Files Manager");
    m_btnOpenFilesView->clicked().connect(this, &Views::ViewTasks::btnOpenFilesViewClicked);
}

void Views::ViewTasks::sendTaskNotification(Wt::Dbo::ptr<Projects::ProjectTask> taskPtr, const std::string &property, const std::string &orgValue, const std::string &newValue)
{
    if(orgValue == newValue)
        return;

    std::string message = "Task \"" + std::to_string(taskPtr.id()) + "\" \"" + property + "\" has changed from \"" + orgValue + "\" to \"" + newValue + "\"";

    Database::Notification *notification = new Database::Notification(message);

    try
    {
        Wt::Dbo::ptr<Database::Notification> notificationPtr = Session::SessionManager::instance().dboSession().createDbo<Database::Notification>(notification);

        if(notificationPtr.get())
        {
            if(taskPtr->project())
            {
                const Wt::Dbo::ptr<Projects::Project> projectPtr = taskPtr->project();

                //notify all users of tasks in parent project of the change
                for(auto iter = projectPtr->tasks().begin(); iter != projectPtr->tasks().end(); ++iter)
                {
                    (*iter)->user().modify()->addNotification(notificationPtr);
                }
            }
            else if(taskPtr->sequence())
            {
                const Wt::Dbo::ptr<Projects::ProjectSequence> sequencePtr = taskPtr->sequence();

                //notify all users of tasks in parent sequence of the change
                for(auto iter = sequencePtr->tasks().begin(); iter != sequencePtr->tasks().end(); ++iter)
                {
                    (*iter)->user().modify()->addNotification(notificationPtr);
                }
            }
            else if(taskPtr->shot())
            {
                const Wt::Dbo::ptr<Projects::ProjectShot> shotPtr = taskPtr->shot();

                //notify all users of tasks in parent shot of the change
                for(auto iter = shotPtr->tasks().begin(); iter != shotPtr->tasks().end(); ++iter)
                {
                    (*iter)->user().modify()->addNotification(notificationPtr);
                }
            }
            else if(taskPtr->asset())
            {
                const Wt::Dbo::ptr<Projects::ProjectAsset> assetPtr = taskPtr->asset();

                //notify all users of tasks in parent asset of the change
                for(auto iter = assetPtr->tasks().begin(); iter != assetPtr->tasks().end(); ++iter)
                {
                    (*iter)->user().modify()->addNotification(notificationPtr);
                }
            }
        }
        else
        {
            delete notification;

            m_logger->log("Error occured while trying to create new notification", Ms::Log::LogMessageType::Error);
        }
    }
    catch(Wt::Dbo::Exception ex)
    {
        delete notification;

        m_logger->log(ex.what(), Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        delete notification;

        m_logger->log("Error occured while trying to create new notification", Ms::Log::LogMessageType::Error);
    }
}

void Views::ViewTasks::prepareView()
{
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    createTasksTableView();

    m_layMain->addWidget(m_qtvTasks);
}
