#ifndef GLOBALDBOSPECIALIZATION_H
#define GLOBlDBOSPECIALIZATION_H

#include <iostream>
#include <string>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>

namespace Database
{
    class Tag;
}

//Tag
namespace Database
{
    struct TagId
    {
    public:
        TagId(){}
        TagId(std::string &name, std::string &content) :
            name(name), content(content){}

        //variables
        std::string name;
        std::string content;

        bool operator ==(const TagId &other) const{return name == other.name && content == other.content;}
        bool operator <(const TagId &other) const{return true;}
    };

    inline std::ostream& operator<< (std::ostream& o, const Database::TagId& tagid)
    {
        return o << tagid.name << "," << tagid.content;
    }
}

namespace Wt
{
    namespace Dbo
    {
        //specify how the TagId type is persisted
        template <class Action>
        void field(Action& action, Database::TagId &tagid, const std::string &columnName, int size = -1)
        {
            field(action, tagid.name, columnName + "Name", size);
            field(action, tagid.content, columnName + "Content", size);
        }

        //override the default primary id column for the Tag class to use std::string
        template<>
        struct dbo_traits<Database::Tag> : public dbo_default_traits
        {
            typedef Database::TagId IdType;
            static IdType invalidId() { return Database::TagId(); }
            static const char *surrogateIdField() { return 0; }
        };
    }
}

#endif // GLOBALDBOSPECIALIZATION_H
