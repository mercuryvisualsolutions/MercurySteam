#ifndef USERSIO_H
#define USERSIO_H

#include <iostream>

namespace Users
{
    class UsersIO
    {
    public:
        UsersIO();

        //creates the default directory structure for user userName
        static bool createUserDirectoryStructure(const std::string &userName);
        //return the absolute path for a user userName, but never checks if directory exist or not
        static std::string getAbsoluteUserDir(const std::string &userName);
        //return the path for a user userName relative to current directory, but never checks if directory exist or not
        static std::string getRelativeUserDir(const std::string &userName);
    };
}

#endif // USERSIO_H
