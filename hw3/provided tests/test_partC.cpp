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
#include "../partC/schedule.h"
#include <cstdlib>
#include <iostream>
#include <fstream>


using mtm::Schedule;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using mtm::Festival;
using mtm::RecurringEvent;
using mtm::OneTimeEvent;
using mtm::DateWrap;
using mtm::BaseEvent;
using mtm::OpenEvent;
using mtm::ClosedEvent;
using mtm::CustomEvent;

static const std::string FILE_PATH = "../../provided/testOutputs/partC";

#define ASSERT_TEST(expr, backUpbuf)                                                         \
     do {                                                                                    \
     cout.rdbuf(backUpbuf);\
         if (!(expr)) {                                                                       \
             cout << "\nAssertion failed at line"<< __LINE__ << "  " << __FILE__ << #expr << endl; \
             result = false;                                                       \
         }                                                                         \
     } while (0);


#define RUN_TEST(test, name)                  \
    do {                                 \
        cout << "+ Running" << (name) << "...";  \
        if (test()) {                                    \
            cout << "[OK]\n" << endl;         \
        } else {                         \
            cout << "[Failed]\n\n <span>To see what the test does and why it failed, please check the link at the top of the page to the test file</span>" << endl;       \
        }                                \
    } while (0);



#define ASSERT(expr, backUpbuf ) ASSERT_TEST(expr, backUpbuf)


#define REDIRECT_OUTPUT(fileName, PATH)\
std::string fileName = PATH;\
std::ofstream out(fileName, std::ios_base::trunc);\
std::streambuf* stream_buffer_cout = cout.rdbuf();\
std::cout.rdbuf(out.rdbuf());



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

struct Filter2 {
    bool operator()(int student) {
        return student >= 2 && student <= 8;
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


struct pred{
    OpenEvent op = OpenEvent(DateWrap(1,1,2000), "an open event");
    ClosedEvent cp = ClosedEvent(DateWrap(1,1,2000), "a closed event");
    CustomEvent<Filter2> cup = CustomEvent<Filter2>(DateWrap(1,1,2000), "a custom event", Filter2());
public:
    bool operator()(const BaseEvent& b) const{
        return b == op || cp == b || cup == b;
    }
};

void test1(const Schedule &schedule) {
    schedule.printAllEvents(); }

void test2(const Schedule& schedule) {
    schedule.printEventDetails(mtm::DateWrap(27, 12, 2020), "Publish Test");
}

void test3(const Schedule &schedule) {
    schedule.printEventDetails(mtm::DateWrap(5, 1, 2021), "Update Q&A");
}

void test4(const mtm::Schedule& schedule) { schedule.printMonthEvents(12, 2020); }

class MutatingPredicate {
    int counter = 0;

public:
    bool operator()(const mtm::BaseEvent& event) {
        ++counter;
        return true;
    }
};

void test5(const mtm::Schedule& schedule) {
    schedule.printSomeEvents(MutatingPredicate(), true);
}

bool testYan1()
{
    bool result = true;
    REDIRECT_OUTPUT(fileName, FILE_PATH + std::string("/your_outputs/testYan1.txt"))
    mtm::Schedule schedule;
    //notice events should be merged lexicographically
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(1, 1, 2020), "YanB"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(1, 1, 2020), "YanR"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(1, 1, 2020), "YanZ"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(1, 1, 2020), "YanA"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(1, 1, 2020), "YanC"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(1, 1, 2020), "YanX"));
    schedule.printAllEvents();
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testYan1.txt")), stream_buffer_cout)
    return result;
}

bool testYan2()
{
    bool result = true;
    REDIRECT_OUTPUT(fileName, FILE_PATH + std::string("/your_outputs/testYan2.txt"))
    mtm::Schedule schedule;
    //notice events should be merged by date
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(20, 1, 2022), "YanZ"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(30, 1, 2020), "YanR"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(1, 1, 2020), "YanZ"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(2, 2, 2020), "YanA"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(1, 2, 2020), "YanC"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(1, 2, 2020), "YanB"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(30, 1, 2020), "YanM"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(1, 1, 2020), "YanX"));
    try{
        schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
         mtm::DateWrap(1, 1, 2020), "YanX"));
    }catch(mtm::EventAlreadyExists&){ 
    }
     try{
        schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
         mtm::DateWrap(1, 2, 2020), "YanC"));
    }catch(mtm::EventAlreadyExists&){ 
    }
    schedule.printAllEvents();
     out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testYan2.txt")),stream_buffer_cout)
    return result;
}

class StudentOddFilter {
    public:
    bool operator()(int student) {
        for(int i = 0; i < 100; i++){
            if(student == i*2){
                return true;
            }
        }
        return false;
    }
};


class Predicate {
    public:
    bool operator()(mtm::BaseEvent& event) {
        if(event.getName() == "YanA") return false;
        return true;
    }
};


bool testYan3()
{
    bool result = true;
    REDIRECT_OUTPUT(fileName, FILE_PATH + std::string("/your_outputs/testYan3.txt"))
    mtm::Festival festival(mtm::DateWrap(21, 10, 2020));

    mtm::OpenEvent open(mtm::DateWrap(21, 10, 2020), "YanA");
    open.registerParticipant(20000);
    festival.add(open);

    mtm::ClosedEvent closed(mtm::DateWrap(21, 10, 2020), "YanC");
    for(int i = 10; i < 20; i++){
        closed.addInvitee(i);
    }
    for(int i = 1; i < 10; i++){
        closed.addInvitee(i);
    }
     for(int i = 10; i < 20; i++){
        closed.registerParticipant(i);
    }
    for(int i = 1; i < 10; i++){
        closed.registerParticipant(i);
    }
    festival.add(closed);

    mtm::CustomEvent<StudentOddFilter> custom(mtm::DateWrap(21, 10, 2020),
                                           "YanB", StudentOddFilter());
    for(int i = 10; i < 20; i++){
        custom.registerParticipant(2*i);
    }
    for(int i = 1; i < 10; i++){
        custom.registerParticipant(2*i);
    }
    festival.add(custom);

    mtm::Schedule schedule;
    schedule.addEvents(festival);
    schedule.registerToEvent(mtm::DateWrap(21, 10, 2020),"YanB",198);
    schedule.printSomeEvents(Predicate(), true);
    out.close();
    ASSERT_TEST(matchFiles(fileName, FILE_PATH + std::string("/expected/testYan3.txt")), stream_buffer_cout)
    return result;
}

bool testYan4()
{
    bool result = true;
    REDIRECT_OUTPUT(fileName, FILE_PATH + std::string("/your_outputs/testYan4.txt"))
    mtm::Schedule schedule;
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(1, 12, 2020), "YanZ"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(27, 12, 2020), "YanW"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(22, 12, 2020), "YanD"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(18, 12, 2020), "YanY"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(14, 12, 2020), "YanE"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(29, 12, 2020), "YanA"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(2, 12, 2020), "YanM"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(30, 12, 2020), "YanS"));
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
        mtm::DateWrap(25, 12, 2020), "YanR"));

    for(int i = 60 ; i<65; i++){
        schedule.registerToEvent(mtm::DateWrap(1, 12, 2020), "YanZ", i);
    }
    for(int i = 20 ; i<25; i++){
        schedule.registerToEvent(mtm::DateWrap(1, 12, 2020), "YanZ", i);
    }
    for(int i = 5 ; i<10; i++){
        schedule.registerToEvent(mtm::DateWrap(14, 12, 2020), "YanE", i);
    }
    schedule.printEventDetails(mtm::DateWrap(14, 12, 2020), "YanE");
    schedule.printEventDetails(mtm::DateWrap(1, 12, 2020), "YanZ");
    schedule.printEventDetails(mtm::DateWrap(25, 12, 2020), "YanR");
    out.close();
    ASSERT_TEST(matchFiles(fileName, FILE_PATH + std::string("/expected/testYan4.txt")),stream_buffer_cout)
    return result;
}

bool allSegelTests(){
    bool result = true;
    REDIRECT_OUTPUT(fileName, FILE_PATH + std::string("/your_outputs/allSegelTests.txt"))
    mtm::Schedule schedule;
    schedule.addEvents(mtm::OneTimeEvent<mtm::OpenEvent>(
            mtm::DateWrap(27, 12, 2020), "Publish Test"));

    mtm::RecurringEvent<mtm::ClosedEvent> closed(mtm::DateWrap(20, 12, 2020),
                                                 "Update Q&A", 6, 5);
    for (mtm::BaseEvent &event : closed) {
        mtm::ClosedEvent &closed_event = dynamic_cast<mtm::ClosedEvent &>(event);
        closed_event.addInvitee(1337);
        closed_event.addInvitee(850);
        closed_event.addInvitee(1500);
    }
    schedule.addEvents(closed);

    schedule.registerToEvent(mtm::DateWrap(20, 12, 2020), "Update Q&A", 850);
    schedule.registerToEvent(mtm::DateWrap(20, 12, 2020), "Update Q&A", 1500);
    schedule.registerToEvent(mtm::DateWrap(5, 1, 2021), "Update Q&A", 850);
    schedule.registerToEvent(mtm::DateWrap(5, 1, 2021), "Update Q&A", 1500);
    schedule.unregisterFromEvent(mtm::DateWrap(20, 12, 2020), "Update Q&A",
                                 1500);
    test1(schedule);
    test2(schedule);
    test3(schedule);
    test4(schedule);
    test5(schedule);
    out.close();
    ASSERT_TEST(matchFiles(fileName, FILE_PATH + std::string("/expected/allSegelTests.txt")), stream_buffer_cout)
    return result;
}

bool testSchedulePolymorphism() {
    bool result = true;
    REDIRECT_OUTPUT(fileName, FILE_PATH + std::string("/your_outputs/testSchedulePolymorphism.txt"))
    Schedule s;
    Festival f(DateWrap(1, 1, 2000));
    for (int i = 0; i < 3; i++) {
        if (i == 1) {
            ClosedEvent *cl = dynamic_cast<ClosedEvent *>(generate(i));
            for (int j = 1; j < 10; j++) {
                cl->addInvitee(j);
            }
            f.add(*cl);
            delete cl;
        } else {
            BaseEvent *b = generate(i);
            f.add(*b);
            delete b;
        }
    }
    s.addEvents(f);
    for (int i = 1; i < 10 ; i++){
        s.registerToEvent(DateWrap(1,1,2000), "an open event", i);
        s.registerToEvent(DateWrap(1,1,2000), "a closed event", i);
        try{
            s.registerToEvent(DateWrap(1,1,2000), "a custom event", i);
        }catch(mtm::RegistrationBlocked&){
            cout << "RegistrationBlocked" << endl;
        }
    }
    RecurringEvent<OpenEvent> recurringEvent(DateWrap(1,1,2000), "an open event", 3, 2);
    try{
        s.addEvents(recurringEvent);
    }catch(mtm::EventAlreadyExists&){
        cout << "EventAlreadyExists" << endl;
    }
    RecurringEvent<ClosedEvent> recurringEvent1(DateWrap(1,1,2000), "an open event", 1,1);
    try{
        s.addEvents(recurringEvent1);
    }catch(mtm::EventAlreadyExists&){
        cout << "EventAlreadyExists" << endl;
    }
    RecurringEvent<OpenEvent> recurringEvent2(DateWrap(2,1,2000), "an open event", 2,2);
    s.addEvents(recurringEvent2);
    s.registerToEvent(DateWrap(2,1,2000), "an open event", 2);
    s.printSomeEvents(pred(), true);
    out.close();
    ASSERT_TEST(matchFiles(fileName, FILE_PATH + std::string("/expected/testSchedulePolymorphism.txt")), stream_buffer_cout)
    return result;
}

/*bool testRegistrationAndUnRegistration(){
    bool result = true;
    REDIRECT_OUTPUT(fileName, FILE_PATH + std::string("/your_outputs/testRegistrationAndUnRegistration.txt"))
    Schedule s;
    RecurringEvent<OpenEvent> recurringEvent(DateWrap(2,1,2000), "an open event", 5,3);
    s.addEvents(recurringEvent);

    return result;
}*/


#define TEST_NAMES\
    X(allSegelTests)\
    X(testYan1)\
    X(testYan2)\
    X(testYan3)\
    X(testYan4)   \
    X(testSchedulePolymorphism)


static const int NUMBER_OF_TESTS = 6;


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
    } else if (argc > 2) {
        std::cout << "invalid arguments" << std::endl;
    } else {
        int i = std::atoi(argv[1]);
        tests[i - 1]();
    }
    return 0;

}
