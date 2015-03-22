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
    m_logger = Session::SessionManager::instance().logger();
    m_propertiesPanel = Session::SessionManager::instance().propertiesPanel();

    prepareView();

    updateGeneralSettingsView();

    //default menu selection
    m_mnuSettings->select(m_mnuSettingsGeneralItem);
    m_mnuProjectSettings->select(m_mnuProjectSettingsTaskActivityTypeItem);
    m_mnuUsersSettings->select(m_mnuUsersSettingsUserTitlesItem);
    m_mnuGlobalSettings->select(m_mnuGlobalSettingsTagsItem);
}

//main
void Views::ViewSettings::mnuSettingsGeneralItemTriggered()
{
    m_stkSettings->setCurrentWidget(m_cntGeneralSettings);
    updateGeneralSettingsView();
}

void Views::ViewSettings::mnuSettingsProjectsItemTriggered()
{
    m_stkSettings->setCurrentWidget(m_cntProjectsSettings);
    updateProjectsSettingsView();
}

void Views::ViewSettings::mnuSettingsUsersItemTriggered()
{
    m_stkSettings->setCurrentWidget(m_cntUsersSettings);
    updateUsersSettingsView();
}

void Views::ViewSettings::mnuSettingsGlobalItemTriggered()
{
    m_stkSettings->setCurrentWidget(m_cntGlobalSettings);
    updateGlobalSettingsView();
}

void Views::ViewSettings::updateView()
{
    if(m_stkSettings->currentWidget() == m_cntGeneralSettings)
        updateGeneralSettingsView();
    else if(m_stkSettings->currentWidget() == m_cntProjectsSettings)
        updateProjectsSettingsView();
    else if(m_stkSettings->currentWidget() == m_cntUsersSettings)
        updateProjectsSettingsView();
    else if(m_stkSettings->currentWidget() == m_cntGlobalSettings)
        updateGlobalSettingsView();
}

void Views::ViewSettings::updateGeneralSettingsView()
{
    m_chkLoadInactiveData->setChecked(AppSettings::instance().isLoadInactiveData());
    m_chkShowExtraColumns->setChecked(AppSettings::instance().isShowExtraColumns());
}

void Views::ViewSettings::updateProjectsSettingsView()
{
    if(m_stkProjectProperties->currentWidget() == m_qtvProjectTaskActivityType)
        updateTaskActivityTypeView();
    else if(m_stkProjectProperties->currentWidget() == m_viewTaskTemplates)
        updateTaskTemplatesView();
    else if(m_stkProjectProperties->currentWidget() == m_viewActivityTemplates)
        updateActivityTemplatesView();
    else if(m_stkProjectProperties->currentWidget() == m_qtvProjectAssetType)
        updateAssetTypeView();
    else if(m_stkProjectProperties->currentWidget() == m_qtvProjectTaskType)
        updateTaskTypeView();
    else if(m_stkProjectProperties->currentWidget() == m_qtvProjectWorkStatus)
        updateWorkStatusView();
}

void Views::ViewSettings::updateUsersSettingsView()
{
    if(m_stkUsersProperties->currentWidget() == m_qtvUserTitle)
        updateUserTitlesView();
}

void Views::ViewSettings::updateGlobalSettingsView()
{
    if(m_stkGlobalProperties->currentWidget() == m_qtvTags)
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

        m_qtvProjectTaskActivityType->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        m_qtvProjectTaskActivityType->clearColumns();

        //add columns
        m_qtvProjectTaskActivityType->addColumn(Ms::Widgets::MQueryTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            m_qtvProjectTaskActivityType->addBaseColumns(flags, editRank);

        transaction.commit();

        m_qtvProjectTaskActivityType->updateView();
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

        m_qtvProjectTaskType->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        m_qtvProjectTaskType->clearColumns();

        //add columns
        m_qtvProjectTaskType->addColumn(Ms::Widgets::MQueryTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            m_qtvProjectTaskType->addBaseColumns(flags, editRank);

        transaction.commit();

        m_qtvProjectTaskType->updateView();
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

        m_qtvProjectAssetType->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        m_qtvProjectAssetType->clearColumns();

        //add columns
        m_qtvProjectAssetType->addColumn(Ms::Widgets::MQueryTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            m_qtvProjectAssetType->addBaseColumns(flags, editRank);

        transaction.commit();

        m_qtvProjectAssetType->updateView();
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

        m_qtvProjectWorkStatus->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        m_qtvProjectWorkStatus->clearColumns();

        //add columns
        m_qtvProjectWorkStatus->addColumn(Ms::Widgets::MQueryTableViewColumn("Status", "Status", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        m_qtvProjectWorkStatus->addColumn(Ms::Widgets::MQueryTableViewColumn("Belongs_To_Work_Status_Type", "Belongs To Work Status Type",
        flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatusType>(&Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatusType>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatusType>().where("Active = ?").bind(true),
         "Work_Status_Type", editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            m_qtvProjectWorkStatus->addBaseColumns(flags, editRank);

        transaction.commit();

        m_qtvProjectWorkStatus->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating work status table view" << std::endl;
    }
}

void Views::ViewSettings::updateTaskTemplatesView()
{
    m_viewTaskTemplates->updateView();
}

void Views::ViewSettings::updateActivityTemplatesView()
{
    m_viewActivityTemplates->updateView();
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

        m_qtvUserTitle->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        m_qtvUserTitle->clearColumns();

        //add columns
        m_qtvUserTitle->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Title", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            m_qtvUserTitle->addBaseColumns(flags, editRank);

        transaction.commit();

        m_qtvUserTitle->updateView();
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

        m_qtvTags->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        m_qtvTags->clearColumns();

        //add columns
        m_qtvTags->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        m_qtvTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        m_qtvTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            m_qtvTags->addBaseColumns(flags, editRank);

        transaction.commit();

        m_qtvTags->updateView();
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

    m_btnCreateTaskActivityType->setHidden(!hasCreateDboPriv);
    m_btnCreateTaskType->setHidden(!hasCreateDboPriv);
    m_btnCreateAssetType->setHidden(!hasCreateDboPriv);
    m_btnCreateWorkStatus->setHidden(!hasCreateDboPriv);
    m_btnCreateUserTitle->setHidden(!hasCreateDboPriv);
    m_btnCreateTag->setHidden(!hasCreateDboPriv);
    m_chkLoadInactiveData->setDisabled(!hasEditPriv);
    m_chkShowExtraColumns->setDisabled(!hasEditPriv);
    m_btnSave->setDisabled(!hasEditPriv);

    m_qtvProjectTaskActivityType->setImportCSVFeatureEnabled(hasCreateDboPriv);
    m_qtvProjectTaskType->setImportCSVFeatureEnabled(hasCreateDboPriv);
    m_qtvProjectAssetType->setImportCSVFeatureEnabled(hasCreateDboPriv);
    m_qtvProjectWorkStatus->setImportCSVFeatureEnabled(hasCreateDboPriv);
    m_qtvUserTitle->setImportCSVFeatureEnabled(hasCreateDboPriv);
    m_qtvTags->setImportCSVFeatureEnabled(hasCreateDboPriv);
}

Wt::Signal<> &Views::ViewSettings::onTabGeneralSelected()
{
    return m_onTabGeneralSelected;
}

Wt::Signal<> &Views::ViewSettings::onTabProjectsSelected()
{
    return m_onTabProjectsSelected;
}

Wt::Signal<> &Views::ViewSettings::onTabUsersSelected()
{
    return m_onTabUsersSelected;
}

Wt::Signal<> &Views::ViewSettings::onTabGlobalSelected()
{
    return m_onTabGlobalSelected;
}

void Views::ViewSettings::btnSaveClicked()
{
    AppSettings::instance().setLoadInactiveData(m_chkLoadInactiveData->isChecked());
    AppSettings::instance().setShowExtraColumns(m_chkShowExtraColumns->isChecked());

    Wt::WMessageBox *msg = new Wt::WMessageBox("Information", "Saved.", Wt::Information, Wt::Ok);
    msg->buttonClicked().connect(std::bind([=]()
    {
        delete msg;
    }));
    msg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewSettings::mnuProjectSettingsTaskActivityTypeItemTriggered()
{
    m_stkProjectProperties->setCurrentWidget(m_cntTaskActivityType);
    updateTaskActivityTypeView();
}

void Views::ViewSettings::mnuProjectSettingsTaskTypeItemTriggered()
{
    m_stkProjectProperties->setCurrentWidget(m_cntTaskType);
    updateTaskTypeView();
}

void Views::ViewSettings::mnuProjectSettingsAssetTypeItemTriggered()
{
    m_stkProjectProperties->setCurrentWidget(m_cntAssetType);
    updateAssetTypeView();
}

void Views::ViewSettings::mnuProjectSettingsWorkStatusItemTriggered()
{
    m_stkProjectProperties->setCurrentWidget(m_cntWorkStatus);
    updateWorkStatusView();
}

void Views::ViewSettings::mnuProjectSettingsTaskTemplatesItemTriggered()
{
    m_stkProjectProperties->setCurrentWidget(m_viewTaskTemplates);
    updateTaskTemplatesView();
}

void Views::ViewSettings::mnuProjectSettingsActivityTemplatesItemTriggered()
{
    m_stkProjectProperties->setCurrentWidget(m_viewActivityTemplates);
    updateActivityTemplatesView();
}

void Views::ViewSettings::mnuUsersSettingsUserTitlesItemTriggered()
{
    m_stkUsersProperties->setCurrentWidget(m_cntUserTitles);
    updateUserTitlesView();
}

void Views::ViewSettings::mnuGlobalSettingsTagsItemTriggered()
{
    m_stkGlobalProperties->setCurrentWidget(m_cntTags);
    updateTagsView();
}

//TaskActivityType
void Views::ViewSettings::createTaskActivityTypeTableView()
{
    m_qtvProjectTaskActivityType = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTaskActivityType>(Session::SessionManager::instance().dboSession());

    m_btnCreateTaskActivityType = m_qtvProjectTaskActivityType->createToolButton("", "icons/Add.png", "Create A New Task Activity Type");
    m_btnCreateTaskActivityType->clicked().connect(this, &Views::ViewSettings::btnCreateTaskActivityTypeClicked);

//    Wt::WPushButton *btn = m_qtvProjectTaskActivityType->createToolButton("", "icons/Remove.png", "Remove Selected Task Activity Type");
//    btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveTaskActivityTypeClicked);

    updateTaskActivityTypeView();
}

void Views::ViewSettings::btnCreateTaskActivityTypeClicked()
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

                    m_logger->log(std::string("Created task activity type ") + dlg->type(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete type;

                    m_logger->log(std::string("Error creating task activity type ") + dlg->type(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                m_logger->log(std::string("Object already exist"), Ms::Log::LogMessageType::Warning);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewSettings::btnRemoveTaskActivityTypeClicked()
{

}

//TaskType
void Views::ViewSettings::createTaskTypeTableView()
{
    m_qtvProjectTaskType = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTaskType>(Session::SessionManager::instance().dboSession());

    m_btnCreateTaskType = m_qtvProjectTaskType->createToolButton("", "icons/Add.png", "Create A New Task Type");
    m_btnCreateTaskType->clicked().connect(this, &Views::ViewSettings::btnCreateTaskTypeClicked);

//    Wt::WPushButton *btn = m_qtvProjectTaskType->createToolButton("", "icons/Remove.png", "Remove Selected Task Type");
//    btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveTaskTypeClicked);

    updateTaskTypeView();
}

void Views::ViewSettings::btnCreateTaskTypeClicked()
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

                    m_logger->log(std::string("Created task type ") + dlg->type(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete type;

                    m_logger->log(std::string("Error creating task type ") + dlg->type(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                m_logger->log(std::string("Object already exist"), Ms::Log::LogMessageType::Warning);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewSettings::btnRemoveTaskTypeClicked()
{

}

//AssetType
void Views::ViewSettings::createAssetTypeTableView()
{
    m_qtvProjectAssetType = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectAssetType>(Session::SessionManager::instance().dboSession());

    m_btnCreateAssetType = m_qtvProjectAssetType->createToolButton("", "icons/Add.png", "Create A New Asset Type");
    m_btnCreateAssetType->clicked().connect(this, &Views::ViewSettings::btnCreateAssetTypeClicked);

//    Wt::WPushButton *btn = m_qtvProjectAssetType->createToolButton("", "icons/Remove.png", "Remove Selected Asset Type");
//    btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveAssetTypeClicked);

    updateAssetTypeView();
}

void Views::ViewSettings::btnCreateAssetTypeClicked()
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

                    m_logger->log(std::string("Created asset type ") + dlg->type(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete type;

                    m_logger->log(std::string("Error creating asset type ") + dlg->type(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                m_logger->log(std::string("Object already exist"), Ms::Log::LogMessageType::Warning);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewSettings::btnRemoveAssetTypeClicked()
{

}

//WorkStatus
void Views::ViewSettings::createWorkStatusTableView()
{
    m_qtvProjectWorkStatus = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectWorkStatus>(Session::SessionManager::instance().dboSession());

    m_btnCreateWorkStatus = m_qtvProjectWorkStatus->createToolButton("", "icons/Add.png", "Create A New Work Status");
    m_btnCreateWorkStatus->clicked().connect(this, &Views::ViewSettings::btnCreateWorkStatusClicked);

//    Wt::WPushButton *btn = m_qtvProjectWorkStatus->createToolButton("", "icons/Remove.png", "Remove Selected Work Status");
//    btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveWorkStatusClicked);

    updateWorkStatusView();
}

void Views::ViewSettings::btnCreateWorkStatusClicked()
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

                    m_logger->log(std::string("Created project work status ") + dlg->type(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete status;

                    m_logger->log(std::string("Error creating project work status ") + dlg->type(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                m_logger->log(std::string("Object already exist"), Ms::Log::LogMessageType::Warning);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewSettings::btnRemoveWorkStatusClicked()
{

}

//UserTitles
void Views::ViewSettings::createUsersTitlesTableView()
{
    m_qtvUserTitle = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::UserTitle>(Session::SessionManager::instance().dboSession());

    m_btnCreateUserTitle = m_qtvUserTitle->createToolButton("", "icons/Add.png", "Create A New User Title");
    m_btnCreateUserTitle->clicked().connect(this, &Views::ViewSettings::btnCreateUserTitleClicked);

    //Wt::WPushButton *btn = m_qtvUserTitle->createToolButton("", "icons/Remove.png", "Remove Selected User Title");
    //btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveUserTitlesClicked);

    updateUserTitlesView();
}

void Views::ViewSettings::btnCreateUserTitleClicked()
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

                    m_logger->log(std::string("Created user title ") + dlg->title(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete title;

                    m_logger->log(std::string("Error creating user title ") + dlg->title(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                m_logger->log(std::string("Object already exist"), Ms::Log::LogMessageType::Warning);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewSettings::btnRemoveUserTitlesClicked()
{

}

//Tags
void Views::ViewSettings::createTagsTableView()
{
    m_qtvTags = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Tag>(Session::SessionManager::instance().dboSession());

    m_btnCreateTag = m_qtvTags->createToolButton("", "icons/Add.png", "Create A New Tag");
    m_btnCreateTag->clicked().connect(this, &Views::ViewSettings::btnCreateTagClicked);

    //Wt::WPushButton *btn = m_qtvTags->createToolButton("", "icons/Remove.png", "Remove Selected Tags");
    //btn->clicked().connect(this, &Views::ViewSettings::_btnRemoveTagClicked);

    updateTagsView();
}

void Views::ViewSettings::btnCreateTagClicked()
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

                m_logger->log(std::string("Created tag ") + dlg->tagName(), Ms::Log::LogMessageType::Info);
            }
            else
            {
                delete tag;

                m_logger->log(std::string("Error creating tag ") + dlg->tagName(), Ms::Log::LogMessageType::Error);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewSettings::btnRemoveTagClicked()
{

}

void Views::ViewSettings::createPropertiesView()
{
    m_cntPropertiesMain = new Wt::WContainerWidget();
    m_propertiesPanel->addPropertiesView(this->id(), m_cntPropertiesMain);
}

void Views::ViewSettings::prepareView()
{
    /*******************--Settings--********************/
    setTitle("<b><i>Settings</i></b>");

    Wt::WVBoxLayout *m_layMain = dynamic_cast<Wt::WVBoxLayout*>(layout());
    m_layMain->setContentsMargins(14,0,14,14);

    m_navBarSettings = new Wt::WNavigationBar();

    m_cntNavBarSettings = new Wt::WContainerWidget();
    m_cntNavBarSettings->addWidget(m_navBarSettings);

    //add our navigation bar to the view
    m_layMain->addWidget(m_cntNavBarSettings);

    m_mnuSettings = new Wt::WMenu(Wt::Horizontal);
    m_navBarSettings->addMenu(m_mnuSettings);

    m_mnuSettingsGeneralItem = new Wt::WMenuItem("General");
    m_mnuSettingsGeneralItem->triggered().connect(this, &Views::ViewSettings::mnuSettingsGeneralItemTriggered);
    m_mnuSettings->addItem(m_mnuSettingsGeneralItem);

    m_mnuSettingsProjectsItem = new Wt::WMenuItem("Projects");
    m_mnuSettingsProjectsItem->triggered().connect(this, &Views::ViewSettings::mnuSettingsProjectsItemTriggered);
    m_mnuSettings->addItem(m_mnuSettingsProjectsItem);

    m_mnuSettingsUsersItem = new Wt::WMenuItem("Users");
    m_mnuSettingsUsersItem->triggered().connect(this, &Views::ViewSettings::mnuSettingsUsersItemTriggered);
    m_mnuSettings->addItem(m_mnuSettingsUsersItem);

    m_mnuSettingsGlobalItem = new Wt::WMenuItem("Global");
    m_mnuSettingsGlobalItem->triggered().connect(this, &Views::ViewSettings::mnuSettingsGlobalItemTriggered);
    m_mnuSettings->addItem(m_mnuSettingsGlobalItem);

    m_stkSettings = new Wt::WStackedWidget();
    m_stkSettings->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);

    m_layMain->addWidget(m_stkSettings, 1);//stack general view

    //General//////////////////////////////////////
    m_laySettingsGeneral = new Wt::WVBoxLayout();
    m_laySettingsGeneral->setContentsMargins(20,10,20,20);
    m_laySettingsGeneral->setSpacing(0);

    m_cntGeneralSettings = new Wt::WContainerWidget();
    m_cntGeneralSettings->setLayout(m_laySettingsGeneral);

    m_grpGeneral = new Wt::WGroupBox();
    m_grpGeneral->setTitle("General");
    m_laySettingsGeneral->addWidget(m_grpGeneral);

    m_chkLoadInactiveData = new Wt::WCheckBox();
    m_chkLoadInactiveData->setText("Load Inactive Data From Database");

    m_grpGeneral->addWidget(m_chkLoadInactiveData);

    new Wt::WBreak(m_grpGeneral);

    m_chkShowExtraColumns = new Wt::WCheckBox();
    m_chkShowExtraColumns->setText("Show Extra Columns In Data Views");

    m_grpGeneral->addWidget(m_chkShowExtraColumns);

    new Wt::WBreak(m_grpGeneral);

    m_btnSave = new Wt::WPushButton("Save");
    m_btnSave->setMinimumSize(60, 20);
    m_btnSave->clicked().connect(this, &Views::ViewSettings::btnSaveClicked);

    m_cntBtnSave = new Wt::WContainerWidget();
    m_cntBtnSave->setContentAlignment(Wt::AlignLeft);
    m_cntBtnSave->setMargin(10);
    m_cntBtnSave->addWidget(m_btnSave);

    m_grpGeneral->addWidget(m_cntBtnSave);

    //add our general view to the Settings view
    m_stkSettings->addWidget(m_cntGeneralSettings);

    /*******************--Projects Main--********************/
    m_layCntProjectsSettings = new Wt::WHBoxLayout();
    m_layCntProjectsSettings->setContentsMargins(0,0,0,0);
    m_layCntProjectsSettings->setSpacing(0);

    m_cntProjectsSettings = new Wt::WContainerWidget();
    m_cntProjectsSettings->setLayout(m_layCntProjectsSettings);

    m_layCntMnuProjectSettings = new Wt::WVBoxLayout();
    m_layCntMnuProjectSettings->setContentsMargins(0,0,0,0);
    m_layCntMnuProjectSettings->setSpacing(0);

    m_cntMnuProjectSettings = new Wt::WContainerWidget();
    m_cntMnuProjectSettings->setStyleClass("panel-border-right");
    m_cntMnuProjectSettings->setLayout(m_layCntMnuProjectSettings);

    m_layCntProjectsSettings->addWidget(m_cntMnuProjectSettings);

    m_mnuProjectSettings = new Wt::WMenu(Wt::Vertical);
    m_mnuProjectSettings->addStyleClass("nav-pills nav-stacked");
    m_mnuProjectSettings->setWidth(250);

    m_mnuProjectSettings->addSectionHeader("Types");

    m_mnuProjectSettingsTaskActivityTypeItem = new Wt::WMenuItem("Task Activity Type");
    m_mnuProjectSettingsTaskActivityTypeItem->triggered().connect(this, &Views::ViewSettings::mnuProjectSettingsTaskActivityTypeItemTriggered);
    m_mnuProjectSettings->addItem(m_mnuProjectSettingsTaskActivityTypeItem);

    m_mnuProjectSettingsTaskTypeItem = new Wt::WMenuItem("Task Type");
    m_mnuProjectSettingsTaskTypeItem->triggered().connect(this, &Views::ViewSettings::mnuProjectSettingsTaskTypeItemTriggered);
    m_mnuProjectSettings->addItem(m_mnuProjectSettingsTaskTypeItem);

    m_mnuProjectSettingsAssetTypeItem = new Wt::WMenuItem("Asset Type");
    m_mnuProjectSettingsAssetTypeItem->triggered().connect(this, &Views::ViewSettings::mnuProjectSettingsAssetTypeItemTriggered);
    m_mnuProjectSettings->addItem(m_mnuProjectSettingsAssetTypeItem);

    m_mnuProjectSettingsWorkStatusItem = new Wt::WMenuItem("Work Status Type");
    m_mnuProjectSettingsWorkStatusItem->triggered().connect(this, &Views::ViewSettings::mnuProjectSettingsWorkStatusItemTriggered);
    m_mnuProjectSettings->addItem(m_mnuProjectSettingsWorkStatusItem);

    m_mnuProjectSettingsTaskTemplatesItem = new Wt::WMenuItem("Task Templates");
    m_mnuProjectSettingsTaskTemplatesItem->triggered().connect(this, &Views::ViewSettings::mnuProjectSettingsTaskTemplatesItemTriggered);
    m_mnuProjectSettings->addItem(m_mnuProjectSettingsTaskTemplatesItem);

    m_mnuProjectSettingsActivityTemplatesItem = new Wt::WMenuItem("Activity Templates");
    m_mnuProjectSettingsActivityTemplatesItem->triggered().connect(this, &Views::ViewSettings::mnuProjectSettingsActivityTemplatesItemTriggered);
    m_mnuProjectSettings->addItem(m_mnuProjectSettingsActivityTemplatesItem);

    m_layCntMnuProjectSettings->addWidget(m_mnuProjectSettings);

    //add our project view to the Settings view
    m_stkSettings->addWidget(m_cntProjectsSettings);

    //add our Projects properties to our settings view
    m_stkProjectProperties = new Wt::WStackedWidget();
    m_stkProjectProperties->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);
    m_layCntProjectsSettings->addWidget(m_stkProjectProperties, 1);

    /*******************--TaskActivityType--********************/
    m_layCntTaskActivityType = new Wt::WVBoxLayout();
    m_layCntTaskActivityType->setContentsMargins(14,0,0,0);
    m_layCntTaskActivityType->setSpacing(0);

    m_cntTaskActivityType = new Wt::WContainerWidget();
    m_cntTaskActivityType->setLayout(m_layCntTaskActivityType);

    //add our TaskActivityType view to the project settings view
    m_stkProjectProperties->addWidget(m_cntTaskActivityType);

    createTaskActivityTypeTableView();

    m_layCntTaskActivityType->addWidget(m_qtvProjectTaskActivityType, 1);

    /*******************--TaskType--********************/
    m_layCntTaskType = new Wt::WVBoxLayout();
    m_layCntTaskType->setContentsMargins(14,0,0,0);
    m_layCntTaskType->setSpacing(0);

    m_cntTaskType = new Wt::WContainerWidget();
    m_cntTaskType->setLayout(m_layCntTaskType);

    //add our TaskType view to the project settings viewP
    m_stkProjectProperties->addWidget(m_cntTaskType);

    createTaskTypeTableView();

    m_layCntTaskType->addWidget(m_qtvProjectTaskType, 1);

    /*******************--AssetType--********************/
    m_layCntAssetType = new Wt::WVBoxLayout();
    m_layCntAssetType->setContentsMargins(14,0,0,0);
    m_layCntAssetType->setSpacing(0);

    m_cntAssetType = new Wt::WContainerWidget();
    m_cntAssetType->setLayout(m_layCntAssetType);

    //add our AssetType view to the project settings view
    m_stkProjectProperties->addWidget(m_cntAssetType);

    createAssetTypeTableView();

    m_layCntAssetType->addWidget(m_qtvProjectAssetType, 1);

    /*******************--WorkStatus--********************/
    m_layCntWorkStatus = new Wt::WVBoxLayout();
    m_layCntWorkStatus->setContentsMargins(14,0,0,0);
    m_layCntWorkStatus->setSpacing(0);

    m_cntWorkStatus = new Wt::WContainerWidget();
    m_cntWorkStatus->setLayout(m_layCntWorkStatus);

    //add our WorkStatus view to the project settings view
    m_stkProjectProperties->addWidget(m_cntWorkStatus);

    createWorkStatusTableView();

    m_layCntWorkStatus->addWidget(m_qtvProjectWorkStatus, 1);

    /*******************--Task Template--********************/
    m_viewTaskTemplates = new Views::ViewTaskTemplates();

    m_stkProjectProperties->addWidget(m_viewTaskTemplates);

    /*******************--Activity Template--********************/
    m_viewActivityTemplates = new Views::ViewActivityTemplates();

    m_stkProjectProperties->addWidget(m_viewActivityTemplates);

    /*******************--Users Main--********************/
    m_layCntUsersSettings = new Wt::WHBoxLayout();
    m_layCntUsersSettings->setContentsMargins(0,0,0,0);
    m_layCntUsersSettings->setSpacing(0);

    m_cntUsersSettings = new Wt::WContainerWidget();
    m_cntUsersSettings->setLayout(m_layCntUsersSettings);

    m_layCntMnuUsersSettings = new Wt::WVBoxLayout();
    m_layCntMnuUsersSettings->setContentsMargins(0,0,0,0);
    m_layCntMnuUsersSettings->setSpacing(0);

    m_cntMnuUsersSettings = new Wt::WContainerWidget();
    m_cntMnuUsersSettings->setStyleClass("panel-border-right");
    m_cntMnuUsersSettings->setLayout(m_layCntMnuUsersSettings);

    m_layCntUsersSettings->addWidget(m_cntMnuUsersSettings);

    m_mnuUsersSettings = new Wt::WMenu(Wt::Vertical);
    m_mnuUsersSettings->addStyleClass("nav-pills nav-stacked");
    m_mnuUsersSettings->setWidth(250);

    m_mnuUsersSettings->addSectionHeader("Types");

    m_mnuUsersSettingsUserTitlesItem = new Wt::WMenuItem("User Titles");
    m_mnuUsersSettingsUserTitlesItem->triggered().connect(this, &Views::ViewSettings::mnuUsersSettingsUserTitlesItemTriggered);
    m_mnuUsersSettings->addItem(m_mnuUsersSettingsUserTitlesItem);

    m_layCntMnuUsersSettings->addWidget(m_mnuUsersSettings);

    //add our User view to the Settings view
    m_stkSettings->addWidget(m_cntUsersSettings);

    //add our Users properties to our settings view
    m_stkUsersProperties = new Wt::WStackedWidget();
    m_stkUsersProperties->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);
    m_layCntUsersSettings->addWidget(m_stkUsersProperties, 1);

    /*******************--UserTitles--********************/
    m_layCntUserTitles = new Wt::WVBoxLayout();
    m_layCntUserTitles->setContentsMargins(14,0,0,0);
    m_layCntUserTitles->setSpacing(0);

    m_cntUserTitles = new Wt::WContainerWidget();
    m_cntUserTitles->setLayout(m_layCntUserTitles);

    //add our Titles view to the User settings view
    m_stkUsersProperties->addWidget(m_cntUserTitles);

    createUsersTitlesTableView();

    m_layCntUserTitles->addWidget(m_qtvUserTitle, 1);

    /*******************--Global Main--********************/
    m_layCntGlobalSettings = new Wt::WHBoxLayout();
    m_layCntGlobalSettings->setContentsMargins(0,0,0,0);
    m_layCntGlobalSettings->setSpacing(0);

    m_cntGlobalSettings = new Wt::WContainerWidget();
    m_cntGlobalSettings->setLayout(m_layCntGlobalSettings);

    m_layCntMnuGlobalSettings = new Wt::WVBoxLayout();
    m_layCntMnuGlobalSettings->setContentsMargins(0,0,0,0);
    m_layCntMnuGlobalSettings->setSpacing(0);

    m_cntMnuGlobalSettings = new Wt::WContainerWidget();
    m_cntMnuGlobalSettings->setStyleClass("panel-border-right");
    m_cntMnuGlobalSettings->setLayout(m_layCntMnuGlobalSettings);

    m_layCntGlobalSettings->addWidget(m_cntMnuGlobalSettings);

    m_mnuGlobalSettings = new Wt::WMenu(Wt::Vertical);
    m_mnuGlobalSettings->addStyleClass("nav-pills nav-stacked");
    m_mnuGlobalSettings->setWidth(250);

    m_mnuGlobalSettings->addSectionHeader("Types");

    m_mnuGlobalSettingsTagsItem = new Wt::WMenuItem("Tags");
    m_mnuGlobalSettingsTagsItem->triggered().connect(this, &Views::ViewSettings::mnuGlobalSettingsTagsItemTriggered);
    m_mnuGlobalSettings->addItem(m_mnuGlobalSettingsTagsItem);

    m_layCntMnuGlobalSettings->addWidget(m_mnuGlobalSettings);

    //add our Global view to the Settings view
    m_stkSettings->addWidget(m_cntGlobalSettings);

    //add our Global properties to our settings view
    m_stkGlobalProperties = new Wt::WStackedWidget();
    m_stkGlobalProperties->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);
    m_layCntGlobalSettings->addWidget(m_stkGlobalProperties, 1);

    /*******************--Tags--********************/
    m_layCntTags = new Wt::WVBoxLayout();
    m_layCntTags->setContentsMargins(14,0,0,0);
    m_layCntTags->setSpacing(0);

    m_cntTags = new Wt::WContainerWidget();
    m_cntTags->setLayout(m_layCntTags);

    //add our Tags view to the User settings view
    m_stkGlobalProperties->addWidget(m_cntTags);

    createTagsTableView();

    m_layCntTags->addWidget(m_qtvTags, 1);

    /*******************--Properties--********************/
    createPropertiesView();
}
