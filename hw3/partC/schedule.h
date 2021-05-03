#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "../partB/event_container.h"
#include "../partB/linked_list.h"
#include <list> 


using std::list;

namespace mtm
{
    class Schedule
    {
        list<BaseEvent*> events_list;
        public:
        Schedule();
        ~Schedule();
        const int min_month = 1;
        const int max_month = 12;
       
        void addEvents(const EventContainer&);
        void registerToEvent(DateWrap date, string name, int student);
        void unregisterFromEvent(DateWrap date, string name, int student);
        void printAllEvents() const;
        template<typename Predicate>
        void printSomeEvents(Predicate predicate, bool verbose = false) const;
        void printMonthEvents(int month, int year) const;
        void printEventDetails(DateWrap date, string name) const; 
    };
    class CompareEvents
    {
    public:
    bool operator()(const BaseEvent*, const BaseEvent*);
    };

    template<typename Predicate>
    void Schedule::printSomeEvents(Predicate predicate, bool verbose) const
    {
        for(mtm::BaseEvent* event : events_list){
            if(predicate(*event)){
                if(verbose){
                    event->printLong(std::cout);
                    std::cout<<std::endl;
                } else {
                    event->printLong(std::cout);
                    std::cout<<std::endl;
                }
            }
        }
    }

}

#endif