#ifndef USERDBOSPECIALIZATION_H
#define USERDBOSPECIALIZATION_H

#include <iostream>
#include <string>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>

namespace Users
{
    class Privilege;
    class Group;
    class UserTitle;
    class User;
}

namespace Wt
{
    namespace Dbo
    {
        //override the default primary id column for the Group class to use std::string
         template<>
         struct dbo_traits<Users::Group> : public dbo_default_traits
        {
            typedef std::string IdType;
            static IdType invalidId() { return std::string(); }
            static const char *surrogateIdField() { return 0; }
        };

        //override the default primary id column for the Privilege class to use std::string
        template<>
        struct dbo_traits<Users::Privilege> : public dbo_default_traits
        {
            typedef std::string IdType;
            static IdType invalidId() { return std::string(); }
            static const char *surrogateIdField() { return 0; }
        };

        //override the default primary id column for the UsersTitle class to use std::string
        template<>
        struct dbo_traits<Users::UserTitle> : public dbo_default_traits
        {
            typedef std::string IdType;
            static IdType invalidId() { return std::string(); }
            static const char *surrogateIdField() { return 0; }
        };

        //override the default primary id column for the Users class to use std::string
        template<>
        struct dbo_traits<Users::User> : public dbo_default_traits
        {
            typedef std::string IdType;
            static IdType invalidId() { return std::string(); }
            static const char *surrogateIdField() { return 0; }
        };
    }
}

#endif // USERDBOSPECIALIZATION_H
