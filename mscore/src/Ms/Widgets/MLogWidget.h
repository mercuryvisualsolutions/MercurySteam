#ifndef MLOGWIDGET_H
#define MLOGWIDGET_H

#include "../Log/MLogGlobals.h"

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

        void log(const std::string &message, Ms::Log::LogMessageType type);

	private:
	    //variables
	    //Wt's
	    Wt::WVBoxLayout *m_layMain;
        Wt::WScrollArea *m_scrLog;
        Wt::WContainerWidget *m_cntLogWidget;
        Wt::WVBoxLayout *m_layCntLogWidget;
        Wt::WContainerWidget *m_cntLogArea;
        Wt::WVBoxLayout *m_layCntLogArea;
	    Wt::WToolBar *m_tbMain;
        Wt::WContainerWidget *m_cntTbMain;
        Wt::WVBoxLayout *m_layCntTbMain;
        Wt::WPushButton *m_btnClearLog;

        unsigned int m_maxLogMessages;
        std::vector<Wt::WText*> m_logMsgs;

	    //functions
	    //slots
        void onBtnClearLogClicked();

        void checkMaxLogMessages();
	    void prepareView();
	};
    }
}

#endif // MLOGWIDGET_H
