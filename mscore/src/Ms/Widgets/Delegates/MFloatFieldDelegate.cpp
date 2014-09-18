#include "MFloatFieldDelegate.h"
#include "../MWidgetFactory.h"

#include <iostream>

#include <Wt/WModelIndex>
#include <Wt/WContainerWidget>
#include <Wt/WAbstractItemModel>
#include <Wt/WLineEdit>
#include <Wt/WVBoxLayout>

Ms::Widgets::Delegates::MFloatFieldDelegate::MFloatFieldDelegate() :
    MItemDelegate()
{
}

Ms::Widgets::Delegates::MFloatFieldDelegate::MFloatFieldDelegate(int editRank) :
    MFloatFieldDelegate()
{
    _editRank = editRank;
}

void Ms::Widgets::Delegates::MFloatFieldDelegate::doCloseEditor(Wt::WWidget *editor, bool save) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WLineEdit *lineEdit = dynamic_cast<Wt::WLineEdit*>(container->widget(0));
    bool valid = false;
    if(save)
        valid = lineEdit->validate() == Wt::WValidator::Valid ? true : false;
    closeEditor()(editor, valid ? save : false);
}

Wt::WWidget *Ms::Widgets::Delegates::MFloatFieldDelegate::createEditor(const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) const
{
    Wt::WContainerWidget *result = new Wt::WContainerWidget();
    result->setLineHeight(25);
    result->setSelectable(true);
    result->setLayout(new Wt::WVBoxLayout());
    result->layout()->setContentsMargins(0,0,0,0);

    boost::any data = index.data(Wt::EditRole);
    Wt::WLineEdit *lineEdit = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[0-9.0]{1,255}", true);

    if(!data.empty())
        lineEdit->setText(std::to_string(boost::any_cast<float>(data)));

    lineEdit->enterPressed().connect(boost::bind(&Ms::Widgets::Delegates::MFloatFieldDelegate::doCloseEditor, this, result, true));
    lineEdit->escapePressed().connect(boost::bind(&Ms::Widgets::Delegates::MFloatFieldDelegate::doCloseEditor, this, result, false));

    if(flags & Wt::RenderFocused)
        lineEdit->setFocus();

    result->layout()->addWidget(lineEdit);

    return result;
}

void Ms::Widgets::Delegates::MFloatFieldDelegate::setModelData(const boost::any &editState, Wt::WAbstractItemModel *model, const Wt::WModelIndex &index) const
{
    model->setData(index, editState, Wt::EditRole);
}

boost::any Ms::Widgets::Delegates::MFloatFieldDelegate::editState(Wt::WWidget *editor) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WLineEdit *lineEdit = dynamic_cast<Wt::WLineEdit*>(container->widget(0));
    return boost::any(std::stof(lineEdit->text()));
}

void Ms::Widgets::Delegates::MFloatFieldDelegate::setEditState(Wt::WWidget *editor, const boost::any &value) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WLineEdit *lineEdit = dynamic_cast<Wt::WLineEdit*>(container->widget(0));
    lineEdit->setText(std::to_string(boost::any_cast<float>(value)));
}
