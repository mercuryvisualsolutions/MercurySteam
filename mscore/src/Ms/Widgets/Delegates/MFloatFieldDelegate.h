#ifndef MFLOATFIELDDELEGATE_H
#define MFLOATFIELDDELEGATE_H

#include "MItemDelegate.h"

namespace Ms
{
    namespace Widgets
    {
        namespace  Delegates
        {
            class MFloatFieldDelegate : public MItemDelegate
            {
            public:
                MFloatFieldDelegate();
                MFloatFieldDelegate(int editRank);

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

#endif // MFLOATFIELDDELEGATE_H
