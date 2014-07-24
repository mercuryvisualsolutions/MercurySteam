#ifndef DATABASEMANAGER_IMPL_H
#define DATABASEMANAGER_IMPL_H

#include <Ms/Exceptions/MNullPointerException.h>

//GLOBAL
template<typename T>
bool Database::DatabaseManager::dboHasData(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::DboData> dboData)
{
    if(!dbo)
        throw std::invalid_argument("Can't check data existence in dbo!, dbo object is null");

    if(!dboData)
        throw std::invalid_argument("Can't check data existence in dbo, data object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MNullPointerException("Can't check data, existence in dbo, DbSession object is null");

    try
    {
        if(Database::DatabaseManager::instance().openTransaction())
        {
            for(auto iter = dbo->data_.begin(); iter != dbo->data_.end(); ++iter)
            {
                if((*iter).id() == dboData.id())
                    return true;
            }
        }
    }
    catch(...)
    {
        _logger->log("Error occured while enumerating through dbo data", Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

template<typename T>
bool Database::DatabaseManager::addDataToDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::DboData> dboData)
{
    if(!dbo)
        throw std::invalid_argument("Can't add data to dbo!, dbo object is null");

    if(!dboData)
        throw std::invalid_argument("Can't add data to dbo, data object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MNullPointerException("Can't add data to dbo, DbSession object is null");

    try
    {
        if(!dboHasData(dbo, dboData))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<T>(dbo)->data_.insert(dboData);
                return true;
            }
        }
    }
    catch(...)
    {
        _logger->log("Error occured while trying to add data to dbo", Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

template<typename T>
bool Database::DatabaseManager::removeDataFromDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::DboData> dboData)
{
    if(!dbo)
        throw std::invalid_argument("Can't remove data from dbo!, dbo object is null");

    if(!dboData)
        throw std::invalid_argument("Can't remove data from dbo, data object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MNullPointerException("Can't remove data from dbo, DbSession object is null");

    try
    {
        if(dboHasData(dbo, dboData))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<T>(dbo)->data_.erase(dboData);
                return true;
            }
        }
    }
    catch(...)
    {
        _logger->log("Error occured while trying to remove data from dbo", Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

template<typename T>
bool Database::DatabaseManager::dboHasNote(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Note> note)
{
    if(!dbo)
        throw std::invalid_argument("Can't check note existence in dbo!, dbo object is null");

    if(!note)
        throw std::invalid_argument("Can't check note existence in dbo, data object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MNullPointerException("Can't check note, existence in dbo, DbSession object is null");

    try
    {
        if(Database::DatabaseManager::instance().openTransaction())
        {
            for(auto iter = dbo->notes_.begin(); iter != dbo->notes_.end(); ++iter)
            {
                if((*iter).id() == note.id())
                    return true;
            }
        }
    }
    catch(...)
    {
        _logger->log("Error occured while enumerating through dbo notes", Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

template<typename T>
bool Database::DatabaseManager::addNoteToDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Note> note)
{
    if(!dbo)
        throw std::invalid_argument("Can't add note to dbo!, dbo object is null");

    if(!note)
        throw std::invalid_argument("Can't add note to dbo, data object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MNullPointerException("Can't add note to dbo, DbSession object is null");

    try
    {
        if(!dboHasNote(dbo, note))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<T>(dbo)->notes_.insert(note);
                return true;
            }
        }
    }
    catch(...)
    {
        _logger->log("Error occured while trying to add note to dbo", Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

template<typename T>
bool Database::DatabaseManager::removeNoteFromDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Note> note)
{
    if(!dbo)
        throw std::invalid_argument("Can't remove note from dbo!, dbo object is null");

    if(!note)
        throw std::invalid_argument("Can't remove note from dbo, data object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MNullPointerException("Can't remove note from dbo, DbSession object is null");

    try
    {
        if(dboHasNote(dbo, note))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<T>(dbo)->notes_.erase(note);
                return true;
            }
        }
    }
    catch(...)
    {
        _logger->log("Error occured while trying to remove note from dbo", Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

template<typename T>
bool Database::DatabaseManager::dboHasTag(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Tag> tag)
{
    if(!dbo)
        throw std::invalid_argument("Can't check tag existence in dbo!, dbo object is null");

    if(!tag)
        throw std::invalid_argument("Can't check tag existence in dbo, tag object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MNullPointerException("Can't check tag, existence in dbo, DbSession object is null");

    try
    {
        if(Database::DatabaseManager::instance().openTransaction())
        {
            for(auto iter = dbo->tags_.begin(); iter != dbo->tags_.end(); ++iter)
            {
                if((*iter).id() == tag.id())
                    return true;
            }
        }
    }
    catch(...)
    {
        _logger->log("Error occured while enumerating through dbo tags", Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

template<typename T>
bool Database::DatabaseManager::addTagToDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Tag> tag)
{
    if(!dbo)
        throw std::invalid_argument("Can't add tag to dbo!, dbo object is null");

    if(!tag)
        throw std::invalid_argument("Can't add tag to dbo, tag object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MNullPointerException("Can't add tag to dbo, DbSession object is null");

    try
    {
        if(!dboHasTag(dbo, tag))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<T>(dbo)->tags_.insert(tag);
                return true;
            }
        }
    }
    catch(...)
    {
        _logger->log("Error occured while trying to add tag to dbo", Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}

template<typename T>
bool Database::DatabaseManager::removeTagFromDbo(Wt::Dbo::ptr<T> dbo, Wt::Dbo::ptr<Database::Tag> tag)
{
    if(!dbo)
        throw std::invalid_argument("Can't remove tag from dbo!, dbo object is null");

    if(!tag)
        throw std::invalid_argument("Can't remove tag from dbo, tag object is null");

    if(!Database::DatabaseManager::instance().session())
        throw Ms::Exceptions::MNullPointerException("Can't remove tag from dbo, DbSession object is null");

    try
    {
        if(dboHasTag(dbo, tag))
        {
            if(Database::DatabaseManager::instance().openTransaction())
            {
                Database::DatabaseManager::instance().modifyDbo<T>(dbo)->tags_.erase(tag);
                return true;
            }
        }
    }
    catch(...)
    {
        _logger->log("Error occured while trying to remove tag from dbo", Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
    }

    return false;
}


template<typename T>
std::vector<std::string> Database::DatabaseManager::getDboQueryIdValue(const Wt::Dbo::ptr<T> &dbo)
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
std::vector<std::string> Database::DatabaseManager::getDboQueryIdValues(const std::vector<Wt::Dbo::ptr<T>> &dboVec)
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

#endif // DATABASEMANAGER_IMPL_H
