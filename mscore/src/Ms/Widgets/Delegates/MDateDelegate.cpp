#include "MDateDelegate.h"

#include <Wt/WModelIndex>
#include <Wt/WContainerWidget>
#include <Wt/WAbstractItemModel>
#include <Wt/WDateEdit>
#include <Wt/WDate>

Ms::Widgets::Delegates::MDateDelegate::MDateDelegate() :
    MItemDelegate()
{
}

Ms::Widgets::Delegates::MDateDelegate::MDateDelegate(int editRank) :
    MDateDelegate()
{
    _editRank = editRank;
}

void Ms::Widgets::Delegates::MDateDelegate::doCloseEditor(Wt::WWidget *editor, bool save) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WDateEdit *dat = dynamic_cast<Wt::WDateEdit*>(container->widget(0));
    bool valid = false;
    if(save)
        valid = dat->validate() == Wt::WDateValidator::Valid ? true : false;
    closeEditor()(editor, valid ? save : false);
}

Wt::WWidget *Ms::Widgets::Delegates::MDateDelegate::createEditor(const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) const
{
    Wt::WContainerWidget *result = new Wt::WContainerWidget();
    result->setLineHeight(25);
    result->setSelectable(true);

    boost::any data = index.data(Wt::EditRole);

    Wt::WDateEdit *dat = new Wt::WDateEdit();

    dat->escapePressed().connect(boost::bind(&Ms::Widgets::Delegates::MDateDelegate::doCloseEditor, this, result, false));
    dat->selected().connect(boost::bind(&Ms::Widgets::Delegates::MDateDelegate::doCloseEditor, this, result, true));
    dat->enterPressed().connect(boost::bind(&Ms::Widgets::Delegates::MDateDelegate::doCloseEditor, this, result, true));

    if(flags & Wt::RenderFocused)
        dat->setFocus();

    if(!data.empty())
        dat->setDate(boost::any_cast<Wt::WDate>(data));

    result->addWidget(dat);

    return result;
}

void Ms::Widgets::Delegates::MDateDelegate::setModelData(const boost::any &editState, Wt::WAbstractItemModel *model, const Wt::WModelIndex &index) const
{
    model->setData(index, editState, Wt::EditRole);
}

boost::any Ms::Widgets::Delegates::MDateDelegate::editState(Wt::WWidget *editor) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WDateEdit *dat = dynamic_cast<Wt::WDateEdit*>(container->widget(0));
    return boost::any(dat->date());
}

void Ms::Widgets::Delegates::MDateDelegate::setEditState(Wt::WWidget *editor, const boost::any &value) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WDateEdit *dat = dynamic_cast<Wt::WDateEdit*>(container->widget(0));
    dat->setDate(boost::any_cast<Wt::WDate>(value));
}
