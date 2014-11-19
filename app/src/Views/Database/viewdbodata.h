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
        void adjustUIPrivileges();

        //Signals
        Wt::Signal<> &addDataRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::DboData>>> &removeDataRequested();

    private:
        //Variables
        Ms::Widgets::MQueryTableViewWidget<Database::DboData> *_qtvData;
        Wt::WPushButton *_btnCreateData;
        
        //UI variables
        Wt::WVBoxLayout *_layMain;

        //Signals
        Wt::Signal<> _addDataRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::DboData>>> _removeDataRequested;

        //Slots
        void _btnCreateDataClicked();
        void _btnRemoveDataClicked();

        //Functions
        void _createDataTableView();
        void _prepareView();
    };
}

#endif // VIEWDBODATA_H
