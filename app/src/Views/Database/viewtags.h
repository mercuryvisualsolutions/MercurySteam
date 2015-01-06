#ifndef VIEWTAGS_H
#define VIEWTAGS_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewTags : public Wt::WContainerWidget
    {
    public:
        ViewTags();

        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *qtvTags();
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *qtvAssignedTags();
        bool isCreateOptionHidden() const;
        void setCreateOptionHidden(bool hidden);

        //Functions
        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

        //Signals
        Wt::Signal<> &createTagRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &assignTagsRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &unassignTagsRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>,bool,bool> &filterByTagsRequested();
        Wt::Signal<> &clearTagsFilterRequested();

    private:
        //Variables
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *m_qtvTags;
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *m_qtvAssignedTags;
        

        //UI variables
        Wt::WVBoxLayout *m_layMain;
        Wt::WContainerWidget *m_cntTags;
        Wt::WVBoxLayout *m_layCntTags;
        Ms::Widgets::MContainerWidget *m_cntAssignedTags;
        Ms::Widgets::MContainerWidget *m_cntAvailableTags;
        Wt::WPushButton *m_btnCreateTag;
        Wt::WPushButton *m_btnAssignTags;
        Wt::WPushButton *m_btnUnassignTags;
        Wt::WPushButton *m_btnFilterByTags;
        Wt::WPushButton *m_btnClearTagsFilter;
        Wt::WPopupMenu *m_mnuFilterByTags;
        Wt::WMenuItem *m_mnuFilterByTagsExactSelectionItem;
        Wt::WMenuItem *m_mnuFilterByTagsAnyOfSelectionItem;
        Wt::WMenuItem *m_mnuFilterByTagsNotInSelectionItem;

        //Signals
        Wt::Signal<> m_createTagRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> m_assignTagsRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> m_unassignTagsRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>,bool,bool> m_filterByTagsRequested;
        Wt::Signal<> m_clearTagsFilterRequested;

        //Slots
        void btnCreateTagClicked();
        void btnAssignTagsClicked();
        void btnUnassignTagsClicked();
        void btnClearTagsFilterClicked();
        void mnuFilterByTagsExactSelectionItemTriggered();
        void mnuFilterByTagsAnyOfSelectionItemTriggered();
        void mnuFilterByTagsNotInSelectionItemTriggered();

        //Functions
        void createTagsTableView();
        void createAssignedTagsTableView();
        void prepareView();
    };
}

#endif // VIEWTAGS_H
