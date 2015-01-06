#include "viewtasktemplate.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"
#include "../../Log/logmanager.h"
#include "dlgcreatetemplate.h"
#include "dlgcreateandedittasktemplateitem.h"

#include <Ms/Widgets/MWidgetFactory.h>

#include <Wt/WApplication>

Views::ViewTaskTemplates::ViewTaskTemplates()
{
    m_logger = Session::SessionManager::instance().logger();

    prepareView();
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskTemplate> *Views::ViewTaskTemplates::qtvTemplates() const
{
    return m_qtvTemplates;
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskTemplateTaskItem> *Views::ViewTaskTemplates::qtvTemplateItems() const
{
    return m_qtvTemplateItems;
}

bool Views::ViewTaskTemplates::isCreateTemplateOptionHidden() const
{
    if(m_btnCreateTemplate)
        return m_btnCreateTemplate->isHidden();

    return false;
}

void Views::ViewTaskTemplates::setCreateTemplateOptionHidden(bool hidden)
{
    if(m_btnCreateTemplate)
        m_btnCreateTemplate->setHidden(hidden);
}

bool Views::ViewTaskTemplates::isCreateTemplateItemOptionHidden() const
{
    if(m_btnCreateTemplateItem)
        return m_btnCreateTemplateItem->isHidden();

    return false;
}

void Views::ViewTaskTemplates::setCreateTemplateItemOptionHidden(bool hidden)
{
    if(m_btnCreateTemplateItem)
        m_btnCreateTemplateItem->setHidden(hidden);
}

bool Views::ViewTaskTemplates::isEditTemplateItemOptionHidden() const
{
    if(m_btnEditTemplateItem)
        return m_btnEditTemplateItem->isHidden();

    return false;
}

void Views::ViewTaskTemplates::setEditTemplateItemOptionHidden(bool hidden)
{
    if(m_btnEditTemplateItem)
        m_btnEditTemplateItem->setHidden(hidden);
}

void Views::ViewTaskTemplates::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasEditPriv = user->hasPrivilege("Edit");
    bool hasCreateDboPriv = user->hasPrivilege("Create DBO");

    m_btnCreateTemplate->setHidden(!hasCreateDboPriv);
    m_btnCreateTemplateItem->setHidden(!hasCreateDboPriv);
    m_btnEditTemplateItem->setHidden(!hasEditPriv);

    m_qtvTemplates->setImportCSVFeatureEnabled(hasCreateDboPriv);
    m_qtvTemplateItems->setImportCSVFeatureEnabled(hasCreateDboPriv);
}

Wt::Signal<> &Views::ViewTaskTemplates::createTemplateRequested()
{
    return m_createTemplateRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>>> &Views::ViewTaskTemplates::removeTemplatesRequested()
{
    return m_removeTemplatesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem>>>  &Views::ViewTaskTemplates::removeTemplateItemsRequested()
{
    return m_removeTemplateItemsRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>>> &Views::ViewTaskTemplates::createTemplateItemRequested()
{
    return m_createTemplateItemRequested;
}

void Views::ViewTaskTemplates::btnCreateTemplateClicked()
{
    Views::Dialogs::DlgCreateTemplate *dlg = new Views::Dialogs::DlgCreateTemplate();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            if(!Session::SessionManager::instance().dboSession().dboExists<Projects::ProjectTaskTemplate>(dlg->name()))
            {
                Projects::ProjectTaskTemplate *taskTemplate = new Projects::ProjectTaskTemplate(dlg->name());
                taskTemplate->setActive(dlg->isActive());

                if(Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTaskTemplate>(taskTemplate))
                {
                    updateTaskTemplatesView();

                    m_createTemplateRequested();

                    m_logger->log(std::string("Created project task template ") + dlg->name(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete taskTemplate;

                    m_logger->log(std::string("Error creating project task template ") + dlg->name(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                m_logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewTaskTemplates::btnRemoveTemplatesClicked()
{
    m_removeTemplatesRequested(m_qtvTemplates->selectedItems());
}

void Views::ViewTaskTemplates::btnCreateTemplateItemClicked()
{
    if(m_qtvTemplates->table()->selectedIndexes().size() != 1)
    {
        m_logger->log("Please select only one template.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditTaskTemplateItem *dlg = new Views::DlgCreateAndEditTaskTemplateItem();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            Wt::Dbo::ptr<Projects::ProjectTaskTemplate> templatePtr = m_qtvTemplates->selectedItems().at(0);

            Projects::ProjectTaskTemplateTaskItem *templateItem = new Projects::ProjectTaskTemplateTaskItem();
            templateItem->setType(dlg->type());
            templateItem->setStatus(dlg->status());
            templateItem->setDescription(dlg->description());
            templateItem->setTaskTemplate(templatePtr);
            templateItem->setActive(dlg->isActive());

            Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem> templateItemPtr =
                    Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTaskTemplateTaskItem>(templateItem);

            if(templateItemPtr.get())
            {
                updateTaskTemplateItemsView();

                m_createTemplateItemRequested(m_qtvTemplates->selectedItems());

                m_logger->log(std::string("Created item for task template ") + templatePtr->name(), Ms::Log::LogMessageType::Info);
            }
            else
            {
                delete templateItem;

                m_logger->log(std::string("Error creating item for task template ") + templatePtr->name(), Ms::Log::LogMessageType::Error);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewTaskTemplates::btnRemoveTemplateItemsClicked()
{
    m_removeTemplateItemsRequested(m_qtvTemplateItems->selectedItems());
}

void Views::ViewTaskTemplates::btnEditTemplateItemsClicked()
{
    if(m_qtvTemplateItems->table()->selectedIndexes().size() == 0)
    {
        m_logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditTaskTemplateItem *dlg = new Views::DlgCreateAndEditTaskTemplateItem(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            for(auto itemPtr : m_qtvTemplateItems->selectedItems())
            {
                if(dlg->editedType())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTaskTemplateTaskItem>(itemPtr)->setType(dlg->type());

                if(dlg->editedStatus())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTaskTemplateTaskItem>(itemPtr)->setStatus(dlg->status());

                if(dlg->editedDescription())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTaskTemplateTaskItem>(itemPtr)->setDescription(dlg->description());

                if(dlg->editedActive())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectTaskTemplateTaskItem>(itemPtr)->setActive(dlg->isActive());
            }

            transaction.commit();

            updateTaskTemplateItemsView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewTaskTemplates::updateView()
{
    updateTaskTemplatesView();
    updateTaskTemplateItemsView();
}

void Views::ViewTaskTemplates::updateTaskTemplatesView()
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>> query;

        int viewRank = Session::SessionManager::instance().user()->viewRank();

        if(AppSettings::instance().isLoadInactiveData())
            query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskTemplate>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskTemplate>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        m_qtvTemplates->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        m_qtvTemplates->clearColumns();

        //add columns
        m_qtvTemplates->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            m_qtvTemplates->addBaseColumns(flags, editRank);

        transaction.commit();

        m_qtvTemplates->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating table view" << std::endl;
    }
}

void Views::ViewTaskTemplates::updateTaskTemplateItemsView()
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem>> query = Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskTemplateTaskItem>();

        if(m_qtvTemplates->table()->selectedIndexes().size() > 0)
        {
            std::vector<std::string> templateIdValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectTaskTemplate>(m_qtvTemplates->selectedItems());

            //generate the where clause
            query.where("Project_Task_Template_Name IN (" + templateIdValues.at(0) + ")");

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Session::SessionManager::instance().user()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("id = ?").bind(-1);//clear the view

        m_qtvTemplateItems->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        m_qtvTemplateItems->clearColumns();

        //add columns
        m_qtvTemplateItems->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "Id", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        m_qtvTemplateItems->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Task_Type", "Type", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectTaskType>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskType>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskType>().where("Active = ?").bind(true),
         "Type", editRank)));

        m_qtvTemplateItems->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
        &Session::SessionManager::instance().dboSession(),AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true), "Status", editRank)));

        m_qtvTemplateItems->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Task_Template_Name", "Template Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        m_qtvTemplateItems->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));

        if(AppSettings::instance().isShowExtraColumns())
            m_qtvTemplateItems->addBaseColumns(flags, editRank);

        transaction.commit();

        m_qtvTemplateItems->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating table view" << std::endl;
    }
}

void Views::ViewTaskTemplates::createTemplatesTableView()
{
    m_qtvTemplates = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTaskTemplate>(Session::SessionManager::instance().dboSession());
    m_qtvTemplates->table()->selectionChanged().connect(this, &Views::ViewTaskTemplates::updateTaskTemplateItemsView);

    m_btnCreateTemplate = m_qtvTemplates->createToolButton("", "icons/Add.png", "Create Template");
    m_btnCreateTemplate->clicked().connect(this, &Views::ViewTaskTemplates::btnCreateTemplateClicked);

    //_btnRemoveTemplates = m_qtvTemplates->createToolButton("", "icons/Remove.png", "Remove Selected Templates");
    //_btnRemoveTemplates->clicked().connect(this, &Views::ViewTaskTemplates::btnRemoveTemplatesClicked);
}

void Views::ViewTaskTemplates::createTemplateItemsTableView()
{
    m_qtvTemplateItems = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTaskTemplateTaskItem>(Session::SessionManager::instance().dboSession());

    m_btnCreateTemplateItem = m_qtvTemplateItems->createToolButton("", "icons/Add.png", "Create New Template Item");
    m_btnCreateTemplateItem->clicked().connect(this, &Views::ViewTaskTemplates::btnCreateTemplateItemClicked);

    m_btnEditTemplateItem = m_qtvTemplateItems->createToolButton("", "icons/Edit.png", "Edit Selected Items");
    m_btnEditTemplateItem->clicked().connect(this, &Views::ViewTaskTemplates::btnEditTemplateItemsClicked);

    //_btnRemoveTemplateItems = m_qtvTemplateItems->createToolButton("", "icons/Remove.png", "Remove Selected Items From Selected Templates");
    //_btnRemoveTemplateItems->clicked().connect(this, &Views::ViewTaskTemplates::btnRemoveTemplateItemsClicked);
}

void Views::ViewTaskTemplates::prepareView()
{
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    //Template/TemplateItems Table Views
    m_cntTemplatesAndItems = new Wt::WContainerWidget();
    m_layMain->addWidget(m_cntTemplatesAndItems);

    m_layCntTemplatesAndItems = new Wt::WVBoxLayout();
    m_layCntTemplatesAndItems->setContentsMargins(0,0,0,0);
    m_layCntTemplatesAndItems->setSpacing(6);

    m_cntTemplatesAndItems->setLayout(m_layCntTemplatesAndItems);

    //Templates
    m_cntTemplate = new Ms::Widgets::MContainerWidget();
    m_cntTemplate->setTitle("<b><i>Templates</i></b>");

    m_layCntTemplatesAndItems->addWidget(m_cntTemplate);

    Wt::WVBoxLayout *_layCntTemplate = dynamic_cast<Wt::WVBoxLayout*>(m_cntTemplate->layout());
    _layCntTemplate->setContentsMargins(0,14,0,0);

    createTemplatesTableView();
    _layCntTemplate->addWidget(m_qtvTemplates, 1);

    //Template Items
    m_cntTemplateItems = new Ms::Widgets::MContainerWidget();
    m_cntTemplateItems->setTitle("<b><i>Template Items</i></b>");

    m_layCntTemplatesAndItems->addWidget(m_cntTemplateItems);

     Wt::WVBoxLayout *_layCntTemplateItems = dynamic_cast<Wt::WVBoxLayout*>(m_cntTemplateItems->layout());
    _layCntTemplateItems->setContentsMargins(0,14,0,0);

    createTemplateItemsTableView();
    _layCntTemplateItems->addWidget(m_qtvTemplateItems, 1);

    m_layCntTemplatesAndItems->setResizable(0,true);
}
