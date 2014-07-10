#ifndef MITEMDELEGATE_H
#define MITEMDELEGATE_H

#include <Wt/WItemDelegate>

namespace Ms
{
    namespace Widgets
    {
        namespace Delegates
        {
            struct WidgetRef
            {
                Wt::WWidget *w;
                WidgetRef(Wt::WWidget *widget) : w(widget) { }
            };

            class MItemDelegate : public Wt::WItemDelegate
            {
            public:
                MItemDelegate();
                MItemDelegate(int editRank);

                //functions
                int editRank();
                void setEditRank(int editRank);

                // WAbstractItemDelegate interface
                virtual Wt::WWidget *update(Wt::WWidget *widget, const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) override;

            protected:
                int _editRank;
            };
        }
    }
}


#endif // MITEMDELEGATE_H
