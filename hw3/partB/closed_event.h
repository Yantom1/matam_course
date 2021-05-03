#ifndef CLOSED_EVENT_H
#define CLOSED_EVENT_H
#include "../partA/date_wrap.h"
#include "base_event.h"
#include <string>
#include "linked_list.h"


namespace mtm
{
    class ClosedEvent: public BaseEvent
    {
        List<int,CompareInt> allowed_participants; // A list to hold the invited students
        public:
        explicit ClosedEvent(DateWrap date, string name);
        ClosedEvent(const ClosedEvent&);
        ClosedEvent& operator=(const ClosedEvent&);
        ~ClosedEvent() = default;

        void addInvitee(int student);
        void registerParticipant(int student) override;
        BaseEvent* clone() const override;
        List<int,CompareInt> getInvitees() const; // getter for allowed participants
        bool isInvited(int student); // returns true/false whether a student is invited
    };
}
#endif