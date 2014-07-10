#include "MCheckBoxDelegate.h"

#include <Wt/WModelIndex>
#include <Wt/WCheckBox>
#include <Wt/WContainerWidget>
#include <Wt/WAbstractItemModel>

Ms::Widgets::Delegates::MCheckBoxDelegate::MCheckBoxDelegate()
{
}

Ms::Widgets::Delegates::MCheckBoxDelegate::MCheckBoxDelegate(int editRank)
{
    _editRank = editRank;
}

Wt::WWidget *Ms::Widgets::Delegates::MCheckBoxDelegate::createEditor(const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) const
{
    Wt::WContainerWidget *result = new Wt::WContainerWidget();
    result->setSelectable(true);

    boost::any data = index.data(Wt::EditRole);
    Wt::WCheckBox *chkEdit = new Wt::WCheckBox();
    chkEdit->changed().connect(boost::bind(&Ms::Widgets::Delegates::MCheckBoxDelegate::doCloseEditor, this, result, true));

    if(flags & Wt::RenderFocused)
        chkEdit->setFocus();

    if(!data.empty())
        chkEdit->setChecked(boost::any_cast<bool>(data));

    result->addWidget(chkEdit);

    return result;
}

void Ms::Widgets::Delegates::MCheckBoxDelegate::doCloseEditor(Wt::WWidget *editor, bool save) const
{
    closeEditor()(editor, save);
}

void Ms::Widgets::Delegates::MCheckBoxDelegate::setModelData(const boost::any &editState, Wt::WAbstractItemModel *model, const Wt::WModelIndex &index) const
{
    model->setData(index, editState, Wt::EditRole);
}

boost::any Ms::Widgets::Delegates::MCheckBoxDelegate::editState(Wt::WWidget *editor) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WCheckBox *chkEdit = dynamic_cast<Wt::WCheckBox*>(container->widget(0));
    return boost::any(chkEdit->isChecked());
}

void Ms::Widgets::Delegates::MCheckBoxDelegate::setEditState(Wt::WWidget *editor, const boost::any &value) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WCheckBox *chkEdit = dynamic_cast<Wt::WCheckBox*>(container->widget(0));
    chkEdit->setChecked(boost::any_cast<bool>(value));
}

Wt::WWidget *Ms::Widgets::Delegates::MCheckBoxDelegate::update(Wt::WWidget *widget, const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags)
{
    Wt::WContainerWidget *result;
    Wt::WCheckBox *chkEdit;
    if(!widget)
    {
        result = new Wt::WContainerWidget();
        chkEdit = new Wt::WCheckBox();
        result->addWidget(chkEdit);
    }
    else
    {
        result = dynamic_cast<Wt::WContainerWidget*>(widget);
        chkEdit = dynamic_cast<Wt::WCheckBox*>(result->widget(0));
    }

    if(flags & Wt::RenderFocused)
        chkEdit->setFocus();

    boost::any data = index.data(Wt::EditRole);
    if(!data.empty())
        chkEdit->setChecked(boost::any_cast<bool>(data));

    chkEdit->changed().connect(std::bind([=]()
    {
        const_cast<Wt::WAbstractItemModel*>(index.model())->setData(index, chkEdit->isChecked(), Wt::EditRole);
    }));

    int rank = boost::any_cast<int>(index.data(Wt::UserRole));
    if(rank > _editRank)//edit rank is not high engough ?
        result->setDisabled(true);//prevent editing

    return result;
}
