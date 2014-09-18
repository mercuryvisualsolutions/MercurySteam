#include "MValidatorFieldDelegate.h"
#include "../MWidgetFactory.h"

#include <Wt/WModelIndex>
#include <Wt/WContainerWidget>
#include <Wt/WAbstractItemModel>
#include <Wt/WLineEdit>
#include <Wt/WVBoxLayout>

Ms::Widgets::Delegates::MValidatorFieldDelegate::MValidatorFieldDelegate() :
    MItemDelegate()
{
    _validatorExp = "[A-Za-z0-9 _-]{4,150}";
    _validatorIsMandatory = true;
}

Ms::Widgets::Delegates::MValidatorFieldDelegate::MValidatorFieldDelegate(const std::string &validatorExp, bool validatorIsMandatory, int editRank) :
    MValidatorFieldDelegate()
{
    _validatorExp = validatorExp;
    _validatorIsMandatory = validatorIsMandatory;
    _editRank = editRank;
}

void Ms::Widgets::Delegates::MValidatorFieldDelegate::doCloseEditor(Wt::WWidget *editor, bool save) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WLineEdit *lineEdit = dynamic_cast<Wt::WLineEdit*>(container->widget(0));
    bool valid = false;
    if(save)
        valid = lineEdit->validate() == Wt::WValidator::Valid ? true : false;

    closeEditor()(editor, save ? valid : save);
}

Wt::WWidget *Ms::Widgets::Delegates::MValidatorFieldDelegate::createEditor(const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) const
{
    Wt::WContainerWidget *result = new Wt::WContainerWidget();
    result->setSelectable(true);
    result->setLayout(new Wt::WVBoxLayout());
    result->layout()->setContentsMargins(0,0,0,0);

    boost::any data = index.data(Wt::EditRole);
    Wt::WLineEdit *lineEdit = Ms::Widgets::MWidgetFactory::createLineEdit("", true, _validatorExp, _validatorIsMandatory);

    if(!data.empty())
        lineEdit->setText(boost::any_cast<std::string>(data));

    lineEdit->enterPressed().connect(boost::bind(&Ms::Widgets::Delegates::MValidatorFieldDelegate::doCloseEditor, this, result, true));
    lineEdit->escapePressed().connect(boost::bind(&Ms::Widgets::Delegates::MValidatorFieldDelegate::doCloseEditor, this, result, false));

    if(flags & Wt::RenderFocused)
        lineEdit->setFocus();

    result->layout()->addWidget(lineEdit);

    return result;
}

void Ms::Widgets::Delegates::MValidatorFieldDelegate::setModelData(const boost::any &editState, Wt::WAbstractItemModel *model, const Wt::WModelIndex &index) const
{
    model->setData(index, editState, Wt::EditRole);
}

boost::any Ms::Widgets::Delegates::MValidatorFieldDelegate::editState(Wt::WWidget *editor) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WLineEdit *lineEdit = dynamic_cast<Wt::WLineEdit*>(container->widget(0));
    return boost::any(lineEdit->text());
}

void Ms::Widgets::Delegates::MValidatorFieldDelegate::setEditState(Wt::WWidget *editor, const boost::any &value) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WLineEdit *lineEdit = dynamic_cast<Wt::WLineEdit*>(container->widget(0));
    lineEdit->setText(boost::any_cast<std::string>(value));
}
