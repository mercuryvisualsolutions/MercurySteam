#include "MQueryTableViewColumn.h"

Ms::Widgets::MQueryTableViewColumn::MQueryTableViewColumn() :
    m_width(150),
    m_flags(Wt::ItemIsSelectable),
    m_isMandatory(false),
    m_ignored(false)
{
    setDelegate(new Ms::Widgets::Delegates::MItemDelegate);
}

Ms::Widgets::MQueryTableViewColumn::MQueryTableViewColumn(const std::string &dbFieldName) :
    MQueryTableViewColumn()
{
    m_dbFieldName = dbFieldName;
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
    return m_dbFieldName;
}

void Ms::Widgets::MQueryTableViewColumn::setDbFieldName(const std::string &dbFieldName)
{
    m_dbFieldName = dbFieldName;
}

std::string Ms::Widgets::MQueryTableViewColumn::displayName() const
{
    return m_displayName;
}

void Ms::Widgets::MQueryTableViewColumn::setDisplayName(const std::string &displayName)
{
    m_displayName = displayName;
}

int Ms::Widgets::MQueryTableViewColumn::width() const
{
    return m_width;
}

void Ms::Widgets::MQueryTableViewColumn::setWidth(int width)
{
    m_width = width;
}

Wt::WFlags<Wt::ItemFlag> Ms::Widgets::MQueryTableViewColumn::flags() const
{
    return m_flags;
}

void Ms::Widgets::MQueryTableViewColumn::setFlags(Wt::WFlags<Wt::ItemFlag> flags)
{
    m_flags = flags;
}

Ms::Widgets::Delegates::MItemDelegate *Ms::Widgets::MQueryTableViewColumn::delegate() const
{
    return m_delegate;
}

void Ms::Widgets::MQueryTableViewColumn::setDelegate(Ms::Widgets::Delegates::MItemDelegate *delegate)
{
    m_delegate = delegate;
}

bool Ms::Widgets::MQueryTableViewColumn::isMandatory() const
{
    return m_isMandatory;
}

void Ms::Widgets::MQueryTableViewColumn::setMandatory(bool isMandatory)
{
    m_isMandatory = isMandatory;
}

bool Ms::Widgets::MQueryTableViewColumn::isIgnored() const
{
    return m_ignored;
}

void Ms::Widgets::MQueryTableViewColumn::setIgnored(bool ignored)
{
    m_ignored = ignored;
}

bool Ms::Widgets::MQueryTableViewColumn::operator ==(const MQueryTableViewColumn &other) const
{
    return m_dbFieldName == other.dbFieldName();
}
