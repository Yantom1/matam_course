#include "festival.h"
#include "../partA/exceptions.h"
#include "linked_list.h"

using mtm::Festival;


Festival::Festival(DateWrap date): EventContainer(), date(date){   
}

void Festival::add(const BaseEvent& event)
{
    if(event.getDate() != date){
        throw DateMismatch();
    }
    BaseEvent* event_clone;
    event_clone = event.clone();
    try{
        events.insert(event_clone);
    } catch(std::bad_alloc& e){
        delete event_clone;
        throw e;
    } 
}
