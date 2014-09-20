#include "viewtaskpipeline.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewTaskPipeline::ViewTaskPipeline()
{
    _prepareView();
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskPipeline> *Views::ViewTaskPipeline::qtvPipelines() const
{
    return _qtvPipelines;
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskPipelineActivityItem> *Views::ViewTaskPipeline::qtvPipelineItems() const
{
    return _qtvPipelineItems;
}

bool Views::ViewTaskPipeline::isCreatePipelineOptionHidden() const
{
    return _btnCreatePipeline->isHidden();
}

void Views::ViewTaskPipeline::setCreatePipelineOptionHidden(bool hidden)
{
    _btnCreatePipeline->setHidden(hidden);
}

bool Views::ViewTaskPipeline::isCreatePipelineItemOptionHidden() const
{
    return _btnCreatePipelineItem->isHidden();
}

void Views::ViewTaskPipeline::setCreatePipelineItemOptionHidden(bool hidden)
{
    _btnCreatePipelineItem->setHidden(hidden);
}

Wt::Signal<> &Views::ViewTaskPipeline::createPipelineRequested()
{
    return _createPipelineRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskPipeline>>> &Views::ViewTaskPipeline::removePipelinesRequested()
{
    return _removePipelinesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskPipelineActivityItem>>>  &Views::ViewTaskPipeline::removePipelineItemsRequested()
{
    return _removePipelineItemsRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskPipeline>>> &Views::ViewTaskPipeline::createPipelineItemRequested()
{
    return _createPipelineItemRequested;
}

void Views::ViewTaskPipeline::_btnCreatePipelineClicked()
{
    _createPipelineRequested();
}

void Views::ViewTaskPipeline::_btnRemovePipelinesClicked()
{
    _removePipelinesRequested(_qtvPipelines->selectedItems());
}

void Views::ViewTaskPipeline::_btnCreatePipelineItemClicked()
{
    _createPipelineItemRequested(_qtvPipelines->selectedItems());
}

void Views::ViewTaskPipeline::_btnRemovePipelineItemsClicked()
{
    _removePipelineItemsRequested(_qtvPipelineItems->selectedItems());
}

void Views::ViewTaskPipeline::_createPipelinesTableView()
{
    _qtvPipelines = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTaskPipeline>(&Database::DatabaseManager::instance());

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        _btnCreatePipeline = _qtvPipelines->createToolButton("", "icons/Add.png", "Create Pipeline");
        _btnCreatePipeline->clicked().connect(this, &Views::ViewTaskPipeline::_btnCreatePipelineClicked);
    }

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //_btnRemovePipelines = _qtvPipelines->createToolButton("", "icons/Remove.png", "Remove Selected Pipelines");
        //_btnRemovePipelines->clicked().connect(this, &Views::ViewTaskPipeline::_btnRemovePipelinesClicked);
    }
}

void Views::ViewTaskPipeline::_createPipelineItemsTableView()
{
    _qtvPipelineItems = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTaskPipelineActivityItem>(&Database::DatabaseManager::instance());
    _qtvPipelineItems->setImportCSVFeatureEnabled(false);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        _btnCreatePipelineItem = _qtvPipelineItems->createToolButton("", "icons/Add.png", "Create New Pipeline Item");
        _btnCreatePipelineItem->clicked().connect(this, &Views::ViewTaskPipeline::_btnCreatePipelineItemClicked);
    }

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //_btnRemovePipelineItems = _qtvPipelineItems->createToolButton("", "icons/Remove.png", "Remove Selected Items From Selected Pipelines");
        //_btnRemovePipelineItems->clicked().connect(this, &Views::ViewTaskPipeline::_btnRemovePipelineItemsClicked);
    }
}

void Views::ViewTaskPipeline::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    //Pipeline/PipelineItems Table Views
    _cntPipelinesAndItems = new Wt::WContainerWidget();
    _layMain->addWidget(_cntPipelinesAndItems);

    _layCntPipelinesAndItems = new Wt::WVBoxLayout();
    _layCntPipelinesAndItems->setContentsMargins(0,0,0,0);
    _layCntPipelinesAndItems->setSpacing(0);

    _cntPipelinesAndItems->setLayout(_layCntPipelinesAndItems);

    //Pipelines
    _cntPipeline = new Ms::Widgets::MContainerWidget();
    _cntPipeline->setTitle("<b><i>Pipelines</i></b>");

    _layCntPipelinesAndItems->addWidget(_cntPipeline);

    Wt::WVBoxLayout *_layCntPipeline = dynamic_cast<Wt::WVBoxLayout*>(_cntPipeline->layout());
    _layCntPipeline->setContentsMargins(0,14,0,0);

    _createPipelinesTableView();
    _layCntPipeline->addWidget(_qtvPipelines, 1);

    //Pipeline Items
    _cntPipelineItems = new Ms::Widgets::MContainerWidget();
    _cntPipelineItems->setTitle("<b><i>Pipeline Items</i></b>");

    _layCntPipelinesAndItems->addWidget(_cntPipelineItems);

     Wt::WVBoxLayout *_layCntPipelineItems = dynamic_cast<Wt::WVBoxLayout*>(_cntPipelineItems->layout());
    _layCntPipelineItems->setContentsMargins(0,14,0,0);

    _createPipelineItemsTableView();
    _layCntPipelineItems->addWidget(_qtvPipelineItems, 1);
}
