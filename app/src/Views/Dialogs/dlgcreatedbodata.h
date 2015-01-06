#ifndef DLGCREATEDBODATA_H
#define DLGCREATEDBODATA_H

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
        class DlgCreateDBOData : public Wt::WDialog
        {
        public:
            DlgCreateDBOData();
            //functions

            std::string key() const;
            std::string value() const;
            bool isActive() const;


        private:
            //variables
            Wt::WVBoxLayout *m_layMain;
            Wt::WLineEdit *m_txtKey;
            Wt::WLineEdit *m_txtValue;
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

#endif // DLGCREATEDBODATA_H
