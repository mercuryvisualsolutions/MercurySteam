#ifndef DBOSESSION_IMPL_H
#define DBOSESSION_IMPL_H

#include <Ms/Exceptions/MNullPointerException.h>

#include "../Log/logmanager.h"

//GLOBAL
template<typename T>
bool Database::DboSession::dboHasData(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::DboData> dboData)
{
    if(!dbo)
        throw std::invalid_argument("Can't check data existence in dbo!, dbo object is null");

    if(!dboData)
        throw std::invalid_argument("Can't check data existence in dbo, data object is null");

    try
    {
        Wt::Dbo::Transaction transaction(*this);

        bool result = false;

        for(auto iter = dbo->data_.begin(); iter != dbo->data_.end(); ++iter)
        {
            if((*iter).id() == dboData.id())
            {
                result = true;

                break;
            }
        }

        transaction.commit();

        return result;
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while enumerating through dbo data"), Ms::Log::LogMessageType::Error);
    }

    return false;
}

template<typename T>
bool Database::DboSession::addDataToDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::DboData> dboData)
{
    if(!dbo)
        throw std::invalid_argument("Can't add data to dbo!, dbo object is null");

    if(!dboData)
        throw std::invalid_argument("Can't add data to dbo, data object is null");

    try
    {
        if(!dboHasData(dbo, dboData))
        {
            Wt::Dbo::Transaction transaction(*this);

            modifyDbo<T>(dbo)->data_.insert(dboData);

            transaction.commit();

            return true;
        }
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to add data to dbo"), Ms::Log::LogMessageType::Error);
    }

    return false;
}

template<typename T>
bool Database::DboSession::removeDataFromDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::DboData> dboData)
{
    if(!dbo)
        throw std::invalid_argument("Can't remove data from dbo!, dbo object is null");

    if(!dboData)
        throw std::invalid_argument("Can't remove data from dbo, data object is null");

    try
    {
        if(dboHasData(dbo, dboData))
        {
            Wt::Dbo::Transaction transaction(*this);

            modifyDbo<T>(dbo)->data_.erase(dboData);

            transaction.commit();

            return true;
        }
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to remove data from dbo"), Ms::Log::LogMessageType::Error);
    }

    return false;
}

template<typename T>
bool Database::DboSession::dboHasNote(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Note> note)
{
    if(!dbo)
        throw std::invalid_argument("Can't check note existence in dbo!, dbo object is null");

    if(!note)
        throw std::invalid_argument("Can't check note existence in dbo, data object is null");

    try
    {
        Wt::Dbo::Transaction transaction(*this);

        bool result = false;

        for(auto iter = dbo->notes_.begin(); iter != dbo->notes_.end(); ++iter)
        {
            if((*iter).id() == note.id())
            {
                result = true;

                break;
            }
        }

        transaction.commit();

        return result;
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while enumerating through dbo notes"), Ms::Log::LogMessageType::Error);
    }

    return false;
}

template<typename T>
bool Database::DboSession::addNoteToDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Note> note)
{
    if(!dbo)
        throw std::invalid_argument("Can't add note to dbo!, dbo object is null");

    if(!note)
        throw std::invalid_argument("Can't add note to dbo, data object is null");

    try
    {
        if(!dboHasNote(dbo, note))
        {
            Wt::Dbo::Transaction transaction(*this);

            modifyDbo<T>(dbo)->notes_.insert(note);

            transaction.commit();

            return true;
        }
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to add note to dbo"), Ms::Log::LogMessageType::Error);
    }

    return false;
}

template<typename T>
bool Database::DboSession::removeNoteFromDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Note> note)
{
    if(!dbo)
        throw std::invalid_argument("Can't remove note from dbo!, dbo object is null");

    if(!note)
        throw std::invalid_argument("Can't remove note from dbo, data object is null");

    try
    {
        if(dboHasNote(dbo, note))
        {
            Wt::Dbo::Transaction transaction(*this);

            modifyDbo<T>(dbo)->notes_.erase(note);

            transaction.commit();

            return true;
        }
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to remove note from dbo"), Ms::Log::LogMessageType::Error);
    }

    return false;
}

template<typename T>
bool Database::DboSession::dboHasTag(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Tag> tag)
{
    if(!dbo)
        throw std::invalid_argument("Can't check tag existence in dbo!, dbo object is null");

    if(!tag)
        throw std::invalid_argument("Can't check tag existence in dbo, tag object is null");

    try
    {
        Wt::Dbo::Transaction transaction(*this);

        bool result = false;

        for(auto iter = dbo->tags_.begin(); iter != dbo->tags_.end(); ++iter)
        {
            if((*iter).id() == tag.id())
            {
                result = true;

                break;
            }
        }

        transaction.commit();

        return result;
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while enumerating through dbo tags"), Ms::Log::LogMessageType::Error);
    }

    return false;
}

template<typename T>
bool Database::DboSession::addTagToDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Tag> tag)
{
    if(!dbo)
        throw std::invalid_argument("Can't add tag to dbo!, dbo object is null");

    if(!tag)
        throw std::invalid_argument("Can't add tag to dbo, tag object is null");

    try
    {
        if(!dboHasTag(dbo, tag))
        {
            Wt::Dbo::Transaction transaction(*this);

            modifyDbo<T>(dbo)->tags_.insert(tag);

            transaction.commit();

            return true;
        }
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to add tag to dbo"), Ms::Log::LogMessageType::Error);
    }

    return false;
}

template<typename T>
bool Database::DboSession::removeTagFromDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Tag> tag)
{
    if(!dbo)
        throw std::invalid_argument("Can't remove tag from dbo!, dbo object is null");

    if(!tag)
        throw std::invalid_argument("Can't remove tag from dbo, tag object is null");

    try
    {
        if(dboHasTag(dbo, tag))
        {
            Wt::Dbo::Transaction transaction(*this);

            modifyDbo<T>(dbo)->tags_.erase(tag);

            transaction.commit();

            return true;
        }
    }
    catch(...)
    {
        Log::LogManager::instance().getGlobalLogger()->log(std::string("Error occured while trying to remove tag from dbo"), Ms::Log::LogMessageType::Error);
    }

    return false;
}

template<typename T>
void Database::DboSession::assignTagsToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    if(dboVec.size() > 0)
    {
        Wt::Dbo::Transaction transaction(*this);

        for(auto &dboPtr : dboVec)
        {
            for(auto &tagPtr : tagVec)
            {
                modifyDbo<T>(dboPtr)->assignTag(tagPtr);
            }
        }

        transaction.commit();
    }
}

template<typename T>
void Database::DboSession::unassignTagsFromDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    if(dboVec.size() > 0)
    {
        Wt::Dbo::Transaction transaction(*this);

        for(auto &dboPtr : dboVec)
        {
            for(auto &tagPtr : tagVec)
            {
                modifyDbo<T>(dboPtr)->unassignTag(tagPtr);
            }
        }

        transaction.commit();
    }
}

template<typename T>
std::vector<std::string> Database::DboSession::getDboQueryIdValue(const Wt::Dbo::ptr<T> &dbo) const
{
    std::vector<std::string> result;
    std::stringstream stream;
    stream << dbo.id();
    std::string str = "";

    while(std::getline(stream, str, ','))
    {
        result.push_back("'" + str + "'");
    }

    return result;
}

template<typename T>
std::vector<std::string> Database::DboSession::getDboQueryIdValues(const std::vector<Wt::Dbo::ptr<T>> &dboVec) const
{
    std::vector<std::string> result;

    for(std::vector<std::string>::size_type i = 0; i < dboVec.size(); ++i)
    {
        std::vector<std::string> vecId = getDboQueryIdValue<T>(dboVec.at(i));

        for(std::vector<std::string>::size_type x = 0; x < vecId.size(); ++x)
        {
            if(result.size() < x + 1)
                result.push_back(std::string());

            result.at(x) += vecId.at(x);

            if(i < dboVec.size() - 1)
                result.at(x) += ",";
        }
    }

    return result;
}

#endif // DBOSESSION_IMPL_H
