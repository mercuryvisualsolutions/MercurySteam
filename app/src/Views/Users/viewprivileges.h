#ifndef VIEWPRIVILEGES_H
#define VIEWPRIVILEGES_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewPrivileges : public Wt::WContainerWidget
    {
    public:
        ViewPrivileges();

        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *qtvPrivileges();
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *qtvAssignedPrivileges();

        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

        //Signals
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &assignPrivilegesRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &unassignPrivilegesRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>,bool,bool> &filterByPrivilegesRequested();
        Wt::Signal<> &clearPrivilegesFilterRequested();

    private:
        //Variables
        
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *m_qtvPrivileges;
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *m_qtvAssignedPrivileges;

        //UI variables
        Wt::WVBoxLayout *m_layMain;
        Wt::WContainerWidget *m_cntPrivileges;
        Wt::WVBoxLayout *m_layCntPrivileges;
        Ms::Widgets::MContainerWidget *m_cntAssignedPrivileges;
        Ms::Widgets::MContainerWidget *m_cntAvailablePrivileges;
        Wt::WPushButton *m_btnAssignPrivileges;
        Wt::WPushButton *m_btnUnassignPrivileges;
        Wt::WPushButton *m_btnFilterByPrivileges;
        Wt::WPushButton *m_btnClearPrivilegeFilter;
        Wt::WPopupMenu *m_mnuFilterByPrivileges;
        Wt::WPopupMenuItem *m_mnuFilterByPrivilegesExactSelectionItem;
        Wt::WPopupMenuItem *m_mnuFilterByPrivilegesAnyOfSelectionItem;
        Wt::WPopupMenuItem *m_mnuFilterByPrivilegesNotInSelectionItem;

        //Signals
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> m_assignPrivilegesRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> m_unassignPrivilegesRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>,bool,bool> m_filterByPrivilegesRequested;
        Wt::Signal<> m_clearPrivilegesFilterRequested;

        //Slots
        void btnAssignPrivilegesClicked();
        void btnUnassignPrivilegesClicked();
        void btnClearPrivilegesFilterClicked();
        void mnuFilterByPrivilegesExactSelectionItemTriggered();
        void mnuFilterByPrivilegesAnyOfSelectionItemTriggered();
        void mnuFilterByPrivilegesNotInSelectionItemTriggered();

        //Functions
        void createPrivilegesTableView();
        void createAssignedPrivilegesTableView();
        void prepareView();
    };
}

#endif // VIEWPRIVILEGES_H
