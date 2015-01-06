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
        Wt::WVBoxLayout *m_layMain;
        Wt::WLineEdit *m_txtGroupName;
        Wt::WSpinBox *m_spnRank;
        Wt::WComboBox *m_cmbActive;
        Wt::WPushButton *m_btnOk;
        Wt::WPushButton *m_btnCancel;

        //functions
        void prepareView();
        bool validate();

        //slots
        void btnOkClicked();
    };
}

#endif // DLGCREATEGROUP_H
