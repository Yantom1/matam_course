#ifndef EVENT_H
#define EVENT_H

#include <stdbool.h>
#include "member.h"
#include "date.h"
#include "priority_queue.h"


typedef struct Event_t *Event;


/*eventCreate- creates a new event by name, id and date. event id should be positive.
               return- NULL if dynamic allocation failed.
                       if succeed returns the new event created */  

Event eventCreate(char* event_name, int event_id, Date date);

/*eventDestroy- destroying the event that sent */

void eventDestroy(Event event);

/*eventCopy- copies the given event to a new one.
           return- NULL if dynamic allocation failed.
                   if succeed returns the copied event created */

Event eventCopy(Event event);

/*eventEqual- compares the given event by their event id
            return- true if same event id
                    false if not same event id */

bool eventEqual(Event event_1, Event event_2);

/*eventGetDate- return the date of the event that sent */

Date eventGetDate(Event event);

/*eventGetName- return the name of the event that sent */

char* eventGetName(Event event);

/*eventGetID- return the ID of the event that sent */

int eventGetID(Event event);

/*eventGetMemberQueue - returning the member queue that in charge on the event that sent */

PriorityQueue eventGetMemberQueue(Event event);

/*eventDateChange - changing the date of the event to the date that nt to the function */

void eventDateChange(Event event, Date date);


#endif