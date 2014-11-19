#include "../Database/dbtables.h"
#include "../Session/sessionmanager.h"

Projects::Project::Project() :
    ProjectDbo()
{
    _init();
}

Projects::Project::Project(const std::string &projectName) :
    Project()
{
    _name = projectName;
}

Projects::Project *Projects::Project::modify()
{
    ProjectDbo::modify();

    return this;
}

std::string Projects::Project::name() const
{
    return _name;
}

void Projects::Project::setName(const std::string &name)
{
    _name = name;
}

bool Projects::Project::hasSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence) const
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool result = false;

    for(auto iter = _sequences.begin(); iter != _sequences.end(); ++iter)
    {
        if((*iter).id() == sequence.id())
        {
            result = true;

            break;
        }
    }

    transaction.commit();

    return result;
}

bool Projects::Project::addSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    if(!hasSequence(sequence))
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        _sequences.insert(sequence);

        transaction.commit();

        return true;
    }

    return false;
}

bool Projects::Project::removeSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    if(hasSequence(sequence))
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        _sequences.erase(sequence);

        transaction.commit();

        return true;
    }

    return false;
}

bool Projects::Project::hasAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset) const
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool result = false;

    for(auto iter = _assets.begin(); iter != _assets.end(); ++iter)
    {
        if((*iter).id() == asset.id())
        {
            result = true;

            break;
        }
    }

    transaction.commit();

    return result;
}

bool Projects::Project::addAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    if(!hasAsset(asset))
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        _assets.insert(asset);

        transaction.commit();

        return true;
    }

    return false;
}

bool Projects::Project::removeAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    if(hasAsset(asset))
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        _assets.erase(asset);

        transaction.commit();

        return true;
    }

    return false;
}

int Projects::Project::durationInFrames() const
{
    return _durationInFrames;
}

void Projects::Project::setDurationInFrames(int durationInFrames)
{
    _durationInFrames = durationInFrames;
}

float Projects::Project::fps() const
{
    return _fps;
}

void Projects::Project::setFps(float fps)
{
    _fps = fps;
}

int Projects::Project::frameWidth() const
{
    return _frameWidth;
}

void Projects::Project::setFrameWidth(int frameWidth)
{
    _frameWidth = frameWidth;
}

int Projects::Project::frameHeight() const
{
    return _frameHeight;
}

void Projects::Project::setFrameHeight(int frameHeight)
{
    _frameHeight = frameHeight;
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

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _sequences.begin(); iter != _sequences.end(); ++iter)//count sequences hours
    {
        if(!(*iter)->active())
            continue;

        totalHours += (*iter)->totalHours();
    }

    for(auto iter = _assets.begin(); iter != _assets.end(); ++iter)//count assets hours
    {
        if(!(*iter)->active())
            continue;

        totalHours += (*iter)->totalHours();
    }

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)//count tasks hours
    {
        if(!(*iter)->active())
            continue;

        totalHours += (*iter)->totalHours();
    }

    transaction.commit();

    return totalHours;
}

int Projects::Project::doneHours() const
{
    int finishedHours = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _sequences.begin(); iter != _sequences.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        finishedHours += (*iter)->doneHours();
    }

    for(auto iter = _assets.begin(); iter != _assets.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        finishedHours += (*iter)->doneHours();
    }

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        finishedHours += (*iter)->doneHours();
    }

    transaction.commit();

    return finishedHours;
}

int Projects::Project::totalAssets() const
{
    int totalAssets = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _assets.begin(); iter != _assets.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalAssets++;
    }

    transaction.commit();

    return totalAssets;
}

int Projects::Project::doneAssets() const
{
    int doneAssets = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _assets.begin(); iter != _assets.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")
        {
            doneAssets++;
        }
    }

    transaction.commit();

    return doneAssets;
}

int Projects::Project::totalSequences() const
{
    int totalSequences = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _sequences.begin(); iter != _sequences.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalSequences++;
    }

    transaction.commit();

    return totalSequences;
}

int Projects::Project::doneSequences() const
{
    int doneSequences = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _sequences.begin(); iter != _sequences.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")
        {
            doneSequences++;
        }
    }

    transaction.commit();

    return doneSequences;
}

int Projects::Project::totalShots() const
{
    int totalShots = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _sequences.begin(); iter != _sequences.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalShots+= (*iter)->totalShots();
    }

    transaction.commit();

    return totalShots;
}

int Projects::Project::doneShots() const
{
    int doneShots= 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _sequences.begin(); iter != _sequences.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        doneShots+= (*iter)->doneShots();
    }

    transaction.commit();

    return doneShots;
}

int Projects::Project::totalTasks() const
{
    int totalTasks = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)//count sequences tasks
    {
        if(!(*iter)->active())
            continue;

        totalTasks++;
    }

    for(auto iter = _sequences.begin(); iter != _sequences.end(); ++iter)//count sequences tasks
    {
        if(!(*iter)->active())
            continue;

        totalTasks += (*iter)->totalTasks();
    }

    for(auto iter = _assets.begin(); iter != _assets.end(); ++iter)//count assets tasks
    {
        if(!(*iter)->active())
            continue;

        totalTasks += (*iter)->totalTasks();
    }

    transaction.commit();

    return totalTasks;
}

int Projects::Project::doneTasks() const
{
    int doneTasks = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)//project tasks
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")
        {
            doneTasks++;
        }
    }

    for(auto iter = _sequences.begin(); iter != _sequences.end(); ++iter)//sequence tasks
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")
        {
            doneTasks += (*iter)->doneTasks();
        }
    }

    for(auto iter = _assets.begin(); iter != _assets.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        if((*iter)->status()->workStatusType()->workStatusType() == "Done")//assets tasks
        {
            doneTasks += (*iter)->doneTasks();
        }
    }

    transaction.commit();

    return doneTasks;
}

int Projects::Project::totalActivities() const
{
    int totalActivities = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalActivities+= (*iter)->totalActivities();
    }

    for(auto iter = _sequences.begin(); iter != _sequences.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalActivities += (*iter)->totalActivities();
    }

    for(auto iter = _assets.begin(); iter != _assets.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        totalActivities += (*iter)->totalActivities();
    }

    transaction.commit();

    return totalActivities;
}

int Projects::Project::doneActivities() const
{
    int doneActivities = 0;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    for(auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        doneActivities+= (*iter)->doneActivities();
    }

    for(auto iter = _sequences.begin(); iter != _sequences.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        doneActivities+= (*iter)->doneActivities();
    }

    for(auto iter = _assets.begin(); iter != _assets.end(); ++iter)
    {
        if(!(*iter)->active())
            continue;

        doneActivities+= (*iter)->doneActivities();
    }

    transaction.commit();

    return doneActivities;
}

Wt::Dbo::ptr<Users::User> Projects::Project::manager() const
{
    return _projectManager;
}

void Projects::Project::setManager(const Wt::Dbo::ptr<Users::User> user)
{
    _projectManager = user;
}

bool Projects::Project::operator ==(const Projects::Project &other) const
{
    return _name == other.name();
}

bool Projects::Project::operator !=(const Projects::Project &other) const
{
    return !operator==(other);
}

void Projects::Project::_init()
{
    thumbnail_ = "pics/NoPreviewBig.png";
    _name = "New Project";
    _durationInFrames = 0;
    _fps = 24.0f;
    _frameWidth = 1920;
    _frameHeight = 1080;
}
