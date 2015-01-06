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

    for(auto iter = m_data.begin(); iter != m_data.end(); ++iter)
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
        m_data.insert(data);

        return true;
    }

    return false;
}

bool Database::Dbo::removeData(Wt::Dbo::ptr<Database::DboData> data)
{
    if(hasData(data))
    {
        m_data.erase(data);

        return true;
    }

    return false;
}

bool Database::Dbo::hasTag(Wt::Dbo::ptr<Database::Tag> tag) const
{
    bool result = false;

    for(auto iter = m_tags.begin(); iter != m_tags.end(); ++iter)
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
        m_tags.insert(tag);

        return true;
    }

    return false;
}

bool Database::Dbo::removeTag(Wt::Dbo::ptr<Database::Tag> tag)
{
    if(hasTag(tag))
    {
        m_tags.erase(tag);

        return true;
    }

    return false;
}

bool Database::Dbo::hasTagAssigned(Wt::Dbo::ptr<Database::Tag> tag) const
{
    bool result = false;

    for(auto iter = m_assignedTags.begin(); iter != m_assignedTags.end(); ++iter)
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
        m_assignedTags.insert(tag);

        return true;
    }

    return false;
}

bool Database::Dbo::unassignTag(Wt::Dbo::ptr<Database::Tag> tag)
{
    if(hasTagAssigned(tag))
    {
        m_assignedTags.erase(tag);

        return true;
    }

    return false;
}

bool Database::Dbo::hasNote(Wt::Dbo::ptr<Database::Note> note) const
{
    bool result = false;

    for(auto iter = m_notes.begin(); iter != m_notes.end(); ++iter)
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
        m_notes.insert(note);

        return true;
    }

    return false;
}

bool Database::Dbo::removeNote(Wt::Dbo::ptr<Database::Note> note)
{
    if(hasNote(note))
    {
        m_notes.erase(note);

        return true;
    }

    return false;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::DboData>> Database::Dbo::data()
{
    return m_data;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Tag>> Database::Dbo::tags()
{
    return m_tags;
}

Wt::Dbo::collection<Wt::Dbo::ptr<Database::Note>> Database::Dbo::notes()
{
    return m_notes;
}

Database::Dbo *Database::Dbo::modify()
{
    Ms::Dbo::MDboBase::modify();

    return this;
}
