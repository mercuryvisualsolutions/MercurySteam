#include <Wt/WMessageBox>

#include "viewsettings.h"
#include "../../Database/dbosession.h"
#include "../../Log/logmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Auth/authmanager.h"
#include "../Dialogs/dlgcreatetag.h"
#include "settingsdialogs.h"

#include <Ms/Widgets/Delegates/MDelegates>
#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewSettings::ViewSettings() :
    Ms::Widgets::MContainerWidget()
{
    _logger = Session::SessionManager::instance().logger();
    _propertiesPanel = Session::SessionManager::instance().propertiesPanel();

    _prepareView();

    updateGeneralSettingsView();

    //default menu selection
    _mnuSettings->select(_mnuSettingsGeneralItem);
    _mnuProjectSettings->select(_mnuProjectSettingsTaskActivityTypeItem);
    _mnuUsersSettings->select(_mnuUsersSettingsUserTitlesItem);
    _mnuGlobalSettings->select(_mnuGlobalSettingsTagsItem);
}

//main
void Views::ViewSettings::_mnuSettingsGeneralItemTriggered()
{
    _stkSettings->setCurrentWidget(_cntGeneralSettings);
    updateGeneralSettingsView();
}

void Views::ViewSettings::_mnuSettingsProjectsItemTriggered()
{
    _stkSettings->setCurrentWidget(_cntProjectsSettings);
    updateProjectsSettingsView();
}

void Views::ViewSettings::_mnuSettingsUsersItemTriggered()
{
    _stkSettings->setCurrentWidget(_cntUsersSettings);
    updateUsersSettingsView();
}

void Views::ViewSettings::_mnuSettingsGlobalItemTriggered()
{
    _stkSettings->setCurrentWidget(_cntGlobalSettings);
    updateGlobalSettingsView();
}

void Views::ViewSettings::updateView()
{
    if(_stkSettings->currentWidget() == _cntGeneralSettings)
        updateGeneralSettingsView();
    else if(_stkSettings->currentWidget() == _cntProjectsSettings)
        updateProjectsSettingsView();
    else if(_stkSettings->currentWidget() == _cntUsersSettings)
        updateProjectsSettingsView();
    else if(_stkSettings->currentWidget() == _cntGlobalSettings)
        updateGlobalSettingsView();
}

void Views::ViewSettings::updateGeneralSettingsView()
{
    _chkLoadInactiveData->setChecked(AppSettings::instance().isLoadInactiveData());
    _chkShowExtraColumns->setChecked(AppSettings::instance().isShowExtraColumns());
}

void Views::ViewSettings::updateProjectsSettingsView()
{
    if(_stkProjectProperties->currentWidget() == _qtvProjectTaskActivityType)
        updateTaskActivityTypeView();
    else if(_stkProjectProperties->currentWidget() == _viewTaskTemplates)
        updateTaskTemplatesView();
    else if(_stkProjectProperties->currentWidget() == _viewActivityTemplates)
        updateActivityTemplatesView();
    else if(_stkProjectProperties->currentWidget() == _qtvProjectAssetType)
        updateAssetTypeView();
    else if(_stkProjectProperties->currentWidget() == _qtvProjectTaskType)
        updateTaskTypeView();
    else if(_stkProjectProperties->currentWidget() == _qtvProjectWorkStatus)
        updateWorkStatusView();
}

void Views::ViewSettings::updateUsersSettingsView()
{
    if(_stkUsersProperties->currentWidget() == _qtvUserTitle)
        updateUserTitlesView();
}

void Views::ViewSettings::updateGlobalSettingsView()
{
    if(_stkGlobalProperties->currentWidget() == _qtvTags)
        updateTagsView();
}

void Views::ViewSettings::updateTaskActivityTypeView()
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskActivityType>> query;

        int viewRank = Session::SessionManager::instance().user()->viewRank();

        if(AppSettings::instance().isLoadInactiveData())
            query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskActivityType>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskActivityType>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        _qtvProjectTaskActivityType->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        _qtvProjectTaskActivityType->clearColumns();

        //add columns
        _qtvProjectTaskActivityType->addColumn(Ms::Widgets::MQueryTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            _qtvProjectTaskActivityType->addBaseColumns(flags, editRank);

        transaction.commit();

        _qtvProjectTaskActivityType->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating task activity type table view" << std::endl;
    }
}

void Views::ViewSettings::updateTaskTypeView()
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskType>> query;

        int viewRank = Session::SessionManager::instance().user()->viewRank();

        if(AppSettings::instance().isLoadInactiveData())
            query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskType>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskType>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        _qtvProjectTaskType->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        _qtvProjectTaskType->clearColumns();

        //add columns
        _qtvProjectTaskType->addColumn(Ms::Widgets::MQueryTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            _qtvProjectTaskType->addBaseColumns(flags, editRank);

        transaction.commit();

        _qtvProjectTaskType->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating task type table view" << std::endl;
    }
}

void Views::ViewSettings::updateAssetTypeView()
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectAssetType>> query;

        int viewRank = Session::SessionManager::instance().user()->viewRank();

        if(AppSettings::instance().isLoadInactiveData())
            query = Session::SessionManager::instance().dboSession().find<Projects::ProjectAssetType>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Session::SessionManager::instance().dboSession().find<Projects::ProjectAssetType>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        _qtvProjectAssetType->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        _qtvProjectAssetType->clearColumns();

        //add columns
        _qtvProjectAssetType->addColumn(Ms::Widgets::MQueryTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            _qtvProjectAssetType->addBaseColumns(flags, editRank);

        transaction.commit();

        _qtvProjectAssetType->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating asset type table view" << std::endl;
    }
}

void Views::ViewSettings::updateWorkStatusView()
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> query;

        int viewRank = Session::SessionManager::instance().user()->viewRank();

        if(AppSettings::instance().isLoadInactiveData())
            query = Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        _qtvProjectWorkStatus->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        _qtvProjectWorkStatus->clearColumns();

        //add columns
        _qtvProjectWorkStatus->addColumn(Ms::Widgets::MQueryTableViewColumn("Status", "Status", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvProjectWorkStatus->addColumn(Ms::Widgets::MQueryTableViewColumn("Belongs_To_Work_Status_Type", "Belongs To Work Status Type",
        flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatusType>(&Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatusType>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatusType>().where("Active = ?").bind(true),
         "Work_Status_Type", editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            _qtvProjectWorkStatus->addBaseColumns(flags, editRank);

        transaction.commit();

        _qtvProjectWorkStatus->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating work status table view" << std::endl;
    }
}

void Views::ViewSettings::updateTaskTemplatesView()
{
    _viewTaskTemplates->updateView();
}

void Views::ViewSettings::updateActivityTemplatesView()
{
    _viewActivityTemplates->updateView();
}

void Views::ViewSettings::updateUserTitlesView()
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Users::UserTitle>> query;

        int viewRank = Session::SessionManager::instance().user()->viewRank();

        if(AppSettings::instance().isLoadInactiveData())
            query = Session::SessionManager::instance().dboSession().find<Users::UserTitle>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Session::SessionManager::instance().dboSession().find<Users::UserTitle>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        _qtvUserTitle->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        _qtvUserTitle->clearColumns();

        //add columns
        _qtvUserTitle->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Title", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            _qtvUserTitle->addBaseColumns(flags, editRank);

        transaction.commit();

        _qtvUserTitle->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating users titles table view" << std::endl;
    }
}

void Views::ViewSettings::updateTagsView()
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query;

        int viewRank = Session::SessionManager::instance().user()->viewRank();

        if(AppSettings::instance().isLoadInactiveData())
            query = Session::SessionManager::instance().dboSession().find<Database::Tag>().where("Type = ? AND View_Rank <= ?").bind("Global").bind(viewRank);
        else
            query = Session::SessionManager::instance().dboSession().find<Database::Tag>().where("Type = ? AND View_Rank <= ? AND Active = ?").bind("Global").bind(viewRank).bind(true);

        _qtvTags->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        _qtvTags->clearColumns();

        //add columns
        _qtvTags->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            _qtvTags->addBaseColumns(flags, editRank);

        transaction.commit();

        _qtvTags->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating tags table view" << std::endl;
    }
}

void Views::ViewSettings::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasEditPriv = user->hasPrivilege("Edit");
    bool hasCreateDboPriv = user->hasPrivilege("Create DBO");

    _btnCreateTaskActivityType->setHidden(!hasCreateDboPriv);
    _btnCreateTaskType->setHidden(!hasCreateDboPriv);
    _btnCreateAssetType->setHidden(!hasCreateDboPriv);
    _btnCreateWorkStatus->setHidden(!hasCreateDboPriv);
    _btnCreateUserTitle->setHidden(!hasCreateDboPriv);
    _btnCreateTag->setHidden(!hasCreateDboPriv);
    _chkLoadInactiveData->setDisabled(!hasEditPriv);
    _chkShowExtraColumns->setDisabled(!hasEditPriv);
    _btnSave->setDisabled(!hasEditPriv);

    _qtvProjectTaskActivityType->setImportCSVFeatureEnabled(hasCreateDboPriv);
    _qtvProjectTaskType->setImportCSVFeatureEnabled(hasCreateDboPriv);
    _qtvProjectAssetType->setImportCSVFeatureEnabled(hasCreateDboPriv);
    _qtvProjectWorkStatus->setImportCSVFeatureEnabled(hasCreateDboPriv);
    _qtvUserTitle->setImportCSVFeatureEnabled(hasCreateDboPriv);
    _qtvTags->setImportCSVFeatureEnabled(hasCreateDboPriv);
}

Wt::Signal<> &Views::ViewSettings::onTabGeneralSelected()
{
    return _onTabGeneralSelected;
}

Wt::Signal<> &Views::ViewSettings::onTabProjectsSelected()
{
    return _onTabProjectsSelected;
}

Wt::Signal<> &Views::ViewSettings::onTabUsersSelected()
{
    return _onTabUsersSelected;
}

Wt::Signal<> &Views::ViewSettings::onTabGlobalSelected()
{
    return _onTabGlobalSelected;
}

void Views::ViewSettings::_btnSaveClicked()
{
    AppSettings::instance().setLoadInactiveData(_chkLoadInactiveData->isChecked());
    AppSettings::instance().setShowExtraColumns(_chkShowExtraColumns->isChecked());

    Wt::WMessageBox *msg = new Wt::WMessageBox("Information", "Saved.", Wt::Information, Wt::Ok);
    msg->buttonClicked().connect(std::bind([=]()
    {
        delete msg;
    }));
    msg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewSettings::_mnuProjectSettingsTaskActivityTypeItemTriggered()
{
    _stkProjectProperties->setCurrentWidget(_cntTaskActivityType);
    updateTaskActivityTypeView();
}

void Views::ViewSettings::_mnuProjectSettingsTaskTypeItemTriggered()
{
    _stkProjectProperties->setCurrentWidget(_cntTaskType);
    updateTaskTypeView();
}

void Views::ViewSettings::_mnuProjectSettingsAssetTypeItemTriggered()
{
    _stkProjectProperties->setCurrentWidget(_cntAssetType);
    updateAssetTypeView();
}

void Views::ViewSettings::_mnuProjectSettingsWorkStatusItemTriggered()
{
    _stkProjectProperties->setCurrentWidget(_cntWorkStatus);
    updateWorkStatusView();
}

void Views::ViewSettings::_mnuProjectSettingsTaskTemplatesItemTriggered()
{
    _stkProjectProperties->setCurrentWidget(_viewTaskTemplates);
    updateTaskTemplatesView();
}

void Views::ViewSettings::_mnuProjectSettingsActivityTemplatesItemTriggered()
{
    _stkProjectProperties->setCurrentWidget(_viewActivityTemplates);
    updateActivityTemplatesView();
}

void Views::ViewSettings::_mnuUsersSettingsUserTitlesItemTriggered()
{
    _stkUsersProperties->setCurrentWidget(_cntUserTitles);
    updateUserTitlesView();
}

void Views::ViewSettings::_mnuGlobalSettingsTagsItemTriggered()
{
    _stkGlobalProperties->setCurrentWidget(_cntTags);
    updateTagsView();
}

//TaskActivityType
void Views::ViewSettings::_createTaskActivityTypeTableView()
{
    _qtvProjectTaskActivityType = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTaskActivityType>(Session::SessionManager::instance().dboSession());

    _btnCreateTaskActivityType = _qtvProjectTaskActivityType->createToolButton("", "icons/Add.png", "Create A New Task Activity Type");
    _btnCreateTaskActivityType->clicked().connect(this, &Views::ViewSettings::_btnCreateTaskActivityTypeClicked);

//    Wt::WPushButton *btn = _qtvProjectTaskActivityType->createToolButton("", "icons/Remove.png", "Remove Selected Task Activity Type");
//    btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveTaskActivityTypeClicked);

    updateTaskActivityTypeView();
}

void Views::ViewSettings::_btnCreateTaskActivityTypeClicked()
{
    Views::DlgCreateTaskActivityType *dlg = new Views::DlgCreateTaskActivityType();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            if(!Session::SessionManager::instance().dboSession().dboExists<Projects::ProjectTaskActivityType>(dlg->type()))
            {
                Projects::ProjectTaskActivityType *type = new Projects::ProjectTaskActivityType(dlg->type());
                type->setActive(dlg->isActive());

                Wt::Dbo::ptr<Projects::ProjectTaskActivityType> typePtr = Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTaskActivityType>(type);

                if(typePtr.get())
                {
                    updateTaskActivityTypeView();

                    _logger->log(std::string("Created task activity type ") + dlg->type(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete type;

                    _logger->log(std::string("Error creating task activity type ") + dlg->type(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                _logger->log(std::string("Object already exist"), Ms::Log::LogMessageType::Warning);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewSettings::_btnRemoveTaskActivityTypeClicked()
{

}

//TaskType
void Views::ViewSettings::_createTaskTypeTableView()
{
    _qtvProjectTaskType = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTaskType>(Session::SessionManager::instance().dboSession());

    _btnCreateTaskType = _qtvProjectTaskType->createToolButton("", "icons/Add.png", "Create A New Task Type");
    _btnCreateTaskType->clicked().connect(this, &Views::ViewSettings::_btnCreateTaskTypeClicked);

//    Wt::WPushButton *btn = _qtvProjectTaskType->createToolButton("", "icons/Remove.png", "Remove Selected Task Type");
//    btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveTaskTypeClicked);

    updateTaskTypeView();
}

void Views::ViewSettings::_btnCreateTaskTypeClicked()
{
    Views::DlgCreateTaskType *dlg = new Views::DlgCreateTaskType();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            if(!Session::SessionManager::instance().dboSession().dboExists<Projects::ProjectTaskType>(dlg->type()))
            {
                Projects::ProjectTaskType *type = new Projects::ProjectTaskType(dlg->type());
                type->setActive(dlg->isActive());

                Wt::Dbo::ptr<Projects::ProjectTaskType> typePtr = Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTaskType>(type);

                if(typePtr.get())
                {
                    updateTaskTypeView();

                    _logger->log(std::string("Created task type ") + dlg->type(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete type;

                    _logger->log(std::string("Error creating task type ") + dlg->type(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                _logger->log(std::string("Object already exist"), Ms::Log::LogMessageType::Warning);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewSettings::_btnRemoveTaskTypeClicked()
{

}

//AssetType
void Views::ViewSettings::_createAssetTypeTableView()
{
    _qtvProjectAssetType = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectAssetType>(Session::SessionManager::instance().dboSession());

    _btnCreateAssetType = _qtvProjectAssetType->createToolButton("", "icons/Add.png", "Create A New Asset Type");
    _btnCreateAssetType->clicked().connect(this, &Views::ViewSettings::_btnCreateAssetTypeClicked);

//    Wt::WPushButton *btn = _qtvProjectAssetType->createToolButton("", "icons/Remove.png", "Remove Selected Asset Type");
//    btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveAssetTypeClicked);

    updateAssetTypeView();
}

void Views::ViewSettings::_btnCreateAssetTypeClicked()
{
    Views::DlgCreateAssetType *dlg = new Views::DlgCreateAssetType();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            if(!Session::SessionManager::instance().dboSession().dboExists<Projects::ProjectAssetType>(dlg->type()))
            {
                Projects::ProjectAssetType *type = new Projects::ProjectAssetType(dlg->type());
                type->setActive(dlg->isActive());

                Wt::Dbo::ptr<Projects::ProjectAssetType> typePtr = Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectAssetType>(type);

                if(typePtr.get())
                {
                    updateAssetTypeView();

                    _logger->log(std::string("Created asset type ") + dlg->type(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete type;

                    _logger->log(std::string("Error creating asset type ") + dlg->type(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                _logger->log(std::string("Object already exist"), Ms::Log::LogMessageType::Warning);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewSettings::_btnRemoveAssetTypeClicked()
{

}

//WorkStatus
void Views::ViewSettings::_createWorkStatusTableView()
{
    _qtvProjectWorkStatus = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectWorkStatus>(Session::SessionManager::instance().dboSession());

    _btnCreateWorkStatus = _qtvProjectWorkStatus->createToolButton("", "icons/Add.png", "Create A New Work Status");
    _btnCreateWorkStatus->clicked().connect(this, &Views::ViewSettings::_btnCreateWorkStatusClicked);

//    Wt::WPushButton *btn = _qtvProjectWorkStatus->createToolButton("", "icons/Remove.png", "Remove Selected Work Status");
//    btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveWorkStatusClicked);

    updateWorkStatusView();
}

void Views::ViewSettings::_btnCreateWorkStatusClicked()
{
    Views::DlgCreateWorkStatus *dlg = new Views::DlgCreateWorkStatus();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            if(!Session::SessionManager::instance().dboSession().dboExists<Projects::ProjectWorkStatus>(dlg->type()))
            {
                Projects::ProjectWorkStatus *status = new Projects::ProjectWorkStatus(dlg->type());
                status->setWorkStatusType(dlg->belongsToType());
                status->setActive(dlg->isActive());

                Wt::Dbo::ptr<Projects::ProjectWorkStatus> statusPtr = Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectWorkStatus>(status);

                if(statusPtr.get())
                {
                    updateWorkStatusView();

                    _logger->log(std::string("Created project work status ") + dlg->type(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete status;

                    _logger->log(std::string("Error creating project work status ") + dlg->type(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                _logger->log(std::string("Object already exist"), Ms::Log::LogMessageType::Warning);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewSettings::_btnRemoveWorkStatusClicked()
{

}

//UserTitles
void Views::ViewSettings::_createUsersTitlesTableView()
{
    _qtvUserTitle = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::UserTitle>(Session::SessionManager::instance().dboSession());

    _btnCreateUserTitle = _qtvUserTitle->createToolButton("", "icons/Add.png", "Create A New User Title");
    _btnCreateUserTitle->clicked().connect(this, &Views::ViewSettings::_btnCreateUserTitleClicked);

    //Wt::WPushButton *btn = _qtvUserTitle->createToolButton("", "icons/Remove.png", "Remove Selected User Title");
    //btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveUserTitlesClicked);

    updateUserTitlesView();
}

void Views::ViewSettings::_btnCreateUserTitleClicked()
{
    Views::DlgCreateUserTitle *dlg = new Views::DlgCreateUserTitle();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            if(!Session::SessionManager::instance().dboSession().dboExists<Users::UserTitle>(dlg->title()))
            {
                Users::UserTitle *title = new Users::UserTitle(dlg->title());
                title->setActive(dlg->isActive());

                Wt::Dbo::ptr<Users::UserTitle> titlePtr = Session::SessionManager::instance().dboSession().createDbo<Users::UserTitle>(title);

                if(titlePtr.get())
                {
                    updateUserTitlesView();

                    _logger->log(std::string("Created user title ") + dlg->title(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete title;

                    _logger->log(std::string("Error creating user title ") + dlg->title(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                _logger->log(std::string("Object already exist"), Ms::Log::LogMessageType::Warning);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewSettings::_btnRemoveUserTitlesClicked()
{

}

//Tags
void Views::ViewSettings::_createTagsTableView()
{
    _qtvTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(Session::SessionManager::instance().dboSession());

    _btnCreateTag = _qtvTags->createToolButton("", "icons/Add.png", "Create A New Tag");
    _btnCreateTag->clicked().connect(this, &Views::ViewSettings::_btnCreateTagClicked);

    //Wt::WPushButton *btn = _qtvTags->createToolButton("", "icons/Remove.png", "Remove Selected Tags");
    //btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveTagClicked);

    updateTagsView();
}

void Views::ViewSettings::_btnCreateTagClicked()
{
    Views::Dialogs::DlgCreateTag *dlg = new Views::Dialogs::DlgCreateTag();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            Database::Tag *tag = new Database::Tag(dlg->tagName(), dlg->tagContent());
            tag->setActive(dlg->isActive());

            Wt::Dbo::ptr<Database::Tag> tagPtr = Session::SessionManager::instance().dboSession().createDbo<Database::Tag>(tag);

            if(tagPtr.get())
            {
                updateTagsView();

                _logger->log(std::string("Created tag ") + dlg->tagName(), Ms::Log::LogMessageType::Info);
            }
            else
            {
                delete tag;

                _logger->log(std::string("Error creating tag ") + dlg->tagName(), Ms::Log::LogMessageType::Error);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewSettings::_btnRemoveTagClicked()
{

}

void Views::ViewSettings::_createPropertiesView()
{
    _cntPropertiesMain = new Wt::WContainerWidget();
    _propertiesPanel->addPropertiesView(this->id(), _cntPropertiesMain);
}

void Views::ViewSettings::_prepareView()
{
    /*******************--Settings--********************/
    setTitle("<b><i>Settings</i></b>");

    Wt::WVBoxLayout *_layMain = dynamic_cast<Wt::WVBoxLayout*>(layout());
    _layMain->setContentsMargins(14,0,14,14);

    _navBarSettings = new Wt::WNavigationBar();

    _cntNavBarSettings = new Wt::WContainerWidget();
    _cntNavBarSettings->addWidget(_navBarSettings);

    //add our navigation bar to the view
    _layMain->addWidget(_cntNavBarSettings);

    _mnuSettings = new Wt::WMenu(Wt::Horizontal);
    _navBarSettings->addMenu(_mnuSettings);

    _mnuSettingsGeneralItem = new Wt::WMenuItem("General");
    _mnuSettingsGeneralItem->triggered().connect(this, &Views::ViewSettings::_mnuSettingsGeneralItemTriggered);
    _mnuSettings->addItem(_mnuSettingsGeneralItem);

    _mnuSettingsProjectsItem = new Wt::WMenuItem("Projects");
    _mnuSettingsProjectsItem->triggered().connect(this, &Views::ViewSettings::_mnuSettingsProjectsItemTriggered);
    _mnuSettings->addItem(_mnuSettingsProjectsItem);

    _mnuSettingsUsersItem = new Wt::WMenuItem("Users");
    _mnuSettingsUsersItem->triggered().connect(this, &Views::ViewSettings::_mnuSettingsUsersItemTriggered);
    _mnuSettings->addItem(_mnuSettingsUsersItem);

    _mnuSettingsGlobalItem = new Wt::WMenuItem("Global");
    _mnuSettingsGlobalItem->triggered().connect(this, &Views::ViewSettings::_mnuSettingsGlobalItemTriggered);
    _mnuSettings->addItem(_mnuSettingsGlobalItem);

    _stkSettings = new Wt::WStackedWidget();
    _stkSettings->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);

    _layMain->addWidget(_stkSettings, 1);//stack general view

    //General//////////////////////////////////////
    _laySettingsGeneral = new Wt::WVBoxLayout();
    _laySettingsGeneral->setContentsMargins(20,10,20,20);
    _laySettingsGeneral->setSpacing(0);

    _cntGeneralSettings = new Wt::WContainerWidget();
    _cntGeneralSettings->setLayout(_laySettingsGeneral);

    _grpGeneral = new Wt::WGroupBox();
    _grpGeneral->setTitle("General");
    _laySettingsGeneral->addWidget(_grpGeneral);

    _chkLoadInactiveData = new Wt::WCheckBox();
    _chkLoadInactiveData->setText("Load Inactive Data From Database");

    _grpGeneral->addWidget(_chkLoadInactiveData);

    new Wt::WBreak(_grpGeneral);

    _chkShowExtraColumns = new Wt::WCheckBox();
    _chkShowExtraColumns->setText("Show Extra Columns In Data Views");

    _grpGeneral->addWidget(_chkShowExtraColumns);

    new Wt::WBreak(_grpGeneral);

    _btnSave = new Wt::WPushButton("Save");
    _btnSave->setMinimumSize(60, 20);
    _btnSave->clicked().connect(this, &Views::ViewSettings::_btnSaveClicked);

    _cntBtnSave = new Wt::WContainerWidget();
    _cntBtnSave->setContentAlignment(Wt::AlignLeft);
    _cntBtnSave->setMargin(10);
    _cntBtnSave->addWidget(_btnSave);

    _grpGeneral->addWidget(_cntBtnSave);

    //add our general view to the Settings view
    _stkSettings->addWidget(_cntGeneralSettings);

    /*******************--Projects Main--********************/
    _layCntProjectsSettings = new Wt::WHBoxLayout();
    _layCntProjectsSettings->setContentsMargins(0,0,0,0);
    _layCntProjectsSettings->setSpacing(0);

    _cntProjectsSettings = new Wt::WContainerWidget();
    _cntProjectsSettings->setLayout(_layCntProjectsSettings);

    _layCntMnuProjectSettings = new Wt::WVBoxLayout();
    _layCntMnuProjectSettings->setContentsMargins(0,0,0,0);
    _layCntMnuProjectSettings->setSpacing(0);

    _cntMnuProjectSettings = new Wt::WContainerWidget();
    //_cntMnuProjectSettings->setStyleClass("panel-border-right");
    _cntMnuProjectSettings->setLayout(_layCntMnuProjectSettings);

    _layCntProjectsSettings->addWidget(_cntMnuProjectSettings);

    _mnuProjectSettings = new Wt::WMenu(Wt::Vertical);
    _mnuProjectSettings->addStyleClass("nav-pills nav-stacked");
    _mnuProjectSettings->setWidth(250);

    _mnuProjectSettings->addSectionHeader("Types");

    _mnuProjectSettingsTaskActivityTypeItem = new Wt::WMenuItem("Task Activity Type");
    _mnuProjectSettingsTaskActivityTypeItem->triggered().connect(this, &Views::ViewSettings::_mnuProjectSettingsTaskActivityTypeItemTriggered);
    _mnuProjectSettings->addItem(_mnuProjectSettingsTaskActivityTypeItem);

    _mnuProjectSettingsTaskTypeItem = new Wt::WMenuItem("Task Type");
    _mnuProjectSettingsTaskTypeItem->triggered().connect(this, &Views::ViewSettings::_mnuProjectSettingsTaskTypeItemTriggered);
    _mnuProjectSettings->addItem(_mnuProjectSettingsTaskTypeItem);

    _mnuProjectSettingsAssetTypeItem = new Wt::WMenuItem("Asset Type");
    _mnuProjectSettingsAssetTypeItem->triggered().connect(this, &Views::ViewSettings::_mnuProjectSettingsAssetTypeItemTriggered);
    _mnuProjectSettings->addItem(_mnuProjectSettingsAssetTypeItem);

    _mnuProjectSettingsWorkStatusItem = new Wt::WMenuItem("Work Status Type");
    _mnuProjectSettingsWorkStatusItem->triggered().connect(this, &Views::ViewSettings::_mnuProjectSettingsWorkStatusItemTriggered);
    _mnuProjectSettings->addItem(_mnuProjectSettingsWorkStatusItem);

    _mnuProjectSettingsTaskTemplatesItem = new Wt::WMenuItem("Task Templates");
    _mnuProjectSettingsTaskTemplatesItem->triggered().connect(this, &Views::ViewSettings::_mnuProjectSettingsTaskTemplatesItemTriggered);
    _mnuProjectSettings->addItem(_mnuProjectSettingsTaskTemplatesItem);

    _mnuProjectSettingsActivityTemplatesItem = new Wt::WMenuItem("Activity Templates");
    _mnuProjectSettingsActivityTemplatesItem->triggered().connect(this, &Views::ViewSettings::_mnuProjectSettingsActivityTemplatesItemTriggered);
    _mnuProjectSettings->addItem(_mnuProjectSettingsActivityTemplatesItem);

    _layCntMnuProjectSettings->addWidget(_mnuProjectSettings);

    //add our project view to the Settings view
    _stkSettings->addWidget(_cntProjectsSettings);

    //add our Projects properties to our settings view
    _stkProjectProperties = new Wt::WStackedWidget();
    _stkProjectProperties->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);
    _layCntProjectsSettings->addWidget(_stkProjectProperties, 1);

    /*******************--TaskActivityType--********************/
    _layCntTaskActivityType = new Wt::WVBoxLayout();
    _layCntTaskActivityType->setContentsMargins(14,0,0,0);
    _layCntTaskActivityType->setSpacing(0);

    _cntTaskActivityType = new Wt::WContainerWidget();
    _cntTaskActivityType->setLayout(_layCntTaskActivityType);

    //add our TaskActivityType view to the project settings view
    _stkProjectProperties->addWidget(_cntTaskActivityType);

    _createTaskActivityTypeTableView();

    _layCntTaskActivityType->addWidget(_qtvProjectTaskActivityType, 1);

    /*******************--TaskType--********************/
    _layCntTaskType = new Wt::WVBoxLayout();
    _layCntTaskType->setContentsMargins(14,0,0,0);
    _layCntTaskType->setSpacing(0);

    _cntTaskType = new Wt::WContainerWidget();
    _cntTaskType->setLayout(_layCntTaskType);

    //add our TaskType view to the project settings viewP
    _stkProjectProperties->addWidget(_cntTaskType);

    _createTaskTypeTableView();

    _layCntTaskType->addWidget(_qtvProjectTaskType, 1);

    /*******************--AssetType--********************/
    _layCntAssetType = new Wt::WVBoxLayout();
    _layCntAssetType->setContentsMargins(14,0,0,0);
    _layCntAssetType->setSpacing(0);

    _cntAssetType = new Wt::WContainerWidget();
    _cntAssetType->setLayout(_layCntAssetType);

    //add our AssetType view to the project settings view
    _stkProjectProperties->addWidget(_cntAssetType);

    _createAssetTypeTableView();

    _layCntAssetType->addWidget(_qtvProjectAssetType, 1);

    /*******************--WorkStatus--********************/
    _layCntWorkStatus = new Wt::WVBoxLayout();
    _layCntWorkStatus->setContentsMargins(14,0,0,0);
    _layCntWorkStatus->setSpacing(0);

    _cntWorkStatus = new Wt::WContainerWidget();
    _cntWorkStatus->setLayout(_layCntWorkStatus);

    //add our WorkStatus view to the project settings view
    _stkProjectProperties->addWidget(_cntWorkStatus);

    _createWorkStatusTableView();

    _layCntWorkStatus->addWidget(_qtvProjectWorkStatus, 1);

    /*******************--Task Template--********************/
    _viewTaskTemplates = new Views::ViewTaskTemplates();

    _stkProjectProperties->addWidget(_viewTaskTemplates);

    /*******************--Activity Template--********************/
    _viewActivityTemplates = new Views::ViewActivityTemplates();

    _stkProjectProperties->addWidget(_viewActivityTemplates);

    /*******************--Users Main--********************/
    _layCntUsersSettings = new Wt::WHBoxLayout();
    _layCntUsersSettings->setContentsMargins(0,0,0,0);
    _layCntUsersSettings->setSpacing(0);

    _cntUsersSettings = new Wt::WContainerWidget();
    _cntUsersSettings->setLayout(_layCntUsersSettings);

    _layCntMnuUsersSettings = new Wt::WVBoxLayout();
    _layCntMnuUsersSettings->setContentsMargins(0,0,0,0);
    _layCntMnuUsersSettings->setSpacing(0);

    _cntMnuUsersSettings = new Wt::WContainerWidget();
    //_cntMnuUsersSettings->setStyleClass("panel-border-right");
    _cntMnuUsersSettings->setLayout(_layCntMnuUsersSettings);

    _layCntUsersSettings->addWidget(_cntMnuUsersSettings);

    _mnuUsersSettings = new Wt::WMenu(Wt::Vertical);
    _mnuUsersSettings->addStyleClass("nav-pills nav-stacked");
    _mnuUsersSettings->setWidth(250);

    _mnuUsersSettings->addSectionHeader("Types");

    _mnuUsersSettingsUserTitlesItem = new Wt::WMenuItem("User Titles");
    _mnuUsersSettingsUserTitlesItem->triggered().connect(this, &Views::ViewSettings::_mnuUsersSettingsUserTitlesItemTriggered);
    _mnuUsersSettings->addItem(_mnuUsersSettingsUserTitlesItem);

    _layCntMnuUsersSettings->addWidget(_mnuUsersSettings);

    //add our User view to the Settings view
    _stkSettings->addWidget(_cntUsersSettings);

    //add our Users properties to our settings view
    _stkUsersProperties = new Wt::WStackedWidget();
    _stkUsersProperties->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);
    _layCntUsersSettings->addWidget(_stkUsersProperties, 1);

    /*******************--UserTitles--********************/
    _layCntUserTitles = new Wt::WVBoxLayout();
    _layCntUserTitles->setContentsMargins(14,0,0,0);
    _layCntUserTitles->setSpacing(0);

    _cntUserTitles = new Wt::WContainerWidget();
    _cntUserTitles->setLayout(_layCntUserTitles);

    //add our Titles view to the User settings view
    _stkUsersProperties->addWidget(_cntUserTitles);

    _createUsersTitlesTableView();

    _layCntUserTitles->addWidget(_qtvUserTitle, 1);

    /*******************--Global Main--********************/
    _layCntGlobalSettings = new Wt::WHBoxLayout();
    _layCntGlobalSettings->setContentsMargins(0,0,0,0);
    _layCntGlobalSettings->setSpacing(0);

    _cntGlobalSettings = new Wt::WContainerWidget();
    _cntGlobalSettings->setLayout(_layCntGlobalSettings);

    _layCntMnuGlobalSettings = new Wt::WVBoxLayout();
    _layCntMnuGlobalSettings->setContentsMargins(0,0,0,0);
    _layCntMnuGlobalSettings->setSpacing(0);

    _cntMnuGlobalSettings = new Wt::WContainerWidget();
    //_cntMnuGlobalSettings->setStyleClass("panel-border-right");
    _cntMnuGlobalSettings->setLayout(_layCntMnuGlobalSettings);

    _layCntGlobalSettings->addWidget(_cntMnuGlobalSettings);

    _mnuGlobalSettings = new Wt::WMenu(Wt::Vertical);
    _mnuGlobalSettings->addStyleClass("nav-pills nav-stacked");
    _mnuGlobalSettings->setWidth(250);

    _mnuGlobalSettings->addSectionHeader("Types");

    _mnuGlobalSettingsTagsItem = new Wt::WMenuItem("Tags");
    _mnuGlobalSettingsTagsItem->triggered().connect(this, &Views::ViewSettings::_mnuGlobalSettingsTagsItemTriggered);
    _mnuGlobalSettings->addItem(_mnuGlobalSettingsTagsItem);

    _layCntMnuGlobalSettings->addWidget(_mnuGlobalSettings);

    //add our Global view to the Settings view
    _stkSettings->addWidget(_cntGlobalSettings);

    //add our Global properties to our settings view
    _stkGlobalProperties = new Wt::WStackedWidget();
    _stkGlobalProperties->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);
    _layCntGlobalSettings->addWidget(_stkGlobalProperties, 1);

    /*******************--Tags--********************/
    _layCntTags = new Wt::WVBoxLayout();
    _layCntTags->setContentsMargins(14,0,0,0);
    _layCntTags->setSpacing(0);

    _cntTags = new Wt::WContainerWidget();
    _cntTags->setLayout(_layCntTags);

    //add our Tags view to the User settings view
    _stkGlobalProperties->addWidget(_cntTags);

    _createTagsTableView();

    _layCntTags->addWidget(_qtvTags, 1);

    /*******************--Properties--********************/
    _createPropertiesView();
}
