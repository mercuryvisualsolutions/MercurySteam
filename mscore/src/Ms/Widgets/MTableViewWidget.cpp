#include "MTableViewWidget.h"

Ms::Widgets::MTableViewWidget::MTableViewWidget()
{
}

Ms::Widgets::MTableViewWidget::~MTableViewWidget()
{
}

const Wt::WTableView *Ms::Widgets::MTableViewWidget::table() const
{
    return _table;
}

const Ms::Core::MItemModel *Ms::Widgets::MTableViewWidget::model() const
{
    return _model;
}

const Wt::WSortFilterProxyModel *Ms::Widgets::MTableViewWidget::proxyModel() const
{
    return _proxyModel;
}

const std::vector<Ms::Core::MTableViewColumn> Ms::Widgets::MTableViewWidget::columns() const
{
    return _columns;
}

const Wt::WToolBar *Ms::Widgets::MTableViewWidget::tbMain() const
{
    return _tbMain;
}

const Wt::WToolBar *Ms::Widgets::MTableViewWidget::tbGlobal() const
{
    return _tbGlobal;
}

void Ms::Widgets::MTableViewWidget::selectAll() const
{

}

void Ms::Widgets::MTableViewWidget::selectnone() const
{

}

void Ms::Widgets::MTableViewWidget::inverseSelection() const
{

}

void Ms::Widgets::MTableViewWidget::importCSV()
{

}

void Ms::Widgets::MTableViewWidget::exportCSV()
{

}
