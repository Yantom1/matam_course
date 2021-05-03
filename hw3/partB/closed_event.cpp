#include "base_event.h"
#include "../partA/exceptions.h"
#include "closed_event.h"
#include "linked_list.h"


using mtm::BaseEvent;
using mtm::ClosedEvent;
using mtm::List;


ClosedEvent::ClosedEvent(DateWrap date, string name):BaseEvent(date, name), allowed_participants(CompareInt()){
}

ClosedEvent::ClosedEvent(const ClosedEvent& event): BaseEvent(event), allowed_participants(event.allowed_participants){
}

ClosedEvent& ClosedEvent::operator=(const ClosedEvent& event){
    if(this == &event){
        return *this;
    }
    name = event.name;
    date = event.date;
    participants = event.participants;
    allowed_participants = event.allowed_participants;
    return *this;
}

void ClosedEvent::addInvitee(int student){
    if(student < min_id || student > max_id){
        throw InvalidStudent();
    }
    if(allowed_participants.exists(student)){
       throw AlreadyInvited();
    }
    allowed_participants.insert(student);
}

void ClosedEvent::registerParticipant(int student){
    if(student < min_id || student > max_id){
        throw InvalidStudent();
    }
    if(participants.exists(student)){
        throw AlreadyRegistered();
    }
    if(allowed_participants.exists(student)){
        participants.insert(student); 
    }else{ 
        throw RegistrationBlocked();
    }
}

BaseEvent* ClosedEvent::clone() const{
    return new ClosedEvent(*this);
}

List<int,BaseEvent::CompareInt> ClosedEvent::getInvitees() const{
    return allowed_participants;
}

bool ClosedEvent::isInvited(int student){
    return allowed_participants.exists(student);
}