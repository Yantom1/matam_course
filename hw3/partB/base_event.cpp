#include <iostream>
#include "base_event.h"
#include "../partA/exceptions.h"
#include <string>
#include "linked_list.h"

using mtm::BaseEvent;
using mtm::List;
using std::ostream;
using std::endl;

BaseEvent::BaseEvent(DateWrap date, string name): date(date), name(name), participants(CompareInt()){
}

BaseEvent::BaseEvent(const BaseEvent& event):  date(event.date), name(event.name), participants(event.participants){
}

BaseEvent& BaseEvent::operator=(const BaseEvent& event){
    if(this == &event){
        return *this;
    }
    name = event.name;
    date = event.date;
    participants = event.participants;
    return *this;
}

void BaseEvent::registerParticipant(int student){
    if(student < min_id || student > max_id){
        throw InvalidStudent();
    }
    if(participants.exists(student)){
        throw AlreadyRegistered();
    }
    participants.insert(student);
}

bool BaseEvent::operator>=(const BaseEvent& event) const{
    return !(*this < event);
}

void BaseEvent::unregisterParticipant(int student){
    if(student < min_id || student > max_id){
        throw InvalidStudent();
    }
    if(!participants.exists(student)){
        throw NotRegistered();
    }
    participants.remove(student);
} 

bool BaseEvent::operator<(const BaseEvent& event) const{
    if(date < event.getDate()){
        return true;
    }
    if(date == event.getDate()){
        if(name < event.getName())
            return true;
    }
    return false;
}

bool BaseEvent::operator==(const BaseEvent& event) const{
    return name == event.getName() && date == event.getDate(); 
}

ostream& BaseEvent::printShort(ostream& os){
    return os << name << " " << date << endl;
}

ostream& BaseEvent::printLong(ostream& os){
    ostream& os_2 = printShort(os);
    for(int i = 0; i < participants.getSize(); i++){
        os_2<< participants[i] << endl; 
    }
    return os_2;
}

DateWrap BaseEvent::getDate() const{
    return date;
}

string BaseEvent::getName() const {
    return name;
}

List<int, BaseEvent::CompareInt> BaseEvent::getParticipants() const{
    return participants;
}

/*
    Normal int comparison
*/
bool BaseEvent::CompareInt::operator()(const int num_1,const int num_2){
    return num_1 < num_2;
}

bool BaseEvent::participantRegistered(int student){
    return participants.exists(student);
}
