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
        Wt::WVBoxLayout *_layMain;
        Wt::WLineEdit *_lnEdit;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        void _prepareView();
        bool _validate();

        //slots
        void _txtTypeKeyWentUp();
        void _btnOkClicked();
    };
}
#endif // DLGCREATEREPO_H
