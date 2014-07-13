#ifndef MLOGWIDGET_H
#define MLOGWIDGET_H

#include "../MLogGlobals.h"

#include <Wt/WVBoxLayout>
#include <Wt/WToolBar>
#include <Wt/WPushButton>
#include <Wt/WContainerWidget>
#include <Wt/WAnimation>
#include <Wt/WPanel>
#include <Wt/WScrollArea>

#include <vector>

namespace Ms
{
    namespace Log
    {
        namespace Widgets
        {
            class MLogWidget : public Wt::WContainerWidget
            {
            public:
                MLogWidget();
                ~MLogWidget();

                unsigned int maxLogMessages();
                void setMaxLogMessages(unsigned int value);
                void clearLog();

                void log(const std::string &message, LogMessageType type);

            private:
                //variables
                //Wt's
                Wt::WVBoxLayout *_layMain;
                Wt::WScrollArea *_scrLog;
                Wt::WContainerWidget *_cntLogWidget;
                Wt::WVBoxLayout *_layCntLogWidget;
                Wt::WContainerWidget *_cntLogArea;
                Wt::WVBoxLayout *_layCntLogArea;
                Wt::WToolBar *_tbMain;
                Wt::WPushButton *_btnClearLog;

                unsigned int _maxLogMessages;
                std::vector<Wt::WText*> _logMsgs;

                //functions
                //slots
                void _onBtnClearLogClicked();

                void _checkMaxLogMessages();
                void _prepareView();
            };
        }
    }
}

#endif // MLOGWIDGET_H
