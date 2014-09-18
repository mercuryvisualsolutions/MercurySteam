#include "../Database/dbtables.h"
#include "../Database/databasemanager.h"

Projects::ProjectTask::ProjectTask() :
    ProjectDbo()
{
    _init();
}

Projects::ProjectTask *Projects::ProjectTask::modify()
{
    ProjectDbo::modify();

    return this;
}

int Projects::ProjectTask::progress() const
{

}

Wt::Dbo::ptr<Projects::ProjectTaskType> Projects::ProjectTask::type() const
{
    return _type;
}

void Projects::ProjectTask::setType(Wt::Dbo::ptr<Projects::ProjectTaskType> type)
{
    _type = type;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectTask::status() const
{
    return _status;
}

void Projects::ProjectTask::setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    _status = status;
}

Wt::Dbo::ptr<Users::User> Projects::ProjectTask::user() const
{
    return _user;
}

void Projects::ProjectTask::setUser(Wt::Dbo::ptr<Users::User> user)
{
    _user = user;
}

Wt::Dbo::ptr<Projects::Project> Projects::ProjectTask::project() const
{
    return _project;
}

void Projects::ProjectTask::setProject(Wt::Dbo::ptr<Projects::Project> project)
{
    _project = project;
}

Wt::Dbo::ptr<Projects::ProjectSequence> Projects::ProjectTask::sequence() const
{
    return _sequence;
}

void Projects::ProjectTask::setSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    _sequence = sequence;
}

Wt::Dbo::ptr<Projects::ProjectShot> Projects::ProjectTask::shot() const
{
    return _shot;
}

void Projects::ProjectTask::setShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    _shot = shot;
}

const Wt::Dbo::ptr<Projects::ProjectAsset> Projects::ProjectTask::asset() const
{
    return _asset;
}

void Projects::ProjectTask::setAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    _asset = asset;
}

bool Projects::ProjectTask::hasActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity) const
{
    if(dboManager_ && dboManager_->openTransaction())
    {
        for(auto iter = _activities.begin(); iter != _activities.end(); ++iter)
        {
            if((*iter).id() == activity.id())
            {
                return true;
            }
        }
    }

    return false;
}

bool Projects::ProjectTask::addActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity)
{
    if(!hasActivity(activity))
    {
        _activities.insert(activity);
        return true;
    }

    return false;
}

bool Projects::ProjectTask::removeActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity)
{
    if(hasActivity(activity))
    {
        _activities.erase(activity);
        return true;
    }

    return false;
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
    _acceptedByUser = false;
}
