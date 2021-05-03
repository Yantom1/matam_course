#ifndef CUSTOM_EVENT_H
#define CUSTOM_EVENT_H
#include "../partA/date_wrap.h"
#include "base_event.h"
#include <string>


namespace mtm
{
    template <class CanRegister>
    class CustomEvent : public BaseEvent
    {
        CanRegister check_condition;
        public:
        explicit CustomEvent(DateWrap date, string name, CanRegister check_condition);
        CustomEvent(const CustomEvent&);
        CustomEvent& operator=(const CustomEvent&);
        ~CustomEvent() = default;

        void registerParticipant(int student) override;
        BaseEvent* clone() const override;
    };

    //==============Implementation==========//
    template <class CanRegister>
    CustomEvent<CanRegister>::CustomEvent(DateWrap date, string name, CanRegister check_condition) 
                                            :BaseEvent(date,name), check_condition(check_condition){
                                
    }

    template <class CanRegister>                                                                                        
    CustomEvent<CanRegister>::CustomEvent(const CustomEvent& event): BaseEvent(event){
        check_condition = event.check_condition;
    }

    template <class CanRegister> 
    CustomEvent<CanRegister>& CustomEvent<CanRegister>::operator=(const CustomEvent& event){
        if(this == &event){
        return *this;
        }
        name = event.name;
        date = event.date;
        participants = event.participants;
        check_condition = event.check_condition;
        return *this;
    }

    template <class CanRegister>
    void CustomEvent<CanRegister>::registerParticipant(int student){
    if(student < min_id || student > max_id){
        throw InvalidStudent();
    }
    if(check_condition(student)){
        if (participants.exists(student)){
            throw AlreadyRegistered();
        }
        participants.insert(student);
    } else {
        throw RegistrationBlocked();
        }
    }

    template <class CanRegister>
    BaseEvent* CustomEvent<CanRegister>::clone() const {
        return new CustomEvent(*this);
    }
} 
#endif
