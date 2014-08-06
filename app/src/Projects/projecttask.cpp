#include "../Database/dbtables.h"
#include "../Database/databasemanager.h"

Projects::ProjectTask::ProjectTask() :
    Database::Dbo()
{
    _init();
}

Projects::ProjectTask *Projects::ProjectTask::modify()
{
    Database::Dbo::modify();

    return this;
}

Wt::WDate Projects::ProjectTask::startDate() const
{
    return _startDate;
}

void Projects::ProjectTask::setStartDate(const Wt::WDate &startDate)
{
    _startDate = startDate;
}

Wt::WDate Projects::ProjectTask::endDate() const
{
    return _endDate;
}

void Projects::ProjectTask::setEndDate(const Wt::WDate &endDate)
{
    _endDate = endDate;
}

std::string Projects::ProjectTask::description() const
{
    return _description;
}

void Projects::ProjectTask::setDescription(const std::string &description)
{
    _description = description;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectTask::status() const
{
    return _status;
}

void Projects::ProjectTask::setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    _status = status;
}

Wt::Dbo::ptr<Projects::ProjectTaskType> Projects::ProjectTask::type() const
{
    return _type;
}

void Projects::ProjectTask::setType(Wt::Dbo::ptr<Projects::ProjectTaskType> type)
{
    _type = type;
}

Wt::Dbo::ptr<Users::User> Projects::ProjectTask::user() const
{
    return _user;
}

void Projects::ProjectTask::setUser(Wt::Dbo::ptr<Users::User> user)
{
    _user = user;
}

Wt::Dbo::ptr<Projects::ProjectShot> Projects::ProjectTask::shot() const
{
    return _shot;
}

void Projects::ProjectTask::setShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    _shot = shot;
}

Wt::Dbo::ptr<Projects::ProjectAsset> Projects::ProjectTask::asset() const
{
    return _asset;
}

void Projects::ProjectTask::setAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    _asset = asset;
}

Wt::Dbo::ptr<Projects::ProjectSequence> Projects::ProjectTask::sequence() const
{
    return _sequence;
}

void Projects::ProjectTask::setShot(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    _sequence = sequence;
}

Wt::Dbo::ptr<Projects::Project> Projects::ProjectTask::project() const
{
    return _project;
}

void Projects::ProjectTask::setShot(Wt::Dbo::ptr<Projects::Project> project)
{
    _project = project;
}

int Projects::ProjectTask::priority() const
{
    return _priority;
}

void Projects::ProjectTask::setPriority(int priority)
{
    _priority = priority;
}

bool Projects::ProjectTask::isAcceptedByUser()
{
    return _acceptedByUser;
}

void Projects::ProjectTask::setAcceptedByUser(bool accepted)
{
    _acceptedByUser = accepted;
}

void Projects::ProjectTask::_init()
{
    dboManager_ = &Database::DatabaseManager::instance();

    thumbnail_ = "pics/NoPreviewBig.png";
    _startDate = Wt::WDate::currentDate();
    _endDate = Wt::WDate::currentDate();
    _description = "";
    _priority = 0;
    _acceptedByUser = false;
}
