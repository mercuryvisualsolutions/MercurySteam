#ifndef DLGSELECTTASKDBO_H
#define DLGSELECTTASKDBO_H

#include <Ms/Widgets/MWidgetFactory.h>

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WVBoxLayout>
#include <Wt/WComboBox>

namespace Views
{
    class DlgTaskSelectDbo : public Wt::WDialog
    {
    public:
        DlgTaskSelectDbo();

        std::string type() const;

    private:
        //variables
        Wt::WVBoxLayout *m_layMain;
        Wt::WComboBox *m_cmbType;
        Wt::WPushButton *m_btnOk;
        Wt::WPushButton *m_btnCancel;

        //functions
        void prepareView();

        //slots
        void btnOkClicked();
    };
}
#endif // DLGSELECTTASKDBO_H
