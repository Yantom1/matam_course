#include "event_container.h"
#include "linked_list.h"
#include "../partA/exceptions.h"
#include "base_event.h"

using mtm::EventContainer;

using mtm::EventContainer;


EventContainer::EventIterator::EventIterator(List<BaseEvent*, EventContainer::CompareEvents> events, int index)
                                                                                    :index(index), events(events){ 
}

EventContainer::EventIterator::EventIterator(const EventIterator& event_iterator)
                                                        :events(EventContainer::CompareEvents())
{
    index = event_iterator.index;
    events = event_iterator.events;
}

EventContainer::EventIterator& EventContainer::EventIterator::operator=(const EventIterator& event_iterator)
{
    if(this == &event_iterator){
        return *this;
    }
    index = event_iterator.index;
    events = event_iterator.events;
    return *this;
}

mtm::BaseEvent& EventContainer::EventIterator::operator*()
{
    return *events[index];
}

EventContainer::EventIterator& EventContainer::EventIterator::operator++()
{
    index++;
    return *this;
}

EventContainer::EventContainer(): events(EventContainer::CompareEvents()) {
}

bool EventContainer::EventIterator::operator==(const EventIterator& event_iterator) const
{
    if(index == events.getSize() && event_iterator.events.getSize() == event_iterator.index){
        return true;
    }
    if(index == events.getSize() || event_iterator.events.getSize()  == event_iterator.index){
        return false;
    }
    if(events[index] == event_iterator.events[index]){
        return true;
    }
    return false;
}

bool EventContainer::EventIterator::operator!=(const EventIterator& event_iterator) const
{
    return !(*this == event_iterator);
}
   
EventContainer::EventIterator EventContainer::begin() const
{
    return EventIterator(events, 0);
}

EventContainer::EventIterator EventContainer::end() const
{
    return EventIterator(events, events.getSize());
}

EventContainer::~EventContainer()
{
    for(int i = 0; i < events.getSize(); i++){
        delete events[i];
    }
}

