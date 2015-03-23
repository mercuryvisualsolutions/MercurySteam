#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::Project::Project() :
    ProjectDbo()
{
    init();
}

Projects::Project::Project(const std::string &projectName) :
    Project()
{
    m_name = projectName;
}

Projects::Project *Projects::Project::modify()
{
    ProjectDbo::modify();

    return this;
}

std::string Projects::Project::name() const
{
    return m_name;
}

void Projects::Project::setName(const std::string &name)
{
    m_name = name;
}

bool Projects::Project::hasSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence) const
{
    bool result = false;

    for(auto iter = m_sequences.begin(); iter != m_sequences.end(); ++iter)
    {
        if((*iter).id() == sequence.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Projects::Project::addSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    if(!hasSequence(sequence))
    {
        m_sequences.insert(sequence);

        return true;
    }

    return false;
}

bool Projects::Project::removeSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    if(hasSequence(sequence))
    {
        m_sequences.erase(sequence);

        return true;
    }

    return false;
}

bool Projects::Project::hasAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset) const
{
    bool result = false;

    for(auto iter = m_assets.begin(); iter != m_assets.end(); ++iter)
    {
        if((*iter).id() == asset.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Projects::Project::addAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    if(!hasAsset(asset))
    {
        m_assets.insert(asset);

        return true;
    }

    return false;
}

bool Projects::Project::removeAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    if(hasAsset(asset))
    {
        m_assets.erase(asset);

        return true;
    }

    return false;
}

int Projects::Project::durationInFrames() const
{
    return m_durationInFrames;
}

void Projects::Project::setDurationInFrames(int durationInFrames)
{
    m_durationInFrames = durationInFrames;
}

float Projects::Project::fps() const
{
    return m_fps;
}

void Projects::Project::setFps(float fps)
{
    m_fps = fps;
}

int Projects::Project::frameWidth() const
{
    return m_frameWidth;
}

void Projects::Project::setFrameWidth(int frameWidth)
{
    m_frameWidth = frameWidth;
}

int Projects::Project::frameHeight() const
{
    return m_frameHeight;
}

void Projects::Project::setFrameHeight(int frameHeight)
{
    m_frameHeight = frameHeight;
}

int Projects::Project::progress() const
{
    int dHours = doneHours();
    int tHours = totalHours();

    return tHours > 0 ? static_cast<float>(static_cast<float>(dHours) / static_cast<float>(tHours)) * 100.0f : 0;
}

int Projects::Project::totalHours() const
{
    int totalHours = 0;

    for(auto iter = m_sequences.begin(); iter != m_sequences.end(); ++iter)//count sequences hours
    {
        if(!(*iter)->active())
            continue;

        totalHours += (*iter)->totalHours();
    }

    for(auto iter = m_assets.begin(); iter != m_assets.end(); ++iter)//count assets hours
    {
        if(!(*iter)->active())
            continue;

        totalHours += (*iter)->totalHours();
    }

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)//count tasks hours
    {
        if(!(*iter)->active())
            continue;

        totalHours += (*iter)->totalHours();
    }

    return totalHours;
}

int Projects::Project::doneHours() const
{
    int finishedHours = 0;

    for(auto iter = m_sequences.begin(); iter != m_sequences.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        finishedHours += (*iter)->doneHours();
    }

    for(auto iter = m_assets.begin(); iter != m_assets.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        finishedHours += (*iter)->doneHours();
    }

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        finishedHours += (*iter)->doneHours();
    }

    return finishedHours;
}

int Projects::Project::totalAssets() const
{
    int totalAssets = 0;

    for(auto iter = m_assets.begin(); iter != m_assets.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalAssets++;
    }

    return totalAssets;
}

int Projects::Project::doneAssets() const
{
    int doneAssets = 0;

    for(auto iter = m_assets.begin(); iter != m_assets.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")
        {
            doneAssets++;
        }
    }

    return doneAssets;
}

int Projects::Project::totalSequences() const
{
    int totalSequences = 0;

    for(auto iter = m_sequences.begin(); iter != m_sequences.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalSequences++;
    }

    return totalSequences;
}

int Projects::Project::doneSequences() const
{
    int doneSequences = 0;

    for(auto iter = m_sequences.begin(); iter != m_sequences.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")
        {
            doneSequences++;
        }
    }

    return doneSequences;
}

int Projects::Project::totalShots() const
{
    int totalShots = 0;

    for(auto iter = m_sequences.begin(); iter != m_sequences.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalShots+= (*iter)->totalShots();
    }

    return totalShots;
}

int Projects::Project::doneShots() const
{
    int doneShots= 0;

    for(auto iter = m_sequences.begin(); iter != m_sequences.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        doneShots+= (*iter)->doneShots();
    }

    return doneShots;
}

int Projects::Project::totalTasks() const
{
    int totalTasks = 0;

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)//count sequences tasks
    {
        if(!(*iter)->active())
            continue;

        totalTasks++;
    }

    for(auto iter = m_sequences.begin(); iter != m_sequences.end(); ++iter)//count sequences tasks
    {
        if(!(*iter)->active())
            continue;

        totalTasks += (*iter)->totalTasks();
    }

    for(auto iter = m_assets.begin(); iter != m_assets.end(); ++iter)//count assets tasks
    {
        if(!(*iter)->active())
            continue;

        totalTasks += (*iter)->totalTasks();
    }

    return totalTasks;
}

int Projects::Project::doneTasks() const
{
    int doneTasks = 0;

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)//project tasks
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")
        {
            doneTasks++;
        }
    }

    for(auto iter = m_sequences.begin(); iter != m_sequences.end(); ++iter)//sequence tasks
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")
        {
            doneTasks += (*iter)->doneTasks();
        }
    }

    for(auto iter = m_assets.begin(); iter != m_assets.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")//assets tasks
        {
            doneTasks += (*iter)->doneTasks();
        }
    }

    return doneTasks;
}

int Projects::Project::totalActivities() const
{
    int totalActivities = 0;

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalActivities+= (*iter)->totalActivities();
    }

    for(auto iter = m_sequences.begin(); iter != m_sequences.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalActivities += (*iter)->totalActivities();
    }

    for(auto iter = m_assets.begin(); iter != m_assets.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalActivities += (*iter)->totalActivities();
    }

    return totalActivities;
}

int Projects::Project::doneActivities() const
{
    int doneActivities = 0;

    for(auto iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        doneActivities+= (*iter)->doneActivities();
    }

    for(auto iter = m_sequences.begin(); iter != m_sequences.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        doneActivities+= (*iter)->doneActivities();
    }

    for(auto iter = m_assets.begin(); iter != m_assets.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        doneActivities+= (*iter)->doneActivities();
    }

    return doneActivities;
}

Wt::Dbo::ptr<Users::User> Projects::Project::manager() const
{
    return m_projectManager;
}

void Projects::Project::setManager(const Wt::Dbo::ptr<Users::User> user)
{
    m_projectManager = user;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence> > Projects::Project::sequences() const
{
    return m_sequences;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset> > Projects::Project::assets() const
{
    return m_assets;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask> > Projects::Project::tasks() const
{
    return m_tasks;
}

bool Projects::Project::operator ==(const Projects::Project &other) const
{
    return m_name == other.name();
}

bool Projects::Project::operator !=(const Projects::Project &other) const
{
    return !operator==(other);
}

void Projects::Project::init()
{
    m_thumbnail = "pics/NoPreviewBig.png";
    m_name = "New Project";
    m_durationInFrames = 0;
    m_fps = 24.0f;
    m_frameWidth = 1920;
    m_frameHeight = 1080;
}
