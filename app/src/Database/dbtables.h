//File contains all Database tables and relationships, all classes that creates database tables should inherit from it and implement specific class functions.

#ifndef DBTABLES_H
#define DBTABLES_H

#include <iostream>
#include <string>

#include <Wt/Dbo/Dbo>
#include <Wt/WGlobal>
#include <Wt/Dbo/Session>
#include <Wt/WDateTime>
#include <Wt/Dbo/WtSqlTraits>

#include <Ms/Dbo/MDbo.h>

#include "dbospecialization.h"
#include "projectdbospecialization.h"
#include "userdbospecialization.h"
#include "globaldbospecialization.h"

namespace Database
{
    class DatabaseManager;

    enum RankFlag
    {
        ViewRank = 0x01,
        EditRank = 0x02,
        CreateRank = 0x04,
        RemoveRank = 0x08,

        COUNT = 4
    };

    class DboData : public Ms::Dbo::MDbo
    {
    public:
        DboData();
        DboData(const std::string &key, const std::string &value);

        //variables


        //functions
        DboData *modify() override;
        std::string key() const;
        void setKey(const std::string &key);
        std::string value() const;
        void setValue(const std::string &value);
        Wt::Dbo::ptr<Users::User> user() const;
        void setUser(Wt::Dbo::ptr<Users::User> user);
        Wt::Dbo::ptr<Users::Group> group() const;
        void setGroup(Wt::Dbo::ptr<Users::Group> group);
        Wt::Dbo::ptr<Projects::Project> project() const;
        void setProject(Wt::Dbo::ptr<Projects::Project> project);
        Wt::Dbo::ptr<Projects::ProjectSequence> sequence() const;
        void setSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence);
        Wt::Dbo::ptr<Projects::ProjectShot> shot() const;
        void setShot(Wt::Dbo::ptr<Projects::ProjectShot> shot);
        Wt::Dbo::ptr<Projects::ProjectAsset> asset() const;
        void setAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset);
        Wt::Dbo::ptr<Projects::ProjectTask> task() const;
        void setTask(Wt::Dbo::ptr<Projects::ProjectTask> task);

        //operators
        bool operator ==(const DboData &other) const;
        bool operator !=(const DboData &other) const;

        //DBO Functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::field(a, _dboKey, "DBOKey");
            Wt::Dbo::field(a, _dboValue, "DBOValue");
            Wt::Dbo::belongsTo(a, _user, "User");
            Wt::Dbo::belongsTo(a, _group, "Group");
            Wt::Dbo::belongsTo(a, _project, "Project");
            Wt::Dbo::belongsTo(a, _sequence, "Project_Sequence");
            Wt::Dbo::belongsTo(a, _shot, "Project_Shot");
            Wt::Dbo::belongsTo(a, _asset, "Project_Asset");
            Wt::Dbo::belongsTo(a, _task, "Project_Task");

            persistExtras_<Action>(a);
        }

    private:
        //variables
        std::string _dboKey;
        std::string _dboValue;
        Wt::Dbo::ptr<Users::User> _user;
        Wt::Dbo::ptr<Users::Group> _group;
        Wt::Dbo::ptr<Projects::Project> _project;
        Wt::Dbo::ptr<Projects::ProjectSequence> _sequence;
        Wt::Dbo::ptr<Projects::ProjectShot> _shot;
        Wt::Dbo::ptr<Projects::ProjectAsset> _asset;
        Wt::Dbo::ptr<Projects::ProjectTask> _task;

        //functions
        void _init();
    };

    class Note : public Ms::Dbo::MDbo
    {
    public:
        Note();
        Note(const std::string &content);

        //variables


        //functions
        Note *modify() override;
        std::string content() const;
        void setContent(const std::string &content);
        Wt::Dbo::ptr<Users::User> user() const;
        void setUser(Wt::Dbo::ptr<Users::User> user);
        Wt::Dbo::ptr<Users::Group> group() const;
        void setGroup(Wt::Dbo::ptr<Users::Group> group);
        Wt::Dbo::ptr<Projects::Project> project() const;
        void setProject(Wt::Dbo::ptr<Projects::Project> project);
        Wt::Dbo::ptr<Projects::ProjectSequence> sequence() const;
        void setSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence);
        Wt::Dbo::ptr<Projects::ProjectShot> shot() const;
        void setShot(Wt::Dbo::ptr<Projects::ProjectShot> shot);
        Wt::Dbo::ptr<Projects::ProjectAsset> asset() const;
        void setAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset);
        Wt::Dbo::ptr<Projects::ProjectTask> task() const;
        void setTask(Wt::Dbo::ptr<Projects::ProjectTask> task);

        //operators
        bool operator ==(const Note &other) const;
        bool operator !=(const Note &other) const;

        //DBO Functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::field(a, _content, "Content");
            Wt::Dbo::belongsTo(a, _user, "User");
            Wt::Dbo::belongsTo(a, _group, "Group");
            Wt::Dbo::belongsTo(a, _project, "Project");
            Wt::Dbo::belongsTo(a, _sequence, "Project_Sequence");
            Wt::Dbo::belongsTo(a, _shot, "Project_Shot");
            Wt::Dbo::belongsTo(a, _asset, "Project_Asset");
            Wt::Dbo::belongsTo(a, _task, "Project_Task");

            persistExtras_<Action>(a);
        }

    private:
        //variables
        std::string _content;
        Wt::Dbo::ptr<Users::User> _user;
        Wt::Dbo::ptr<Users::Group> _group;
        Wt::Dbo::ptr<Projects::Project> _project;
        Wt::Dbo::ptr<Projects::ProjectSequence> _sequence;
        Wt::Dbo::ptr<Projects::ProjectShot> _shot;
        Wt::Dbo::ptr<Projects::ProjectAsset> _asset;
        Wt::Dbo::ptr<Projects::ProjectTask> _task;

        //functions
        void _init();
    };

    class Tag : public Ms::Dbo::MDbo
    {
    public:
        Tag();
        Tag(const std::string &name, const std::string &content);

        //variables

        //functions
        Tag *modify() override;
        const TagId id() const;
        std::string name() const;
        void setName(const std::string &name);
        std::string content() const;
        void setContent(const std::string &content);
        const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> users() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group>> groups() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project>> projects() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>> sequences() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>> shots() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>> assets() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> tasks() const;

        //operators
        bool operator ==(const Tag &other) const;
        bool operator !=(const Tag &other) const;

        //DBO Functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, _tagId, "Tag_", 255);
            Wt::Dbo::hasMany(a, _users, Wt::Dbo::ManyToMany, "rel_user_tags");
            Wt::Dbo::hasMany(a, _groups, Wt::Dbo::ManyToMany, "rel_group_tags");
            Wt::Dbo::hasMany(a, _projects, Wt::Dbo::ManyToMany, "rel_project_tags");
            Wt::Dbo::hasMany(a, _sequences, Wt::Dbo::ManyToMany, "rel_project_sequence_tags");
            Wt::Dbo::hasMany(a, _shots, Wt::Dbo::ManyToMany, "rel_project_shot_tags");
            Wt::Dbo::hasMany(a, _assets, Wt::Dbo::ManyToMany, "rel_project_asset_tags");
            Wt::Dbo::hasMany(a, _tasks, Wt::Dbo::ManyToMany, "rel_project_task_tags");

            persistExtras_<Action>(a);
        }

    private:
        //variables
        TagId _tagId;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> _users;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group>> _groups;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project>> _projects;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>> _sequences;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>> _shots;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>> _assets;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> _tasks;

        //functions
        void _init();
    };
}

namespace Users
{
    class UsersManager;

    enum PrivilegeFlags
    {
        View = 0x01,
        Edit = 0x02,
        Create = 0x04,
        Remove = 0x08,
        CheckIn = 0x10,
        CheckOut = 0x20,
        CreateRepos = 0x40,

        COUNT = 7
    };

    inline PrivilegeFlags operator|(PrivilegeFlags a, PrivilegeFlags b)
    {return static_cast<PrivilegeFlags>(static_cast<int>(a) | static_cast<int>(b));}

    //auth typedefs
    using AuthInfo = Wt::Auth::Dbo::AuthInfo<User>;

    //user data typedefs

    class Privilege : public Ms::Dbo::MDbo
    {
        friend class Users::UsersManager;
        friend class Database::DatabaseManager;

    public:
        Privilege();
        Privilege(const std::string &privilegeName);

        //variables


        //functions
        Privilege *modify() override;
        std::string name() const;
        void setName(const std::string &name);
        const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group>> groups() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group>>::size_type numGroups() const;

        //operators
        bool operator ==(const Privilege &other) const;
        bool operator !=(const Privilege &other) const;

        //DBO Functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, _name, "Name", 255);
            Wt::Dbo::hasMany(a, _groups, Wt::Dbo::ManyToMany, "rel_group_privileges");//create a ManyToMany relationship to the table "group"

            persistExtras_<Action>(a);
        }

    private:
        void _init();
        std::string _name;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group>> _groups;//groups this privilege in used into
    };

    class Group : public Ms::Dbo::MDbo
    {
        friend class Users::UsersManager;
        friend class Database::DatabaseManager;

    public:
        Group();
        Group(const std::string &groupName, int rank = 0);

        //variables
        std::string name() const;
        void setName(const std::string &name);
        int rank() const;
        void setRank(int rank);
        const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Privilege>> privileges() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> users() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>> data();
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>> tags();
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>> notes();
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::Privilege>>::size_type numPrivileges() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>>::size_type numUsers() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>>::size_type numData() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>>::size_type numNotes() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>>::size_type numTags() const;

        //functions
        Group *modify() override;

        //operators
        bool operator ==(const Group &other) const;
        bool operator !=(const Group &other) const;

        //DBO Functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, _name, "Name", 255);
            Wt::Dbo::field(a, _rank, "Rank");
            Wt::Dbo::hasMany(a, _privileges, Wt::Dbo::ManyToMany, "rel_group_privileges");//create a ManyToMany relationship to the table "privilege"
            Wt::Dbo::hasMany(a, _users, Wt::Dbo::ManyToOne, "Group");//create a ManyToMany relationship to the table "user"
            Wt::Dbo::hasMany(a, _data, Wt::Dbo::ManyToOne, "Group");//create a ManyToOne relationship to the table "data
            Wt::Dbo::hasMany(a, _notes, Wt::Dbo::ManyToOne, "Group");//create a ManyToOne relationship to the table "note
            Wt::Dbo::hasMany(a, _tags, Wt::Dbo::ManyToMany, "rel_group_tags");//create a ManyToMany relationship to the table "tag

            persistExtras_<Action>(a);
        }

    private:
        //variables
        std::string _name;
        int _rank;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::Privilege>> _privileges;//privileges this group has
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> _users;//users in this group
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>> _data;//custom data this group has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>> _notes;//notes this group has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>> _tags;//tags this group has

        //functions
        void _init();

    };

    class UserTitle : public Ms::Dbo::MDbo
    {
        friend class Users::UsersManager;
        friend class Database::DatabaseManager;

    public:
        UserTitle();
        UserTitle(const std::string &titleName);

        //variables

        //functions
        UserTitle *modify() override;
        std::string name() const;
        void setName(const std::string &name);
        const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> users() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>>::size_type numUsers() const;

        //operators
        bool operator ==(const UserTitle &other) const;
        bool operator !=(const UserTitle &other) const;

        //DBO Functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, _name, "Name", 255);
            Wt::Dbo::hasMany(a, _users, Wt::Dbo::ManyToOne, "Title");//create a ManyToMany relationship to the table "user"

            persistExtras_<Action>(a);
        }

    private:
        //variables
        std::string _name;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> _users;//users using this title

        //functions
        void _init();

    };

    class User : public Ms::Dbo::MDbo
    {
        friend class Users::UsersManager;
        friend class Database::DatabaseManager;

    public:
        User();
        User(const std::string &name);
        User(const std::string &name, const std::string &emailAddress);

        //functions
        User *modify() override;
        std::string name() const;
        void setName(const std::string &name);
        std::string idNumber() const;
        void setIdNumber(const std::string &idNumber);
        std::string phoneNumber() const;
        void setPhoneNumber(const std::string &phoneNumber);
        std::string emailAddress() const;
        void setEmailAddress(const std::string &emailAddress);
        Wt::WDateTime lastLogin() const;
        std::string address() const;
        void setAddress(const std::string &address);
        Wt::Dbo::ptr<Users::Group> group() const;//group this user in
        void setGroup(Wt::Dbo::ptr<Users::Group> group);
        Wt::Dbo::ptr<Users::UserTitle> title() const;//title this user has
        void setTitle(Wt::Dbo::ptr<Users::UserTitle> title);
        Wt::Dbo::weak_ptr<Projects::Project> project() const;
        void setProject(Wt::Dbo::weak_ptr<Projects::Project> project);//project this user manage
        int createRank() const;
        void setCreateRank(int rank);
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> tasks();
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>> data();
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>> notes();
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>>::size_type numTasks() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>>::size_type numData() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>>::size_type numNotes() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>>::size_type numTags() const;

        //operators
        bool operator ==(const User &other) const;
        bool operator !=(const User &other) const;

        //DBO Functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, _name, "Name", 255);
            Wt::Dbo::field(a, _idNumber, "Id_Number", 14);
            Wt::Dbo::field(a, _phoneNumber, "Phone_Number", 255);
            Wt::Dbo::field(a, _emailAddress, "Email_Address", 255);
            Wt::Dbo::field(a, _lastLogin, "Last_Login");
            Wt::Dbo::field(a, _address, "Address", 255);
            Wt::Dbo::field(a, _createRank, "Create_Rank");
            Wt::Dbo::belongsTo(a, _group, "Group");//create a OneToMany relationship to the table "group"
            Wt::Dbo::belongsTo(a, _title, "Title");//create a OneToMany relationship to the table "title"
            Wt::Dbo::hasMany(a, _tasks, Wt::Dbo::ManyToOne, "Task_User");//create a ManyToOne relationship to the table "project_task"
            Wt::Dbo::hasOne(a, _project, "Project_Manager");//create a ManyToOne relationship to the table "project"
            Wt::Dbo::hasMany(a, _data, Wt::Dbo::ManyToOne, "User");//create a ManyToOne relationship to the table "data
            Wt::Dbo::hasMany(a, _notes, Wt::Dbo::ManyToOne, "User");//create a ManyToOne relationship to the table "note
            Wt::Dbo::hasMany(a, _tags, Wt::Dbo::ManyToMany, "rel_user_tags");//create a ManyToMany relationship to the table "tag

            persistExtras_<Action>(a);
        }

    private:
        //variables
        std::string _name;
        std::string _idNumber;
        std::string _phoneNumber;
        std::string _emailAddress;
        Wt::WDateTime _lastLogin;
        std::string _address;
        Wt::Dbo::ptr<Users::Group> _group;//group this user in
        Wt::Dbo::ptr<Users::UserTitle> _title;//title this user has
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> _tasks;//tasks this user has
        Wt::Dbo::weak_ptr<Projects::Project> _project;//project this user manage
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>> _data;//custom data this user has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>> _notes;//notes this user has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>> _tags;//tags this user has
        int _createRank;

        //functions
        void _init();
    };
}

namespace Projects
{
    class ProjectsManager;

    class ProjectWorkStatusType : public Ms::Dbo::MDbo
    {
        friend class Projects::ProjectsManager;
        friend class Database::DatabaseManager;

    public:
        ProjectWorkStatusType();
        ProjectWorkStatusType(const std::string &workStatusType);

        //variables

        //functions
        ProjectWorkStatusType *modify() override;
        std::string workStatusType() const;
        void setWorkStatusType(const std::string &type);
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectWorkStatus>>::size_type numWorkStatus() const;

        //operators
        bool operator ==(const ProjectWorkStatusType &other) const;
        bool operator !=(const ProjectWorkStatusType &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, _workStatusType, "Work_Status_Type", 255);
            Wt::Dbo::hasMany(a, _projectWorkStatus, Wt::Dbo::ManyToOne, "Belongs_To");//create a ManyToOne relationship to the table "project_work_status"

            persistExtras_<Action>(a);
        }

    private:
        //variabls
        std::string _workStatusType;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> _projectWorkStatus;//projectWorkStatus using this type

        //functions
        void _init();
    };

    class ProjectAssetType : public Ms::Dbo::MDbo
    {
        friend class Projects::ProjectsManager;
        friend class Database::DatabaseManager;

    public:
        ProjectAssetType();
        ProjectAssetType(const std::string &type);

        //variables
        std::string type() const;
        void setType(const std::string &type);
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>>::size_type numAssets() const;//assets using this type

        //functions
        ProjectAssetType *modify() override;

        //operators
        bool operator ==(const ProjectAssetType &other) const;
        bool operator !=(const ProjectAssetType &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, _type, "Type", 255);
            Wt::Dbo::hasMany(a, _assets, Wt::Dbo::ManyToOne, "Asset");//create a ManyToOne relationship to the table "project_asset_type"

            persistExtras_<Action>(a);
        }

    private:
        //variables
        std::string _type;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>> _assets;//assets using this type

        //functions
        void  _init();
    };

    class ProjectWorkStatus : public Ms::Dbo::MDbo
    {
        friend class Projects::ProjectsManager;
        friend class Database::DatabaseManager;

    public:
        ProjectWorkStatus();
        ProjectWorkStatus(const std::string &status);

        //variables

        //functions
        ProjectWorkStatus *modify() override;
        std::string status() const;
        void setStatus(const std::string &status);
        Wt::Dbo::ptr<Projects::ProjectWorkStatusType> workStatusType() const;
        void setWorkStatusType(const Wt::Dbo::ptr<Projects::ProjectWorkStatusType> &workStatusType);
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>>::size_type numSequences() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>>::size_type numTasks() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>>::size_type numAssets() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>>::size_type numShots() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project>>::size_type numProjects() const;

        //operators
        bool operator ==(const ProjectWorkStatus &other) const;
        bool operator !=(const ProjectWorkStatus &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, _status, "Status", 255);
            Wt::Dbo::belongsTo(a, _workStatusType, "Belongs_To");//create a ManyToOne relationship to the table "project_work_status_type"
            Wt::Dbo::hasMany(a, _sequences, Wt::Dbo::ManyToOne, "Current");//create a ManyToOne relationship to the table "project_sequence"
            Wt::Dbo::hasMany(a, _tasks, Wt::Dbo::ManyToOne, "Current");//create a ManyToOne relationship to the table "project_task"
            Wt::Dbo::hasMany(a, _assets, Wt::Dbo::ManyToOne, "Current");//create a ManyToOne relationship to the table "project_asset"
            Wt::Dbo::hasMany(a, _shots, Wt::Dbo::ManyToOne, "Current");//create a ManyToOne relationship to the table "project_shot"
            Wt::Dbo::hasMany(a, _projects, Wt::Dbo::ManyToOne, "Current");//create a ManyToOne relationship to the table "project"

            persistExtras_<Action>(a);
        }

    private:
        //variables
        std::string _status;
        Wt::Dbo::ptr<Projects::ProjectWorkStatusType> _workStatusType;//WorkStatusType this belongs to
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>> _sequences;//sequences using this type
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> _tasks;//tasks using this type
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>> _assets;//assets using this type
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>> _shots;//shots using this type
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project>> _projects;//projects using this type

        //functions
        void _init();
    };

    class ProjectSequence : public Ms::Dbo::MDbo
    {
        friend class Projects::ProjectsManager;
        friend class Database::DatabaseManager;

    public:
        ProjectSequence();
        ProjectSequence(const std::string &sequenceName);

        //variables

        //functions
        ProjectSequence *modify() override;
        const ProjectSequenceId id() const;
        std::string name() const;
        void setName(const std::string &name);
        std::string projectName() const;
        Wt::Dbo::ptr<Projects::Project> project() const;
        void setProject(Wt::Dbo::ptr<Projects::Project> project);
        Wt::WDate startDate() const;
        void setStartDate(const Wt::WDate &startDate);
        Wt::WDate endDate() const;
        void setEndDate(const Wt::WDate &endDate);
        int durationInFrames() const;
        void setDurationInFrames(int durationInFrames);
        float fps() const;
        void setFps(float fps);
        int frameWidth() const;
        void setFrameWidth(int frameWidth);
        int frameHeight() const;
        void setFrameHeight(int frameHeight);
        std::string description() const;
        void setDescription(const std::string &description);
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        void setStatus(const Wt::Dbo::ptr<Projects::ProjectWorkStatus> status);
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>>::size_type numShots() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>>::size_type numData() const;//custom data this sequence has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>>::size_type numNotes() const;//notes this sequence has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>>::size_type numTags() const;//tags this sequence has

        //operators
        bool operator ==(const ProjectSequence &other) const;
        bool operator !=(const ProjectSequence &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, _id, "Sequence_", 255);
            Wt::Dbo::field(a, _startDate, "Start_Date");
            Wt::Dbo::field(a, _endDate, "End_Date");
            Wt::Dbo::field(a, _durationInFrames, "Duration_In_Frames");
            Wt::Dbo::field(a, _fps, "FPS");
            Wt::Dbo::field(a, _frameWidth, "Frame_Width");
            Wt::Dbo::field(a, _frameHeight, "Frame_Height");
            Wt::Dbo::field(a, _description, "Description");
            Wt::Dbo::belongsTo(a, _status, "Current");//create a ManyToOne relationship to the table "project_work_status"
            Wt::Dbo::hasMany(a, _shots, Wt::Dbo::ManyToOne, "Shot_Sequence");//create a ManyToOne relationship to the table "project_shot"
            Wt::Dbo::hasMany(a, _data, Wt::Dbo::ManyToOne, "Project_Sequence");
            Wt::Dbo::hasMany(a, _notes, Wt::Dbo::ManyToOne, "Project_Sequence");
            Wt::Dbo::hasMany(a, _tags, Wt::Dbo::ManyToMany, "rel_project_sequence_tags");

            persistExtras_<Action>(a);
        }

    private:
        //functions
        ProjectSequenceId _id;
        Wt::WDate _startDate;
        Wt::WDate _endDate;
        int _durationInFrames;
        float _fps;
        int _frameWidth;
        int _frameHeight;
        std::string _description;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> _status;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>> _shots;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>> _data;//ustom data this sequence has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>> _notes;//notes this sequence has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>> _tags;//tags this sequence has

        //variables
        void _init();
    };

    class ProjectTask : public Ms::Dbo::MDbo
    {
        friend class Projects::ProjectsManager;
        friend class Database::DatabaseManager;

    public:
        ProjectTask();

        //variables
        Wt::WDate startDate() const;
        void setStartDate(const Wt::WDate &startDate);
        Wt::WDate endDate() const;
        void setEndDate(const Wt::WDate &endDate);
        std::string description() const;
        void setDescription(const std::string &description);
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        void setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status);
        Wt::Dbo::ptr<Projects::ProjectTaskType> type() const;
        void setType(Wt::Dbo::ptr<Projects::ProjectTaskType> type);
        Wt::Dbo::ptr<Users::User> user() const;
        void setUser(Wt::Dbo::ptr<Users::User> user);
        Wt::Dbo::ptr<Projects::ProjectShot> shot() const;
        void setShot(Wt::Dbo::ptr<Projects::ProjectShot> shot);
        Wt::Dbo::ptr<Projects::ProjectAsset> asset() const;
        void setAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset);
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>>::size_type numData() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>>::size_type numNotes() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>>::size_type numTags() const;

        //functions
        ProjectTask *modify() override;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::field(a, _startDate, "Start_Date");
            Wt::Dbo::field(a, _endDate, "End_Date");
            Wt::Dbo::field(a, _description, "Description");
            Wt::Dbo::belongsTo(a, _status, "Current");//create a ManyToOne relationship to the table "project_work_status"
            Wt::Dbo::belongsTo(a, _type, "Task");//create a ManyToOne relationship to the table "project_shot_task_type"
            Wt::Dbo::belongsTo(a, _user, "Task_User");//create a ManyToOne relationship to the table "user"
            Wt::Dbo::belongsTo(a, _shot, "Task_Shot");//create a ManyToOne relationship to the table "project_shot_task_type"
            Wt::Dbo::belongsTo(a, _asset, "Task_Asset");//create a ManyToOne relationship to the table "project_asset"
            Wt::Dbo::hasMany(a, _data, Wt::Dbo::ManyToOne, "Project_Task");
            Wt::Dbo::hasMany(a, _notes, Wt::Dbo::ManyToOne, "Project_Task");
            Wt::Dbo::hasMany(a, _tags, Wt::Dbo::ManyToMany, "rel_project_task_tags");

            persistExtras_<Action>(a);
        }

    private:
        //variables
        Wt::WDate _startDate;
        Wt::WDate _endDate;
        std::string _description;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> _status;
        Wt::Dbo::ptr<Projects::ProjectTaskType> _type;
        Wt::Dbo::ptr<Users::User> _user;
        Wt::Dbo::ptr<Projects::ProjectShot> _shot;
        Wt::Dbo::ptr<Projects::ProjectAsset> _asset;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>> _data;//custom data this task has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>> _notes;//notes this task has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>> _tags;//tags this task has
    };

    class ProjectTaskType : public Ms::Dbo::MDbo
    {
        friend class Projects::ProjectsManager;
        friend class Database::DatabaseManager;

    public:
        ProjectTaskType();
        ProjectTaskType(const std::string &type);

        //variables

        //functions
        ProjectTaskType *modify() override;
        std::string type() const;
        void setType(const std::string &type);
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>>::size_type numTasks() const;

        //operators
        bool operator ==(const ProjectTaskType &other) const;
        bool operator !=(const ProjectTaskType &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, _type, "Type", 255);
            Wt::Dbo::hasMany(a, _tasks, Wt::Dbo::ManyToOne, "Task");//create a ManyToOne relationship to the table "project_shot_task"

            persistExtras_<Action>(a);
        }

    private:
        //variables
        std::string _type;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> _tasks;//tasks using this type

        //functions
        void _init();
    };

    class ProjectShot : public Ms::Dbo::MDbo
    {
        friend class Projects::ProjectsManager;
        friend class Database::DatabaseManager;

    public:
        ProjectShot();
        ProjectShot(const std::string &shotName);

        //variables

        //functions
        ProjectShot *modify() override;
        const ProjectShotId id() const;
        std::string name() const;
        void setName(const std::string &name);
        std::string sequenceName() const;
        Wt::Dbo::ptr<Projects::ProjectSequence> sequence() const;
        void setSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence);
        std::string projectName() const;
        Wt::Dbo::ptr<Projects::Project> project() const;
        Wt::WDate startDate() const;
        void setStartDate(const Wt::WDate &startDate);
        Wt::WDate endDate() const;
        void setEndDate(const Wt::WDate &endDate);
        int durationInFrames() const;
        void setDurationInFrames(int durationInFrames);
        float fps() const;
        void setFps(float fps);
        int frameWidth() const;
        void setFrameWidth(int frameWidth);
        int frameHeight() const;
        void setFrameHeight(int frameHeight);
        std::string description() const;
        void setDescription(const std::string &description);
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        void setStatus(const Wt::Dbo::ptr<Projects::ProjectWorkStatus> status);
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>>::size_type numTasks() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>>::size_type numData() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>>::size_type numNotes() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>>::size_type numTags() const;

        //operators
        bool operator ==(const ProjectShot &other) const;
        bool operator !=(const ProjectShot &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {

            Wt::Dbo::id(a, _id, "Shot_", 255);
            Wt::Dbo::field(a, _startDate, "Start_Date");
            Wt::Dbo::field(a, _endDate, "End_Date");
            Wt::Dbo::field(a, _durationInFrames, "Duration_In_Frames");
            Wt::Dbo::field(a, _fps, "FPS");
            Wt::Dbo::field(a, _frameWidth, "Frame_Width");
            Wt::Dbo::field(a, _frameHeight, "Frame_Height");
            Wt::Dbo::field(a, _description, "Description");
            Wt::Dbo::belongsTo(a, _status, "Current");//create a ManyToOne relationship to the table "project_work_status"
            Wt::Dbo::hasMany(a, _tasks, Wt::Dbo::ManyToOne, "Task_Shot");//create a ManyToOne relationship to the table "project_task"
            Wt::Dbo::hasMany(a, _data, Wt::Dbo::ManyToOne, "Project_Shot");//create a ManyToOne relationship to the table "data"
            Wt::Dbo::hasMany(a, _notes, Wt::Dbo::ManyToOne, "Project_Shot");//create a ManyToOne relationship to the table "note"
            Wt::Dbo::hasMany(a, _tags, Wt::Dbo::ManyToMany, "rel_project_shot_tags");//create a ManyToMany relationship to the table "tag"

            persistExtras_<Action>(a);
        }

    private:
        //variables
        ProjectShotId _id;
        Wt::WDate _startDate;
        Wt::WDate _endDate;
        int _durationInFrames;
        float _fps;
        int _frameWidth;
        int _frameHeight;
        std::string _description;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> _status;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> _tasks;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>> _data;//custom data this task has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>> _notes;//notes this task has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>> _tags;//tags this task has

        //functions
        void _init();
    };

    class ProjectAsset : public Ms::Dbo::MDbo
    {
        friend class Projects::ProjectsManager;
        friend class Database::DatabaseManager;

    public:
        ProjectAsset();
        ProjectAsset(const std::string &assetName);

        //variables


        //functions
        ProjectAsset *modify() override;
        const ProjectAssetId id() const;
        const std::string name() const;
        void setName(const std::string &name);
        std::string projectName() const;
        Wt::Dbo::ptr<Projects::Project> project() const;
        void setProject(Wt::Dbo::ptr<Projects::Project> project);
        Wt::WDate startDate() const;
        void setStartDate(const Wt::WDate &startDate);
        Wt::WDate endDate() const;
        void setEndDate(const Wt::WDate &endDate);
        std::string description() const;
        void setDescription(const std::string &description);
        Wt::Dbo::ptr<Projects::ProjectAssetType> type() const;
        void setType(const Wt::Dbo::ptr<Projects::ProjectAssetType> type);
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        void setStatus(const Wt::Dbo::ptr<Projects::ProjectWorkStatus> status);
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>>::size_type numTasks() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>>::size_type numData() const;//ustom data this sequence has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>>::size_type numNotes() const;//notes this sequence has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>>::size_type numTags() const;//tags this sequence has

        //operators
        bool operator ==(const ProjectAsset &other) const;
        bool operator !=(const ProjectAsset &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, _id, "Asset_", 255);
            Wt::Dbo::field(a, _startDate, "Start_Date");
            Wt::Dbo::field(a, _endDate, "End_Date");
            Wt::Dbo::field(a, _description, "Description", 255);
            Wt::Dbo::belongsTo(a, _status, "Current");//create a ManyToOne relationship to the table "project_work_status"
            Wt::Dbo::belongsTo(a, _type, "Asset");//create a ManyToOne relationship to the table "project_asset_type"
            Wt::Dbo::hasMany(a, _tasks, Wt::Dbo::ManyToOne, "Task_Asset");//create a ManyToOne relationship to the table "project_task"
            Wt::Dbo::hasMany(a, _data, Wt::Dbo::ManyToOne, "Project_Asset");//create a ManyToOne relationship to the table "data"
            Wt::Dbo::hasMany(a, _notes, Wt::Dbo::ManyToOne, "Project_Asset");//create a ManyToOne relationship to the table "note"
            Wt::Dbo::hasMany(a, _tags, Wt::Dbo::ManyToMany, "rel_project_asset_tags");//create a ManyToMany relationship to the table "tag"

            persistExtras_<Action>(a);
        }

    private:
        //variables
        ProjectAssetId _id;
        Wt::WDate _startDate;
        Wt::WDate _endDate;
        std::string _description;
        Wt::Dbo::ptr<Projects::ProjectAssetType> _type;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> _status;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> _tasks;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>> _data;//custom data this asset has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>> _notes;//notes this asset has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>> _tags;//tags this asset has

        //functions
        void _init();
    };

    class Project : public Ms::Dbo::MDbo
    {
        friend class Projects::ProjectsManager;
        friend class Database::DatabaseManager;

    public:
        Project();
        Project(const std::string &projectName);

        //variables


        //functions
        Project *modify() override;
        std::string name() const;
        void setName(const std::string &name);
        Wt::WDate startDate() const;
        void setStartDate(const Wt::WDate &startDate);
        Wt::WDate endDate() const;
        void setEndDate(const Wt::WDate &endDate);
        int durationInFrames() const;
        void setDurationInFrames(int durationInFrames);
        float fps() const;
        void setFps(float fps);
        int frameWidth() const;
        void setFrameWidth(int frameWidth);
        int frameHeight() const;
        void setFrameHeight(int frameHeight);
        std::string description() const;
        void setDescription(const std::string &description);
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        void setStatus(const Wt::Dbo::ptr<Projects::ProjectWorkStatus> status);
        Wt::Dbo::ptr<Users::User> manager() const;
        void setManager(const Wt::Dbo::ptr<Users::User> user);
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>>::size_type numSequences() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>>::size_type numAssets() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>>::size_type numData() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>>::size_type numNotes() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>>::size_type numTags() const;

        //operators
        bool operator ==(const Project &other) const;
        bool operator !=(const Project &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, _name, "Project_Name", 255);
            Wt::Dbo::field(a, _startDate, "Start_Date");
            Wt::Dbo::field(a, _endDate, "End_Date");
            Wt::Dbo::field(a, _durationInFrames, "Duration_In_Frames");
            Wt::Dbo::field(a, _fps, "FPS");
            Wt::Dbo::field(a, _frameWidth, "Frame_Width");
            Wt::Dbo::field(a, _frameHeight, "Frame_Height");
            Wt::Dbo::field(a, _description, "Description");
            Wt::Dbo::field(a, _progress, "Progress");
            Wt::Dbo::belongsTo(a, _status, "Current");//create a ManyToOne relationship to the table "project_work_status"
            Wt::Dbo::belongsTo(a, _projectManager, "Project_Manager");//create a ManyToOne relationship to the table "users"
            Wt::Dbo::hasMany(a, _sequences, Wt::Dbo::ManyToOne, "Sequence_Project");//create a ManyToOne relationship to the table "project_sequence"
            Wt::Dbo::hasMany(a, _assets, Wt::Dbo::ManyToOne, "Asset_Project");//create a ManyToOne relationship to the table "project_asset"
            Wt::Dbo::hasMany(a, _data, Wt::Dbo::ManyToOne, "Project");//create a ManyToOne relationship to the table "data"
            Wt::Dbo::hasMany(a, _notes, Wt::Dbo::ManyToOne, "Project");//create a ManyToOne relationship to the table "note"
            Wt::Dbo::hasMany(a, _tags, Wt::Dbo::ManyToMany, "rel_project_tags");//create a ManyToMany relationship to the table "tag"

            persistExtras_<Action>(a);
        }

    private:
        //variables
        std::string _name;
        Wt::WDate _startDate;
        Wt::WDate _endDate;
        int _durationInFrames;
        float _fps;
        int _frameWidth;
        int _frameHeight;
        int _progress;
        std::string _description;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> _status;
        Wt::Dbo::ptr<Users::User> _projectManager;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>> _sequences;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>> _assets;
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>> _data;//custom data this project has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>> _notes;//notes this project has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>> _tags;//tags this project has

        //functions
        void _init();
    };
}


#endif // DBTABLES_H
