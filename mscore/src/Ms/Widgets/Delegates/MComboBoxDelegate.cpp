#include "MComboBoxDelegate.h"

#include <Wt/WStandardItemModel>
#include <Wt/WStandardItem>

Ms::Widgets::Delegates::MComboBoxDelegate::MComboBoxDelegate() :
    MItemDelegate()
{
}

Ms::Widgets::Delegates::MComboBoxDelegate::MComboBoxDelegate(std::vector<std::string> items, int editRank) :
    MComboBoxDelegate()
{
    _editRank = editRank;
    _items = items;
}

void Ms::Widgets::Delegates::MComboBoxDelegate::setItems(std::vector<std::string> items)
{
    _items = items;
}

std::vector<std::string> Ms::Widgets::Delegates::MComboBoxDelegate::items()
{
    return _items;
}

Wt::WWidget *Ms::Widgets::Delegates::MComboBoxDelegate::createEditor(const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) const
{
    Wt::WContainerWidget *result = new Wt::WContainerWidget();
    result->setLineHeight(25);

    result->setSelectable(true);
    result->setLayout(new Wt::WVBoxLayout());
    result->layout()->setContentsMargins(0,0,0,0);

    boost::any data = index.data(Wt::EditRole);

    Wt::WComboBox *comboBox = new Wt::WComboBox();

    Wt::WStandardItemModel *model = new Wt::WStandardItemModel();

    comboBox->setModel(model);

    for(auto &item : _items)
        model->appendRow(new Wt::WStandardItem(item.c_str()));
        //comboBox->addItem(item.c_str());

    comboBox->escapePressed().connect(boost::bind(&Ms::Widgets::Delegates::MComboBoxDelegate::doCloseEditor, this, result, false));
    comboBox->clicked().connect(boost::bind(&Ms::Widgets::Delegates::MComboBoxDelegate::doCloseEditor, this, result, true));
    comboBox->enterPressed().connect(boost::bind(&Ms::Widgets::Delegates::MComboBoxDelegate::doCloseEditor, this, result, true));

    if(flags & Wt::RenderFocused)
        comboBox->setFocus();

    if(!data.empty())
        comboBox->setValueText(boost::any_cast<Wt::WString>(data));

    result->layout()->addWidget(comboBox);

    return result;
}

void Ms::Widgets::Delegates::MComboBoxDelegate::doCloseEditor(Wt::WWidget *editor, bool save) const
{
    closeEditor()(editor, save);
}

void Ms::Widgets::Delegates::MComboBoxDelegate::setModelData(const boost::any &editState, Wt::WAbstractItemModel *model, const Wt::WModelIndex &index) const
{
    model->setData(index, editState, Wt::EditRole);
}

boost::any Ms::Widgets::Delegates::MComboBoxDelegate::editState(Wt::WWidget *editor) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WComboBox *comboBox = dynamic_cast<Wt::WComboBox*>(container->widget(0));
    return boost::any(comboBox->currentText());
}

void Ms::Widgets::Delegates::MComboBoxDelegate::setEditState(Wt::WWidget *editor, const boost::any &value) const
{
    Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
    Wt::WComboBox *comboBox = dynamic_cast<Wt::WComboBox*>(container->widget(0));
    comboBox->setValueText(boost::any_cast<Wt::WString>(value));
}

Wt::WWidget *Ms::Widgets::Delegates::MComboBoxDelegate::update(Wt::WWidget *widget, const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags)
{
    return MItemDelegate::update(widget, index, flags);
}
