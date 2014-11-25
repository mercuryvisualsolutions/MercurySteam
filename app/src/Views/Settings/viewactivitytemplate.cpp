#include "viewactivitytemplate.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"
#include "../../Log/logmanager.h"
#include "dlgcreatetemplate.h"
#include "dlgcreateandeditactivitytemplateitem.h"

#include <Ms/Widgets/MWidgetFactory.h>

#include <Wt/WApplication>

Views::ViewActivityTemplates::ViewActivityTemplates()
{
    _logger = Session::SessionManager::instance().logger();

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
    if(_btnCreateTemplate)
        return _btnCreateTemplate->isHidden();

    return false;
}

void Views::ViewActivityTemplates::setCreateTemplateOptionHidden(bool hidden)
{
    if(_btnCreateTemplate)
        _btnCreateTemplate->setHidden(hidden);
}

bool Views::ViewActivityTemplates::isCreateTemplateItemOptionHidden() const
{
    if(_btnCreateTemplateItem)
        return _btnCreateTemplateItem->isHidden();

    return false;
}

void Views::ViewActivityTemplates::setCreateTemplateItemOptionHidden(bool hidden)
{
    if(_btnCreateTemplateItem)
        _btnCreateTemplateItem->setHidden(hidden);
}

bool Views::ViewActivityTemplates::isEditTemplateItemOptionHidden() const
{
    if(_btnEditTemplateItem)
        return _btnEditTemplateItem->isHidden();

    return false;
}

void Views::ViewActivityTemplates::setEditTemplateItemOptionHidden(bool hidden)
{
    if(_btnEditTemplateItem)
        _btnEditTemplateItem->setHidden(hidden);
}

void Views::ViewActivityTemplates::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasEditPriv = user->hasPrivilege("Edit");
    bool hasCreateDBOPriv = user->hasPrivilege("Create DBO");

    _btnCreateTemplate->setHidden(!hasCreateDBOPriv);
    _btnCreateTemplateItem->setHidden(!hasCreateDBOPriv);
    _btnEditTemplateItem->setHidden(!hasEditPriv);

    _qtvTemplates->setImportCSVFeatureEnabled(hasCreateDBOPriv);
    _qtvTemplateItems->setImportCSVFeatureEnabled(hasCreateDBOPriv);
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
            if(!Session::SessionManager::instance().dboSession().dboExists<Projects::ProjectActivityTemplate>(dlg->name()))
            {
                Projects::ProjectActivityTemplate *activityTemplate = new Projects::ProjectActivityTemplate(dlg->name());
                activityTemplate->setActive(dlg->isActive());

                if(Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectActivityTemplate>(activityTemplate))
                {
                    updateActivityTemplatesView();

                    _createTemplateRequested();

                    _logger->log(std::string("Created project activity template ") + dlg->name(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete activityTemplate;

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

    Views::DlgCreateAndEditActivityTemplateItem *dlg = new Views::DlgCreateAndEditActivityTemplateItem();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::ptr<Projects::ProjectActivityTemplate> templatePtr = _qtvTemplates->selectedItems().at(0);

            Projects::ProjectActivityTemplateActivityItem *templateItem = new Projects::ProjectActivityTemplateActivityItem();
            templateItem->setType(dlg->type());
            templateItem->setStatus(dlg->status());
            templateItem->setDescription(dlg->description());
            templateItem->setActivityTemplate(templatePtr);
            templateItem->setHours(dlg->hours());
            templateItem->setActive(dlg->isActive());

            Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem> templateItemPtr =
                    Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectActivityTemplateActivityItem>(templateItem);

            if(templateItemPtr.get())
            {
                updateActivityTemplateItemsView();

                _createTemplateItemRequested(_qtvTemplates->selectedItems());

                _logger->log(std::string("Created item for activity template ") + templatePtr->name(), Ms::Log::LogMessageType::Info);
            }
            else
            {
                delete templateItem;

                _logger->log(std::string("Error creating item for activity template ") + templatePtr->name(), Ms::Log::LogMessageType::Error);
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

    Views::DlgCreateAndEditActivityTemplateItem *dlg = new Views::DlgCreateAndEditActivityTemplateItem(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            for(auto itemPtr : _qtvTemplateItems->selectedItems())
            {
                if(dlg->editedType())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectActivityTemplateActivityItem>(itemPtr)->setType(dlg->type());

                if(dlg->editedStatus())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectActivityTemplateActivityItem>(itemPtr)->setStatus(dlg->status());

                if(dlg->editedHours())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectActivityTemplateActivityItem>(itemPtr)->setHours(dlg->hours());

                if(dlg->editedDescription())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectActivityTemplateActivityItem>(itemPtr)->setDescription(dlg->description());

                if(dlg->editedActive())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectActivityTemplateActivityItem>(itemPtr)->setActive(dlg->isActive());
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
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectActivityTemplate>> query;

        int viewRank = Session::SessionManager::instance().user()->viewRank();

        if(AppSettings::instance().isLoadInactiveData())
            query = Session::SessionManager::instance().dboSession().find<Projects::ProjectActivityTemplate>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Session::SessionManager::instance().dboSession().find<Projects::ProjectActivityTemplate>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        _qtvTemplates->setQuery(query);

        transaction.commit();

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        _qtvTemplates->clearColumns();

        //add columns
        _qtvTemplates->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

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
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem>> query = Session::SessionManager::instance().dboSession().find<Projects::ProjectActivityTemplateActivityItem>();

        if(_qtvTemplates->table()->selectedIndexes().size() > 0)
        {
            std::vector<std::string> templateIdValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectActivityTemplate>(_qtvTemplates->selectedItems());

            //generate the where clause
            query.where("Project_Activity_Template_Name IN (" + templateIdValues.at(0) + ")");

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Session::SessionManager::instance().user()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("id = ?").bind(-1);//clear the view

        _qtvTemplateItems->setQuery(query);

        transaction.commit();

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        _qtvTemplateItems->clearColumns();

        //add columns
        _qtvTemplateItems->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "Id", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        _qtvTemplateItems->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Task_Activity_Type", "Type", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectTaskActivityType>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskActivityType>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectTaskActivityType>().where("Active = ?").bind(true),
         "Type", editRank)));

        _qtvTemplateItems->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
        &Session::SessionManager::instance().dboSession(),AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true), "Status", editRank)));

        _qtvTemplateItems->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Activity_Template_Name", "Template Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        _qtvTemplateItems->addColumn(Ms::Widgets::MQueryTableViewColumn("Hours", "Hours", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));

        _qtvTemplateItems->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));

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
    _qtvTemplates = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectActivityTemplate>(Session::SessionManager::instance().dboSession());
    _qtvTemplates->table()->selectionChanged().connect(this, &Views::ViewActivityTemplates::updateActivityTemplateItemsView);

    _btnCreateTemplate = _qtvTemplates->createToolButton("", "icons/Add.png", "Create Template");
    _btnCreateTemplate->clicked().connect(this, &Views::ViewActivityTemplates::_btnCreateTemplateClicked);

    //_btnRemoveTemplates = _qtvTemplates->createToolButton("", "icons/Remove.png", "Remove Selected Templates");
    //_btnRemoveTemplates->clicked().connect(this, &Views::ViewActivityTemplates::_btnRemoveTemplatesClicked);
}

void Views::ViewActivityTemplates::_createTemplateItemsTableView()
{
    _qtvTemplateItems = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectActivityTemplateActivityItem>(Session::SessionManager::instance().dboSession());

    _btnCreateTemplateItem = _qtvTemplateItems->createToolButton("", "icons/Add.png", "Create New Template Item");
    _btnCreateTemplateItem->clicked().connect(this, &Views::ViewActivityTemplates::_btnCreateTemplateItemClicked);

    _btnEditTemplateItem = _qtvTemplateItems->createToolButton("", "icons/Edit.png", "Edit Selected Items");
    _btnEditTemplateItem->clicked().connect(this, &Views::ViewActivityTemplates::_btnEditTemplateItemsClicked);

    //_btnRemoveTemplateItems = _qtvTemplateItems->createToolButton("", "icons/Remove.png", "Remove Selected Items From Selected Templates");
    //_btnRemoveTemplateItems->clicked().connect(this, &Views::ViewActivityTemplates::_btnRemoveTemplateItemsClicked);
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
    _layCntTemplatesAndItems->setSpacing(6);

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

    _layCntTemplatesAndItems->setResizable(0,true);
}
