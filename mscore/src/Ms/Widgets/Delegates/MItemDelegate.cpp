#include "MItemDelegate.h"

#include <Wt/WModelIndex>
#include <Wt/WTextArea>

#include <Wt/WBoostAny>
#include <Wt/WTextArea>
#include <Wt/WHBoxLayout>
#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include <Wt/WAbstractItemModel>
#include <Wt/WLabel>

Ms::Widgets::Delegates::MItemDelegate::MItemDelegate() :
    Wt::WItemDelegate()
{
    _editRank = 0;
}

Ms::Widgets::Delegates::MItemDelegate::MItemDelegate(int editRank) :
    MItemDelegate()
{
    setEditRank(editRank);
}

int Ms::Widgets::Delegates::MItemDelegate::editRank()
{
    return _editRank;
}

void Ms::Widgets::Delegates::MItemDelegate::setEditRank(int editRank)
{
    _editRank = editRank;
}

Wt::WWidget *Ms::Widgets::Delegates::MItemDelegate::update(Wt::WWidget *widget, const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags)
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
            display->setLineHeight(25);

            Wt::WLabel *text = new Wt::WLabel();
            text->setTextFormat(Wt::PlainText);
            text->setWordWrap(true);

            boost::any data = index.data(Wt::EditRole);

            if(!data.empty())
                text->setText(Wt::asString(data));

            display->addWidget(text);

            widgetRef.w = display;//assign the render widget to our reference
        }
    }

    boost::any rankData = index.data(Wt::UserRole);

    if(!rankData.empty())
    {
        int rank = boost::any_cast<int>(rankData);
        if(rank > _editRank)//edit rank is not high engough ?
            widgetRef.w->setDisabled(true);//prevent editing
    }

    return widgetRef.w;//return wehther the editor or the display widget
}

boost::any Ms::Widgets::Delegates::MItemDelegate::editState(Wt::WWidget *widget) const
{
    Wt::WContainerWidget *cnt = dynamic_cast<Wt::WContainerWidget*>(widget);
    Wt::WTextArea *txtArea = dynamic_cast<Wt::WTextArea*>(cnt->widget(0));
    return boost::any(txtArea->text());
}

void Ms::Widgets::Delegates::MItemDelegate::setEditState(Wt::WWidget *widget, const boost::any &value) const
{
    Wt::WContainerWidget *cnt = dynamic_cast<Wt::WContainerWidget*>(widget);
    Wt::WTextArea *txtArea = dynamic_cast<Wt::WTextArea*>(cnt->widget(0));
    txtArea->setText(boost::any_cast<std::string>(value));
}

void Ms::Widgets::Delegates::MItemDelegate::setModelData(const boost::any &editState, Wt::WAbstractItemModel *model, const Wt::WModelIndex &index) const
{
    model->setData(index, editState, Wt::EditRole);
}

void Ms::Widgets::Delegates::MItemDelegate::doCloseEditor(Wt::WWidget *editor, bool save) const
{
    closeEditor()(editor, save);
}

Wt::WWidget *Ms::Widgets::Delegates::MItemDelegate::createEditor(const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) const
{
    Wt::WHBoxLayout *layCnt = new Wt::WHBoxLayout();
    layCnt->setContentsMargins(0,0,0,0);
    layCnt->setSpacing(0);

    Wt::WContainerWidget *cnt = new Wt::WContainerWidget();
    cnt->setSelectable(true);
    cnt->setLayout(layCnt);

    Wt::WTextArea *txtArea = new Wt::WTextArea();
    txtArea->setText(Wt::asString(index.data(Wt::EditRole)));
    txtArea->escapePressed().connect(boost::bind(&Ms::Widgets::Delegates::MItemDelegate::doCloseEditor, this, cnt, false));
    if(flags & Wt::RenderFocused)
        txtArea->setFocus();

    layCnt->addWidget(txtArea, 1);

    Wt::WPushButton *btnSave = new Wt::WPushButton("S");
    btnSave->setWidth(20);
    btnSave->clicked().connect(boost::bind(&Ms::Widgets::Delegates::MItemDelegate::doCloseEditor, this, cnt, true));

    layCnt->addWidget(btnSave);

    return cnt;
}
