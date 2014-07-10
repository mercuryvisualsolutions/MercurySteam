#ifndef PROJECTDBOSPECIALIZATION_H
#define PROJECTDBOSPECIALIZATION_H

#include <iostream>
#include <string>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>

namespace Projects
{
    class ProjectWorkStatusType;
    class ProjectAssetType;
    class ProjectWorkStatus;
    class ProjectSequence;
    class ProjectTaskType;
    class ProjectTask;
    class ProjectShot;
    class ProjectAsset;
    class Project;
}

namespace Wt
{
    namespace Dbo
    {
        //override the default primary id column for the Project class to use std::string
        template<>
        struct dbo_traits<Projects::Project> : public dbo_default_traits
        {
            typedef std::string IdType;
            static IdType invalidId() { return std::string(); }
            static const char *surrogateIdField() { return 0; }
        };

        //override the default primary id column for the ProjectAssetType class to use std::string
        template<>
        struct dbo_traits<Projects::ProjectAssetType> : public dbo_default_traits
        {
            typedef std::string IdType;
            static IdType invalidId() { return std::string(); }
            static const char *surrogateIdField() { return 0; }
        };

        //override the default primary id column for the ProjectWorkStatusType class to use std::string
        template<>
        struct dbo_traits<Projects::ProjectWorkStatusType> : public dbo_default_traits
        {
            typedef std::string IdType;
            static IdType invalidId() { return std::string(); }
            static const char *surrogateIdField() { return 0; }
        };

        //override the default primary id column for the ProjectWorkStatus class to use std::string
        template<>
        struct dbo_traits<Projects::ProjectWorkStatus> : public dbo_default_traits
        {
            typedef std::string IdType;
            static IdType invalidId() { return std::string(); }
            static const char *surrogateIdField() { return 0; }
        };

        //override the default primary id column for the ProjectTaskType class to use std::string
        template<>
        struct dbo_traits<Projects::ProjectTaskType> : public dbo_default_traits
        {
            typedef std::string IdType;
            static IdType invalidId() { return std::string(); }
            static const char *surrogateIdField() { return 0; }
        };
    }
}

//Project Sequence
namespace Projects
{
    struct ProjectSequenceId
    {
    public:
        ProjectSequenceId(){}
        ProjectSequenceId(std::string name, Wt::Dbo::ptr<Projects::Project> project) :
            name(name), project(project){}

        //variables
        std::string name;
        Wt::Dbo::ptr<Projects::Project> project;

        bool operator ==(const ProjectSequenceId &other) const{return name == other.name && project.id() == other.project.id();}
        bool operator <(const ProjectSequenceId &other) const{return true;}
    };

    inline std::ostream& operator<< (std::ostream& o, const Projects::ProjectSequenceId& psid)
    {
        return o << psid.name << "," << psid.project.id();
    }
}

namespace Wt
{
    namespace Dbo
    {
        //specify how the ProjectSequenceId type is persisted
        template <class Action>
        void field(Action& action, Projects::ProjectSequenceId &psid, const std::string &columnName, int size = -1)
        {
            field(action, psid.name, columnName + "Name", size);
            belongsTo(action, psid.project, columnName + "Project");
        }

        //override the default primary id column for the ProjectSequence class to use ProjectSequenceId
        template<>
        struct dbo_traits<Projects::ProjectSequence> : public dbo_default_traits
        {
            typedef Projects::ProjectSequenceId IdType;
            static IdType invalidId() { return Projects::ProjectSequenceId(); }
            static const char *surrogateIdField() { return 0; }
        };
    }
}

//Project Asset
namespace Projects
{
    struct ProjectAssetId
    {
    public:
        ProjectAssetId(){}
        ProjectAssetId(std::string name, Wt::Dbo::ptr<Projects::Project> project) :
            name(name), project(project){}

        //variables
        std::string name;
        Wt::Dbo::ptr<Projects::Project> project;

        bool operator ==(const ProjectAssetId &other) const{return name == other.name && project.id() == other.project.id();}
        bool operator <(const ProjectAssetId &other) const{return true;}
    };

    inline std::ostream& operator<< (std::ostream& o, const Projects::ProjectAssetId& paid)
    {
        return o << paid.name << "," << paid.project.id();
    }
}

//specify how the ProjectAssetId type is persisted
namespace Wt
{
    namespace Dbo
    {
        template <class Action>
        void field(Action& action, Projects::ProjectAssetId &paid, const std::string &columnName, int size = -1)
        {
            field(action, paid.name, columnName + "Name", size);
            belongsTo(action, paid.project, columnName + "Project");
        }

        //override the default primary id column for the ProjectAsset class to use ProjectAssetId
        template<>
        struct dbo_traits<Projects::ProjectAsset> : public dbo_default_traits
        {
            typedef Projects::ProjectAssetId IdType;
            static IdType invalidId() { return Projects::ProjectAssetId(); }
            static const char *surrogateIdField() { return 0; }
        };
    }
}

//Project Shot
namespace Projects
{
    struct ProjectShotId
    {
    public:
        ProjectShotId(){}
        ProjectShotId(std::string name, Wt::Dbo::ptr<Projects::ProjectSequence> sequence) :
            name(name), sequence(sequence){}

        //variables
        std::string name;
        Wt::Dbo::ptr<Projects::ProjectSequence> sequence;

        bool operator ==(const ProjectShotId &other) const{return name == other.name && sequence.id() == other.sequence.id();}
        bool operator <(const ProjectShotId &other) const{return true;}
    };

    inline std::ostream& operator<< (std::ostream& o, const Projects::ProjectShotId& psid)
    {
        return o << psid.name << "," << psid.sequence.id();
    }
}

namespace Wt
{
    namespace Dbo
    {
        //specify how the ProjectShotId type is persisted
        template <class Action>
        void field(Action& action, Projects::ProjectShotId &psid, const std::string &columnName, int size = -1)
        {
            field(action, psid.name, columnName + "Name", size);
            belongsTo(action, psid.sequence, columnName + "Sequence");
        }

        //override the default primary id column for the ProjectShot class to use ProjectShotId
        template<>
        struct dbo_traits<Projects::ProjectShot> : public dbo_default_traits
        {
            typedef Projects::ProjectShotId IdType;
            static IdType invalidId() { return Projects::ProjectShotId(); }
            static const char *surrogateIdField() { return 0; }
        };
    }
}

#endif // PROJECTDBOSPECIALIZATION_H
