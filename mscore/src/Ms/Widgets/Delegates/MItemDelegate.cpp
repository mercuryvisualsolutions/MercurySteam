#include "MItemDelegate.h"

#include <Wt/WModelIndex>
#include <Wt/WTextArea>

#include <Wt/WBoostAny>
#include <Wt/WTextArea>

Ms::Widgets::Delegates::MItemDelegate::MItemDelegate()
{
}

Ms::Widgets::Delegates::MItemDelegate::MItemDelegate(int editRank)
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
    Wt::WWidget *wid = Wt::WItemDelegate::update(widget, index, flags);
//    Wt::WTextArea *txtArea;
//    if(!widget)
//        txtArea = new Wt::WTextArea(Wt::asString(index.data(Wt::DisplayRole)));
//    else
//        txtArea = dynamic_cast<Wt::WTextArea*>(widget);

//    if(flags & Wt::RenderFocused)
//        txtArea->setFocus();

    boost::any rankData = index.data(Wt::UserRole);

    if(!rankData.empty())
    {
        int rank = boost::any_cast<int>(rankData);
        if(rank > _editRank)//edit rank is not high engough ?
            wid->setDisabled(true);//prevent editing
    }

    return wid;
}
