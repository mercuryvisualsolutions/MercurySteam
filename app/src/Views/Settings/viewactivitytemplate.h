#ifndef VIEWTASKPIPELINE_H
#define VIEWTASKPIPELINE_H

#include "../../Database/dbtables.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewTaskPipeline : public Wt::WContainerWidget
    {
    public:
        ViewTaskPipeline();

        const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskPipeline> *qtvPipelines() const;
        const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskPipelineActivityItem> *qtvPipelineItems() const;
        bool isCreatePipelineOptionHidden() const;
        void setCreatePipelineOptionHidden(bool hidden);
        bool isCreatePipelineItemOptionHidden() const;
        void setCreatePipelineItemOptionHidden(bool hidden);

        //Signals
        Wt::Signal<> &createPipelineRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskPipeline>>> &removePipelinesRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskPipeline>>> &createPipelineItemRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskPipelineActivityItem>>> &removePipelineItemsRequested();

    private:
        //Variables
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskPipeline> *_qtvPipelines;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskPipelineActivityItem> *_qtvPipelineItems;

        //UI variables
        Wt::WVBoxLayout *_layMain;
        Wt::WContainerWidget *_cntPipelinesAndItems;
        Wt::WVBoxLayout *_layCntPipelinesAndItems;
        Ms::Widgets::MContainerWidget *_cntPipeline;
        Ms::Widgets::MContainerWidget *_cntPipelineItems;
        Wt::WPushButton *_btnCreatePipeline;
        Wt::WPushButton *_btnRemovePipeline;
        Wt::WPushButton *_btnCreatePipelineItem;
        Wt::WPushButton *_btnRemovePipelineItem;

        //Signals
        Wt::Signal<> _createPipelineRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskPipeline>>> _removePipelinesRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskPipeline>>> _createPipelineItemRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskPipelineActivityItem>>> _removePipelineItemsRequested;

        //Slots
        void _btnCreatePipelineClicked();
        void _btnRemovePipelinesClicked();
        void _btnCreatePipelineItemClicked();
        void _btnRemovePipelineItemsClicked();

        //Functions
        void _createPipelinesTableView();
        void _createPipelineItemsTableView();
        void _prepareView();
    };
}

#endif // VIEWTASKPIPELINE_H
