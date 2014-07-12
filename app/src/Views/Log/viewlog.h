#ifndef VIEWLOG_H
#define VIEWLOG_H

#include <Wt/WVBoxLayout>
#include <Wt/WToolBar>
#include <Wt/WPushButton>
#include <Wt/WContainerWidget>
#include <Wt/WAnimation>
#include <Wt/WPanel>
#include <Wt/WScrollArea>

#include <vector>

#include <Ms/Widgets/MLogWidget.h>

namespace Views
{
    class ViewLog : public Wt::WContainerWidget
    {
    public:
        ViewLog();

        const Ms::Widgets::MLogWidget *logWidget() const;

        void info(const std::string &message);
        void warning(const std::string &message);
        void error(const std::string &message);
        void fatal(const std::string &message);

    private:
        //variables
        //Wt's
        Wt::WVBoxLayout *_layMain;
        Wt::WPanel *_panLog;

        Ms::Widgets::MLogWidget *_logWidget;

        //functions
        void _prepareView();
    };
}

#endif // VIEWLOG_H
