#ifndef VIEWDBODATA_H
#define VIEWDBODATA_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewDboData : public Wt::WContainerWidget
    {
    public:
        ViewDboData();

        Ms::Widgets::MQueryTableViewWidget<Database::DboData> *qtvData();

        //functions
        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

        //Signals
        Wt::Signal<> &addDataRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::DboData>>> &removeDataRequested();

    private:
        //Variables
        Ms::Widgets::MQueryTableViewWidget<Database::DboData> *m_qtvData;
        Wt::WPushButton *m_btnCreateData;
        
        //UI variables
        Wt::WVBoxLayout *m_layMain;

        //Signals
        Wt::Signal<> m_addDataRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::DboData>>> m_removeDataRequested;

        //Slots
        void btnCreateDataClicked();
        void btnRemoveDataClicked();

        //Functions
        void createDataTableView();
        void prepareView();
    };
}

#endif // VIEWDBODATA_H
