#ifndef MINTFIELDDELEGATE_H
#define MINTFIELDDELEGATE_H

#include "MItemDelegate.h"

namespace Ms
{
    namespace Widgets
    {
        namespace  Delegates
        {
            class MIntFieldDelegate : public MItemDelegate
            {
            public:
                MIntFieldDelegate();
                MIntFieldDelegate(int editRank);

                //functions
                Wt::WWidget *createEditor(const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) const override;
                void doCloseEditor(Wt::WWidget *editor, bool save) const;
                void setModelData(const boost::any &editState, Wt::WAbstractItemModel *model, const Wt::WModelIndex &index) const override;
                boost::any editState(Wt::WWidget *editor) const override;
                void setEditState(Wt::WWidget *editor, const boost::any &value) const override;
            };
        }
    }
}

#endif // MINTFIELDDELEGATE_H
