#ifndef MCONTAINERWIDGET_H
#define MCONTAINERWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WText>

namespace Ms
{
    namespace Widgets
    {
        class MContainerWidget : public Wt::WContainerWidget
        {
        public:
            MContainerWidget();

            //functions
            const Wt::WString title() const;
            void setTitle(const Wt::WString &title);

            //signals
            Wt::Signal<> &onExposed();
            Wt::Signal<> &onHidden();

            //Wt::WContainerWidget interface
            virtual void setHidden	(bool hidden, const Wt::WAnimation &animation = Wt::WAnimation()) override;

        private:
            //signals
            Wt::Signal<> m_onExposed;
            Wt::Signal<> m_onHidden;

            //variables
            Wt::WVBoxLayout *m_layMain;
            Wt::WText *m_txtViewTitle;
            Wt::WContainerWidget *m_cntTxtViewTitle;

            //funvtions
            void prepareView();
        };
    }
}

#endif // MCONTAINERWIDGET_H
