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

        void adjustUIPrivileges();

        //Signals
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &assignPrivilegesRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &unassignPrivilegesRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>,bool> &filterByPrivilegesRequested();
        Wt::Signal<> &clearPrivilegesFilterRequested();

    private:
        //Variables
        
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvPrivileges;
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvAssignedPrivileges;

        //UI variables
        Wt::WVBoxLayout *_layMain;
        Wt::WContainerWidget *_cntPrivileges;
        Wt::WVBoxLayout *_layCntPrivileges;
        Ms::Widgets::MContainerWidget *_cntAssignedPrivileges;
        Ms::Widgets::MContainerWidget *_cntAvailablePrivileges;
        Wt::WPushButton *_btnAssignPrivileges;
        Wt::WPushButton *_btnUnassignPrivileges;
        Wt::WPushButton *_btnFilterByPrivileges;
        Wt::WPushButton *_btnClearPrivilegeFilter;
        Wt::WPopupMenu *_mnuFilterByPrivileges;
        Wt::WPopupMenuItem *_mnuFilterByPrivilegesExactSelectionItem;
        Wt::WPopupMenuItem *_mnuFilterByPrivilegesAnyOfSelectionItem;

        //Signals
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> _assignPrivilegesRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> _unassignPrivilegesRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>,bool> _filterByPrivilegesRequested;
        Wt::Signal<> _clearPrivilegesFilterRequested;

        //Slots
        void _btnAssignPrivilegesClicked();
        void _btnUnassignPrivilegesClicked();
        void _btnClearPrivilegesFilterClicked();
        void _mnuFilterByPrivilegesExactSelectionItemTriggered();
        void _mnuFilterByPrivilegesAnyOfSelectionItemTriggered();

        //Functions
        void _createPrivilegesTableView();
        void _createAssignedPrivilegesTableView();
        void _prepareView();
    };
}

#endif // VIEWPRIVILEGES_H
