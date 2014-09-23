#ifndef DLGSELECTTASKPIPELINE_H
#define DLGSELECTTASKPIPELINE_H

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WTextArea>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/QueryModel>
#include <Wt/Dbo/Query>

#include "../../Database/dbtables.h"

#include <Ms/Widgets/MWidgetFactory.h>

namespace Views
{
    class DlgSelectTaskPipeline : public Wt::WDialog
    {
    public:
        DlgSelectTaskPipeline();

        std::string assetName() const;
        Wt::Dbo::ptr<Projects::ProjectTaskPipeline> pipeline() const;

    private:
        //variables
        Wt::WHBoxLayout *_layMain;
        Wt::WContainerWidget *_cntLeft;
        Wt::WVBoxLayout *_layLeft;
        Wt::WComboBox *_cmbPipeline;
        Wt::WContainerWidget *_cntCmbPipeline;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskPipeline>> *_mdlCmbPipeline;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        //functions
        void _prepareView();
        void _createCmbPipeline();
        bool _validate();

        //slots
        void _btnOkClicked();
    };
}

#endif // DLGSELECTTASKPIPELINE_H
