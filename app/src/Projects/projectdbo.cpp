#include "../Database/dbtables.h"
#include "../Database/dbosession.h"

Projects::ProjectDbo::ProjectDbo() :
    Database::Dbo()
{
    init();
}

Projects::ProjectDbo *Projects::ProjectDbo::modify()
{
    Database::Dbo::modify();

    return this;
}

Wt::WDate Projects::ProjectDbo::startDate() const
{
    return m_startDate;
}

void Projects::ProjectDbo::setStartDate(const Wt::WDate &startDate)
{
    m_startDate = startDate;
}

Wt::WDate Projects::ProjectDbo::endDate() const
{
    return m_endDate;
}

void Projects::ProjectDbo::setEndDate(const Wt::WDate &endDate)
{
    m_endDate = endDate;
}

std::string Projects::ProjectDbo::description() const
{
    return m_description;
}

void Projects::ProjectDbo::setDescription(const std::string &description)
{
    m_description = description;
}

int Projects::ProjectDbo::priority() const
{
    return m_priority;
}

void Projects::ProjectDbo::setPriority(int priority)
{
    m_priority = priority;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectDbo::status() const
{
    return m_status;
}

void Projects::ProjectDbo::setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    m_status = status;
}

void Projects::ProjectDbo::init()
{
    m_startDate = Wt::WDate::currentDate();
    m_endDate = Wt::WDate::currentDate();
    m_description = "";
    m_priority = 0;
}
