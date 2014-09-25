#ifndef VIEWTASKTEMPLATE_H
#define VIEWTASKTEMPLATE_H

#include "../../Database/dbtables.h"
#include "../../Log/logger.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewTaskTemplates : public Wt::WContainerWidget
    {
    public:
        ViewTaskTemplates();

        const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskTemplate> *qtvTemplates() const;
        const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskTemplateTaskItem> *qtvTemplateItems() const;
        void updateView();
        void updateTaskTemplatesView();
        void updateTaskTemplateItemsView();
        bool isCreateTemplateOptionHidden() const;
        void setCreateTemplateOptionHidden(bool hidden);
        bool isCreateTemplateItemOptionHidden() const;
        void setCreateTemplateItemOptionHidden(bool hidden);
        bool isEditTemplateItemOptionHidden() const;
        void setEditTemplateItemOptionHidden(bool hidden);

        //Signals
        Wt::Signal<> &createTemplateRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>>> &removeTemplatesRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>>> &createTemplateItemRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem>>> &removeTemplateItemsRequested();

    private:
        //Variables
        Log::Logger *_logger;

        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskTemplate> *_qtvTemplates;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskTemplateTaskItem> *_qtvTemplateItems;

        //UI variables
        Wt::WVBoxLayout *_layMain;
        Wt::WContainerWidget *_cntTemplatesAndItems;
        Wt::WVBoxLayout *_layCntTemplatesAndItems;
        Ms::Widgets::MContainerWidget *_cntTemplate;
        Ms::Widgets::MContainerWidget *_cntTemplateItems;
        Wt::WPushButton *_btnCreateTemplate;
        Wt::WPushButton *_btnRemoveTemplate;
        Wt::WPushButton *_btnCreateTemplateItem;
        Wt::WPushButton *_btnEditTemplateItem;
        Wt::WPushButton *_btnRemoveTemplateItem;

        //Signals
        Wt::Signal<> _createTemplateRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>>> _removeTemplatesRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>>> _createTemplateItemRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem>>> _removeTemplateItemsRequested;

        //Slots
        void _btnCreateTemplateClicked();
        void _btnRemoveTemplatesClicked();
        void _btnCreateTemplateItemClicked();
        void _btnRemoveTemplateItemsClicked();
        void _btnEditTemplateItemsClicked();

        //Functions
        void _createTemplatesTableView();
        void _createTemplateItemsTableView();
        void _prepareView();
    };
}

#endif // VIEWTASKTEMPLATE_H
