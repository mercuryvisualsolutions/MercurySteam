#ifndef MVIEWBASE_H
#define MVIEWBASE_H

#include <Wt/WContainerWidget>

namespace Ms
{
    namespace Views
    {
        class MViewBase : public Wt::WContainerWidget
        {
        public:
            MViewBase();

            Wt::Signal<> &onExposed();
            Wt::Signal<> &onHidden();

            virtual void setHidden	(bool hidden, const Wt::WAnimation &animation) override;

        private:
            //signals
            Wt::Signal<> _onExposed;
            Wt::Signal<> _onHidden;
        };
    }
}

#endif // MVIEWBASE_H
