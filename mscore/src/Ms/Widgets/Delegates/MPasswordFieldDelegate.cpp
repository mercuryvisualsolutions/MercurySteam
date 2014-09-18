#include "MPasswordFieldDelegate.h"
#include "../MWidgetFactory.h"

#include <Wt/WModelIndex>
#include <Wt/WContainerWidget>
#include <Wt/WAbstractItemModel>
#include <Wt/WLineEdit>
#include <Wt/WLabel>
#include <Wt/WHBoxLayout>

Ms::Widgets::Delegates::MPasswordFieldDelegate::MPasswordFieldDelegate() :
    MItemDelegate()
{
    _validatorExp = "[A-Za-z0-9_-]{6,14}";
    _validatorIsMandatory = true;
}

Ms::Widgets::Delegates::MPasswordFieldDelegate::MPasswordFieldDelegate(const std::string &validatorExp, bool validatorIsMandatory, int editRank) :
    MPasswordFieldDelegate()
{
    _validatorExp = validatorExp;
    _validatorIsMandatory = validatorIsMandatory;
    _editRank = editRank;
}

void Ms::Widgets::Delegates::MPasswordFieldDelegate::doCloseEditor(Wt::WWidget *editor, bool save) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WLineEdit *lineEdit = dynamic_cast<Wt::WLineEdit*>(container->widget(0));
    bool valid = false;
    if(save)
        valid = lineEdit->validate() == Wt::WValidator::Valid ? true : false;
    closeEditor()(editor, valid ? save : false);
}

Wt::WWidget *Ms::Widgets::Delegates::MPasswordFieldDelegate::createEditor(const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) const
{
    Wt::WContainerWidget *result = new Wt::WContainerWidget();
    result->setLineHeight(25);
    result->setSelectable(true);

    boost::any data = index.data(Wt::EditRole);
    Wt::WLineEdit *lineEdit = Ms::Widgets::MWidgetFactory::createLineEdit("", true, _validatorExp, _validatorIsMandatory);

    if(!data.empty())
        lineEdit->setText(boost::any_cast<std::string>(data));

    lineEdit->setEchoMode(Wt::WLineEdit::Password);
    lineEdit->enterPressed().connect(boost::bind(&Ms::Widgets::Delegates::MPasswordFieldDelegate::doCloseEditor, this, result, true));
    lineEdit->escapePressed().connect(boost::bind(&Ms::Widgets::Delegates::MPasswordFieldDelegate::doCloseEditor, this, result, false));

    if(flags & Wt::RenderFocused)
        lineEdit->setFocus();

    result->setLayout(new Wt::WHBoxLayout());
    result->layout()->setContentsMargins(1, 1, 1, 1);
    result->layout()->addWidget(lineEdit);

    return result;
}

void Ms::Widgets::Delegates::MPasswordFieldDelegate::setModelData(const boost::any &editState, Wt::WAbstractItemModel *model, const Wt::WModelIndex &index) const
{
    model->setData(index, editState, Wt::EditRole);
}

boost::any Ms::Widgets::Delegates::MPasswordFieldDelegate::editState(Wt::WWidget *editor) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WLineEdit *lineEdit = dynamic_cast<Wt::WLineEdit*>(container->widget(0));
    return boost::any(lineEdit->text());
}

void Ms::Widgets::Delegates::MPasswordFieldDelegate::setEditState(Wt::WWidget *editor, const boost::any &value) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WLineEdit *lineEdit = dynamic_cast<Wt::WLineEdit*>(container->widget(0));
    lineEdit->setText(boost::any_cast<std::string>(value));
}

Wt::WWidget *Ms::Widgets::Delegates::MPasswordFieldDelegate::update(Wt::WWidget *widget, const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags)
{
    bool editing = widget && (widget->objectName() == "editor");//if widget is not null and the editor is open

    if(flags & Wt::RenderEditing)//if request for editor
    {
        if(!editing)//if not editing already
        {
            widget = createEditor(index, flags);//create the editor
            widget->setObjectName("editor");//assign a name to the editor so we can know later if it's open
        }
    }
    else//not requesting editing
    {
        if(editing)//if we were editing
            widget = 0;//NULL the pointer
    }

    Delegates::WidgetRef widgetRef(widget);//get a reference to current widget

    if(!(flags & Wt::RenderEditing))//if not request for editing (normal update)
    {
        if(!widgetRef.w)//if the reference to our widget is NULL, we create the render widget
        {
            Wt::WContainerWidget *display = new Wt::WContainerWidget();
            display->setLayout(new Wt::WHBoxLayout());
            display->layout()->setContentsMargins(1, 1, 1, 1);
            Wt::WLabel *label = new Wt::WLabel();
            label->setText("******");
            display->layout()->addWidget(label);

            widgetRef.w = display;//assign the render widget to our reference
        }
    }

    return widgetRef.w;//return wehther the editor or the display widget
}
