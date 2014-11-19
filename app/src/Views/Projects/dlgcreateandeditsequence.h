#ifndef DLGCREATESEQUENCE_H
#define DLGCREATESEQUENCE_H

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
    class DlgCreateAndEditSequence : public Wt::WDialog
    {
    public:
        DlgCreateAndEditSequence(bool editing = false);

        //functions
        std::string sequenceName() const;
        Wt::WDate startDate() const;
        Wt::WDate endDate() const;
        int duration() const;
        float fps() const;
        int frameWidth() const;
        int frameHeight() const;
        int priority() const;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
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
        Wt::WLineEdit *_txtSequenceName;
        Wt::WDateEdit *_datStartDate;
        Wt::WDateEdit *_datEndDate;
        Wt::WSpinBox *_spnDuration;
        Wt::WDoubleSpinBox *_spnFPS;
        Wt::WSpinBox *_spnWidth;
        Wt::WSpinBox *_spnHeight;
        Wt::WSpinBox *_spnPriority;
        Wt::WComboBox *_cmbStatus;
        Wt::WContainerWidget *_cntCmbStatus;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> *_mdlCmbStatus;
        Wt::WTextArea *_txtDescription;
        Wt::WComboBox *_cmbActive;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        bool _editing;

        //functions
        void _prepareView();
        void _createCmbStatus();
        bool _validate();

        //slots
        void _btnOkClicked();
    };
}

#endif // DLGCREATESEQUENCE_H
