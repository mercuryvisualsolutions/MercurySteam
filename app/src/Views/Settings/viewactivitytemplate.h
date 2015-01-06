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
        
        Log::Logger *m_logger;

        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectActivityTemplate> *m_qtvTemplates;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectActivityTemplateActivityItem> *m_qtvTemplateItems;

        //UI variables
        Wt::WVBoxLayout *m_layMain;
        Wt::WContainerWidget *m_cntTemplatesAndItems;
        Wt::WVBoxLayout *m_layCntTemplatesAndItems;
        Ms::Widgets::MContainerWidget *m_cntTemplate;
        Ms::Widgets::MContainerWidget *m_cntTemplateItems;
        Wt::WPushButton *m_btnCreateTemplate;
        Wt::WPushButton *m_btnRemoveTemplate;
        Wt::WPushButton *m_btnCreateTemplateItem;
        Wt::WPushButton *m_btnEditTemplateItem;
        Wt::WPushButton *m_btnRemoveTemplateItem;

        //Signals
        Wt::Signal<> m_createTemplateRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectActivityTemplate>>> m_removeTemplatesRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectActivityTemplate>>> m_createTemplateItemRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem>>> m_removeTemplateItemsRequested;

        //Slots
        void btnCreateTemplateClicked();
        void btnRemoveTemplatesClicked();
        void btnCreateTemplateItemClicked();
        void btnRemoveTemplateItemsClicked();
        void btnEditTemplateItemsClicked();

        //Functions
        void createTemplatesTableView();
        void createTemplateItemsTableView();
        void prepareView();
    };
}

#endif // VIEWACTIVITYTEMPLATE_H
