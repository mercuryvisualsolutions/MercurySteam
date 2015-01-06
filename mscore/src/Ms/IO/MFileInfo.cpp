#include "MFileInfo.h"

#define BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>

#include "IO.h"

Ms::IO::MFileInfo::MFileInfo()
{
}

Ms::IO::MFileInfo::MFileInfo(const std::string &fileName)
{
    m_fullName = fileName;
    extractName();
    extractExtension();
}

const std::string Ms::IO::MFileInfo::name() const
{
    return m_name;
}

const std::string Ms::IO::MFileInfo::fullName() const
{
    return m_fullName;
}

const std::string Ms::IO::MFileInfo::extension() const
{
    return m_extension;
}

u_int64_t Ms::IO::MFileInfo::size() const
{
    return boost::filesystem::file_size(m_fullName);
}

const std::string Ms::IO::MFileInfo::lastModifiedDate() const
{
    time_t rawtime = boost::filesystem::last_write_time(m_fullName);
    char buffer [80];

    strftime(buffer,80,"%Y-%m-%d %I:%M%p",localtime(&rawtime));
    std::string result(buffer);

    return result;
}

void Ms::IO::MFileInfo::extractName()
{
    m_name = m_fullName.substr(m_fullName.find_last_of(Ms::IO::dirSeparator()) + 1, m_fullName.length() -1);
}

void Ms::IO::MFileInfo::extractExtension()
{
    m_extension = m_fullName.substr(m_fullName.find_last_of('.') + 1, m_fullName.length() -1);
}
