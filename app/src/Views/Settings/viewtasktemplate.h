#ifndef VIEWTASKTEMPLATE_H
#define VIEWTASKTEMPLATE_H

#include "../../Session/sessionmanager.h"
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

        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

        //Signals
        Wt::Signal<> &createTemplateRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>>> &removeTemplatesRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>>> &createTemplateItemRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem>>> &removeTemplateItemsRequested();

    private:
        //Variables
        
        Log::Logger *m_logger;

        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskTemplate> *m_qtvTemplates;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskTemplateTaskItem> *m_qtvTemplateItems;

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
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>>> m_removeTemplatesRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>>> m_createTemplateItemRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem>>> m_removeTemplateItemsRequested;

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

#endif // VIEWTASKTEMPLATE_H
