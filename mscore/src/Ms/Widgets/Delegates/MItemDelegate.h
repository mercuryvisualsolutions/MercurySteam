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
                virtual boost::any editState(Wt::WWidget *widget) const;
                virtual void setEditState(Wt::WWidget *widget, const boost::any &value) const;
                virtual void setModelData(const boost::any &editState, Wt::WAbstractItemModel *model, const Wt::WModelIndex &index) const;
                void doCloseEditor(Wt::WWidget *editor, bool save) const;

            protected:
                int m_editRank;
                virtual Wt::WWidget *createEditor(const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) const;
            };
        }
    }
}


#endif // MITEMDELEGATE_H
