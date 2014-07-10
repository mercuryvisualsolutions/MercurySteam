#ifndef DLGCREATESHOT_H
#define DLGCREATESHOT_H

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

#include "../../Database/dbtables.h"

#include <Ms/Widgets/MWidgetFactory.h>

namespace Views
{
    class DlgCreateShot : public Wt::WDialog
    {
    public:
        DlgCreateShot();

        //functions
        std::string shotName() const;
        Wt::WDate startDate() const;
        Wt::WDate endDate() const;
        int duration() const;
        float fps() const;
        int frameWidth() const;
        int frameHeight() const;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        std::string description() const;
        bool isActive() const;

    private:
        //variables
        Wt::WHBoxLayout *_layMain;
        Wt::WContainerWidget *_cntLeft;
        Wt::WVBoxLayout *_layLeft;
        Wt::WContainerWidget *_cntRight;
        Wt::WVBoxLayout *_layRight;
        Wt::WLineEdit *_txtShotName;
        Wt::WDateEdit *_datStartDate;
        Wt::WDateEdit *_datEndDate;
        Wt::WSpinBox *_spnDuration;
        Wt::WDoubleSpinBox *_spnFPS;
        Wt::WSpinBox *_spnWidth;
        Wt::WSpinBox *_spnHeight;
        Wt::WComboBox *_cmbStatus;
        Wt::WContainerWidget *_cntCmbStatus;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> *_mdlCmbStatus;
        Wt::WTextArea *_txtDescription;
        Wt::WCheckBox *_chkActive;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        //functions
        void _prepareView();
        void _createCmbStatus();
        bool _validate();

        //slots
        void _btnOkClicked();
    };
}

#endif // DLGCREATESHOT_H
