#ifndef FESTIVAL_H
#define FESTIVAL_H

#include "event_container.h"
#include "base_event.h"

namespace mtm
{
    class Festival: public EventContainer 
    {   
        DateWrap date;
        public:
        explicit Festival(DateWrap date);
        ~Festival() = default;
        
        void add(const BaseEvent& event) override;
    };
}


#endif