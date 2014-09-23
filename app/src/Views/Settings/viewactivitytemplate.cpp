#include "viewactivitytemplate.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"
#include "../../Log/logmanager.h"
#include "dlgcreatetemplate.h"
#include "dlgcreateandedittaskpipelineactivityitem.h"

#include <Ms/Widgets/MWidgetFactory.h>

#include <Wt/WApplication>

Views::ViewActivityTemplates::ViewActivityTemplates()
{
    _logger = Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId());

    _prepareView();
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectActivityTemplate> *Views::ViewActivityTemplates::qtvTemplates() const
{
    return _qtvTemplates;
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectActivityTemplateActivityItem> *Views::ViewActivityTemplates::qtvTemplateItems() const
{
    return _qtvTemplateItems;
}

bool Views::ViewActivityTemplates::isCreateTemplateOptionHidden() const
{
    return _btnCreateTemplate->isHidden();
}

void Views::ViewActivityTemplates::setCreateTemplateOptionHidden(bool hidden)
{
    _btnCreateTemplate->setHidden(hidden);
}

bool Views::ViewActivityTemplates::isCreateTemplateItemOptionHidden() const
{
    return _btnCreateTemplateItem->isHidden();
}

void Views::ViewActivityTemplates::setCreateTemplateItemOptionHidden(bool hidden)
{
    _btnCreateTemplateItem->setHidden(hidden);
}

bool Views::ViewActivityTemplates::isEditTemplateItemOptionHidden() const
{
    return _btnEditTemplateItem->isHidden();
}

void Views::ViewActivityTemplates::setEditTemplateItemOptionHidden(bool hidden)
{
    _btnEditTemplateItem->setHidden(hidden);
}

Wt::Signal<> &Views::ViewActivityTemplates::createTemplateRequested()
{
    return _createTemplateRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectActivityTemplate>>> &Views::ViewActivityTemplates::removeTemplatesRequested()
{
    return _removeTemplatesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem>>>  &Views::ViewActivityTemplates::removeTemplateItemsRequested()
{
    return _removeTemplateItemsRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectActivityTemplate>>> &Views::ViewActivityTemplates::createTemplateItemRequested()
{
    return _createTemplateItemRequested;
}

void Views::ViewActivityTemplates::_btnCreateTemplateClicked()
{
    Views::Dialogs::DlgCreateTemplate *dlg = new Views::Dialogs::DlgCreateTemplate();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            if(!Database::DatabaseManager::instance().dboExists<Projects::ProjectActivityTemplate>(dlg->name()))
            {
                Projects::ProjectActivityTemplate *pipeline = new Projects::ProjectActivityTemplate(dlg->name());
                pipeline->setActive(dlg->isActive());

                if(Database::DatabaseManager::instance().createDbo<Projects::ProjectActivityTemplate>(pipeline))
                {
                    updateActivityTemplatesView();

                    _createTemplateRequested();

                    _logger->log(std::string("Created project activity template ") + dlg->name(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete pipeline;

                    _logger->log(std::string("Error creating project activity template ") + dlg->name(), Ms::Log::LogMessageType::Error);
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

void Views::ViewActivityTemplates::_btnRemoveTemplatesClicked()
{
    _removeTemplatesRequested(_qtvTemplates->selectedItems());
}

void Views::ViewActivityTemplates::_btnCreateTemplateItemClicked()
{
    if(_qtvTemplates->table()->selectedIndexes().size() != 1)
    {
        _logger->log("Please select only one template.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditActivityTemplateActivityItem *dlg = new Views::DlgCreateAndEditActivityTemplateActivityItem();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::ptr<Projects::ProjectActivityTemplate> pipelinePtr = _qtvTemplates->selectedItems().at(0);

            Projects::ProjectActivityTemplateActivityItem *pipelineActivityItem = new Projects::ProjectActivityTemplateActivityItem();
            pipelineActivityItem->setType(dlg->type());
            pipelineActivityItem->setStatus(dlg->status());
            pipelineActivityItem->setActivityTemplate(pipelinePtr);
            pipelineActivityItem->setActive(dlg->isActive());

            Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem> pipelineActivityItemPtr =
                    Database::DatabaseManager::instance().createDbo<Projects::ProjectActivityTemplateActivityItem>(pipelineActivityItem);

            if(pipelineActivityItemPtr.get())
            {
                updateActivityTemplateItemsView();

                _createTemplateItemRequested(_qtvTemplates->selectedItems());

                _logger->log(std::string("Created item for activity template ") + pipelinePtr->name(), Ms::Log::LogMessageType::Info);
            }
            else
            {
                delete pipelineActivityItem;

                _logger->log(std::string("Error creating item for activity template ") + pipelinePtr->name(), Ms::Log::LogMessageType::Error);
            }
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewActivityTemplates::_btnRemoveTemplateItemsClicked()
{
    _removeTemplateItemsRequested(_qtvTemplateItems->selectedItems());
}

void Views::ViewActivityTemplates::_btnEditTemplateItemsClicked()
{
    if(_qtvTemplateItems->table()->selectedIndexes().size() == 0)
    {
        _logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditActivityTemplateActivityItem *dlg = new Views::DlgCreateAndEditActivityTemplateActivityItem(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            for(auto itemPtr : _qtvTemplateItems->selectedItems())
            {
                if(dlg->editedType())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectActivityTemplateActivityItem>(itemPtr)->setType(dlg->type());

                if(dlg->editedStatus())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectActivityTemplateActivityItem>(itemPtr)->setStatus(dlg->status());

                if(dlg->editedHours())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectActivityTemplateActivityItem>(itemPtr)->setHours(dlg->hours());

                if(dlg->editedDescription())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectActivityTemplateActivityItem>(itemPtr)->setDescription(dlg->description());

                if(dlg->editedActive())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectActivityTemplateActivityItem>(itemPtr)->setActive(dlg->isActive());
            }

            updateActivityTemplateItemsView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewActivityTemplates::updateView()
{
    updateActivityTemplatesView();
    updateActivityTemplateItemsView();
}

void Views::ViewActivityTemplates::updateActivityTemplatesView()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectActivityTemplate>> query;

        int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

        if(AppSettings::instance().isLoadInactiveData())
            query = Database::DatabaseManager::instance().session()->find<Projects::ProjectActivityTemplate>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Database::DatabaseManager::instance().session()->find<Projects::ProjectActivityTemplate>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

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

void Views::ViewActivityTemplates::updateActivityTemplateItemsView()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem>> query = Database::DatabaseManager::instance().session()->find<Projects::ProjectActivityTemplateActivityItem>();

        if(_qtvTemplates->table()->selectedIndexes().size() > 0)
        {
            std::vector<std::string> pipelineIdValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectActivityTemplate>(_qtvTemplates->selectedItems());

            //generate the where clause
            query.where("Project_Activity_Template_Name IN (" + pipelineIdValues.at(0) + ")");

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

        _qtvTemplateItems->addColumn(Ms::Widgets::MTableViewColumn("Project_Task_Activity_Type", "Type", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectTaskActivityType>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskActivityType>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectTaskActivityType>().where("Active = ?").bind(true),
         "Type", editRank)));

        _qtvTemplateItems->addColumn(Ms::Widgets::MTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
        Database::DatabaseManager::instance().session(),AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true), "Status", editRank)));

        _qtvTemplateItems->addColumn(Ms::Widgets::MTableViewColumn("Project_Activity_Template_Name", "Template Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        _qtvTemplateItems->addColumn(Ms::Widgets::MTableViewColumn("Hours", "Hours", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));

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

void Views::ViewActivityTemplates::_createTemplatesTableView()
{
    _qtvTemplates = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectActivityTemplate>(&Database::DatabaseManager::instance());
    _qtvTemplates->table()->selectionChanged().connect(this, &Views::ViewActivityTemplates::updateActivityTemplateItemsView);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        _btnCreateTemplate = _qtvTemplates->createToolButton("", "icons/Add.png", "Create Template");
        _btnCreateTemplate->clicked().connect(this, &Views::ViewActivityTemplates::_btnCreateTemplateClicked);
    }

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //_btnRemoveTemplates = _qtvTemplates->createToolButton("", "icons/Remove.png", "Remove Selected Templates");
        //_btnRemoveTemplates->clicked().connect(this, &Views::ViewActivityTemplates::_btnRemoveTemplatesClicked);
    }
}

void Views::ViewActivityTemplates::_createTemplateItemsTableView()
{
    _qtvTemplateItems = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectActivityTemplateActivityItem>(&Database::DatabaseManager::instance());
    _qtvTemplateItems->setImportCSVFeatureEnabled(false);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        _btnCreateTemplateItem = _qtvTemplateItems->createToolButton("", "icons/Add.png", "Create New Template Item");
        _btnCreateTemplateItem->clicked().connect(this, &Views::ViewActivityTemplates::_btnCreateTemplateItemClicked);
    }

    //requires "edit" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        _btnEditTemplateItem = _qtvTemplateItems->createToolButton("", "icons/Edit.png", "Edit Selected Items");
        _btnEditTemplateItem->clicked().connect(this, &Views::ViewActivityTemplates::_btnEditTemplateItemsClicked);
    }

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //_btnRemoveTemplateItems = _qtvTemplateItems->createToolButton("", "icons/Remove.png", "Remove Selected Items From Selected Templates");
        //_btnRemoveTemplateItems->clicked().connect(this, &Views::ViewActivityTemplates::_btnRemoveTemplateItemsClicked);
    }
}

void Views::ViewActivityTemplates::_prepareView()
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
