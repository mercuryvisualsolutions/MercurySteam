#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::ProjectTask::ProjectTask() :
    ProjectDbo()
{
    init();
}

Projects::ProjectTask *Projects::ProjectTask::modify()
{
    ProjectDbo::modify();

    return this;
}

int Projects::ProjectTask::progress() const
{
    int dHours = doneHours();
    int tHours = totalHours();

    return tHours > 0 ? static_cast<float>(static_cast<float>(dHours) / static_cast<float>(tHours)) * 100.0f : 0;
}

int Projects::ProjectTask::totalHours() const
{
    int totalHours = 0;

    for(auto iter = m_activities.begin(); iter != m_activities.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalHours += (*iter)->hours();
    }

    return totalHours;
}

int Projects::ProjectTask::doneHours() const
{
    int finishedHours = 0;

    for(auto iter = m_activities.begin(); iter != m_activities.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")//finished task
            finishedHours += (*iter)->hours();
    }

    return finishedHours;
}

Wt::Dbo::ptr<Projects::ProjectTaskType> Projects::ProjectTask::type() const
{
    return m_type;
}

void Projects::ProjectTask::setType(Wt::Dbo::ptr<Projects::ProjectTaskType> type)
{
    m_type = type;
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Projects::ProjectTask::status() const
{
    return m_status;
}

void Projects::ProjectTask::setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status)
{
    m_status = status;
}

Wt::Dbo::ptr<Users::User> Projects::ProjectTask::user() const
{
    return m_user;
}

void Projects::ProjectTask::setUser(Wt::Dbo::ptr<Users::User> user)
{
    m_user = user;
}

Wt::Dbo::ptr<Projects::Project> Projects::ProjectTask::project() const
{
    return m_project;
}

void Projects::ProjectTask::setProject(Wt::Dbo::ptr<Projects::Project> project)
{
    m_project = project;
}

Wt::Dbo::ptr<Projects::ProjectSequence> Projects::ProjectTask::sequence() const
{
    return m_sequence;
}

void Projects::ProjectTask::setSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    m_sequence = sequence;
}

Wt::Dbo::ptr<Projects::ProjectShot> Projects::ProjectTask::shot() const
{
    return m_shot;
}

void Projects::ProjectTask::setShot(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    m_shot = shot;
}

const Wt::Dbo::ptr<Projects::ProjectAsset> Projects::ProjectTask::asset() const
{
    return m_asset;
}

void Projects::ProjectTask::setAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    m_asset = asset;
}

bool Projects::ProjectTask::hasActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity) const
{
    bool result = false;

    for(auto iter = m_activities.begin(); iter != m_activities.end(); ++iter)
    {
        if((*iter).id() == activity.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Projects::ProjectTask::addActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity)
{
    if(!hasActivity(activity))
    {
        m_activities.insert(activity);

        return true;
    }

    return false;
}

bool Projects::ProjectTask::removeActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity)
{
    if(hasActivity(activity))
    {
        m_activities.erase(activity);

        return true;
    }

    return false;
}

bool Projects::ProjectTask::isAcceptedByUser()
{
    return m_acceptedByUser;
}

void Projects::ProjectTask::setAcceptedByUser(bool accepted)
{
    m_acceptedByUser = accepted;
}

int Projects::ProjectTask::totalActivities() const
{
    return m_activities.size();
}

int Projects::ProjectTask::doneActivities() const
{
    int doneActivities = 0;

    for(auto iter = m_activities.begin(); iter != m_activities.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")
            doneActivities++;
    }

    return doneActivities;
}

void Projects::ProjectTask::init()
{
    m_thumbnail = "pics/NoPreviewBig.png";
    m_acceptedByUser = false;
}
