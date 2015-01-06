#ifndef DLGCREATETEMPLATE_H
#define DLGCREATETEMPLATE_H

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
    namespace Dialogs
    {
        class DlgCreateTemplate : public Wt::WDialog
        {
        public:
            DlgCreateTemplate();
            //functions

            std::string name() const;
            bool isActive() const;


        private:
            //variables
            Wt::WVBoxLayout *m_layMain;
            Wt::WLineEdit *m_txtName;
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
}

#endif // DLGCREATETEMPLATE_H
