#ifndef OPEN_EVENT_H
#define OPEN_EVENT_H
#include "../partA/date_wrap.h"
#include "base_event.h"
#include <string>

namespace mtm
{
    class OpenEvent: public BaseEvent
    {
        public:
        explicit OpenEvent(DateWrap date, string name);
        OpenEvent(const OpenEvent&);
        OpenEvent& operator=(const OpenEvent&);
        ~OpenEvent() = default;

        BaseEvent* clone() const override;
    };
}
#endif
