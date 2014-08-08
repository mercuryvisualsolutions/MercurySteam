#ifndef DLGSELECTTASKDBO_H
#define DLGSELECTTASKDBO_H

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WVBoxLayout>
#include <Wt/WComboBox>

#include <Ms/Widgets/MWidgetFactory.h>

namespace Views
{
    class DlgTaskSelectDbo : public Wt::WDialog
    {
    public:
        DlgTaskSelectDbo();

        std::string type() const;

    private:
        //variables
        Wt::WVBoxLayout *_layMain;
        Wt::WComboBox *_cmbType;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        //functions
        void _prepareView();

        //slots
        void _btnOkClicked();
    };
}
#endif // DLGSELECTTASKDBO_H
