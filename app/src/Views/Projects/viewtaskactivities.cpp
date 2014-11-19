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
    _logger = Session::SessionManager::instance().logger();

    _prepareView();

    adjustUIPrivileges();
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskActivity> *Views::ViewTaskActivity::qtvTaskActivities() const
{
    return _qtvTaskActivities;
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

        _qtvTaskActivities->setQuery(query);

        transaction.commit();

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        _qtvTaskActivities->clearColumns();

        //add columns
        _qtvTaskActivities->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "Id", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        _qtvTaskActivities->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Task_Activity_Type", "Type", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectTaskActivityType>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskActivityType>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskActivityType>().where("Active = ?").bind(true),
         "Type", editRank)));

        _qtvTaskActivities->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
        &Session::SessionManager::instance().dboSession(),AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true), "Status", editRank)));

        _qtvTaskActivities->addColumn(Ms::Widgets::MQueryTableViewColumn("Hours", "Hours", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));

        _qtvTaskActivities->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Task_id", "Task Id", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        _qtvTaskActivities->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));

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

bool Views::ViewTaskActivity::isCreateForTemplateOptionHidden()
{
    return _btnCreateTaskActivitiesForTemplate->isHidden();
}

void Views::ViewTaskActivity::setCreateForTemplateOptionHidden(bool hidden) const
{
    _btnCreateTaskActivitiesForTemplate->setHidden(hidden);
}

bool Views::ViewTaskActivity::isEditOptionHidden()
{
    if(_btnEditTaskActivities)
        return _btnEditTaskActivities->isHidden();

    return false;
}

void Views::ViewTaskActivity::setEditOptionHidden(bool hidden) const
{
    if(_btnEditTaskActivities)
        _btnEditTaskActivities->setHidden(hidden);
}

void Views::ViewTaskActivity::adjustUIPrivileges()
{

}

//bool Views::ViewTaskActivity::isRemoveOptionHidden()
//{
//    if(_btnRemoveTaskActivities)
//        return _btnRemoveTaskActivities->isHidden();

//    return false;
//}

//void Views::ViewTaskActivity::setRemoveOptionHidden(bool hidden) const
//{
//    if(_btnRemoveTaskActivities)
//        _btnRemoveTaskActivities->setHidden(hidden);
//}

Wt::Signal<> &Views::ViewTaskActivity::createTaskActivityRequested()
{
    return _createTaskActivityRequested;
}

Wt::Signal<> &Views::ViewTaskActivity::createTaskActivitiesForTemplateRequested()
{
    return _createTaskActivitiesForTemplateRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskActivity>>> &Views::ViewTaskActivity::removeTaskActivitiesRequested()
{
    return _removeTaskActivitiesRequested;
}

void Views::ViewTaskActivity::_btnCreateTaskActivityClicked()
{
    _createTaskActivityRequested();
}

void Views::ViewTaskActivity::_btnCreateTaskActivitiesForTemplateClicked()
{
    _createTaskActivitiesForTemplateRequested();
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
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTaskActivity>(activityPtr)->setType(dlg->type());

                if(dlg->editedHours())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTaskActivity>(activityPtr)->setHours(dlg->hours());

                if(dlg->editedStatus())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTaskActivity>(activityPtr)->setStatus(dlg->status());

                if(dlg->editedActive())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTaskActivity>(activityPtr)->setActive(dlg->isActive());
            }

            _qtvTaskActivities->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewTaskActivity::_createTaskActivitysTableView()
{
    _qtvTaskActivities = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTaskActivity>(Session::SessionManager::instance().dboSession());

    _btnCreateTaskActivity = _qtvTaskActivities->createToolButton("", "icons/Add.png", "Create A New Activity");
    _btnCreateTaskActivity->clicked().connect(this, &Views::ViewTaskActivity::_btnCreateTaskActivityClicked);

    _btnCreateTaskActivitiesForTemplate = _qtvTaskActivities->createToolButton("", "icons/Template.png", "Create Activities Based On Template");
    _btnCreateTaskActivitiesForTemplate->clicked().connect(this, &Views::ViewTaskActivity::_btnCreateTaskActivitiesForTemplateClicked);

    //_btnRemoveTaskActivities = _qtvTaskActivities->createToolButton("", "icons/Remove.png", "Remove Selected Task Activities");
    //_btnRemoveTaskActivities->clicked().connect(this, &Views::ViewTaskActivity::_btnRemoveTaskActivitiesClicked);

    _btnEditTaskActivities = _qtvTaskActivities->createToolButton("", "icons/Edit.png", "Edit Selected Assets");
    _btnEditTaskActivities->clicked().connect(this, &Views::ViewTaskActivity::_btnEditTaskActivitiesClicked);
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
