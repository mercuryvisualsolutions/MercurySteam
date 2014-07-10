#ifndef MPASSWORDFIELDDELEGATE_H
#define MPASSWORDFIELDDELEGATE_H

#include "MItemDelegate.h"

namespace Ms
{
    namespace Widgets
    {
        namespace Delegates
        {
            class MPasswordFieldDelegate : public MItemDelegate
            {
            public:
                MPasswordFieldDelegate();
                MPasswordFieldDelegate(const std::string &validatorExp, bool validatorIsMandatory = true, int editRank = 0);

                //functions
                Wt::WWidget *createEditor(const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) const override;
                void doCloseEditor(Wt::WWidget *editor, bool save) const;
                void setModelData(const boost::any &editState, Wt::WAbstractItemModel *model, const Wt::WModelIndex &index) const override;
                boost::any editState(Wt::WWidget *editor) const override;
                void setEditState(Wt::WWidget *editor, const boost::any &value) const override;
                Wt::WWidget *update(Wt::WWidget *widget, const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) override;
            private:
                std::string _validatorExp;
                bool _validatorIsMandatory;
            };
        }
    }
}

#endif // MPASSWORDFIELDDELEGATE_H
