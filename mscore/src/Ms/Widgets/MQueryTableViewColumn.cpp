#include "MQueryTableViewColumn.h"

Ms::Widgets::MQueryTableViewColumn::MQueryTableViewColumn()
{
    setMandatory(false);
    setIgnored(false);
    setFlags(Wt::ItemIsSelectable);
    setDelegate(new Ms::Widgets::Delegates::MItemDelegate);
}

Ms::Widgets::MQueryTableViewColumn::MQueryTableViewColumn(const std::string &dbFieldName) :
    MQueryTableViewColumn()
{
    _dbFieldName = dbFieldName;
}

Ms::Widgets::MQueryTableViewColumn::MQueryTableViewColumn(const std::string &dbFieldName, const std::string &displayName) :
    MQueryTableViewColumn(dbFieldName)
{
    setDisplayName(displayName);
}

Ms::Widgets::MQueryTableViewColumn::MQueryTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags) :
    MQueryTableViewColumn(dbFieldName, displayName)
{
    setFlags(flags);
}

Ms::Widgets::MQueryTableViewColumn::MQueryTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags,
                                        Ms::Widgets::Delegates::MItemDelegate *delegate) :
    MQueryTableViewColumn(dbFieldName, displayName, flags)
{
    setDelegate(delegate);
}

Ms::Widgets::MQueryTableViewColumn::MQueryTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags,
                                        Ms::Widgets::Delegates::MItemDelegate *delegate, bool isMandatory) :
    MQueryTableViewColumn(dbFieldName, displayName, flags, delegate)
{
    setMandatory(isMandatory);
}

Ms::Widgets::MQueryTableViewColumn::MQueryTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags,
                                        Ms::Widgets::Delegates::MItemDelegate *delegate, bool isMandatory, bool isIgnored) :
    MQueryTableViewColumn(dbFieldName, displayName, flags, delegate, isMandatory)
{
    setIgnored(isIgnored);
}

Ms::Widgets::MQueryTableViewColumn::MQueryTableViewColumn(const std::string &dbFieldName, const std::string &displayName,
                                        Wt::WFlags<Wt::ItemFlag> flags, Ms::Widgets::Delegates::MItemDelegate *delegate,
                                        bool isMandatory, bool isIgnored, int width) :
    MQueryTableViewColumn(dbFieldName, displayName, flags, delegate, isMandatory, isIgnored)
{
    setWidth(width);
}

std::string Ms::Widgets::MQueryTableViewColumn::dbFieldName() const
{
    return _dbFieldName;
}

void Ms::Widgets::MQueryTableViewColumn::setDbFieldName(const std::string &dbFieldName)
{
    _dbFieldName = dbFieldName;
}

std::string Ms::Widgets::MQueryTableViewColumn::displayName() const
{
    return _displayName;
}

void Ms::Widgets::MQueryTableViewColumn::setDisplayName(const std::string &displayName)
{
    _displayName = displayName;
}

int Ms::Widgets::MQueryTableViewColumn::width() const
{
    return _width;
}

void Ms::Widgets::MQueryTableViewColumn::setWidth(int width)
{
    _width = width;
}

Wt::WFlags<Wt::ItemFlag> Ms::Widgets::MQueryTableViewColumn::flags() const
{
    return _flags;
}

void Ms::Widgets::MQueryTableViewColumn::setFlags(Wt::WFlags<Wt::ItemFlag> flags)
{
    _flags = flags;
}

Ms::Widgets::Delegates::MItemDelegate *Ms::Widgets::MQueryTableViewColumn::delegate() const
{
    return _delegate;
}

void Ms::Widgets::MQueryTableViewColumn::setDelegate(Ms::Widgets::Delegates::MItemDelegate *delegate)
{
    _delegate = delegate;
}

bool Ms::Widgets::MQueryTableViewColumn::isMandatory() const
{
    return _isMandatory;
}

void Ms::Widgets::MQueryTableViewColumn::setMandatory(bool isMandatory)
{
    _isMandatory = isMandatory;
}

bool Ms::Widgets::MQueryTableViewColumn::isIgnored() const
{
    return _ignored;
}

void Ms::Widgets::MQueryTableViewColumn::setIgnored(bool ignored)
{
    _ignored = ignored;
}

bool Ms::Widgets::MQueryTableViewColumn::operator ==(const MQueryTableViewColumn &other) const
{
    return _dbFieldName == other.dbFieldName();
}
