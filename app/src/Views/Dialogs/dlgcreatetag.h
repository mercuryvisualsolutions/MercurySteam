#ifndef DLGCREATETAG_H
#define DLGCREATETAG_H

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
    namespace Dialogs
    {
        class DlgCreateTag : public Wt::WDialog
        {
        public:
            DlgCreateTag();

            //functions
            std::string tagName() const;
            std::string tagContent() const;
            bool isActive() const;

        private:
            //variables
            Wt::WVBoxLayout *m_layMain;
            Wt::WLineEdit *m_txtTagName;
            Wt::WTextArea *m_txtTagContent;
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
#endif // DLGCREATETAG_H
