#ifndef VIEWACTIVITYTEMPLATE_H
#define VIEWACTIVITYTEMPLATE_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"
#include "../../Log/logger.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewActivityTemplates : public Wt::WContainerWidget
    {
    public:
        ViewActivityTemplates();

        const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectActivityTemplate> *qtvTemplates() const;
        const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectActivityTemplateActivityItem> *qtvTemplateItems() const;
        void updateView();
        void updateActivityTemplatesView();
        void updateActivityTemplateItemsView();
        bool isCreateTemplateOptionHidden() const;
        void setCreateTemplateOptionHidden(bool hidden);
        bool isCreateTemplateItemOptionHidden() const;
        void setCreateTemplateItemOptionHidden(bool hidden);
        bool isEditTemplateItemOptionHidden() const;
        void setEditTemplateItemOptionHidden(bool hidden);

        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

        //Signals
        Wt::Signal<> &createTemplateRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectActivityTemplate>>> &removeTemplatesRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectActivityTemplate>>> &createTemplateItemRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem>>> &removeTemplateItemsRequested();

    private:
        //Variables
        
        Log::Logger *_logger;

        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectActivityTemplate> *_qtvTemplates;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectActivityTemplateActivityItem> *_qtvTemplateItems;

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
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectActivityTemplate>>> _removeTemplatesRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectActivityTemplate>>> _createTemplateItemRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem>>> _removeTemplateItemsRequested;

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

#endif // VIEWACTIVITYTEMPLATE_H
