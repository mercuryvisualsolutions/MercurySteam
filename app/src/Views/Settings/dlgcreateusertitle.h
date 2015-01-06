#ifndef DLGCREATETITLE_H
#define DLGCREATETITLE_H

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WRegExpValidator>
#include <Wt/WSpinBox>
#include <Wt/WCheckBox>
#include <Wt/WVBoxLayout>
#include <Wt/WTextArea>

#include <Ms/Widgets/MWidgetFactory.h>

namespace Views
{
    class DlgCreateUserTitle : public Wt::WDialog
    {
    public:
        DlgCreateUserTitle();

        //functions
        std::string title() const;
        bool isActive() const;

    private:
        //variables
        Wt::WVBoxLayout *m_layMain;
        Wt::WLineEdit *m_txtTitle;
        Wt::WComboBox *m_cmbActive;
        Wt::WPushButton *m_btnOk;
        Wt::WPushButton *m_btnCancel;

        //functions
        void prepareView();
        bool validate();

        //slots
        void _txtTitleKeyWentUp();
        void btnOkClicked();
    };
}
#endif // DLGCREATETITLE_H
