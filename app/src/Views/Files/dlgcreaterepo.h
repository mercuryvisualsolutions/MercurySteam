#ifndef DLGCREATEREPO_H
#define DLGCREATEREPO_H

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WRegExpValidator>
#include <Wt/WSpinBox>
#include <Wt/WCheckBox>
#include <Wt/WVBoxLayout>
#include <Wt/WTextArea>

namespace Views
{
    class DlgCreateRepo : public Wt::WDialog
    {
    public:
        DlgCreateRepo();
        const std::string text() const;

    private:
        //variables
        Wt::WVBoxLayout *m_layMain;
        Wt::WLineEdit *m_lnEdit;
        Wt::WPushButton *m_btnOk;
        Wt::WPushButton *m_btnCancel;

        void prepareView();
        bool validate();

        //slots
        void txtTypeKeyWentUp();
        void btnOkClicked();
    };
}
#endif // DLGCREATEREPO_H
