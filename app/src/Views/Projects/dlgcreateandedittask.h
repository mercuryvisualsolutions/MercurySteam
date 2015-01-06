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
        Wt::WHBoxLayout *m_layMain;
        Wt::WContainerWidget *m_cntLeft;
        Wt::WVBoxLayout *m_layLeft;
        Wt::WContainerWidget *m_cntRight;
        Wt::WVBoxLayout *m_layRight;
        Wt::WDateEdit *m_datStartDate;
        Wt::WDateEdit *m_datEndDate;
        Wt::WSpinBox *m_spnPriority;
        Wt::WComboBox *m_cmbStatus;
        Wt::WContainerWidget *m_cntCmbStatus;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> *m_mdlCmbStatus;
        Wt::WComboBox *m_cmbThumbnail;
        Wt::WComboBox *m_cmbUser;
        Wt::WContainerWidget *m_cntCmbUser;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::User>> *m_mdlCmbUser;
        Wt::WComboBox *m_cmbType;
        Wt::WContainerWidget *m_cntCmbType;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskType>> *m_mdlCmbType;
        Wt::WTextArea *m_txtDescription;
        Wt::WComboBox *m_cmbActive;
        Wt::WPushButton *m_btnOk;
        Wt::WPushButton *m_btnCancel;

        bool m_editing;

        void prepareView();
        void createCmbUser();
        void createCmbStatus();
        void createCmbType();
        bool validate();

        //slots
        void btnOkClicked();
    };
}


#endif // DLGCREATETASK_H
