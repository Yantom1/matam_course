#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "event.h"

// Struct Decleration

struct Event_t{
    int event_id;
    char* event_name;
    PriorityQueue member_queue;
    Date event_date;
};

// Function Decleration

static PQElement copyMember(PQElement member);
static bool equalMembers(PQElement member1, PQElement member2);
static void destroyMember(PQElement member);
static PQElementPriority copyID(PQElementPriority ID);
static int compareID(PQElementPriority ID1, PQElementPriority ID2);
static void destroyID(PQElementPriority ID);


// Function Implementations

// General functions to use in PQ
static PQElement copyMember(PQElement member)
{
    return memberCopy(member);
}

static bool equalMembers(PQElement member1, PQElement member2)
{
    return memberEqual(member1, member2);
}

static void destroyMember(PQElement member)
{
    memberDestroy(member);
}

static PQElementPriority copyID(PQElementPriority ID)
{
    if(!ID)
    {
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if(!copy)
    {
        return NULL;
    }

    *copy = *(int*)ID;
    return copy;
}

static int compareID(PQElementPriority ID1, PQElementPriority ID2)
{
    return (*(int *)ID2 - *(int *)ID1);
}

static void destroyID(PQElementPriority ID)
{
    free(ID);
}

// Main functions

Event eventCreate(char* event_name, int event_id, Date date)
{
    assert(event_name != NULL  || date != NULL || event_id >= 0);
 
    Event event = malloc(sizeof(*event));
    if(event == NULL){
        return NULL;
    }
    
    event->event_date = dateCopy(date);
    if(event->event_date == NULL){
        free(event);
        return NULL;
    }

    event->event_id = event_id;

    char* event_name_copy = malloc(sizeof(char)*(strlen(event_name) + 1));
    if(event_name_copy == NULL){
        free(event->event_date);
        free(event);
        return NULL;
    }
    strcpy(event_name_copy,event_name);
    event->event_name = event_name_copy;

    PriorityQueue member_queue = pqCreate(copyMember, destroyMember, equalMembers, copyID, destroyID, compareID);
    if(member_queue == NULL){
        free(event->event_date);
        free(event->event_name);
        free(event);
        return NULL;
    }
    event->member_queue = member_queue;
    return event;
}

void eventDestroy(Event event)
{
    assert(event != NULL);

    free(event->event_name);
    dateDestroy(event->event_date);
    pqDestroy(event->member_queue);
    free(event);
}

Event eventCopy(Event event)
{
    assert(event != NULL);

    PriorityQueue member_queue_copy = pqCopy(event->member_queue);
    if(member_queue_copy == NULL)
    {
        
        return NULL;
    }

    Event event_copy = eventCreate(event->event_name, event->event_id, event->event_date);
    if(event_copy == NULL)
    {
        pqDestroy(member_queue_copy);
        return NULL;
    }
    
    pqDestroy(event_copy->member_queue);
    event_copy->member_queue = member_queue_copy;

    return event_copy;
}

bool eventEqual(Event event_1, Event event_2)
{
    assert(event_1 == NULL || event_2 == NULL);

    return event_1->event_id == event_2->event_id;
}

Date eventGetDate(Event event)
{
    assert(event != NULL);

    return event->event_date;
}

char* eventGetName(Event event)
{
    assert(event != NULL);
    return event->event_name;
}

int eventGetID(Event event)
{
    assert(event != NULL);

    return event->event_id;
}

PriorityQueue eventGetMemberQueue(Event event)
{
    assert(event != NULL);

    return event->member_queue;
}

void eventDateChange(Event event, Date date)
{
    dateDestroy(event->event_date);
    Date new_date = dateCopy(date);
    event->event_date = new_date;
}