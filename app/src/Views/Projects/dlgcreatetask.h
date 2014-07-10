#ifndef DLGCREATETASK_H
#define DLGCREATETASK_H


#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WCheckBox>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WDateEdit>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WTextArea>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/QueryModel>
#include <Wt/Dbo/Query>

#include "../../Database/dbtables.h"

#include <Ms/Widgets/MWidgetFactory.h>

namespace Views
{
    class DlgCreateTask : public Wt::WDialog
    {
    public:
        DlgCreateTask();

        Wt::Dbo::ptr<Projects::ProjectTaskType> type() const;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        Wt::Dbo::ptr<Users::User> user() const;
        Wt::WDate startDate() const;
        Wt::WDate endDate() const;
        std::string description() const;
        bool isActive() const;

    private:
        //variables
        Wt::WHBoxLayout *_layMain;
        Wt::WContainerWidget *_cntLeft;
        Wt::WVBoxLayout *_layLeft;
        Wt::WContainerWidget *_cntRight;
        Wt::WVBoxLayout *_layRight;
        Wt::WDateEdit *_datStartDate;
        Wt::WDateEdit *_datEndDate;
        Wt::WComboBox *_cmbStatus;
        Wt::WContainerWidget *_cntCmbStatus;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> *_mdlCmbStatus;
        Wt::WComboBox *_cmbUser;
        Wt::WContainerWidget *_cntCmbUser;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::User>> *_mdlCmbUser;
        Wt::WComboBox *_cmbType;
        Wt::WContainerWidget *_cntCmbType;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskType>> *_mdlCmbType;
        Wt::WTextArea *_txtDescription;
        Wt::WCheckBox *_chkActive;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        void _prepareView();
        void _createCmbUser();
        void _createCmbStatus();
        void _createCmbType();
        bool _validate();

        //slots
        void _btnOkClicked();
    };
}


#endif // DLGCREATETASK_H
