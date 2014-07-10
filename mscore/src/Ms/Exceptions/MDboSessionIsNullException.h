#ifndef MDOBSESSIONISNULLEXCEPTION_H
#define MDOBSESSIONISNULLEXCEPTION_H

#include <iostream>

namespace Ms
{
    namespace Exceptions
    {
        class MDboSessionIsNullException : public std::exception
        {
        public:
            MDboSessionIsNullException() throw();
            MDboSessionIsNullException(const std::string &message) throw();
            virtual ~MDboSessionIsNullException() throw();

            // exception interface
        public:
            virtual const char *what() throw();

        private:
            std::string _message;
        };
    }
}

#endif // MDOBSESSIONISNULLEXCEPTION_H
