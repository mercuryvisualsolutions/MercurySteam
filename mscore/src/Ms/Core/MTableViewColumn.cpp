#include "MTableViewColumn.h"

Ms::Core::MTableViewColumn::MTableViewColumn() :
    _name("New Column"), _width(150), _flags(Wt::ItemIsSelectable), _isMandatory(false), _isIgnored(false)
{
}

Ms::Core::MTableViewColumn::MTableViewColumn(const std::string &name, int width, Wt::WFlags<Wt::ItemFlag> flags, bool isMandatory, bool isIgnored) :
    MTableViewColumn()
{
    _name = name;
    _width = width;
    _flags = flags;
    _isMandatory = isMandatory;
    _isIgnored = isIgnored;
}

std::string Ms::Core::MTableViewColumn::name() const
{
    return _name;
}

void Ms::Core::MTableViewColumn::setName(std::string &name)
{
    _name = name;
}

int Ms::Core::MTableViewColumn::width() const
{
    return _width;
}

void Ms::Core::MTableViewColumn::setWidth(int width)
{
    _width = width;
}

Wt::WFlags<Wt::ItemFlag> Ms::Core::MTableViewColumn::flags() const
{
    return _flags;
}

void Ms::Core::MTableViewColumn::setFlags(Wt::WFlags<Wt::ItemFlag> flags)
{
    _flags = flags;
}

Ms::Widgets::Delegates::MItemDelegate *Ms::Core::MTableViewColumn::delegate() const
{
    return _delegate;
}

void Ms::Core::MTableViewColumn::setDelegate(Ms::Widgets::Delegates::MItemDelegate *delegate)
{
    _delegate = delegate;
}

bool Ms::Core::MTableViewColumn::isMandatory() const
{
    return _isMandatory;
}

void Ms::Core::MTableViewColumn::setMandatory(bool isMandatory)
{
    _isMandatory = isMandatory;
}

bool Ms::Core::MTableViewColumn::isIgnored() const
{
    return _isIgnored;
}

void Ms::Core::MTableViewColumn::setIgnored(bool isIgnored)
{
    _isIgnored = isIgnored;
}

bool Ms::Core::MTableViewColumn::operator=(const Ms::Core::MTableViewColumn &other) const
{
    return _name == other.name();
}
