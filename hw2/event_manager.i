%module event_manager 
%{
#include "event_manager.h"
#include "date.h"
%}

typedef struct EventManager_t* EventManager;
typedef struct Date_t *Date;

typedef enum EventManagerResult_t {
    EM_SUCCESS,
    EM_OUT_OF_MEMORY,
    EM_NULL_ARGUMENT,
    EM_INVALID_DATE,
    EM_INVALID_EVENT_ID,
    EM_EVENT_ALREADY_EXISTS,
    EM_EVENT_ID_ALREADY_EXISTS,
    EM_EVENT_NOT_EXISTS,
    EM_EVENT_ID_NOT_EXISTS,
    EM_INVALID_MEMBER_ID,
    EM_MEMBER_ID_ALREADY_EXISTS,
    EM_MEMBER_ID_NOT_EXISTS,
    EM_EVENT_AND_MEMBER_ALREADY_LINKED,
    EM_EVENT_AND_MEMBER_NOT_LINKED,
    EM_ERROR
} EventManagerResult;

EventManager createEventManager(Date date);

void destroyEventManager(EventManager em);

EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id);

EventManagerResult emAddEventByDiff(EventManager em, char* eventName, int days, int event_id);

EventManagerResult emRemoveEvent(EventManager em, int event_id);

EventManagerResult emChangeEventDate(EventManager em, int event_id, Date new_date);

EventManagerResult emAddMember(EventManager em, char* member_name, int member_id);

EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id);

EventManagerResult emRemoveMemberFromEvent (EventManager em, int member_id, int event_id);

EventManagerResult emTick(EventManager em, int days);

int emGetEventsAmount(EventManager em);

char* emGetNextEvent(EventManager em);

void emPrintAllEvents(EventManager em, const char* file_name);

void emPrintAllResponsibleMembers(EventManager em, const char* file_name);

Date dateCreate(int day, int month, int year);

void dateDestroy(Date date);

Date dateCopy(Date date);

bool dateGet(Date date, int* day, int* month, int* year);

int dateCompare(Date date1, Date date2);

void dateTick(Date date);
