#include "MTableViewColumn.h"

Ms::Widgets::MTableViewColumn::MTableViewColumn()
{
    setMandatory(false);
    setIgnored(false);
    setFlags(Wt::ItemIsSelectable);
    setDelegate(new Ms::Widgets::Delegates::MItemDelegate);
}

Ms::Widgets::MTableViewColumn::MTableViewColumn(const std::string &dbFieldName) :
    MTableViewColumn()
{
    _dbFieldName = dbFieldName;
}

Ms::Widgets::MTableViewColumn::MTableViewColumn(const std::string &dbFieldName, const std::string &displayName) :
    MTableViewColumn(dbFieldName)
{
    setDisplayName(displayName);
}

Ms::Widgets::MTableViewColumn::MTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags) :
    MTableViewColumn(dbFieldName, displayName)
{
    setFlags(flags);
}

Ms::Widgets::MTableViewColumn::MTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags,
                                        Ms::Widgets::Delegates::MItemDelegate *delegate) :
    MTableViewColumn(dbFieldName, displayName, flags)
{
    setDelegate(delegate);
}

Ms::Widgets::MTableViewColumn::MTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags,
                                        Ms::Widgets::Delegates::MItemDelegate *delegate, bool isMandatory) :
    MTableViewColumn(dbFieldName, displayName, flags, delegate)
{
    setMandatory(isMandatory);
}

Ms::Widgets::MTableViewColumn::MTableViewColumn(const std::string &dbFieldName, const std::string &displayName, Wt::WFlags<Wt::ItemFlag> flags,
                                        Ms::Widgets::Delegates::MItemDelegate *delegate, bool isMandatory, bool isIgnored) :
    MTableViewColumn(dbFieldName, displayName, flags, delegate, isMandatory)
{
    setIgnored(isIgnored);
}

Ms::Widgets::MTableViewColumn::MTableViewColumn(const std::string &dbFieldName, const std::string &displayName,
                                        Wt::WFlags<Wt::ItemFlag> flags, Ms::Widgets::Delegates::MItemDelegate *delegate,
                                        bool isMandatory, bool isIgnored, int width) :
    MTableViewColumn(dbFieldName, displayName, flags, delegate, isMandatory, isIgnored)
{
    setWidth(width);
}

std::string Ms::Widgets::MTableViewColumn::dbFieldName() const
{
    return _dbFieldName;
}

void Ms::Widgets::MTableViewColumn::setDbFieldName(const std::string &dbFieldName)
{
    _dbFieldName = dbFieldName;
}

std::string Ms::Widgets::MTableViewColumn::displayName() const
{
    return _displayName;
}

void Ms::Widgets::MTableViewColumn::setDisplayName(const std::string &displayName)
{
    _displayName = displayName;
}

int Ms::Widgets::MTableViewColumn::width() const
{
    return _width;
}

void Ms::Widgets::MTableViewColumn::setWidth(int width)
{
    _width = width;
}

Wt::WFlags<Wt::ItemFlag> Ms::Widgets::MTableViewColumn::flags() const
{
    return _flags;
}

void Ms::Widgets::MTableViewColumn::setFlags(Wt::WFlags<Wt::ItemFlag> flags)
{
    _flags = flags;
}

Ms::Widgets::Delegates::MItemDelegate *Ms::Widgets::MTableViewColumn::delegate() const
{
    return _delegate;
}

void Ms::Widgets::MTableViewColumn::setDelegate(Ms::Widgets::Delegates::MItemDelegate *delegate)
{
    _delegate = delegate;
}

bool Ms::Widgets::MTableViewColumn::isMandatory() const
{
    return _isMandatory;
}

void Ms::Widgets::MTableViewColumn::setMandatory(bool isMandatory)
{
    _isMandatory = isMandatory;
}

bool Ms::Widgets::MTableViewColumn::isIgnored() const
{
    return _ignored;
}

void Ms::Widgets::MTableViewColumn::setIgnored(bool ignored)
{
    _ignored = ignored;
}

bool Ms::Widgets::MTableViewColumn::operator ==(const MTableViewColumn &other) const
{
    return _dbFieldName == other.dbFieldName();
}
