#ifndef DLGCREATEGROUP_H
#define DLGCREATEGROUP_H

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WRegExpValidator>
#include <Wt/WCheckBox>
#include <Wt/WVBoxLayout>
#include <Wt/WTextArea>

#include <Ms/Widgets/MWidgetFactory.h>

namespace Views
{
    class DlgCreateGroup : public Wt::WDialog
    {
    public:
        DlgCreateGroup();

        //functions
        const std::string groupName() const;
        int rank() const;
        bool isActive() const;

    private:
        //variables
        Wt::WVBoxLayout *_layMain;
        Wt::WLineEdit *_txtGroupName;
        Wt::WSpinBox *_spnRank;
        Wt::WComboBox *_cmbActive;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        //functions
        void _prepareView();
        bool _validate();

        //slots
        void _btnOkClicked();
    };
}

#endif // DLGCREATEGROUP_H
