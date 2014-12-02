#ifndef DLGCREATETASK_H
#define DLGCREATETASK_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"

#include <Ms/Widgets/MWidgetFactory.h>

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

namespace Views
{
    class DlgCreateAndEditTask : public Wt::WDialog
    {
    public:
        DlgCreateAndEditTask(bool editing = false);

        Wt::Dbo::ptr<Projects::ProjectTaskType> type() const;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        std::string thumbnail() const;
        Wt::Dbo::ptr<Users::User> user() const;
        Wt::WDate startDate() const;
        Wt::WDate endDate() const;
        int priority() const;
        std::string description() const;
        bool isActive() const;

        bool isEditing();

        bool editedStartDate() const;
        bool editedEndDate() const;
        bool editedPriority() const;
        bool editedType() const;
        bool editedStatus() const;
        bool editedThumbnail() const;
        bool editedUser() const;
        bool editedDescription() const;
        bool editedActive() const;

    private:
        //variables
        
        //UI
        Wt::WHBoxLayout *_layMain;
        Wt::WContainerWidget *_cntLeft;
        Wt::WVBoxLayout *_layLeft;
        Wt::WContainerWidget *_cntRight;
        Wt::WVBoxLayout *_layRight;
        Wt::WDateEdit *_datStartDate;
        Wt::WDateEdit *_datEndDate;
        Wt::WSpinBox *_spnPriority;
        Wt::WComboBox *_cmbStatus;
        Wt::WContainerWidget *_cntCmbStatus;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> *_mdlCmbStatus;
        Wt::WComboBox *_cmbThumbnail;
        Wt::WComboBox *_cmbUser;
        Wt::WContainerWidget *_cntCmbUser;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::User>> *_mdlCmbUser;
        Wt::WComboBox *_cmbType;
        Wt::WContainerWidget *_cntCmbType;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskType>> *_mdlCmbType;
        Wt::WTextArea *_txtDescription;
        Wt::WComboBox *_cmbActive;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        bool _editing;

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
