#ifndef MCOMBOBOXDELEGATE_H
#define MCOMBOBOXDELEGATE_H

#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WComboBox>

#include "MItemDelegate.h"

namespace Ms
{
    namespace Widgets
    {
        namespace Delegates
        {
            class MComboBoxDelegate  : public MItemDelegate
            {
            public:
                MComboBoxDelegate();
                MComboBoxDelegate(std::vector<std::string> items, int editRank = 0);

                //functions
                void setItems(std::vector<std::string> items);
                std::vector<std::string> items();
                Wt::WWidget *createEditor(const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) const override;
                void doCloseEditor(Wt::WWidget *editor, bool save) const;
                void setModelData(const boost::any &editState, Wt::WAbstractItemModel *model, const Wt::WModelIndex &index) const override;
                boost::any editState(Wt::WWidget *editor) const override;
                void setEditState(Wt::WWidget *editor, const boost::any &value) const override;
                Wt::WWidget *update(Wt::WWidget *widget, const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) override;

            private:
                std::vector<std::string> _items;
            };
        }
    }
}

#endif // MCOMBOBOXDELEGATE_H
