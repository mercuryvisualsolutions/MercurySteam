#include "viewtasktemplate.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"
#include "../../Log/logmanager.h"
#include "dlgcreatetemplate.h"
#include "dlgcreateandedittasktemplateitem.h"

#include <Ms/Widgets/MWidgetFactory.h>

#include <Wt/WApplication>

Views::ViewTaskTemplates::ViewTaskTemplates()
{
    _logger = Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId());

    _prepareView();
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskTemplate> *Views::ViewTaskTemplates::qtvTemplates() const
{
    return _qtvTemplates;
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskTemplateTaskItem> *Views::ViewTaskTemplates::qtvTemplateItems() const
{
    return _qtvTemplateItems;
}

bool Views::ViewTaskTemplates::isCreateTemplateOptionHidden() const
{
    return _btnCreateTemplate->isHidden();
}

void Views::ViewTaskTemplates::setCreateTemplateOptionHidden(bool hidden)
{
    _btnCreateTemplate->setHidden(hidden);
}

bool Views::ViewTaskTemplates::isCreateTemplateItemOptionHidden() const
{
    return _btnCreateTemplateItem->isHidden();
}

void Views::ViewTaskTemplates::setCreateTemplateItemOptionHidden(bool hidden)
{
    _btnCreateTemplateItem->setHidden(hidden);
}

bool Views::ViewTaskTemplates::isEditTemplateItemOptionHidden() const
{
    return _btnEditTemplateItem->isHidden();
}

void Views::ViewTaskTemplates::setEditTemplateItemOptionHidden(bool hidden)
{
    _btnEditTemplateItem->setHidden(hidden);
}

Wt::Signal<> &Views::ViewTaskTemplates::createTemplateRequested()
{
    return _createTemplateRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>>> &Views::ViewTaskTemplates::removeTemplatesRequested()
{
    return _removeTemplatesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem>>>  &Views::ViewTaskTemplates::removeTemplateItemsRequested()
{
    return _removeTemplateItemsRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>>> &Views::ViewTaskTemplates::createTemplateItemRequested()
{
    return _createTemplateItemRequested;
}

void Views::ViewTaskTemplates::_btnCreateTemplateClicked()
{
    Views::Dialogs::DlgCreateTemplate *dlg = new Views::Dialogs::DlgCreateTemplate();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            if(!Database::DatabaseManager::instance().dboExists<Projects::ProjectTaskTemplate>(dlg->name()))
            {
                Projects::ProjectTaskTemplate *taskTemplate = new Projects::ProjectTaskTemplate(dlg->name());
                taskTemplate->setActive(dlg->isActive());

                if(Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskTemplate>(taskTemplate))
                {
                    updateTaskTemplatesView();

                    _createTemplateRequested();

                    _logger->log(std::string("Created project task template ") + dlg->name(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete taskTemplate;

                    _logger->log(std::string("Error creating project task template ") + dlg->name(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                _logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
            }
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewTaskTemplates::_btnRemoveTemplatesClicked()
{
    _removeTemplatesRequested(_qtvTemplates->selectedItems());
}

void Views::ViewTaskTemplates::_btnCreateTemplateItemClicked()
{
    if(_qtvTemplates->table()->selectedIndexes().size() != 1)
    {
        _logger->log("Please select only one template.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditTaskTemplateItem *dlg = new Views::DlgCreateAndEditTaskTemplateItem();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::ptr<Projects::ProjectTaskTemplate> templatePtr = _qtvTemplates->selectedItems().at(0);

            Projects::ProjectTaskTemplateTaskItem *templateItem = new Projects::ProjectTaskTemplateTaskItem();
            templateItem->setType(dlg->type());
            templateItem->setStatus(dlg->status());
            templateItem->setDescription(dlg->description());
            templateItem->setTaskTemplate(templatePtr);
            templateItem->setActive(dlg->isActive());

            Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem> templateItemPtr =
                    Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskTemplateTaskItem>(templateItem);

            if(templateItemPtr.get())
            {
                updateTaskTemplateItemsView();

                _createTemplateItemRequested(_qtvTemplates->selectedItems());

                _logger->log(std::string("Created item for task template ") + templatePtr->name(), Ms::Log::LogMessageType::Info);
            }
            else
            {
                delete templateItem;

                _logger->log(std::string("Error creating item for task template ") + templatePtr->name(), Ms::Log::LogMessageType::Error);
            }
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewTaskTemplates::_btnRemoveTemplateItemsClicked()
{
    _removeTemplateItemsRequested(_qtvTemplateItems->selectedItems());
}

void Views::ViewTaskTemplates::_btnEditTemplateItemsClicked()
{
    if(_qtvTemplateItems->table()->selectedIndexes().size() == 0)
    {
        _logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditTaskTemplateItem *dlg = new Views::DlgCreateAndEditTaskTemplateItem(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            for(auto itemPtr : _qtvTemplateItems->selectedItems())
            {
                if(dlg->editedType())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTaskTemplateTaskItem>(itemPtr)->setType(dlg->type());

                if(dlg->editedStatus())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTaskTemplateTaskItem>(itemPtr)->setStatus(dlg->status());

                if(dlg->editedDescription())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTaskTemplateTaskItem>(itemPtr)->setDescription(dlg->description());

                if(dlg->editedActive())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectTaskTemplateTaskItem>(itemPtr)->setActive(dlg->isActive());
            }

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
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>> query;

        int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

        if(AppSettings::instance().isLoadInactiveData())
            query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskTemplate>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskTemplate>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        _qtvTemplates->setQuery(query);

        bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Auth::AuthManager::instance().currentUser()->editRank();

        _qtvTemplates->clearColumns();

        //add columns
        _qtvTemplates->addColumn(Ms::Widgets::MTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        if(AppSettings::instance().isShowExtraColumns())
            _qtvTemplates->addBaseColumns(flags, editRank);

        _qtvTemplates->updateView();
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
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem>> query = Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskTemplateTaskItem>();

        if(_qtvTemplates->table()->selectedIndexes().size() > 0)
        {
            std::vector<std::string> templateIdValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectTaskTemplate>(_qtvTemplates->selectedItems());

            //generate the where clause
            query.where("Project_Task_Template_Name IN (" + templateIdValues.at(0) + ")");

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("id = ?").bind(-1);//clear the view

        _qtvTemplateItems->setQuery(query);

        bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Auth::AuthManager::instance().currentUser()->editRank();

        _qtvTemplateItems->clearColumns();

        //add columns
        _qtvTemplateItems->addColumn(Ms::Widgets::MTableViewColumn("id", "Id", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        _qtvTemplateItems->addColumn(Ms::Widgets::MTableViewColumn("Project_Task_Type", "Type", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectTaskType>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskType>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskType>().where("Active = ?").bind(true),
         "Type", editRank)));

        _qtvTemplateItems->addColumn(Ms::Widgets::MTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
        Database::DatabaseManager::instance().session(),AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true), "Status", editRank)));

        _qtvTemplateItems->addColumn(Ms::Widgets::MTableViewColumn("Project_Task_Template_Name", "Template Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        _qtvTemplateItems->addColumn(Ms::Widgets::MTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));

        if(AppSettings::instance().isShowExtraColumns())
            _qtvTemplateItems->addBaseColumns(flags, editRank);

        _qtvTemplateItems->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating table view" << std::endl;
    }
}

void Views::ViewTaskTemplates::_createTemplatesTableView()
{
    _qtvTemplates = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTaskTemplate>(&Database::DatabaseManager::instance());
    _qtvTemplates->table()->selectionChanged().connect(this, &Views::ViewTaskTemplates::updateTaskTemplateItemsView);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        _btnCreateTemplate = _qtvTemplates->createToolButton("", "icons/Add.png", "Create Template");
        _btnCreateTemplate->clicked().connect(this, &Views::ViewTaskTemplates::_btnCreateTemplateClicked);
    }

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //_btnRemoveTemplates = _qtvTemplates->createToolButton("", "icons/Remove.png", "Remove Selected Templates");
        //_btnRemoveTemplates->clicked().connect(this, &Views::ViewTaskTemplates::_btnRemoveTemplatesClicked);
    }
}

void Views::ViewTaskTemplates::_createTemplateItemsTableView()
{
    _qtvTemplateItems = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTaskTemplateTaskItem>(&Database::DatabaseManager::instance());
    _qtvTemplateItems->setImportCSVFeatureEnabled(false);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        _btnCreateTemplateItem = _qtvTemplateItems->createToolButton("", "icons/Add.png", "Create New Template Item");
        _btnCreateTemplateItem->clicked().connect(this, &Views::ViewTaskTemplates::_btnCreateTemplateItemClicked);
    }

    //requires "edit" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        _btnEditTemplateItem = _qtvTemplateItems->createToolButton("", "icons/Edit.png", "Edit Selected Items");
        _btnEditTemplateItem->clicked().connect(this, &Views::ViewTaskTemplates::_btnEditTemplateItemsClicked);
    }

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //_btnRemoveTemplateItems = _qtvTemplateItems->createToolButton("", "icons/Remove.png", "Remove Selected Items From Selected Templates");
        //_btnRemoveTemplateItems->clicked().connect(this, &Views::ViewTaskTemplates::_btnRemoveTemplateItemsClicked);
    }
}

void Views::ViewTaskTemplates::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    //Template/TemplateItems Table Views
    _cntTemplatesAndItems = new Wt::WContainerWidget();
    _layMain->addWidget(_cntTemplatesAndItems);

    _layCntTemplatesAndItems = new Wt::WVBoxLayout();
    _layCntTemplatesAndItems->setContentsMargins(0,0,0,0);
    _layCntTemplatesAndItems->setSpacing(0);

    _cntTemplatesAndItems->setLayout(_layCntTemplatesAndItems);

    //Templates
    _cntTemplate = new Ms::Widgets::MContainerWidget();
    _cntTemplate->setTitle("<b><i>Templates</i></b>");

    _layCntTemplatesAndItems->addWidget(_cntTemplate);

    Wt::WVBoxLayout *_layCntTemplate = dynamic_cast<Wt::WVBoxLayout*>(_cntTemplate->layout());
    _layCntTemplate->setContentsMargins(0,14,0,0);

    _createTemplatesTableView();
    _layCntTemplate->addWidget(_qtvTemplates, 1);

    //Template Items
    _cntTemplateItems = new Ms::Widgets::MContainerWidget();
    _cntTemplateItems->setTitle("<b><i>Template Items</i></b>");

    _layCntTemplatesAndItems->addWidget(_cntTemplateItems);

     Wt::WVBoxLayout *_layCntTemplateItems = dynamic_cast<Wt::WVBoxLayout*>(_cntTemplateItems->layout());
    _layCntTemplateItems->setContentsMargins(0,14,0,0);

    _createTemplateItemsTableView();
    _layCntTemplateItems->addWidget(_qtvTemplateItems, 1);
}
