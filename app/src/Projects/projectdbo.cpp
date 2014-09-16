#include "../Database/dbtables.h"
#include "../Database/databasemanager.h"

Projects::ProjectDbo::ProjectDbo() :
    Database::Dbo()
{
    _init();
}

Projects::ProjectDbo *Projects::ProjectDbo::modify()
{
    Database::Dbo::modify();

    return this;
}

Wt::WDate Projects::ProjectDbo::startDate() const
{
    return _startDate;
}

void Projects::ProjectDbo::setStartDate(const Wt::WDate &startDate)
{
    _startDate = startDate;
}

Wt::WDate Projects::ProjectDbo::endDate() const
{
    return _endDate;
}

void Projects::ProjectDbo::setEndDate(const Wt::WDate &endDate)
{
    _endDate = endDate;
}

std::string Projects::ProjectDbo::description() const
{
    return _description;
}

void Projects::ProjectDbo::setDescription(const std::string &description)
{
    _description = description;
}

int Projects::ProjectDbo::priority() const
{
    return _priority;
}

void Projects::ProjectDbo::setPriority(int priority)
{
    _priority = priority;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectDbo::status() const
{
    return _status;
}

void Projects::ProjectDbo::setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    _status = status;
}

void Projects::ProjectDbo::_init()
{
    _startDate = Wt::WDate::currentDate();
    _endDate = Wt::WDate::currentDate();
    _description = "";
    _priority = 0;
}
