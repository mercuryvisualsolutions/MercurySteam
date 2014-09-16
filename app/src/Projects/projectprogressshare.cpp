#include "../Database/dbtables.h"
#include "../Database/databasemanager.h"

Projects::ProjectProgressShare::ProjectProgressShare() :
    Ms::Dbo::MDboBase()
{
    _init();
}

Projects::ProjectProgressShare::ProjectProgressShare(const std::string &name, int value) :
    ProjectProgressShare()
{
    _name = name;
    _value = value;
}

Projects::ProjectProgressShare *Projects::ProjectProgressShare::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}

const std::string Projects::ProjectProgressShare::name() const
{
    return _name;
}

void Projects::ProjectProgressShare::setName(const std::string &name)
{
    _name = name;
}

int Projects::ProjectProgressShare::value() const
{
    return _value;
}

void Projects::ProjectProgressShare::setValue(int value)
{
    _value = value;
}

bool Projects::ProjectProgressShare::operator ==(const Projects::ProjectProgressShare &other) const
{
    return _name == other.name() && _value == other.value();
}

bool Projects::ProjectProgressShare::operator !=(const Projects::ProjectProgressShare &other) const
{
    return !operator==(other);
}

void Projects::ProjectProgressShare::_init()
{
    _name = "New Progress Share";
    _value = 0;
}


