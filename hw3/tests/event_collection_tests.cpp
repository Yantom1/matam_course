#include <iostream>
#include <string>

#include "test_utilities.h"
#include "../partA/exceptions.h"
#include "../partA/date_wrap.h"
#include "../partB/open_event.h"
#include "../partB/closed_event.h"
#include "../partB/custom_event.h"
#include "../partB/festival.h"
#include "../partB/event_container.h"
#include "../partB/recurring_event.h"
#include "../partB/one_time_event.h"


using std::string;
using std::cout;
using std::endl;
using namespace mtm;

#define NUMBER_TESTS 2

template <class T> void print(const T& x) { cout << x << endl; }

struct StudentFilter {
    bool operator()(int student) {
        return student == 1 || student == 3 || student == 20000;
    }
};

Festival initFestivalEventList(){
    DateWrap date(28,2,2021);
    Festival festival(date);
    
    festival.add(OpenEvent(date, "Travis Scott performance"));
    festival.add(CustomEvent<StudentFilter>(date, "Water fight", StudentFilter()));
    festival.add(OpenEvent(date, "Kendrick Lamar performance"));
    festival.add(ClosedEvent(date, "Drake performance"));
    festival.add(ClosedEvent(date, "Eating comptetition"));
    return festival;
}

bool testFestivalCreateDestroy() {
    bool result = true;

    string name = "Hot event";
    OpenEvent event(DateWrap(), name);
    ClosedEvent event2(DateWrap(), name);
    CustomEvent<StudentFilter> event3(DateWrap(), name, StudentFilter());
    Festival festival = initFestivalEventList();
    return result;
}

typedef mtm::EventContainer::EventIterator Iter;
bool testRecurring(){
    bool result = true;
    //bool passed = true;
    mtm::RecurringEvent<mtm::OpenEvent> recurring(mtm::DateWrap(21, 10, 2020),
                                               "movie night", 15, 7);
    for (Iter iter = recurring.begin(); iter != recurring.end(); ++iter) {
        mtm::BaseEvent& event = *iter;
        event.printLong(std::cout);
    }
    mtm::RecurringEvent<mtm::OpenEvent> recurring2(mtm::DateWrap(21, 10, 2020),
                                               "crazy night", 500, 7);
    int counter = 0;
    for (Iter iter = recurring2.begin(); iter != recurring2.end(); ++iter) {
        counter++;
    }
    ASSERT_TEST(counter == 500, returnLabel);


    returnLabel:
    return result;
}


/* bool test(){
    bool result = true;
    bool passed = true;

returnLabel:
    return result;
}*/

bool (*tests[]) (void) = {
        testFestivalCreateDestroy,
        testRecurring,
};

const char* testNames[] = {
        "testFestivalCreateDestroy",
        "testRecurring"
        
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
            RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
        fprintf(stdout, "Usage: priority_queue_tests <test index>\n");
        return 0;
    }

    int test_idx = strtol(argv[1], NULL, 10);
    if (test_idx < 1 || test_idx > NUMBER_TESTS) {
        fprintf(stderr, "Invalid test index %d\n", test_idx);
        return 0;
    }

    RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
    return 0;
}
