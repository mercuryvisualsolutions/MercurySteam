#include "MDateFieldDelegate.h"

#include <Wt/WModelIndex>
#include <Wt/WContainerWidget>
#include <Wt/WAbstractItemModel>
#include <Wt/WLineEdit>
#include <Wt/WDateValidator>
#include <Wt/WVBoxLayout>

Ms::Widgets::Delegates::MDateFieldDelegate::MDateFieldDelegate() :
    MItemDelegate()
{
}

Ms::Widgets::Delegates::MDateFieldDelegate::MDateFieldDelegate(int editRank) :
    MDateFieldDelegate()
{
    m_editRank = editRank;
}

void Ms::Widgets::Delegates::MDateFieldDelegate::doCloseEditor(Wt::WWidget *editor, bool save) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WLineEdit *lineEdit = dynamic_cast<Wt::WLineEdit*>(container->widget(0));
    bool valid = false;
    if(save)
        valid = lineEdit->validate() == Wt::WDateValidator::Valid ? true : false;
    closeEditor()(editor, valid ? save : false);
}

Wt::WWidget *Ms::Widgets::Delegates::MDateFieldDelegate::createEditor(const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) const
{
    Wt::WContainerWidget *result = new Wt::WContainerWidget();
    result->setLineHeight(25);
    result->setSelectable(true);
    result->setLayout(new Wt::WVBoxLayout());
    result->layout()->setContentsMargins(0,0,0,0);

    boost::any data = index.data(Wt::EditRole);
    Wt::WLineEdit *lineEdit = new Wt::WLineEdit();
    Wt::WDateValidator *val = new Wt::WDateValidator();
    lineEdit->setValidator(val);

    if(!data.empty())
        lineEdit->setText(boost::any_cast<Wt::WDate>(data).toString());

    lineEdit->enterPressed().connect(boost::bind(&Ms::Widgets::Delegates::MDateFieldDelegate::doCloseEditor, this, result, true));
    lineEdit->escapePressed().connect(boost::bind(&Ms::Widgets::Delegates::MDateFieldDelegate::doCloseEditor, this, result, false));

    if(flags & Wt::RenderFocused)
        lineEdit->setFocus();

    result->layout()->addWidget(lineEdit);

    return result;
}

void Ms::Widgets::Delegates::MDateFieldDelegate::setModelData(const boost::any &editState, Wt::WAbstractItemModel *model, const Wt::WModelIndex &index) const
{
    model->setData(index, editState, Wt::EditRole);
}

boost::any Ms::Widgets::Delegates::MDateFieldDelegate::editState(Wt::WWidget *editor) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WLineEdit *lineEdit = dynamic_cast<Wt::WLineEdit*>(container->widget(0));
    return boost::any(Wt::WDate::fromString(lineEdit->text()));
}

void Ms::Widgets::Delegates::MDateFieldDelegate::setEditState(Wt::WWidget *editor, const boost::any &value) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WLineEdit *lineEdit = dynamic_cast<Wt::WLineEdit*>(container->widget(0));
    lineEdit->setText(boost::any_cast<Wt::WDate>(value).toString());
}
