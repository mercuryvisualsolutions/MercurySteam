#ifndef DLGCREATETASKTYPE_H
#define DLGCREATETASKTYPE_H

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WRegExpValidator>
#include <Wt/WCheckBox>
#include <Wt/WVBoxLayout>
#include <Wt/WTextArea>
#include <Wt/WComboBox>

#include <Ms/Widgets/MWidgetFactory.h>

namespace Views
{
    class DlgCreateTaskType : public Wt::WDialog
    {
    public:
        DlgCreateTaskType();

        //functions
        std::string type() const;
        bool isActive() const;

    private:
        //variables
        Wt::WVBoxLayout *m_layMain;
        Wt::WLineEdit *m_txtType;
        Wt::WComboBox *m_cmbActive;
        Wt::WPushButton *m_btnOk;
        Wt::WPushButton *m_btnCancel;

        //functions
        void prepareView();
        bool validate();

        //slots
        void txtTypeKeyWentUp();
        void btnOkClicked();
    };
}

#endif // DLGCREATETASKTYPE_H
