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

#include <Ms/Dbo/MDboBase.h>

#include "dbospecialization.h"
#include "projectdbospecialization.h"
#include "userdbospecialization.h"
#include "globaldbospecialization.h"

namespace Database
{
    class DboSession;

    class DboData : public Ms::Dbo::MDboBase
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
            Wt::Dbo::field(a, m_dboKey, "DBOKey");
            Wt::Dbo::field(a, m_dboValue, "DBOValue");
            Wt::Dbo::belongsTo(a, m_user, "User");
            Wt::Dbo::belongsTo(a, m_group, "Group");
            Wt::Dbo::belongsTo(a, m_project, "Project");
            Wt::Dbo::belongsTo(a, m_sequence, "Project_Sequence");
            Wt::Dbo::belongsTo(a, m_shot, "Project_Shot");
            Wt::Dbo::belongsTo(a, m_asset, "Project_Asset");
            Wt::Dbo::belongsTo(a, m_task, "Project_Task");

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        std::string m_dboKey;
        std::string m_dboValue;
        Wt::Dbo::ptr<Users::User> m_user;
        Wt::Dbo::ptr<Users::Group> m_group;
        Wt::Dbo::ptr<Projects::Project> m_project;
        Wt::Dbo::ptr<Projects::ProjectSequence> m_sequence;
        Wt::Dbo::ptr<Projects::ProjectShot> m_shot;
        Wt::Dbo::ptr<Projects::ProjectAsset> m_asset;
        Wt::Dbo::ptr<Projects::ProjectTask> m_task;

        //functions
        void init();
    };

    class Note : public Ms::Dbo::MDboBase
    {
    public:
        Note();
        explicit Note(const std::string &content);

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
            Wt::Dbo::field(a, m_content, "Content");
            Wt::Dbo::belongsTo(a, m_user, "User");
            Wt::Dbo::belongsTo(a, m_group, "Group");
            Wt::Dbo::belongsTo(a, m_project, "Project");
            Wt::Dbo::belongsTo(a, m_sequence, "Project_Sequence");
            Wt::Dbo::belongsTo(a, m_shot, "Project_Shot");
            Wt::Dbo::belongsTo(a, m_asset, "Project_Asset");
            Wt::Dbo::belongsTo(a, m_task, "Project_Task");

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        std::string m_content;
        Wt::Dbo::ptr<Users::User> m_user;
        Wt::Dbo::ptr<Users::Group> m_group;
        Wt::Dbo::ptr<Projects::Project> m_project;
        Wt::Dbo::ptr<Projects::ProjectSequence> m_sequence;
        Wt::Dbo::ptr<Projects::ProjectShot> m_shot;
        Wt::Dbo::ptr<Projects::ProjectAsset> m_asset;
        Wt::Dbo::ptr<Projects::ProjectTask> m_task;

        //functions
        void init();
    };

    class Tag : public Ms::Dbo::MDboBase
    {
    public:
        Tag();
        Tag(const std::string &name, const std::string &content);

        //variables

        //functions
        Tag *modify() override;
        std::string name() const;
        void setName(const std::string &name);
        std::string content() const;
        void setContent(const std::string &content);
        std::string type() const;
        void setType(const std::string &type);
        const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> users() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group>> groups() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project>> projects() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>> sequences() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>> shots() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>> assets() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> tasks() const;

        const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> usersAssignedTo() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group>> groupsAssignedTo() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project>> projectsAssignedTo() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>> sequencesAssignedTo() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>> shotsAssignedTo() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>> assetsAssignedTo() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> tasksAssignedTo() const;

        //operators
        bool operator ==(const Tag &other) const;
        bool operator !=(const Tag &other) const;

        //DBO Functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::field(a, m_name, "Name");
            Wt::Dbo::field(a, m_content, "Content");
            Wt::Dbo::field(a, m_type, "Type");
            Wt::Dbo::hasMany(a, m_users, Wt::Dbo::ManyToMany, "rel_user_tags");
            Wt::Dbo::hasMany(a, m_groups, Wt::Dbo::ManyToMany, "rel_group_tags");
            Wt::Dbo::hasMany(a, m_projects, Wt::Dbo::ManyToMany, "rel_project_tags");
            Wt::Dbo::hasMany(a, m_sequences, Wt::Dbo::ManyToMany, "rel_project_sequence_tags");
            Wt::Dbo::hasMany(a, m_shots, Wt::Dbo::ManyToMany, "rel_project_shot_tags");
            Wt::Dbo::hasMany(a, m_assets, Wt::Dbo::ManyToMany, "rel_project_asset_tags");
            Wt::Dbo::hasMany(a, m_tasks, Wt::Dbo::ManyToMany, "rel_project_task_tags");

            Wt::Dbo::hasMany(a, m_usersAssignedTo, Wt::Dbo::ManyToMany, "rel_user_assigned_tags");
            Wt::Dbo::hasMany(a, m_groupsAssignedTo, Wt::Dbo::ManyToMany, "rel_group_assigned_tags");
            Wt::Dbo::hasMany(a, m_projectsAssignedTo, Wt::Dbo::ManyToMany, "rel_project_assigned_tags");
            Wt::Dbo::hasMany(a, m_sequencesAssignedTo, Wt::Dbo::ManyToMany, "rel_project_sequence_assigned_tags");
            Wt::Dbo::hasMany(a, m_shotsAssignedTo, Wt::Dbo::ManyToMany, "rel_project_shot_assigned_tags");
            Wt::Dbo::hasMany(a, m_assetsAssignedTo, Wt::Dbo::ManyToMany, "rel_project_asset_assigned_tags");
            Wt::Dbo::hasMany(a, m_tasksAssignedTo, Wt::Dbo::ManyToMany, "rel_project_task_assigned_tags");

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        std::string m_name;
        std::string m_content;
        std::string m_type;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> m_users;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group>> m_groups;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project>> m_projects;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>> m_sequences;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>> m_shots;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>> m_assets;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> m_tasks;

        Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> m_usersAssignedTo;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group>> m_groupsAssignedTo;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project>> m_projectsAssignedTo;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>> m_sequencesAssignedTo;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>> m_shotsAssignedTo;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>> m_assetsAssignedTo;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> m_tasksAssignedTo;

        //functions
        void init();
    };

    class Dbo : public Ms::Dbo::MDboBase
    {
    public:
        Dbo();

        bool hasData(Wt::Dbo::ptr<Database::DboData> data) const;
        bool addData(Wt::Dbo::ptr<Database::DboData> data);
        bool removeData(Wt::Dbo::ptr<Database::DboData> data);
        bool hasTag(Wt::Dbo::ptr<Database::Tag> tag) const;
        bool addTag(Wt::Dbo::ptr<Database::Tag> tag);
        bool removeTag(Wt::Dbo::ptr<Database::Tag> tag);
        bool hasTagAssigned(Wt::Dbo::ptr<Database::Tag> tag) const;
        bool assignTag(Wt::Dbo::ptr<Database::Tag> tag);
        bool unassignTag(Wt::Dbo::ptr<Database::Tag> tag);
        bool hasNote(Wt::Dbo::ptr<Database::Note> note) const;
        bool addNote(Wt::Dbo::ptr<Database::Note> note);
        bool removeNote(Wt::Dbo::ptr<Database::Note> note);

        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>> data();
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>> tags();
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>> notes();

        //functions
        Dbo *modify() override;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    protected:
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>> m_data;//custom data this group has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>> m_notes;//notes this group has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>> m_tags;//tags this group has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>> m_assignedTags;//tags assigned to this group
    };

    class Token : public Ms::Dbo::MDboBase
    {
    public:
        Token();
        Token(const std::string &value, const Wt::WDateTime &expires);

        //variables

        //functions
        Token *modify() override;
        std::string value() const;
        void setValue(const std::string &value);
        Wt::WDateTime expires() const;
        void setExpires(const Wt::WDateTime &expires);

        const Wt::Dbo::ptr<Users::User> user() const;

        //operators
        bool operator ==(const Tag &other) const;
        bool operator !=(const Tag &other) const;

        //DBO Functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::field(a, m_value, "Value");
            Wt::Dbo::field(a, m_expires, "Expires");

            Wt::Dbo::belongsTo(a, m_user, "Token_User");

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        std::string m_value;
        Wt::WDateTime m_expires;
        Wt::Dbo::ptr<Users::User> m_user;
    };

//    class Notifier : public Database::Dbo
//    {
//    public:
//        Notifier();

//        template<class Action>
//        void persist(Action &a)
//        {
//            Database::Dbo::persist<Action>(a);
//        }

//    private:
//        Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> m_users;//users subscriped to this dbo
//    };

    class Notification : public Ms::Dbo::MDboBase
    {
    public:
        Notification();
        explicit Notification(const std::string &message);

        //variables

        //functions
        Notification *modify() override;

        std::string message() const;
        void setMessage(const std::string &message);
        bool hasUser(Wt::Dbo::ptr<Users::User> user) const;
        bool addUser(Wt::Dbo::ptr<Users::User> user);
        bool removeUser(Wt::Dbo::ptr<Users::User> user);

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::field(a, m_message, "Message", 255);

            Wt::Dbo::hasMany(a, m_users, Wt::Dbo::ManyToMany, "rel_user_notifications");

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        std::string m_message;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> m_users;//users who should receive this notification
    };
}

namespace Users
{
    class UsersManager;

    //user data typedefs

    class Privilege : public Ms::Dbo::MDboBase
    {
        friend class Users::UsersManager;
        friend class Database::DboSession;

    public:
        Privilege();
        explicit Privilege(const std::string &privilegeName);

        //variables


        //functions
        Privilege *modify() override;
        std::string name() const;
        void setName(const std::string &name);
        const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group>> groups() const;

        //operators
        bool operator ==(const Privilege &other) const;
        bool operator !=(const Privilege &other) const;

        //DBO Functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, m_name, "Name", 255);
            Wt::Dbo::hasMany(a, m_groups, Wt::Dbo::ManyToMany, "rel_group_privileges");//create a ManyToMany relationship to the table "group"

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        std::string m_name;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::Group>> m_groups;//groups this privilege in used into

        //functions
        void init();
    };

    class UserTitle : public Ms::Dbo::MDboBase
    {
        friend class Users::UsersManager;
        friend class Database::DboSession;

    public:
        UserTitle();
        explicit UserTitle(const std::string &titleName);

        //variables

        //functions
        UserTitle *modify() override;
        std::string name() const;
        void setName(const std::string &name);
        const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> users() const;

        //operators
        bool operator ==(const UserTitle &other) const;
        bool operator !=(const UserTitle &other) const;

        //DBO Functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, m_name, "Name", 255);
            Wt::Dbo::hasMany(a, m_users, Wt::Dbo::ManyToOne, "Title");//create a ManyToMany relationship to the table "user"

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        std::string m_name;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> m_users;//users using this title

        //functions
        void init();

    };

    class Group : public Database::Dbo
    {
        friend class Users::UsersManager;
        friend class Database::DboSession;

    public:
        Group();
        Group(const std::string &groupName, int rank = 0);

        //variables
        std::string name() const;
        void setName(const std::string &name);
        int rank() const;
        void setRank(int rank);
        bool hasUser (Wt::Dbo::ptr<Users::User> user) const;
        bool addUser (Wt::Dbo::ptr<Users::User> user);
        bool removeUser (Wt::Dbo::ptr<Users::User> user);
        bool hasPrivilege(Wt::Dbo::ptr<Privilege> privilege) const;
        bool hasPrivilege(const char *privilegeName) const;
        bool addPrivilege(Wt::Dbo::ptr<Privilege> privilege);
        bool removePrivilege(Wt::Dbo::ptr<Privilege> privilege);
        const Wt::Dbo::collection<Wt::Dbo::ptr<Users::Privilege>> privileges() const;
        const Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> users() const;

        //functions
        Group *modify() override;

        //operators
        bool operator ==(const Group &other) const;
        bool operator !=(const Group &other) const;

        //DBO Functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, m_name, "Name", 255);
            Wt::Dbo::field(a, m_rank, "Rank");
            Wt::Dbo::hasMany(a, m_privileges, Wt::Dbo::ManyToMany, "rel_group_privileges");//create a ManyToMany relationship to the table "privilege"
            Wt::Dbo::hasMany(a, m_users, Wt::Dbo::ManyToOne, "Group");//create a ManyToMany relationship to the table "user"
            Wt::Dbo::hasMany(a, m_data, Wt::Dbo::ManyToOne, "Group");//create a ManyToOne relationship to the table "data"
            Wt::Dbo::hasMany(a, m_notes, Wt::Dbo::ManyToOne, "Group");//create a ManyToOne relationship to the table "note"
            Wt::Dbo::hasMany(a, m_tags, Wt::Dbo::ManyToMany, "rel_group_tags");//create a ManyToMany relationship to the table "tag"
            Wt::Dbo::hasMany(a, m_assignedTags, Wt::Dbo::ManyToMany, "rel_group_assigned_tags");//create a ManyToMany relationship to the table "tag"

            Dbo::persist<Action>(a);
        }

    private:
        //variables
        std::string m_name;
        int m_rank;
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::Privilege>> m_privileges;//privileges this group has
        Wt::Dbo::collection<Wt::Dbo::ptr<Users::User>> m_users;//users in this group

        //functions
        void init();
    };

    class User : public Database::Dbo
    {
        friend class Users::UsersManager;
        friend class Database::DboSession;

    public:
        User();
        explicit User(const std::string &name);
        User(const std::string &name, const std::string &emailAddress);

        //functions
        User *modify() override;
        std::string name() const;
        void setName(const std::string &name);
        std::string password() const;
        void setPassword(const std::string &password);
        std::string passwordMethod() const;
        void setPasswordMethod(const std::string &passwordMethod);
        std::string passwordSalt() const;
        void setPasswordSalt(const std::string &passwordSalt);
        int failedLoginAttempts() const;
        void setFailedLoginAttempts(int failedLoginAttempts);
        Wt::WDateTime lastLoginAttempt() const;
        void setLastLoginAttempt(const Wt::WDateTime &lastLoginAttempt);
        std::string oAuthId() const;
        void setOAuthId(const std::string &oAuthId);
        std::string oAuthProvider() const;
        void setOAuthProvider(const std::string &oAuthProvider);
        bool hasPrivilege(Wt::Dbo::ptr<Users::Privilege> privilege) const;
        bool hasPrivilege(const char *privilegeName) const;
        bool hasTask(Wt::Dbo::ptr<Projects::ProjectTask> task) const;
        bool addTask(Wt::Dbo::ptr<Projects::ProjectTask> task);
        bool removeTask(Wt::Dbo::ptr<Projects::ProjectTask> task);
        bool hasNotification(Wt::Dbo::ptr<Database::Notification> notification) const;
        bool addNotification(Wt::Dbo::ptr<Database::Notification> notification);
        bool removeNotification(Wt::Dbo::ptr<Database::Notification> notification);

        int createRank() const;
        void setCreateRank(int rank);
        std::string idNumber() const;
        void setIdNumber(const std::string &idNumber);
        bool isAvailable() const;
        void setAvailable(bool available);
        std::string phoneNumber() const;
        void setPhoneNumber(const std::string &phoneNumber);
        std::string emailAddress() const;
        void setEmailAddress(const std::string &emailAddress);
        std::string address() const;
        void setAddress(const std::string &address);
        Wt::Dbo::ptr<Users::Group> group() const;//group this user in
        void setGroup(Wt::Dbo::ptr<Users::Group> group);
        Wt::Dbo::ptr<Users::UserTitle> title() const;//title this user has
        void setTitle(Wt::Dbo::ptr<Users::UserTitle> title);
        Wt::Dbo::weak_ptr<Projects::Project> project() const;
        void setProject(Wt::Dbo::weak_ptr<Projects::Project> project);//project this user manage
        Wt::WDateTime lastSeenNotificationsDate() const;
        void setLastSeenNotificationsDate(const Wt::WDateTime & date);
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> tasks();
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Token>> authTokens() const;

        //operators
        bool operator ==(const User &other) const;
        bool operator !=(const User &other) const;

        //DBO Functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, m_name, "Name", 255);
            Wt::Dbo::field(a, m_password, "Password", 255);
            Wt::Dbo::field(a, m_passwordMethod, "Password_Method", 255);
            Wt::Dbo::field(a, m_passwordSalt, "Password_Salt", 255);
            Wt::Dbo::field(a, m_failedLoginAttempts, "Failed_Login_Attempts", 255);
            Wt::Dbo::field(a, m_lastLoginAttempt, "Last_Login_Attempt");
            Wt::Dbo::field(a, m_oAuthId, "OAuth_Id", 255);
            Wt::Dbo::field(a, m_oAuthProvider, "OAuth_Provider", 255);
            Wt::Dbo::field(a, m_available, "Available");
            Wt::Dbo::field(a, m_idNumber, "Id_Number", 14);
            Wt::Dbo::field(a, m_phoneNumber, "Phone_Number", 255);
            Wt::Dbo::field(a, m_emailAddress, "Email_Address", 255);
            Wt::Dbo::field(a, m_address, "Address", 255);
            Wt::Dbo::field(a, m_createRank, "Create_Rank");
            Wt::Dbo::field(a, m_lastSeenNotificationDate, "Last_Seen_Notification_Date");

            Wt::Dbo::belongsTo(a, m_group, "Group");//create a OneToMany relationship to the table "group"
            Wt::Dbo::belongsTo(a, m_title, "Title");//create a OneToMany relationship to the table "title"
            Wt::Dbo::hasMany(a, m_tasks, Wt::Dbo::ManyToOne, "Task_User");//create a ManyToOne relationship to the table "project_task"
            Wt::Dbo::hasMany(a, m_authTokens, Wt::Dbo::ManyToOne, "Token_User");//create a ManyToOne relationship to the table "token"
            Wt::Dbo::hasOne(a, m_project, "Project_Manager");//create a ManyToOne relationship to the table "project"
            Wt::Dbo::hasMany(a, m_data, Wt::Dbo::ManyToOne, "User");//create a ManyToOne relationship to the table "data
            Wt::Dbo::hasMany(a, m_notes, Wt::Dbo::ManyToOne, "User");//create a ManyToOne relationship to the table "note
            Wt::Dbo::hasMany(a, m_tags, Wt::Dbo::ManyToMany, "rel_user_tags");//create a ManyToMany relationship to the table "tag
            Wt::Dbo::hasMany(a, m_assignedTags, Wt::Dbo::ManyToMany, "rel_user_assigned_tags");//create a ManyToMany relationship to the table "tag"
            Wt::Dbo::hasMany(a, m_notifications, Wt::Dbo::ManyToMany, "rel_user_notifications");//create a many to many relationship to the table notifications

            Dbo::persist<Action>(a);
        }

    private:
        //variables
        std::string m_name;
        std::string m_password;
        std::string m_passwordMethod;
        std::string m_passwordSalt;
        int m_failedLoginAttempts;
        Wt::WDateTime m_lastLoginAttempt;
        std::string m_oAuthId;
        std::string m_oAuthProvider;
        bool m_available;
        std::string m_idNumber;
        std::string m_phoneNumber;
        std::string m_emailAddress;
        std::string m_address;
        Wt::WDateTime m_lastSeenNotificationDate;
        Wt::Dbo::ptr<Users::Group> m_group;//group this user in
        Wt::Dbo::ptr<Users::UserTitle> m_title;//title this user has
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> m_tasks;//tasks this user has
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Token>> m_authTokens;//tokens this user has
        Wt::Dbo::weak_ptr<Projects::Project> m_project;//project this user manage
        Wt::Dbo::collection<Wt::Dbo::ptr<Database::Notification>> m_notifications;//notifications this user has
        int m_createRank;

        //functions
        void init();
    };
}

namespace Projects
{
    class ProjectsManager;
    class ProjectTaskTemplate;
    class ProjectActivityTemplate;
    class ProjectTaskActivity;

    class ProjectDbo : public Database::Dbo
    {
    public:
        ProjectDbo();

        ProjectDbo *modify() override;

        virtual Wt::WDate startDate() const;
        virtual void setStartDate(const Wt::WDate &startDate);
        virtual Wt::WDate endDate() const;
        virtual void setEndDate(const Wt::WDate &endDate);
        virtual std::string description() const;
        virtual void setDescription(const std::string &description);
        virtual int progress() const = 0;
        virtual int totalHours() const = 0;
        virtual int doneHours() const = 0;
        virtual int priority() const;
        virtual void setPriority(int priority);
        virtual Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        virtual void setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status);

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::field(a, m_startDate, "Start_Date");
            Wt::Dbo::field(a, m_endDate, "End_Date");
            Wt::Dbo::field(a, m_description, "Description");
            Wt::Dbo::field(a, m_priority, "Priority");

            Dbo::persist<Action>(a);
        }

    protected:
        Wt::WDate m_startDate;
        Wt::WDate m_endDate;
        std::string m_description;
        int m_priority;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> m_status;

    private:
        void init();
    };

    class ProjectTaskActivityType : public Ms::Dbo::MDboBase
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        ProjectTaskActivityType();
        explicit ProjectTaskActivityType(const std::string &type);

        //variables

        //functions
        ProjectTaskActivityType *modify() override;
        std::string type() const;
        void setType(const std::string &type);
        bool hasActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity) const;
        bool addActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity);
        bool removeActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity);

        //operators
        bool operator ==(const ProjectTaskActivityType &other) const;
        bool operator !=(const ProjectTaskActivityType &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, m_type, "Type", 255);
            Wt::Dbo::hasMany(a, m_activities, Wt::Dbo::ManyToOne, "Project_Task_Activity");

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        std::string m_type;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTaskActivity>> m_activities;

        //functions
        void init();
    };

    class ProjectTaskActivity : public Ms::Dbo::MDboBase
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        ProjectTaskActivity();

        //variables

        //functions
        ProjectTaskActivity *modify() override;
        int hours() const;
        void setHours(int hours);
        std::string description() const;
        void setDescription(const std::string &description);
        const Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        void setStatus(const Wt::Dbo::ptr<Projects::ProjectWorkStatus> status);
        const Wt::Dbo::ptr<ProjectTaskActivityType> type() const;
        void setType(const Wt::Dbo::ptr<Projects::ProjectTaskActivityType> type);
        const Wt::Dbo::ptr<Projects::ProjectTask> task() const;
        void setTask(Wt::Dbo::ptr<Projects::ProjectTask> task);

        //operators
        bool operator ==(const ProjectTaskActivity &other) const;
        bool operator !=(const ProjectTaskActivity &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::field(a, m_hours, "Hours");
            Wt::Dbo::field(a, m_description, "Description", 255);
            Wt::Dbo::belongsTo(a, m_status, "Current");//create a ManyToOne relationship to the table "project_work_status"
            Wt::Dbo::belongsTo(a, m_type, "Project_Task_Activity");
            Wt::Dbo::belongsTo(a, m_task, "Project_Task");//create a ManyToOne relationship to the table "project_task"

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        int m_hours;
        std::string m_description;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> m_status;
        Wt::Dbo::ptr<Projects::ProjectTaskActivityType> m_type;
        Wt::Dbo::ptr<Projects::ProjectTask> m_task;

        //functions
        void init();
    };

    class ProjectActivityTemplateActivityItem : public Ms::Dbo::MDboBase
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        ProjectActivityTemplateActivityItem();
        ~ProjectActivityTemplateActivityItem();

        //variables

        //functions
        ProjectActivityTemplateActivityItem *modify() override;
        const Wt::Dbo::ptr<Projects::ProjectTaskActivityType> type() const;
        void setType(Wt::Dbo::ptr<Projects::ProjectTaskActivityType> type);
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        void setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status);
        const Wt::Dbo::ptr<Projects::ProjectActivityTemplate> activityTemplate() const;
        void setActivityTemplate(Wt::Dbo::ptr<Projects::ProjectActivityTemplate> activityTemplate);
        std::string description() const;
        void setDescription(const std::string &description);
        int hours() const;
        void setHours(int hours);

        //operators
        bool operator ==(const ProjectActivityTemplateActivityItem &other) const;
        bool operator !=(const ProjectActivityTemplateActivityItem &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::belongsTo(a, m_type, "Project_Task_Activity");
            Wt::Dbo::belongsTo(a, m_status, "Current");//create a ManyToOne relationship to the table "project_work_status"
            Wt::Dbo::belongsTo(a, m_activityTemplate, "Project_Activity_Template");//create a ManyToOne relationship to the table "project_pipeline"
            Wt::Dbo::field(a, m_hours, "Hours");
            Wt::Dbo::field(a, m_description, "Description");

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        Wt::Dbo::ptr<Projects::ProjectTaskActivityType> m_type;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> m_status;
        Wt::Dbo::ptr<Projects::ProjectActivityTemplate> m_activityTemplate;
        std::string m_description;
        int m_hours;

        //functions
        void init();
    };

    class ProjectActivityTemplate : public Ms::Dbo::MDboBase
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        ProjectActivityTemplate();
        explicit ProjectActivityTemplate(const std::string &name);

        //variables

        //functions
        ProjectActivityTemplate *modify() override;
        const std::string name() const;
        void setName(const std::string &name);
        bool hasItem(Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem> taskItem) const;
        bool addItem(Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem> taskItem);
        bool removeItem(Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem> taskItem);
        bool createActivitiesForProjectTask(Wt::Dbo::ptr<Projects::ProjectTask> task);
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem>> items() const;

        //operators
        bool operator ==(const ProjectActivityTemplate &other) const;
        bool operator !=(const ProjectActivityTemplate &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, m_name, "Name", 255);
            Wt::Dbo::hasMany(a, m_items, Wt::Dbo::ManyToOne, "Project_Activity_Template");//create a ManyToOne relationship to the table "project_task"

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        std::string m_name;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectActivityTemplateActivityItem>> m_items;

        //functions
        void init();
    };

    class ProjectTaskTemplateTaskItem : public Ms::Dbo::MDboBase
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        ProjectTaskTemplateTaskItem();
        ~ProjectTaskTemplateTaskItem();

        //variables

        //functions
        ProjectTaskTemplateTaskItem *modify() override;
        const Wt::Dbo::ptr<Projects::ProjectTaskType> type() const;
        void setType(Wt::Dbo::ptr<Projects::ProjectTaskType> type);
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        void setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status);
        const Wt::Dbo::ptr<Projects::ProjectTaskTemplate> taskTemplate() const;
        void setTaskTemplate(Wt::Dbo::ptr<Projects::ProjectTaskTemplate> taskTemplate);
        std::string description() const;
        void setDescription(const std::string &description);

        //operators
        bool operator ==(const ProjectTaskTemplateTaskItem &other) const;
        bool operator !=(const ProjectTaskTemplateTaskItem &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::belongsTo(a, m_type, "Project_Task");
            Wt::Dbo::belongsTo(a, m_status, "Current");//create a ManyToOne relationship to the table "project_work_status"
            Wt::Dbo::belongsTo(a, _taskTemplate, "Project_Task_Template");//create a ManyToOne relationship to the table "project_task_template"
            Wt::Dbo::field(a, m_description, "Description");

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        Wt::Dbo::ptr<Projects::ProjectTaskType> m_type;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> m_status;
        Wt::Dbo::ptr<Projects::ProjectTaskTemplate> _taskTemplate;
        std::string m_description;

        //functions
        void init();
    };

    class ProjectTaskTemplate : public Ms::Dbo::MDboBase
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        ProjectTaskTemplate();
        explicit ProjectTaskTemplate(const std::string &name);

        //variables

        //functions
        ProjectTaskTemplate *modify() override;
        const std::string name() const;
        void setName(const std::string &name);
        bool hasItem(Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem> taskItem) const;
        bool addItem(Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem> taskItem);
        bool removeItem(Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem> taskItem);
        bool createTasksForProjectAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset);
        bool createTasksForProjectShot(Wt::Dbo::ptr<Projects::ProjectShot> shot);
        bool createTasksForProjectSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence);
        bool createTasksForProject(Wt::Dbo::ptr<Projects::Project> project);
        const Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem>> items() const;

        //operators
        bool operator ==(const ProjectTaskTemplate &other) const;
        bool operator !=(const ProjectTaskTemplate &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, m_name, "Name", 255);
            Wt::Dbo::hasMany(a, m_items, Wt::Dbo::ManyToOne, "Project_Task_Template");//create a ManyToOne relationship to the table "project_task_template"

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        std::string m_name;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTaskTemplateTaskItem>> m_items;

        //functions
        void init();
    };

    class ProjectWorkStatusType : public Ms::Dbo::MDboBase
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        ProjectWorkStatusType();
        explicit ProjectWorkStatusType(const std::string &workStatusType);

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
            Wt::Dbo::id(a, m_workStatusType, "Work_Status_Type", 255);
            Wt::Dbo::hasMany(a, m_projectWorkStatus, Wt::Dbo::ManyToOne, "Belongs_To");//create a ManyToOne relationship to the table "project_work_status"

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variabls
        std::string m_workStatusType;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> m_projectWorkStatus;//projectWorkStatus using this type

        //functions
        void init();
    };

    class ProjectAssetType : public Ms::Dbo::MDboBase
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        ProjectAssetType();
        explicit ProjectAssetType(const std::string &type);

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
            Wt::Dbo::id(a, m_type, "Type", 255);
            Wt::Dbo::hasMany(a, m_assets, Wt::Dbo::ManyToOne, "Asset");//create a ManyToOne relationship to the table "project_asset_type"

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        std::string m_type;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>> m_assets;//assets using this type

        //functions
        void  init();
    };

    class ProjectWorkStatus : public Ms::Dbo::MDboBase
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        ProjectWorkStatus();
        explicit ProjectWorkStatus(const std::string &status);

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
            Wt::Dbo::id(a, m_status, "Status", 255);
            Wt::Dbo::belongsTo(a, m_workStatusType, "Belongs_To");//create a ManyToOne relationship to the table "project_work_status_type"
            Wt::Dbo::hasMany(a, m_sequences, Wt::Dbo::ManyToOne, "Current");//create a ManyToOne relationship to the table "project_sequence"
            Wt::Dbo::hasMany(a, m_tasks, Wt::Dbo::ManyToOne, "Current");//create a ManyToOne relationship to the table "project_task"
            Wt::Dbo::hasMany(a, m_assets, Wt::Dbo::ManyToOne, "Current");//create a ManyToOne relationship to the table "project_asset"
            Wt::Dbo::hasMany(a, m_shots, Wt::Dbo::ManyToOne, "Current");//create a ManyToOne relationship to the table "project_shot"
            Wt::Dbo::hasMany(a, m_projects, Wt::Dbo::ManyToOne, "Current");//create a ManyToOne relationship to the table "project"

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        std::string m_status;
        Wt::Dbo::ptr<Projects::ProjectWorkStatusType> m_workStatusType;//WorkStatusType this belongs to
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>> m_sequences;//sequences using this type
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> m_tasks;//tasks using this type
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>> m_assets;//assets using this type
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>> m_shots;//shots using this type
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::Project>> m_projects;//projects using this type

        //functions
        void init();
    };

    class ProjectSequence : public ProjectDbo
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        ProjectSequence();
        explicit ProjectSequence(const std::string &sequenceName);

        //variables

        //functions
        virtual int progress() const override;
        virtual int totalHours() const override;
        virtual int doneHours() const override;

        int totalShots() const;
        int doneShots() const;
        int totalTasks() const;
        int doneTasks() const;
        int totalActivities() const;
        int doneActivities() const;

        ProjectSequence *modify() override;
        const ProjectSequenceId id() const;
        std::string name() const;
        void setName(const std::string &name);
        std::string projectName() const;
        Wt::Dbo::ptr<Projects::Project> project() const;
        void setProject(Wt::Dbo::ptr<Projects::Project> project);
        bool hasShot(Wt::Dbo::ptr<Projects::ProjectShot> shot) const;
        bool addShot(Wt::Dbo::ptr<Projects::ProjectShot> shot);
        bool removeShot(Wt::Dbo::ptr<Projects::ProjectShot> shot);
        int durationInFrames() const;
        void setDurationInFrames(int durationInFrames);
        float fps() const;
        void setFps(float fps);
        int frameWidth() const;
        void setFrameWidth(int frameWidth);
        int frameHeight() const;
        void setFrameHeight(int frameHeight);
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>> shots() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> tasks() const;

        //operators
        bool operator ==(const ProjectSequence &other) const;
        bool operator !=(const ProjectSequence &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, m_id, "Sequence_", 255);
            Wt::Dbo::field(a, m_durationInFrames, "Duration_In_Frames");
            Wt::Dbo::field(a, m_fps, "FPS");
            Wt::Dbo::field(a, m_frameWidth, "Frame_Width");
            Wt::Dbo::field(a, m_frameHeight, "Frame_Height");
            Wt::Dbo::belongsTo(a, m_status, "Current");//create a ManyToOne relationship to the table "project_work_status"
            Wt::Dbo::hasMany(a, m_shots, Wt::Dbo::ManyToOne, "Shot_Sequence");//create a ManyToOne relationship to the table "project_shot"
            Wt::Dbo::hasMany(a, m_tasks, Wt::Dbo::ManyToOne, "Task_Sequence");//create a ManyToOne relationship to the table "project_task"
            Wt::Dbo::hasMany(a, m_data, Wt::Dbo::ManyToOne, "Project_Sequence");
            Wt::Dbo::hasMany(a, m_notes, Wt::Dbo::ManyToOne, "Project_Sequence");
            Wt::Dbo::hasMany(a, m_tags, Wt::Dbo::ManyToMany, "rel_project_sequence_tags");
            Wt::Dbo::hasMany(a, m_assignedTags, Wt::Dbo::ManyToMany, "rel_project_sequence_assigned_tags");//create a ManyToMany relationship to the table "tag"

            ProjectDbo::persist<Action>(a);
        }

    private:
        //functions
        ProjectSequenceId m_id;
        int m_durationInFrames;
        float m_fps;
        int m_frameWidth;
        int m_frameHeight;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectShot>> m_shots;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> m_tasks;

        //variables
        void init();
    };

    class ProjectTask : public ProjectDbo
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        ProjectTask();

        //variables
        virtual int progress() const override;
        virtual int totalHours() const override;
        virtual int doneHours() const override;

        Wt::Dbo::ptr<Projects::ProjectTaskType> type() const;
        void setType(Wt::Dbo::ptr<Projects::ProjectTaskType> type);
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        void setStatus(Wt::Dbo::ptr<Projects::ProjectWorkStatus> status);
        Wt::Dbo::ptr<Users::User> user() const;
        void setUser(Wt::Dbo::ptr<Users::User> user);
        Wt::Dbo::ptr<Projects::Project> project() const;
        void setProject(Wt::Dbo::ptr<Projects::Project> project);
        Wt::Dbo::ptr<Projects::ProjectSequence> sequence() const;
        void setSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence);
        Wt::Dbo::ptr<Projects::ProjectShot> shot() const;
        void setShot(Wt::Dbo::ptr<Projects::ProjectShot> shot);
        const Wt::Dbo::ptr<Projects::ProjectAsset> asset() const;
        void setAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset);
        bool hasActivity(Wt::Dbo::ptr<Projects::ProjectTaskActivity> activity) const;
        bool addActivity(Wt::Dbo::ptr<ProjectTaskActivity> activity);
        bool removeActivity(Wt::Dbo::ptr<ProjectTaskActivity> activity);
        bool isAcceptedByUser();
        void setAcceptedByUser(bool accepted);

        int totalActivities() const;
        int doneActivities() const;

        //functions
        ProjectTask *modify() override;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::field(a, m_acceptedByUser, "Accepted_By_User");
            Wt::Dbo::belongsTo(a, m_status, "Current");//create a ManyToOne relationship to the table "project_work_status"
            Wt::Dbo::belongsTo(a, m_type, "Task");//create a ManyToOne relationship to the table "project_task_type"
            Wt::Dbo::belongsTo(a, m_user, "Task_User");//create a ManyToOne relationship to the table "user"
            Wt::Dbo::belongsTo(a, m_shot, "Task_Shot");//create a ManyToOne relationship to the table "project_shot_task_type"
            Wt::Dbo::belongsTo(a, m_asset, "Task_Asset");//create a ManyToOne relationship to the table "project_asset"
            Wt::Dbo::belongsTo(a, m_sequence, "Task_Sequence");//create a ManyToOne relationship to the table "project_sequence"
            Wt::Dbo::belongsTo(a, m_project, "Task_Project");//create a ManyToOne relationship to the table "project"
            Wt::Dbo::hasMany(a, m_activities, Wt::Dbo::ManyToOne, "Project_Task");
            Wt::Dbo::hasMany(a, m_data, Wt::Dbo::ManyToOne, "Project_Task");
            Wt::Dbo::hasMany(a, m_notes, Wt::Dbo::ManyToOne, "Project_Task");
            Wt::Dbo::hasMany(a, m_tags, Wt::Dbo::ManyToMany, "rel_project_task_tags");//create a ManyToMany relationship to the table "tag"
            Wt::Dbo::hasMany(a, m_assignedTags, Wt::Dbo::ManyToMany, "rel_project_task_assigned_tags");//create a ManyToMany relationship to the table "tag"

            ProjectDbo::persist<Action>(a);
        }

    private:
        //variables
        Wt::Dbo::ptr<Projects::ProjectTaskType> m_type;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> m_status;
        Wt::Dbo::ptr<Users::User> m_user;
        Wt::Dbo::ptr<Projects::ProjectShot> m_shot;
        Wt::Dbo::ptr<Projects::ProjectAsset> m_asset;
        Wt::Dbo::ptr<Projects::ProjectSequence> m_sequence;
        Wt::Dbo::ptr<Projects::Project> m_project;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTaskActivity>> m_activities;
        bool m_acceptedByUser;

        //functions
        void init();
    };

    class ProjectTaskType : public Ms::Dbo::MDboBase
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        ProjectTaskType();
        explicit ProjectTaskType(const std::string &type);

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
            Wt::Dbo::id(a, m_type, "Type", 255);
            Wt::Dbo::hasMany(a, m_tasks, Wt::Dbo::ManyToOne, "Task");//create a ManyToOne relationship to the table "project_shot_task"

            Ms::Dbo::MDboBase::persist<Action>(a);
        }

    private:
        //variables
        std::string m_type;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> m_tasks;//tasks using this type

        //functions
        void init();
    };

    class ProjectShot : public ProjectDbo
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        ProjectShot();
        explicit ProjectShot(const std::string &shotName);

        //variables

        //functions
        virtual int progress() const override;
        virtual int totalHours() const override;
        virtual int doneHours() const override;

        int totalTasks() const;
        int doneTasks() const;
        int totalActivities() const;
        int doneActivities() const;

        ProjectShot *modify() override;
        const ProjectShotId id() const;
        std::string name() const;
        void setName(const std::string &name);
        std::string sequenceName() const;
        Wt::Dbo::ptr<Projects::ProjectSequence> sequence() const;
        void setSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence);
        std::string projectName() const;
        Wt::Dbo::ptr<Projects::Project> project() const;
        bool hasTask(Wt::Dbo::ptr<Projects::ProjectTask> task) const;
        bool addTask(Wt::Dbo::ptr<Projects::ProjectTask> task);
        bool removeTask(Wt::Dbo::ptr<Projects::ProjectTask> task); 
        int durationInFrames() const;
        void setDurationInFrames(int durationInFrames);
        float fps() const;
        void setFps(float fps);
        int frameWidth() const;
        void setFrameWidth(int frameWidth);
        int frameHeight() const;
        void setFrameHeight(int frameHeight);
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> tasks() const;

        //operators
        bool operator ==(const ProjectShot &other) const;
        bool operator !=(const ProjectShot &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {

            Wt::Dbo::id(a, m_id, "Shot_", 255);
            Wt::Dbo::field(a, m_durationInFrames, "Duration_In_Frames");
            Wt::Dbo::field(a, m_fps, "FPS");
            Wt::Dbo::field(a, m_frameWidth, "Frame_Width");
            Wt::Dbo::field(a, m_frameHeight, "Frame_Height");
            Wt::Dbo::belongsTo(a, m_status, "Current");//create a ManyToOne relationship to the table "project_work_status"
            Wt::Dbo::hasMany(a, m_tasks, Wt::Dbo::ManyToOne, "Task_Shot");//create a ManyToOne relationship to the table "project_task"
            Wt::Dbo::hasMany(a, m_data, Wt::Dbo::ManyToOne, "Project_Shot");//create a ManyToOne relationship to the table "data"
            Wt::Dbo::hasMany(a, m_notes, Wt::Dbo::ManyToOne, "Project_Shot");//create a ManyToOne relationship to the table "note"
            Wt::Dbo::hasMany(a, m_tags, Wt::Dbo::ManyToMany, "rel_project_shot_tags");//create a ManyToMany relationship to the table "tag"
            Wt::Dbo::hasMany(a, m_assignedTags, Wt::Dbo::ManyToMany, "rel_project_shot_assigned_tags");//create a ManyToMany relationship to the table "tag"

            ProjectDbo::persist<Action>(a);
        }

    private:
        //variables
        ProjectShotId m_id;
        int m_durationInFrames;
        float m_fps;
        int m_frameWidth;
        int m_frameHeight;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> m_tasks;

        //functions
        void init();
    };

    class ProjectAsset : public ProjectDbo
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        ProjectAsset();
        explicit ProjectAsset(const std::string &assetName);

        //variables


        //functions
        virtual int progress() const override;
        virtual int totalHours() const override;
        virtual int doneHours() const override;

        int totalTasks() const;
        int doneTasks() const;
        int totalActivities() const;
        int doneActivities() const;

        ProjectAsset *modify() override;
        const ProjectAssetId id() const;
        const std::string name() const;
        void setName(const std::string &name);
        std::string projectName() const;
        Wt::Dbo::ptr<Projects::Project> project() const;
        void setProject(Wt::Dbo::ptr<Projects::Project> project);
        bool hasTask(Wt::Dbo::ptr<Projects::ProjectTask> task) const;
        bool addTask(Wt::Dbo::ptr<Projects::ProjectTask> task);
        bool removeTask(Wt::Dbo::ptr<Projects::ProjectTask> task);
        Wt::Dbo::ptr<Projects::ProjectAssetType> type() const;
        void setType(const Wt::Dbo::ptr<Projects::ProjectAssetType> type);
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> tasks() const;

        //operators
        bool operator ==(const ProjectAsset &other) const;
        bool operator !=(const ProjectAsset &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, m_id, "Asset_", 255);
            Wt::Dbo::belongsTo(a, m_status, "Current");//create a ManyToOne relationship to the table "project_work_status"
            Wt::Dbo::belongsTo(a, m_type, "Asset");//create a ManyToOne relationship to the table "project_asset_type"
            Wt::Dbo::hasMany(a, m_tasks, Wt::Dbo::ManyToOne, "Task_Asset");//create a ManyToOne relationship to the table "project_task"
            Wt::Dbo::hasMany(a, m_data, Wt::Dbo::ManyToOne, "Project_Asset");//create a ManyToOne relationship to the table "data"
            Wt::Dbo::hasMany(a, m_notes, Wt::Dbo::ManyToOne, "Project_Asset");//create a ManyToOne relationship to the table "note"
            Wt::Dbo::hasMany(a, m_tags, Wt::Dbo::ManyToMany, "rel_project_asset_tags");//create a ManyToMany relationship to the table "tag"
            Wt::Dbo::hasMany(a, m_assignedTags, Wt::Dbo::ManyToMany, "rel_project_asset_assigned_tags");//create a ManyToMany relationship to the table "tag"

            ProjectDbo::persist<Action>(a);
        }

    private:
        //variables
        ProjectAssetId m_id;
        Wt::Dbo::ptr<Projects::ProjectAssetType> m_type;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> m_tasks;

        //functions
        void init();
    };

    class Project : public ProjectDbo
    {
        friend class Projects::ProjectsManager;
        friend class Database::DboSession;

    public:
        Project();
        explicit Project(const std::string &projectName);

        //variables


        //functions
        virtual int progress() const override;
        virtual int totalHours() const override;
        virtual int doneHours() const override;

        int totalAssets() const;
        int doneAssets() const;
        int totalSequences() const;
        int doneSequences() const;
        int totalShots() const;
        int doneShots() const;
        int totalTasks() const;
        int doneTasks() const;
        int totalActivities() const;
        int doneActivities() const;

        Project *modify() override;
        std::string name() const;
        void setName(const std::string &name);
        bool hasSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence) const;
        bool addSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence);
        bool removeSequence(Wt::Dbo::ptr<Projects::ProjectSequence> sequence);
        bool hasAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset) const;
        bool addAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset);
        bool removeAsset(Wt::Dbo::ptr<Projects::ProjectAsset> asset);
        int durationInFrames() const;
        void setDurationInFrames(int durationInFrames);
        float fps() const;
        void setFps(float fps);
        int frameWidth() const;
        void setFrameWidth(int frameWidth);
        int frameHeight() const;
        void setFrameHeight(int frameHeight);
        Wt::Dbo::ptr<Users::User> manager() const;
        void setManager(const Wt::Dbo::ptr<Users::User> user);
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>> sequences() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>> assets() const;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> tasks() const;

        //operators
        bool operator ==(const Project &other) const;
        bool operator !=(const Project &other) const;

        //DBO functions
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::id(a, m_name, "Project_Name", 255);
            Wt::Dbo::field(a, m_durationInFrames, "Duration_In_Frames");
            Wt::Dbo::field(a, m_fps, "FPS");
            Wt::Dbo::field(a, m_frameWidth, "Frame_Width");
            Wt::Dbo::field(a, m_frameHeight, "Frame_Height");
            Wt::Dbo::belongsTo(a, m_status, "Current");//create a ManyToOne relationship to the table "project_work_status"
            Wt::Dbo::belongsTo(a, m_projectManager, "Project_Manager");//create a ManyToOne relationship to the table "users"
            Wt::Dbo::hasMany(a, m_tasks, Wt::Dbo::ManyToOne, "Task_Project");//create a ManyToOne relationship to the table "project_task"
            Wt::Dbo::hasMany(a, m_sequences, Wt::Dbo::ManyToOne, "Sequence_Project");//create a ManyToOne relationship to the table "project_sequence"
            Wt::Dbo::hasMany(a, m_assets, Wt::Dbo::ManyToOne, "Asset_Project");//create a ManyToOne relationship to the table "project_asset"
            Wt::Dbo::hasMany(a, m_data, Wt::Dbo::ManyToOne, "Project");//create a ManyToOne relationship to the table "data"
            Wt::Dbo::hasMany(a, m_notes, Wt::Dbo::ManyToOne, "Project");//create a ManyToOne relationship to the table "note"
            Wt::Dbo::hasMany(a, m_tags, Wt::Dbo::ManyToMany, "rel_project_tags");//create a ManyToMany relationship to the table "tag"
            Wt::Dbo::hasMany(a, m_assignedTags, Wt::Dbo::ManyToMany, "rel_project_assigned_tags");//create a ManyToMany relationship to the table "tag"

            ProjectDbo::persist<Action>(a);
        }

    private:
        //variables
        std::string m_name;
        int m_durationInFrames;
        float m_fps;
        int m_frameWidth;
        int m_frameHeight;
        Wt::Dbo::ptr<Users::User> m_projectManager;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectSequence>> m_sequences;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectAsset>> m_assets;
        Wt::Dbo::collection<Wt::Dbo::ptr<Projects::ProjectTask>> m_tasks;

        //functions
        void init();
    };
}


#endif // DBTABLES_H
