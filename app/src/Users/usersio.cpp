#include "usersio.h"
#include "../Settings/appsettings.h"

#include <Ms/IO/IO.h>

Users::UsersIO::UsersIO()
{
}

bool Users::UsersIO::createUserDirectoryStructure(const std::string &userName)
{
    bool result;
    std::string path = AppSettings::instance().docRoot() + Ms::IO::dirSeparator() +
            AppSettings::instance().dataDir() + Ms::IO::dirSeparator() + "users" +
            Ms::IO::dirSeparator() + userName;

    path = Ms::IO::absolutePath(path);

    result =  Ms::IO::createPath(path);
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "thumbnails");
    result =  Ms::IO::createPath(path + Ms::IO::dirSeparator() + "tmp");

    return result;
}

std::string Users::UsersIO::getAbsoluteUserDir(const std::string &userName)
{
    std::string dir = Ms::IO::absolutePath(AppSettings::instance().docRoot() + Ms::IO::dirSeparator() +
            AppSettings::instance().dataDir() + Ms::IO::dirSeparator() + "users" +
                                              Ms::IO::dirSeparator() + userName);
    if(!Ms::IO::dirExists(dir))
        createUserDirectoryStructure(userName);

    return dir;
}

std::string Users::UsersIO::getRelativeUserDir(const std::string &userName)
{
    getAbsoluteUserDir(userName);
    return AppSettings::instance().dataDir() + Ms::IO::dirSeparator() + "users" + Ms::IO::dirSeparator() + userName;
}

std::string Users::UsersIO::getUserTempDir(const std::string &userName)
{
    return getAbsoluteUserDir(userName) + Ms::IO::dirSeparator() + "tmp";
}
