#ifndef DLGCREATENOTE_H
#define DLGCREATENOTE_H

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
        class DlgCreateNote : public Wt::WDialog
        {
        public:
            DlgCreateNote();
            //functions
    
            std::string content() const;
            bool isActive() const;
    
    
        private:
            //variables
            Wt::WVBoxLayout *m_layMain;
            Wt::WTextArea *m_txtContent;
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

#endif // DLGCREATENOTE_H
