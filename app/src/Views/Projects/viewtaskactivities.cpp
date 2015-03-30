#include "viewtaskactivities.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"
#include "../../Projects/projectsio.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Log/logmanager.h"
#include "dlgcreateandedittaskactivity.h"

#include <Wt/WApplication>

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewTaskActivity::ViewTaskActivity()
{
    m_logger = Session::SessionManager::instance().logger();

    prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskActivity> *Views::ViewTaskActivity::qtvTaskActivities() const
{
    return m_qtvTaskActivities;
}

void Views::ViewTaskActivity::updateView(const std::vector<Wt::Dbo::ptr<Projects::ProjectTask>> &taskVec) const
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskActivity>> query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskActivity>();

        if(taskVec.size() > 0)
        {
            std::vector<std::string> taskActivitiesIdValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectTask>(taskVec);

            //generate the where clause
            query.where("Project_Task_id IN (" + taskActivitiesIdValues.at(0) + ")");

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Session::SessionManager::instance().user()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("id = ?").bind(-1);//clear the view

        m_qtvTaskActivities->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        m_qtvTaskActivities->clearColumns();

        //add columns
        m_qtvTaskActivities->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "Id", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        m_qtvTaskActivities->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Task_Activity_Type", "Type", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectTaskActivityType>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskActivityType>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskActivityType>().where("Active = ?").bind(true),
         "Type", editRank)));

        m_qtvTaskActivities->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
        &Session::SessionManager::instance().dboSession(),AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true), "Status", editRank)));

        m_qtvTaskActivities->addColumn(Ms::Widgets::MQueryTableViewColumn("Hours", "Hours", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));

        m_qtvTaskActivities->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Task_id", "Task Id", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        m_qtvTaskActivities->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));

        if(AppSettings::instance().isShowExtraColumns())
            m_qtvTaskActivities->addBaseColumns(flags, editRank);

        transaction.commit();

        m_qtvTaskActivities->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating task activities view" << std::endl;
    }
}

bool Views::ViewTaskActivity::isCreateOptionHidden()
{
    return m_btnCreateTaskActivity->isHidden();
}

void Views::ViewTaskActivity::setCreateOptionHidden(bool hidden) const
{
    m_btnCreateTaskActivity->setHidden(hidden);
}

bool Views::ViewTaskActivity::isCreateForTemplateOptionHidden()
{
    return m_btnCreateTaskActivitiesForTemplate->isHidden();
}

void Views::ViewTaskActivity::setCreateForTemplateOptionHidden(bool hidden) const
{
    m_btnCreateTaskActivitiesForTemplate->setHidden(hidden);
}

bool Views::ViewTaskActivity::isEditOptionHidden()
{
    if(m_btnEditTaskActivities)
        return m_btnEditTaskActivities->isHidden();

    return false;
}

void Views::ViewTaskActivity::setEditOptionHidden(bool hidden) const
{
    if(m_btnEditTaskActivities)
        m_btnEditTaskActivities->setHidden(hidden);
}

void Views::ViewTaskActivity::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasEditPriv = user->hasPrivilege("Edit");
    bool hasCreateProjectsPriv = user->hasPrivilege("Create Projects");

    m_btnCreateTaskActivity->setHidden(!hasCreateProjectsPriv);
    m_btnCreateTaskActivitiesForTemplate->setHidden(!hasCreateProjectsPriv);
    m_btnEditTaskActivities->setHidden(!hasEditPriv);

    m_qtvTaskActivities->setImportCSVFeatureEnabled(hasCreateProjectsPriv);
}

//bool Views::ViewTaskActivity::isRemoveOptionHidden()
//{
//    if(m_btnRemoveTaskActivities)
//        return m_btnRemoveTaskActivities->isHidden();

//    return false;
//}

//void Views::ViewTaskActivity::setRemoveOptionHidden(bool hidden) const
//{
//    if(m_btnRemoveTaskActivities)
//        m_btnRemoveTaskActivities->setHidden(hidden);
//}

Wt::Signal<> &Views::ViewTaskActivity::createTaskActivityRequested()
{
    return m_createTaskActivityRequested;
}

Wt::Signal<> &Views::ViewTaskActivity::createTaskActivitiesForTemplateRequested()
{
    return m_createTaskActivitiesForTemplateRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskActivity>>> &Views::ViewTaskActivity::removeTaskActivitiesRequested()
{
    return m_removeTaskActivitiesRequested;
}

void Views::ViewTaskActivity::btnCreateTaskActivityClicked()
{
    m_createTaskActivityRequested();
}

void Views::ViewTaskActivity::btnCreateTaskActivitiesForTemplateClicked()
{
    m_createTaskActivitiesForTemplateRequested();
}

void Views::ViewTaskActivity::btnRemoveTaskActivitiesClicked()
{
    m_removeTaskActivitiesRequested(m_qtvTaskActivities->selectedItems());
}

void Views::ViewTaskActivity::btnEditTaskActivitiesClicked()
{
    if(m_qtvTaskActivities->table()->selectedIndexes().size() == 0)
    {
        m_logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditTaskActivity *dlg = new Views::DlgCreateAndEditTaskActivity(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            for(auto activityPtr : m_qtvTaskActivities->selectedItems())
            {
                if(dlg->editedType())
                {
                    sendTaskNotification(activityPtr, "Type", activityPtr->type()->type(), dlg->type()->type());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTaskActivity>(activityPtr)->setType(dlg->type());
                }

                if(dlg->editedHours())
                {
                    sendTaskNotification(activityPtr, "Hours", Wt::asString(activityPtr->hours()).toUTF8(), Wt::asString(dlg->hours()).toUTF8());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTaskActivity>(activityPtr)->setHours(dlg->hours());
                }

                if(dlg->editedStatus())
                {
                    sendTaskNotification(activityPtr, "Status", activityPtr->status()->status(), dlg->status()->status());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTaskActivity>(activityPtr)->setStatus(dlg->status());
                }

                if(dlg->editedActive())
                {
                    sendTaskNotification(activityPtr, "Active", Wt::asString(activityPtr->active()).toUTF8(), Wt::asString(dlg->isActive()).toUTF8());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTaskActivity>(activityPtr)->setActive(dlg->isActive());
                }
            }

            transaction.commit();

            m_qtvTaskActivities->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewTaskActivity::taskActivityDataAboutToBeChanged(const Wt::WModelIndex &index, const boost::any &value, int role)
{
    //get shot
    Wt::Dbo::ptr<Projects::ProjectTaskActivity> taskActivityPtr = m_qtvTaskActivities->itemForModelIndex(index);

    std::string headerName = Wt::asString(index.model()->headerData(index.column())).toUTF8();
    std::string orgValue = Wt::asString(index.data(role)).toUTF8();
    std::string newValue = Wt::asString(value).toUTF8();

    if(orgValue == newValue)
        return;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string message = "Task activity \"" + std::to_string(taskActivityPtr.id()) + "\" \"" + headerName + "\" has changed from \"" + orgValue + "\" to \"" + newValue + "\"";

    Database::Notification *notification = new Database::Notification(message);

    try
    {
        Wt::Dbo::ptr<Database::Notification> notificationPtr = Session::SessionManager::instance().dboSession().createDbo<Database::Notification>(notification);

        if(notificationPtr.get())
        {
            //notify the user of the parent task of current activity
            taskActivityPtr->task()->user().modify()->addNotification(notificationPtr);
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

void Views::ViewTaskActivity::createTaskActivitysTableView()
{
    m_qtvTaskActivities = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTaskActivity>(Session::SessionManager::instance().dboSession());

    Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<Projects::ProjectTaskActivity>> *model = const_cast<Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<Projects::ProjectTaskActivity>>*>(m_qtvTaskActivities->model());

    model->dataAboutToBeChanged().connect(this, &Views::ViewTaskActivity::taskActivityDataAboutToBeChanged);

    m_btnCreateTaskActivity = m_qtvTaskActivities->createToolButton("", "icons/Add.png", "Create A New Activity");
    m_btnCreateTaskActivity->clicked().connect(this, &Views::ViewTaskActivity::btnCreateTaskActivityClicked);

    m_btnCreateTaskActivitiesForTemplate = m_qtvTaskActivities->createToolButton("", "icons/Template.png", "Create Activities Based On Template");
    m_btnCreateTaskActivitiesForTemplate->clicked().connect(this, &Views::ViewTaskActivity::btnCreateTaskActivitiesForTemplateClicked);

    //m_btnRemoveTaskActivities = m_qtvTaskActivities->createToolButton("", "icons/Remove.png", "Remove Selected Task Activities");
    //m_btnRemoveTaskActivities->clicked().connect(this, &Views::ViewTaskActivity::btnRemoveTaskActivitiesClicked);

    m_btnEditTaskActivities = m_qtvTaskActivities->createToolButton("", "icons/Edit.png", "Edit Selected Assets");
    m_btnEditTaskActivities->clicked().connect(this, &Views::ViewTaskActivity::btnEditTaskActivitiesClicked);
}

void Views::ViewTaskActivity::sendTaskNotification(Wt::Dbo::ptr<Projects::ProjectTaskActivity> taskActivityPtr, const std::string &property, const std::string &orgValue, const std::string &newValue)
{
    if(orgValue == newValue)
        return;

    std::string message = "Task activity \"" + std::to_string(taskActivityPtr.id()) + "\" \"" + property + "\" has changed from \"" + orgValue + "\" to \"" + newValue + "\"";

    Database::Notification *notification = new Database::Notification(message);

    try
    {
        Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = taskActivityPtr->task();

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

void Views::ViewTaskActivity::prepareView()
{
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    createTaskActivitysTableView();

    m_layMain->addWidget(m_qtvTaskActivities);
}
