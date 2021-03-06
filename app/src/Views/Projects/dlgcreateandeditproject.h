#ifndef DLGCREATEPROJECT_H
#define DLGCREATEPROJECT_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"

#include <Ms/Widgets/MWidgetFactory.h>

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WSpinBox>
#include <Wt/WDoubleSpinBox>
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
    class DlgCreateAndEditProject : public Wt::WDialog
    {
    public:
        DlgCreateAndEditProject(bool editing = false);
        //functions
        std::string projectName() const;
        Wt::WDate startDate() const;
        Wt::WDate endDate() const;
        int duration() const;
        float fps() const;
        int frameWidth() const;
        int frameHeight() const;
        int priority() const;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        Wt::Dbo::ptr<Users::User> manager() const;
        std::string description() const;
        bool isActive() const;

        bool isEditing();

        bool editedStartDate() const;
        bool editedEndDate() const;
        bool editedDuration() const;
        bool editedFps() const;
        bool editedFrameWidth() const;
        bool editedFrameHeight() const;
        bool editedPriority() const;
        bool editedStatus() const;
        bool editedManager() const;
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
        Wt::WLineEdit *m_txtProjectName;
        Wt::WDateEdit *m_datStartDate;
        Wt::WDateEdit *m_datEndDate;
        Wt::WSpinBox *m_spnDuration;
        Wt::WDoubleSpinBox *m_spnFPS;
        Wt::WSpinBox *m_spnWidth;
        Wt::WSpinBox *m_spnHeight;
        Wt::WSpinBox *m_spnPriority;
        Wt::WComboBox *m_cmbStatus;
        Wt::WContainerWidget *m_cntCmbStatus;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> *m_mdlCmbStatus;
        Wt::WComboBox *m_cmbManager;
        Wt::WContainerWidget *m_cntCmbManager;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::User>> *m_mdlCmbManager;
        Wt::WTextArea *m_txtDescription;
        Wt::WComboBox *m_cmbActive;
        Wt::WPushButton *m_btnOk;
        Wt::WPushButton *m_btnCancel;

        bool m_editing;

        //functions
        void prepareView();        
        void createCmbManager();
        void createCmbStatus();
        bool validate();

        //slots
        void btnOkClicked();
    };
}

#endif // DLGCREATEPROJECT_H
