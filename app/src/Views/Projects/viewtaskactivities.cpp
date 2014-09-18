#include "viewtaskactivities.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"
#include "../../Projects/projectsio.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Log/logmanager.h"
#include "dlgcreateandedittaskactivity.h"

#include <Wt/WApplication>

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewTaskActivity::ViewTaskActivity()
{
    _logger = Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId());

    _prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskActivity> *Views::ViewTaskActivity::qtvTaskActivities() const
{
    return _qtvTaskActivities;
}

void Views::ViewTaskActivity::updateView(const std::vector<Wt::Dbo::ptr<Projects::ProjectTask>> &taskVec) const
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskActivity>> query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskActivity>();

        if(taskVec.size() > 0)
        {
            std::vector<std::string> taskActivitiesIdValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectTask>(taskVec);

            //generate the where clause
            query.where("Project_Task_id IN (" + taskActivitiesIdValues.at(0) + ")");

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("id = ?").bind(-1);//clear the view

        _qtvTaskActivities->setQuery(query);

        bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Auth::AuthManager::instance().currentUser()->editRank();

        _qtvTaskActivities->clearColumns();

        //add columns
        _qtvTaskActivities->addColumn(Ms::Widgets::MTableViewColumn("id", "Id", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        _qtvTaskActivities->addColumn(Ms::Widgets::MTableViewColumn("Project_Task_Activity_Type", "Type", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectTaskActivityType>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskActivityType>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskActivityType>().where("Active = ?").bind(true),
         "Type", editRank)));

        _qtvTaskActivities->addColumn(Ms::Widgets::MTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
        Database::DatabaseManager::instance().session(),AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true), "Status", editRank)));

        _qtvTaskActivities->addColumn(Ms::Widgets::MTableViewColumn("Hours", "Hours", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));

        _qtvTaskActivities->addColumn(Ms::Widgets::MTableViewColumn("Project_Task_id", "Task Id", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        _qtvTaskActivities->addColumn(Ms::Widgets::MTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));

        if(AppSettings::instance().isShowExtraColumns())
            _qtvTaskActivities->addBaseColumns(flags, editRank);

        _qtvTaskActivities->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating task activities view" << std::endl;
    }
}

bool Views::ViewTaskActivity::isCreateOptionHidden()
{
    return _btnCreateTaskActivity->isHidden();
}

void Views::ViewTaskActivity::setCreateOptionHidden(bool hidden) const
{
    _btnCreateTaskActivity->setHidden(hidden);
}

bool Views::ViewTaskActivity::isEditOptionHidden()
{
    return _btnEditTaskActivities->isHidden();
}

void Views::ViewTaskActivity::setEditOptionHidden(bool hidden) const
{
    _btnEditTaskActivities->setHidden(hidden);
}

//bool Views::ViewTaskActivity::isRemoveOptionHidden()
//{
//    return _btnRemoveTaskActivities->isHidden();
//}

//void Views::ViewTaskActivity::setRemoveOptionHidden(bool hidden) const
//{
//    _btnRemoveTaskActivities->setHidden(hidden);
//}

Wt::Signal<> &Views::ViewTaskActivity::createTaskActivityRequested()
{
    return _createTaskActivityRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskActivity>>> &Views::ViewTaskActivity::removeTaskActivitiesRequested()
{
    return _removeTaskActivitiesRequested;
}

void Views::ViewTaskActivity::_btnCreateTaskActivityClicked()
{
    _createTaskActivityRequested();
}

void Views::ViewTaskActivity::_btnRemoveTaskActivitiesClicked()
{
    _removeTaskActivitiesRequested(_qtvTaskActivities->selectedItems());
}

void Views::ViewTaskActivity::_btnEditTaskActivitiesClicked()
{
    if(_qtvTaskActivities->table()->selectedIndexes().size() == 0)
    {
        _logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditTaskActivity *dlg = new Views::DlgCreateAndEditTaskActivity(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            for(auto activityPtr : _qtvTaskActivities->selectedItems())
            {
                if(dlg->editedType())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTaskActivity>(activityPtr)->setType(dlg->type());

                if(dlg->editedHours())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTaskActivity>(activityPtr)->setHours(dlg->hours());

                if(dlg->editedStatus())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTaskActivity>(activityPtr)->setStatus(dlg->status());

                if(dlg->editedActive())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTaskActivity>(activityPtr)->setActive(dlg->isActive());
            }

            _qtvTaskActivities->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewTaskActivity::_createTaskActivitysTableView()
{
    _qtvTaskActivities = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTaskActivity>(&Database::DatabaseManager::instance());

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        _btnCreateTaskActivity = _qtvTaskActivities->createToolButton("", "icons/Add.png", "Create A New Task Activity");
        _btnCreateTaskActivity->clicked().connect(this, &Views::ViewTaskActivity::_btnCreateTaskActivityClicked);

        _qtvTaskActivities->setImportCSVFeatureEnabled(true);
    }
    else
        _qtvTaskActivities->setImportCSVFeatureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //_btnRemoveTaskActivities = _qtvTaskActivities->createToolButton("", "icons/Remove.png", "Remove Selected Task Activities");
        //_btnRemoveTaskActivities->clicked().connect(this, &Views::ViewTaskActivity::_btnRemoveTaskActivitiesClicked);
    }

    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        _btnEditTaskActivities = _qtvTaskActivities->createToolButton("", "icons/Edit.png", "Edit Selected Assets");
        _btnEditTaskActivities->clicked().connect(this, &Views::ViewTaskActivity::_btnEditTaskActivitiesClicked);
    }
}

void Views::ViewTaskActivity::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _createTaskActivitysTableView();

    _layMain->addWidget(_qtvTaskActivities);
}
