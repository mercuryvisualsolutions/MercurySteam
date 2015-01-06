#include "MTableViewColumn.h"

Ms::Core::MTableViewColumn::MTableViewColumn() :
    m_name("New Column"), m_width(150), m_flags(Wt::ItemIsSelectable), m_isMandatory(false), m_isIgnored(false)
{
}

Ms::Core::MTableViewColumn::MTableViewColumn(const std::string &name, Widgets::Delegates::MItemDelegate *delegate, int width, Wt::WFlags<Wt::ItemFlag> flags, bool isMandatory, bool isIgnored) :
    MTableViewColumn()
{
    m_name = name;
    m_delegate = delegate;
    m_width = width;
    m_flags = flags;
    m_isMandatory = isMandatory;
    m_isIgnored = isIgnored;
}

std::string Ms::Core::MTableViewColumn::name() const
{
    return m_name;
}

void Ms::Core::MTableViewColumn::setName(std::string &name)
{
    m_name = name;
}

int Ms::Core::MTableViewColumn::width() const
{
    return m_width;
}

void Ms::Core::MTableViewColumn::setWidth(int width)
{
    m_width = width;
}

Wt::WFlags<Wt::ItemFlag> Ms::Core::MTableViewColumn::flags() const
{
    return m_flags;
}

void Ms::Core::MTableViewColumn::setFlags(Wt::WFlags<Wt::ItemFlag> flags)
{
    m_flags = flags;
}

Ms::Widgets::Delegates::MItemDelegate *Ms::Core::MTableViewColumn::delegate() const
{
    return m_delegate;
}

void Ms::Core::MTableViewColumn::setDelegate(Ms::Widgets::Delegates::MItemDelegate *delegate)
{
    m_delegate = delegate;
}

bool Ms::Core::MTableViewColumn::isMandatory() const
{
    return m_isMandatory;
}

void Ms::Core::MTableViewColumn::setMandatory(bool isMandatory)
{
    m_isMandatory = isMandatory;
}

bool Ms::Core::MTableViewColumn::isIgnored() const
{
    return m_isIgnored;
}

void Ms::Core::MTableViewColumn::setIgnored(bool isIgnored)
{
    m_isIgnored = isIgnored;
}

bool Ms::Core::MTableViewColumn::operator=(const Ms::Core::MTableViewColumn &other) const
{
    return m_name == other.name();
}
