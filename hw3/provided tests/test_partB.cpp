
#include "../partB/base_event.h"
#include "../partB/closed_event.h"
#include "../partB/custom_event.h"
#include "../partA/date_wrap.h"
#include "../partB/event_container.h"
#include "../partA/exceptions.h"
#include "../partB/festival.h"
#include "../partB/one_time_event.h"
#include "../partB/open_event.h"
#include "../partB/recurring_event.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <typeinfo>


using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::fstream;
typedef mtm::EventContainer::EventIterator Iter;
using mtm::OpenEvent;
using mtm::ClosedEvent;
using mtm::CustomEvent;
using mtm::BaseEvent;
using mtm::DateWrap;
using mtm::Festival;
using mtm::RecurringEvent;
using mtm::OneTimeEvent;

static const std::string FILE_PATH = "../../provided/testOutputs/partB";

/**Prints which assertion failed and in which file*/
#define ASSERT_TEST(expr)                                                         \
     do {                                                                          \
         if (!(expr)) {                                                            \
             cout << "\nAssertion failed at line"<< __LINE__ << "  " << __FILE__ << #expr << endl; \
             result = false;                                                       \
         }                                                                         \
     } while (0);

#define RUN_TEST(test, name)                  \
    do {                                 \
        cout << "+ Running" << (name) << "...";  \
        if (test()) {                    \
            cout << "[OK]\n" << endl;         \
        } else {                         \
            cout << "[Failed]\n\n <span>To see what the test does and why it failed, please check the link at the top of the page to the test file</span>" << endl;       \
        }                                \
    } while (0);



#define ASSERT(expr) ASSERT_TEST(expr)


#define OPEN_FILE(streamName, name) std::string fileName = name;\
std::ofstream streamName(fileName, std::ofstream::trunc | std::ofstream::in);\
if(!(streamName).is_open()){\
throw FileFailed();\
}


class FileFailed: std::exception {
public:
    FileFailed() = default;
    ~FileFailed() = default;
};


bool matchFiles(const std::string& out, const std::string& exp) {
    ifstream output(out);
    if (!output) {
        cout << "can't open file" << endl;
    }
    ifstream expOutput(exp);
    if (!expOutput) {
        cout << "can't open file" << endl;
    }
    while (!output.eof()) {
        char c;
        output >> std::noskipws >> c;
        char ex;
        expOutput >> std::noskipws >> ex;
        if (ex != c) {
            return false;
        }
    }
    return true;
}

void printEventsShort(mtm::EventContainer& events, ofstream& stream) {
    for (Iter iter = events.begin(); iter != events.end(); ++iter) {
        mtm::BaseEvent& event = *iter;
        event.printShort(stream);
    }
}
void printEventsLong(mtm::EventContainer& events, ofstream& stream) {
    for (Iter iter = events.begin(); iter != events.end(); ++iter) {
        mtm::BaseEvent& event = *iter;
        event.printLong(stream);
    }
}


struct StudentFilter {
    bool operator()(int student) {
        return student == 1 || student == 3 || student == 20000 || student == 60 || student == 150000;
    }
};

struct Filter2 {
    bool operator()(int student) {
        return student >= 5 && student <= 8;
    }
};


BaseEvent* generate(int i) {
    switch (i) {
        case 0:
            return new OpenEvent(DateWrap(1, 1, 2000), "an open event");
        case 1:
            return new ClosedEvent(DateWrap(1, 1, 2000), "a closed event");
        case 2:
            return new CustomEvent<Filter2>(DateWrap(1, 1, 2000), "a custom event", Filter2());
        default:
            return nullptr;
    }
}

class CustomEventWrap :public CustomEvent<Filter2> {
public:
    CustomEventWrap(const DateWrap& date, const std::string& name)
            :CustomEvent<Filter2>(date, name, Filter2()) {}
    ~CustomEventWrap() override = default;
};

/**__________________________________________________________________________*/
/** HERE START THE TESTS*/
bool test1SegelProvided() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/test1SegelProvided.txt"))
    mtm::Festival festival(mtm::DateWrap(21, 10, 2020));
    festival.add(mtm::OpenEvent(mtm::DateWrap(21, 10, 2020), "Performance 1"));
    mtm::ClosedEvent closed(mtm::DateWrap(21, 10, 2020), "Performance 2");
    closed.addInvitee(1);
    closed.addInvitee(500);
    festival.add(closed);
    printEventsShort(festival, out);

    mtm::RecurringEvent<mtm::OpenEvent> recurring(mtm::DateWrap(21, 10, 2020),
                                                  "Wednesday Noon", 13, 7);
    printEventsShort(recurring, out);

    mtm::OneTimeEvent<mtm::OpenEvent> one_time(mtm::DateWrap(2, 3, 80),
                                               "A long time ago");
    printEventsShort(one_time, out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/test1SegelProvided.txt")))
    return result;
}

void test2SegelProvided_aux(mtm::BaseEvent& event, ofstream& stream) {
    event.registerParticipant(1);
    event.registerParticipant(20000);
    event.unregisterParticipant(1);
    event.registerParticipant(3);
    mtm::BaseEvent* clone = event.clone();
    clone->printShort(stream);
    clone->printLong(stream);
    delete clone;
}


bool test2SegelProvided() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/test2SegelProvided.txt"))
    mtm::OpenEvent open(mtm::DateWrap(21, 10, 2020), "An Open Event");
    test2SegelProvided_aux(open, out);

    mtm::ClosedEvent closed(mtm::DateWrap(21, 10, 2020), "A Closed Event");
    closed.addInvitee(1);
    closed.addInvitee(3);
    closed.addInvitee(20000);
    test2SegelProvided_aux(closed, out);
    mtm::CustomEvent<StudentFilter> custom(mtm::DateWrap(21, 10, 2020),
                                           "A Custom Event", StudentFilter());
    test2SegelProvided_aux(custom, out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/test2SegelProvided.txt")))
    return result;
}




bool testConstructorOpenEvent()
{
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testConstructorOpenEvent.txt"))
    OpenEvent event1(DateWrap(1, 2, 2000), "event1");
    event1.printShort(out);
    event1.printLong(out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testConstructorOpenEvent.txt")))
    return result;
}

bool testRegisterParticipantOpenEvent()
{
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testRegisterParticipantOpenEvent.txt"))
    try {
        OpenEvent event1(DateWrap(0, 1, 2000), "event1");
    }
    catch (mtm::InvalidDate&) {
        out << "InvalidDate" << endl;
    }
    OpenEvent event1(DateWrap(1, 1, 2000), "event1");
    for (int i = 0; i < 10; i++) {
        try {
            event1.registerParticipant(i);
        }
        catch (mtm::InvalidStudent&) {
            out << "InvalidStudent" << endl;
        }
    }
    try {
        event1.registerParticipant(2);
    }
    catch (mtm::AlreadyRegistered&) {
        out << "AlreadyRegistered" << endl;
    }
    event1.printLong(out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testRegisterParticipantOpenEvent.txt")))
    return result;
}


// seraj func
bool testUnregisterParticipantOpenEvent_CreatorSiraj()
{
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testUnRegisterParticipantOpenEvent1.txt"))
    OpenEvent event1(DateWrap(1, 1, 2000), "event1");
    for (int i = 1; i <= 5; i++) {
        event1.registerParticipant(i * i);
    }
    event1.printLong(out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testUnRegisterParticipantOpenEvent1.txt")))
    std::string fileName2 = FILE_PATH + std::string("/your_outputs/testUnRegisterParticipantOpenEvent2.txt");
    std::ofstream out2(fileName2, std::ios_base::in | std::ofstream::trunc);
    if (!out2.is_open()) {
        throw FileFailed();
    }
    for (int i = 0; i <= 25; i++) {
        try {
            event1.unregisterParticipant(i);
        }
        catch (mtm::NotRegistered&) {
            out2 << "NotRegistered" << endl;
        }
        catch (mtm::InvalidStudent&) {
            out2 << "InvalidStudent" << endl;
        }
    }
    event1.printLong(out2);
    out2.close();
    ASSERT(matchFiles(fileName2, FILE_PATH + std::string("/expected/testUnRegisterParticipantOpenEvent2.txt")))
    return result;
}

bool testCloneOpenEvent() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testCloneOpenEvent.txt"))
    OpenEvent event1(DateWrap(19, 2, 2021), "mtm exam");
    event1.registerParticipant(3);
    try {
        event1.registerParticipant(3);
    }
    catch (mtm::AlreadyRegistered&) {
        out << "AlreadyRegistered" << endl;
    }
    event1.registerParticipant(2);
    event1.registerParticipant(4);
    event1.registerParticipant(5);
    BaseEvent* eventPtr = event1.clone();
    ASSERT(typeid(*eventPtr).name() == typeid(event1).name());
    eventPtr->printLong(out);
    event1.printLong(out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testCloneOpenEvent.txt")))
    delete eventPtr;
    return result;
}

bool testClosedEventConstructor()
{
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testClosedEventConstructor.txt"))
    ClosedEvent event1(DateWrap(1, 1, 2000), "closed event");
    try {
        ClosedEvent event2(DateWrap(0, 1, 2000), "closed event");
    }
    catch (mtm::InvalidDate&) {
        out << "InvalidDate" << endl;
    }
    event1.printLong(out);
    event1.printShort(out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testClosedEventConstructor.txt")))
    return result;
}

bool testClosedEventRegisterAndUnRegister() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testClosedEventRegisterAndUnRegister.txt"))
    ClosedEvent event(DateWrap(1, 1, 2000), "AMEEEERCA");
    for (int i = 0; i < 4; i++) {
        try {
            event.addInvitee(i);
        }
        catch (mtm::InvalidStudent&) {
            out << "InvalidStudent" << endl;
        }
    }
    try {
        event.addInvitee(2);
    }
    catch (mtm::AlreadyInvited&) {
        out << "AlreadyInvited" << endl;
    }
    for (int i = 1; i < 5; i++) {
        try {
            event.registerParticipant(i);
        }
        catch (mtm::RegistrationBlocked&) {
            out << "RegistrationBlocked" << endl;
        }
    }
    event.printLong(out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testClosedEventRegisterAndUnRegister.txt")))
    return result;
}

bool testClosedEventClone() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testClosedEventClone.txt"))
    ClosedEvent ev(DateWrap(1, 1, 2000), "ev");
    for (int i = 1; i < 4; i++) {
        ev.addInvitee(i);
        ev.registerParticipant(i);
    }
    ev.unregisterParticipant(3);
    ev.registerParticipant(3);//to check when unregistering if uninviting too
    BaseEvent* evPtr = ev.clone();
    evPtr->printLong(out);
    ev.printLong(out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testClosedEventClone.txt")))
    delete evPtr;
    return result;
}

bool testCustomEventConstructor() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testCustomEventConstructor.txt"))
    CustomEvent<Filter2> ev(DateWrap(1, 1, 2000), "ev", Filter2());
    try {
        CustomEvent<Filter2> ev2(DateWrap(1, 0, 2000), "ev", Filter2());
    }
    catch (mtm::InvalidDate&) {
        out << "InvalidDate" << endl;
    }
    ev.printLong(out);
    ev.printShort(out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testCustomEventConstructor.txt")))
    return result;
}

bool testCustomEventParticipants() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testCustomEventParticipants.txt"))
    CustomEvent<Filter2> ev1(DateWrap(1, 1, 2000), "ev1", Filter2());
    for (int i = 0; i < 9; i++) {
        try {
            ev1.registerParticipant(i);
        }
        catch (mtm::InvalidStudent&) {
            out << "InvalidStudent" << endl;
        }
        catch (mtm::RegistrationBlocked&) {
            out << "RegistrationBlocked" << endl;
        }
    }
    try {
        ev1.unregisterParticipant(3);
    }
    catch (mtm::NotRegistered&) {
        out << "NotRegistered" << endl;
    }
    ev1.unregisterParticipant(5);
    try {
        ev1.unregisterParticipant(2);
    }
    catch (mtm::NotRegistered&) {
        out << "NotRegistered" << endl;
    }
    ev1.printLong(out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testCustomEventParticipants.txt")))
    return result;
}

bool testCustomEventClone() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testCustomEventClone.txt"))
    CustomEvent<Filter2> ev1(DateWrap(1, 1, 2000), "ev1", Filter2());
    ev1.registerParticipant(6);
    ev1.registerParticipant(7);
    BaseEvent* ev2 = ev1.clone();
    try {
        ev2->registerParticipant(4);
    }
    catch (mtm::RegistrationBlocked&) {
        out << "RegistrationBlocked" << endl;
    }
    try {
        ev2->registerParticipant(6);
    }
    catch (mtm::AlreadyRegistered&) {
        out << "AlreadyRegistered" << endl;
    }
    ev1.printLong(out);
    ev2->printLong(out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testCustomEventClone.txt")))
    delete ev2;
    return result;
}

bool testFestivalConstructor() {
    bool result = true;
    Festival festival(DateWrap(1, 1, 2000));
    ASSERT(festival.begin() == festival.end())
    return result;
}

bool testFestivalAddAndIterator() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testFestivalAddAndIterator.txt"))
    Festival festival(DateWrap(1, 1, 2000));
    OpenEvent ev1(DateWrap(1, 1, 2000), "ev1");
    OpenEvent ev2(DateWrap(2, 1, 2000), "ev2");
    OpenEvent ev3(DateWrap(1, 1, 2000), "ev3");
    ev1.registerParticipant(4);
    ev1.registerParticipant(5);
    ev3.registerParticipant(4);
    ev3.registerParticipant(5);
    festival.add(ev1);
    festival.add(ev3);
    try {
        festival.add(ev2);
    }
    catch (mtm::DateMismatch&) {
        out << "DateMismatch" << endl;
    }
    printEventsLong(festival, out);
    out.close();
    Festival festival1(DateWrap(1, 1, 2000));
    Iter iter(festival1.begin());
    Iter iter2(festival.begin());
    iter = iter2;
    ASSERT(iter == iter2)
    Iter iter3(festival1.begin());
    ASSERT(iter != iter3);
    Iter iter4(iter2);
    ASSERT(iter2 == iter4);
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testFestivalAddAndIterator.txt")))
    return result;
}

bool testFestivalPolymorphism() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testFestivalPolymorphism.txt"))
    Festival festival(DateWrap(1, 1, 2000));
    OpenEvent openEvent(DateWrap(1, 1, 2000), "an open event");
    ClosedEvent closedEvent(DateWrap(1, 1, 2000), "a closed event");
    CustomEvent<Filter2> customEvent(DateWrap(1, 1, 2000), "a custom event", Filter2());
    festival.add(openEvent);
    closedEvent.addInvitee(5);
    closedEvent.addInvitee(6);
    festival.add(closedEvent);
    festival.add(customEvent);
    for (int i = 5; i < 8; i++) {
        for (Iter iter(festival.begin()); iter != festival.end(); ++iter) {
            try {
                (*iter).registerParticipant(i);
            }
            catch (mtm::RegistrationBlocked&) {//throws an exception because of closed event
                out << "RegistrationBlocked" << endl;
            }
        }
    }
    Iter iter(festival.begin());
    ++iter;
    try {
        (*iter).registerParticipant(9);
    }
    catch (mtm::RegistrationBlocked&) {
        out << "RegistrationBlocked" << endl;
    }
    printEventsLong(festival, out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testFestivalPolymorphism.txt")))
    return result;
}

bool testRecurringEventConstructor() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testRecurringEventConstructor.txt"))
    RecurringEvent<OpenEvent> recurringEvent(DateWrap(1, 1, 2000), "ev1", 8, 2);
    try {
        RecurringEvent<OpenEvent> ev(DateWrap(1, 1, 2000), "ev1", 0, 2);
    }
    catch (mtm::InvalidNumber&) {
        out << "InvalidNumber" << endl;
    }
    try {
        RecurringEvent<OpenEvent> ev1(DateWrap(1, 1, 2000), "ev1", 8, 0);
    }
    catch (mtm::InvalidInterval&) {
        out << "InvalidInterval" << endl;
    }
    printEventsShort(recurringEvent, out);
    RecurringEvent<OpenEvent> recurringEvent2(DateWrap(1, 1, 2000), "ev1", 1, 2);
    printEventsShort(recurringEvent2, out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testRecurringEventConstructor.txt")))
    return result;
}

bool testRecurringEventAdd() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testRecurringEventAdd.txt"))
    RecurringEvent<ClosedEvent> recurringEvent(DateWrap(1, 1, 2000), "ev1", 8, 2);
    try {
        recurringEvent.add(OpenEvent(DateWrap(1, 1, 2000), "ev2"));
    }
    catch (mtm::NotSupported&) {
        out << "NotSupported" << endl;
    }
    for (Iter iter(recurringEvent.begin()); iter != recurringEvent.end(); ++iter) {
        try {
            (*iter).registerParticipant(1);
        }
        catch (mtm::RegistrationBlocked&) {
            out << "RegistrationBlocked" << endl;
        }
    }
    RecurringEvent<CustomEventWrap> recurring(DateWrap(1, 1, 2000), "ev2", 3, 2);
    Iter iter(recurring.begin());
    (*iter).registerParticipant(5);
    (*iter).registerParticipant(6);
    (*iter).registerParticipant(7);
    (*iter).registerParticipant(8);
    try {
        (*iter).registerParticipant(10);
    }
    catch (mtm::RegistrationBlocked&) {
        out << "RegistrationBlocked" << endl;
    }
    ++iter;
    (*iter).registerParticipant(5);
    (*iter).registerParticipant(6);
    (*iter).registerParticipant(7);
    ++iter;
    (*iter).registerParticipant(5);
    (*iter).registerParticipant(6);
    printEventsLong(recurring, out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testRecurringEventAdd.txt")))
    return result;
}

bool testOneTimeEventConstructor() {
    bool result = true;
    OneTimeEvent<OpenEvent> ev1(DateWrap(1, 1, 2000), "ev1");
    Iter iter(ev1.begin());
    ASSERT(++iter == ev1.end());
    return result;
}

bool testOneTimeEventAdd() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testOneTimeEventAdd.txt"))
    OneTimeEvent<CustomEventWrap> ev1(DateWrap(1, 1, 2000), "ev1");
    try {
        ev1.add(ClosedEvent(DateWrap(1, 1, 2000), "ev2"));
    }
    catch (mtm::NotSupported&) {
        out << "NotSupported" << endl;
    }
    Iter iter(ev1.begin());
    (*iter).registerParticipant(6);
    (*iter).registerParticipant(7);
    (*iter).registerParticipant(8);
    try {
        (*iter).registerParticipant(8);
    }
    catch (mtm::AlreadyRegistered&) {
        out << "AlreadyRegistered" << endl;
    }
    try {
        (*iter).registerParticipant(0);
    }
    catch (mtm::InvalidStudent&) {
        out << "InvalidStudent" << endl;
    }
    printEventsLong(ev1, out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testOneTimeEventAdd.txt")))
    return result;
}

bool testBigEventContainersAndPolymorphism() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testBigEventContainersAndPolymorphism.txt"))
    Festival festival(DateWrap(1, 1, 2000));
    for (int i = 0; i < 3; i++) {
        if (i == 1) {
            ClosedEvent* cptr = dynamic_cast<ClosedEvent*>(generate(i));
            cptr->addInvitee(4);
            cptr->addInvitee(5);
            cptr->addInvitee(6);
            festival.add(*cptr);
            delete cptr;
        }
        else {
            BaseEvent* ptr = generate(i);
            festival.add(*ptr);
            delete ptr;
        }
    }
    for (int i = 3; i < 7; i++) {
        for (Iter iter(festival.begin()); iter != festival.end(); ++iter) {
            try {
                (*iter).registerParticipant(i);
            }
            catch (mtm::RegistrationBlocked&) {
                out << "RegistrationBlocked" << endl;
            }
        }
    }
    printEventsLong(festival, out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testBigEventContainersAndPolymorphism.txt")))
    return result;
}

bool testEventCreateDestroy_CreatorYan() {
    bool result = true;
    string name = "Hot event";
    OpenEvent event(DateWrap(1, 1, 2000), name);
    ClosedEvent event2(DateWrap(1, 1, 2000), name);
    CustomEvent<StudentFilter> event3(DateWrap(1, 1, 2000), name, StudentFilter());
    return result;
}

bool testOpenEventRegister_CreatorYan() {
    bool result = true;
    bool passed = true;
    string name = "Hot event";
    DateWrap date(8, 1, 2021);
    OpenEvent event(date, name);
    event.registerParticipant(6);
    event.registerParticipant(18);
    try {
        passed = false;
        event.registerParticipant(6);
    }
    catch (mtm::AlreadyRegistered& e) {
        passed = true;
    }
    ASSERT_TEST(passed)
    try {
        passed = false;
        event.registerParticipant(18);
    }
    catch (mtm::AlreadyRegistered& e) {
        passed = true;
    }
    ASSERT_TEST(passed)
    try {
        passed = false;
        event.registerParticipant(-1);
    }
    catch (mtm::InvalidStudent& e) {
        passed = true;
    }
    ASSERT_TEST(passed)
    try {
        passed = false;
        event.registerParticipant(1234567891);
    }
    catch (mtm::InvalidStudent& e) {
        passed = true;
    }
    ASSERT_TEST(passed)
    return result;
}

bool testOpenEventUnregister_CreatorYan() {
    bool result = true;
    bool passed = true;
    string name = "Hot event";
    DateWrap date(8, 1, 2021);
    OpenEvent event(date, name);
    event.registerParticipant(6);
    event.registerParticipant(18);
    event.unregisterParticipant(6);
    ASSERT_TEST(passed)
    try {
        passed = false;
        event.registerParticipant(18);
    }
    catch (mtm::AlreadyRegistered& e) {
        passed = true;
    }
    try {
        passed = false;
        event.unregisterParticipant(6);
    }
    catch (mtm::NotRegistered& e) {
        passed = true;
    }
    ASSERT_TEST(passed)
    try {
        passed = false;
        event.unregisterParticipant(-1);
    }
    catch (mtm::InvalidStudent& e) {
        passed = true;
    }
    ASSERT_TEST(passed)
    try {
        passed = false;
        event.unregisterParticipant(1234567891);
    }
    catch (mtm::InvalidStudent& e) {
        passed = true;
    }
    ASSERT_TEST(passed)
    return result;
}

bool testClosedEventRegister_CreatorYan() {
    bool result = true;
    bool passed = true; // To check exceptions
    string name = "Cold event";
    DateWrap date(8, 1, 2021);
    ClosedEvent event(date, name);

    for (int i = 1; i < 10; i++) {
        event.addInvitee(i);
    }

    for (int i = 1; i < 10; i++) {
        try {
            passed = false;
            event.addInvitee(i);
        }
        catch (mtm::AlreadyInvited& e) {
            passed = true;
        }
        ASSERT_TEST(passed)
    }

    try {
        passed = false;
        event.addInvitee(-20);
    }
    catch (mtm::InvalidStudent&) {
        passed = true;
    }
    ASSERT_TEST(passed)

    try {
        passed = false;
        event.addInvitee(1234567891);
    }
    catch (mtm::InvalidStudent&) {
        passed = true;
    }
    ASSERT_TEST(passed)

    // Participants tests
    for (int i = 1; i < 10; i++) {
        event.registerParticipant(i);
    }

    for (int i = 1; i < 10; i++) {
        try {
            passed = false;
            event.registerParticipant(i);
        }
        catch (mtm::AlreadyRegistered& e) {
            passed = true;
        }
        ASSERT_TEST(passed)
    }
    try {
        event.registerParticipant(200);
        passed = false;
    }
    catch (mtm::RegistrationBlocked& e) {
        passed = true;
    }
    ASSERT_TEST(passed)

    try {
        event.registerParticipant(1234567891);
        passed = false;
    }
    catch (mtm::InvalidStudent& e) {
        passed = true;
    }
    ASSERT_TEST(passed)

    try {
        event.registerParticipant(-20);
        passed = false;
    }
    catch (mtm::InvalidStudent& e) {
        passed = true;
    }
    ASSERT_TEST(passed)
    return result;
}

bool testCustomEventRegister_CreatorYan() {
    bool result = true;
    bool passed = true;
    string name = "Lukewarm event";
    DateWrap date(5, 5, 2021);
    CustomEvent<StudentFilter> event(date, name, StudentFilter());

    event.registerParticipant(1);
    event.registerParticipant(3);
    event.registerParticipant(20000);

    try {
        passed = false;
        event.registerParticipant(1);
    }
    catch (mtm::AlreadyRegistered& e) {
        passed = true;
    }
    ASSERT_TEST(passed)
    try {
        event.registerParticipant(3);
        passed = false;
    }
    catch (mtm::AlreadyRegistered& e) {
        passed = true;
    }
    ASSERT_TEST(passed)
    try {
        passed = false;
        event.registerParticipant(20000);
    }
    catch (mtm::AlreadyRegistered& e) {
        passed = true;
    }
    ASSERT_TEST(passed)
    try {
        passed = false;
        event.registerParticipant(55);
    }
    catch (mtm::RegistrationBlocked& e) {
        passed = true;
    }
    ASSERT_TEST(passed)
    try {
        passed = false;
        event.registerParticipant(1);
    }
    catch (mtm::AlreadyRegistered& e) {
        passed = true;
    }
    ASSERT_TEST(passed)

    try {
        passed = false;
        event.registerParticipant(1234567891);
    }
    catch (mtm::InvalidStudent& e) {
        passed = true;
    }
    ASSERT_TEST(passed)

    try {
        passed = false;
        event.registerParticipant(-20);
    }
    catch (mtm::InvalidStudent& e) {
        passed = true;
    }
    ASSERT_TEST(passed)
    return result;
}

bool testOpenEventPrints_CreatorYan() {
    bool result = true;
    string name = "Hot event";
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testOpenEventPrints_CreatorYan.txt"))
    DateWrap date(8, 1, 2021);
    OpenEvent event(date, name);
    event.registerParticipant(6);
    event.registerParticipant(18);
    event.printShort(out);
    event.printLong(out);
    out.close();
    ASSERT_TEST(matchFiles(fileName, FILE_PATH + std::string("/expected/testOpenEventPrints_CreatorYan.txt")))
    return result;
}

bool testClosedEventPrints_CreatorYan() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testClosedEventPrints_CreatorYan.txt"))
    string name = "Mediocore event";
    DateWrap date(8, 1, 2021);
    ClosedEvent event(date, name);
    for (int i = 100; i > 0; i--) {
        event.addInvitee(i * i);
        event.registerParticipant(i * i);
    }
    event.printShort(out);
    event.printLong(out);
    out.close();
    ASSERT_TEST(matchFiles(fileName, FILE_PATH + std::string("/expected/testClosedEventPrints_CreatorYan.txt")))
    return result;
}

bool testCustomEventPrints_CreatorYan() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testCustomEventPrints_CreatorYan.txt"))
    string name = "Very interesting event";
    DateWrap date(8, 1, 2021);

    CustomEvent<StudentFilter> event(date, name, StudentFilter());
    event.registerParticipant(20000);
    event.registerParticipant(1);
    event.registerParticipant(3);
    event.registerParticipant(60);
    event.registerParticipant(150000);
    event.printShort(out);
    event.printLong(out);
    out.close();
    ASSERT_TEST(matchFiles(fileName, FILE_PATH + std::string("/expected/testCustomEventPrints_CreatorYan.txt")))
    return result;
}


bool testHugeEventContainers(){
    bool result  = true;

    return result;
}
const int NUMBER_OF_TESTS = 28;


#define TEST_NAMES \
    X(test1SegelProvided) \
    X(test2SegelProvided) \
    X(testConstructorOpenEvent) \
    X(testRegisterParticipantOpenEvent)\
    X(testUnregisterParticipantOpenEvent_CreatorSiraj) \
    X(testCloneOpenEvent) \
    X(testClosedEventConstructor)      \
    X(testClosedEventRegisterAndUnRegister)\
    X(testClosedEventClone)     \
    X(testCustomEventConstructor)      \
    X(testCustomEventParticipants)     \
    X(testCustomEventClone)     \
    X(testFestivalConstructor)  \
    X(testFestivalAddAndIterator)      \
    X(testFestivalPolymorphism) \
    X(testRecurringEventConstructor)   \
    X(testRecurringEventAdd)    \
    X(testOneTimeEventConstructor)  \
    X(testOneTimeEventAdd)\
    X(testBigEventContainersAndPolymorphism)           \
    X(testEventCreateDestroy_CreatorYan)               \
    X(testOpenEventRegister_CreatorYan)\
    X(testOpenEventUnregister_CreatorYan)              \
    X(testClosedEventRegister_CreatorYan)              \
    X(testCustomEventRegister_CreatorYan)               \
    X(testOpenEventPrints_CreatorYan)               \
    X(testClosedEventPrints_CreatorYan)               \
    X(testCustomEventPrints_CreatorYan)

const char* testNames[] = {
#define X(name) #name,
        TEST_NAMES
#undef X
};
bool (*tests[])(void) = {
#define X(test_name) test_name,
        TEST_NAMES
#undef X
};


int main(int argc, char* argv[]) {
    if (argc < 2) {
        for (int i = 0; i < NUMBER_OF_TESTS; i++) {
            RUN_TEST(tests[i], testNames[i])
        }
    }
    else if (argc > 2) {
        std::cout << "invalid arguments" << std::endl;
    }
    else {
        int i = std::atoi(argv[1]);
        tests[i - 1]();
    }
    return 0;
}

