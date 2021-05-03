#include "schedule.h"
#include <list>
#include "../partB/event_container.h"
#include "../partA/exceptions.h"
#include <algorithm>
using mtm::Schedule;

Schedule::Schedule(): events_list(){}

void Schedule::addEvents(const EventContainer& events)
{
    for(mtm::EventContainer::EventIterator events_iterator = events.begin(); 
                                            events_iterator!= events.end(); ++events_iterator){
        mtm::BaseEvent& current_event = *events_iterator;
        for(mtm::BaseEvent* event : events_list){
            if(event->getName() == current_event.getName() && event->getDate() == current_event.getDate())
                throw EventAlreadyExists();
        }
    }
    mtm::BaseEvent* event_clone;
    for(mtm::EventContainer::EventIterator events_iterator = events.begin();
                                            events_iterator!= events.end(); ++events_iterator){
        mtm::BaseEvent& event = *events_iterator;
        event_clone = event.clone();
        try{
            events_list.push_back(event_clone);
        } catch(std::bad_alloc& e){
            delete event_clone;
            throw e;
        }
    }
    events_list.sort(mtm::CompareEvents());
}

Schedule::~Schedule()
{
    for(mtm::BaseEvent* event : events_list){
        delete event;
    }
}


bool mtm::CompareEvents::operator()(const BaseEvent* event1, const BaseEvent* event2)
{
    if(event1->getDate() < event2->getDate()){
        return true;
    }
    if(event1->getDate() == event2->getDate()){
        if(event1->getName() < event2->getName())
            return true;
    }
    return false;
}

void Schedule::registerToEvent(DateWrap date, string name, int student)
{
    for(mtm::BaseEvent* event : events_list){
        if(event->getName() == name && event->getDate() == date){
            event->registerParticipant(student);
            return;
        }
    }
    throw EventDoesNotExist();
}

void Schedule::unregisterFromEvent(DateWrap date, string name, int student)
{
    for(mtm::BaseEvent* event : events_list){
        if(event->getName() == name && event->getDate() == date){
            event->unregisterParticipant(student);
            return;
        }
    }
    throw EventDoesNotExist();
}

void Schedule::printAllEvents() const
{
    for(mtm::BaseEvent* event : events_list){
        event->printShort(std::cout);
        std::cout<<std::endl;
    }
}



void Schedule::printMonthEvents(int month, int year) const
{
    if(month < min_month || month > max_month){
        throw InvalidNumber();
    }
    for(mtm::BaseEvent* event : events_list){
        DateWrap date = event->getDate();
        if(date.month() == month && date.year() == year){
            event->printShort(std::cout);
            std::cout<<std::endl;
        }
    }
}

void Schedule::printEventDetails(DateWrap date,string name) const
{
    for(mtm::BaseEvent* event : events_list){
        if(event->getName() == name && event->getDate() == date){
            event->printLong(std::cout);
            std::cout<<std::endl;
            break;
        }
    }
}
