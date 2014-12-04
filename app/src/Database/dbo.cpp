#include "dbtables.h"
#include "../Session/sessionmanager.h"

#include <Ms/Core/Dbo/MDboSession.h>

Database::Dbo::Dbo() :
    Ms::Dbo::MDboBase()
{
}

bool Database::Dbo::hasData(Wt::Dbo::ptr<Database::DboData> data) const
{
    bool result = false;

    for(auto iter = data_.begin(); iter != data_.end(); ++iter)
    {
        if((*iter).id() == data.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Database::Dbo::addData(Wt::Dbo::ptr<Database::DboData> data)
{
    if(!hasData(data))
    {
        data_.insert(data);

        return true;
    }

    return false;
}

bool Database::Dbo::removeData(Wt::Dbo::ptr<Database::DboData> data)
{
    if(hasData(data))
    {
        data_.erase(data);

        return true;
    }

    return false;
}

bool Database::Dbo::hasTag(Wt::Dbo::ptr<Database::Tag> tag) const
{
    bool result = false;

    for(auto iter = tags_.begin(); iter != tags_.end(); ++iter)
    {
        if((*iter).id() == tag.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Database::Dbo::addTag(Wt::Dbo::ptr<Database::Tag> tag)
{
    if(!hasTag(tag))
    {
        tags_.insert(tag);

        return true;
    }

    return false;
}

bool Database::Dbo::removeTag(Wt::Dbo::ptr<Database::Tag> tag)
{
    if(hasTag(tag))
    {
        tags_.erase(tag);

        return true;
    }

    return false;
}

bool Database::Dbo::hasTagAssigned(Wt::Dbo::ptr<Database::Tag> tag) const
{
    bool result = false;

    for(auto iter = assignedTags_.begin(); iter != assignedTags_.end(); ++iter)
    {
        if((*iter).id() == tag.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Database::Dbo::assignTag(Wt::Dbo::ptr<Database::Tag> tag)
{
    if(!hasTagAssigned(tag))
    {
        assignedTags_.insert(tag);

        return true;
    }

    return false;
}

bool Database::Dbo::unassignTag(Wt::Dbo::ptr<Database::Tag> tag)
{
    if(hasTagAssigned(tag))
    {
        assignedTags_.erase(tag);

        return true;
    }

    return false;
}

bool Database::Dbo::hasNote(Wt::Dbo::ptr<Database::Note> note) const
{
    bool result = false;

    for(auto iter = notes_.begin(); iter != notes_.end(); ++iter)
    {
        if((*iter).id() == note.id())
        {
            result = true;

            break;
        }
    }

    return result;
}

bool Database::Dbo::addNote(Wt::Dbo::ptr<Database::Note> note)
{
    if(!hasNote(note))
    {
        notes_.insert(note);

        return true;
    }

    return false;
}

bool Database::Dbo::removeNote(Wt::Dbo::ptr<Database::Note> note)
{
    if(hasNote(note))
    {
        notes_.erase(note);

        return true;
    }

    return false;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>> Database::Dbo::data()
{
    return data_;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>> Database::Dbo::tags()
{
    return tags_;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>> Database::Dbo::notes()
{
    return notes_;
}

Database::Dbo *Database::Dbo::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}
