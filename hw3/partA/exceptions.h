#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>
#include <new>

namespace mtm{
    class Exception: public std::exception{};

    class InvalidDate: public Exception{};
    class NegativeDays: public Exception{};
    class AlreadyRegistered: public Exception{};
    class InvalidStudent: public Exception{};
    class NotRegistered: public Exception{};
    class AlreadyInvited: public Exception{};
    class RegistrationBlocked: public Exception{};
    class OutOfBoundaries: public Exception{};
    class DateMismatch: public Exception{};
    class InvalidNumber: public Exception{};
    class InvalidInterval: public Exception{};
    class NotSupported: public Exception{};
    class EventAlreadyExists: public Exception{};
    class EventDoesNotExist: public Exception{};
}
#endif 