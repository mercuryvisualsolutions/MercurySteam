#ifndef VIEWLOG_H
#define VIEWLOG_H

#include <Ms/Widgets/MLogWidget.h>
#include <Ms/Widgets/MPropertiesPanel.h>

#include <Wt/WVBoxLayout>
#include <Wt/WToolBar>
#include <Wt/WPushButton>
#include <Wt/WContainerWidget>
#include <Wt/WAnimation>
#include <Wt/WPanel>
#include <Wt/WScrollArea>

#include <vector>

namespace Views
{
    class ViewLog : public Wt::WContainerWidget
    {
    public:
        ViewLog();

        //functions
        const Ms::Widgets::MLogWidget *logWidget() const;

    private:
        //variables
        //Wt's
        Wt::WVBoxLayout *_layMain;
        Wt::WPanel *_panLog;

        //functions
        void _prepareView();
    };
}

#endif // VIEWLOG_H
