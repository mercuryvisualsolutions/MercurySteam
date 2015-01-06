#ifndef MNULLPOINTEREXCEPTION_H
#define MNULLPOINTEREXCEPTION_H

#include <iostream>

namespace Ms
{
    namespace Exceptions
    {
        class MNullPointerException : public std::exception
        {
        public:
            MNullPointerException() throw();
            MNullPointerException(const std::string &message) throw();
            virtual ~MNullPointerException() throw();

            // exception interface
        public:
            virtual const char *what() throw();

        private:
            std::string m_message;
        };
    }
}

#endif // MNULLPOINTEREXCEPTION_H
