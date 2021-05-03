#include "test_utilities.h"
#include "../partB/linked_list.h"
#include "../partA/exceptions.h"
#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;
using namespace mtm;

#define NUMBER_TESTS 8

class CompareInt{
            public:
            bool operator()(const int num_1,const int num_2){
                return num_1 < num_2;
            }
};

class TestEvent{
    public:
    int num;
    string name;
    TestEvent(int num=0, string name=""): num(num), name(name){}
};

class CompareTestEvent{
    public:
    bool operator()(TestEvent event_1, TestEvent event_2){
        if(event_1.name == event_2.name){
            return event_1.num < event_2.num;
        } else {
            return event_1.name < event_2.name;
        }
    }
};


bool testListCreateDestroy() {
    bool result = true;

    List<int, CompareInt> list((CompareInt()));
    ASSERT_TEST(list.isEmpty() == true, returnListCreateDestroy);
    
returnListCreateDestroy:
    return result;
}

bool testListInsertSize(){
    bool result = true;

    List<int, CompareInt> list((CompareInt()));
    list.insert(1);
    ASSERT_TEST(list.isEmpty() == false, returnLabel);
    ASSERT_TEST(list.getSize() == 1, returnLabel);

    list.insert(2);
    ASSERT_TEST(list.isEmpty() == false, returnLabel);
    ASSERT_TEST(list.getSize() == 2, returnLabel);

    for(int i = 3; i < 1000; i++){
        list.insert(i);
    }

    ASSERT_TEST(list.isEmpty() == false, returnLabel);
    ASSERT_TEST(list.getSize() == 999, returnLabel);
returnLabel:
    return result;
}

 bool testListSubscriptOperator(){
    bool result = true, passed = true;

    List<int, CompareInt> list((CompareInt()));
    list.insert(10);
    list.insert(20);
    list.insert(30);

    ASSERT_TEST(list[0] == 10, returnLabel);
    ASSERT_TEST(list[1] == 20, returnLabel);
    ASSERT_TEST(list[2] == 30, returnLabel);

    list.insert(15);
    ASSERT_TEST(list[1] == 15, returnLabel);
    ASSERT_TEST(list[2] == 20, returnLabel);

    try{
        passed = false;
        list[-1];
    } catch(mtm::OutOfBoundaries& e){
        passed = true;
    }
    ASSERT_TEST(passed, returnLabel);

    try{
        passed = false;
        list[18];
    } catch(mtm::OutOfBoundaries& e){
        passed = true;
    }
    ASSERT_TEST(passed, returnLabel);

returnLabel:
    return result;
} 

bool testListRemove(){
    bool result = true;

    List<int, CompareInt> list((CompareInt()));
    for(int i = 0; i < 10; i++){
        list.insert(i);
    }

    list.remove(0);
    ASSERT_TEST(list.getSize() == 9, returnLabel);

    list.remove(9);
    ASSERT_TEST(list.getSize() == 8, returnLabel);

    list.remove(6);
    list.remove(5);

    ASSERT_TEST(list.getSize() == 6, returnLabel);
returnLabel:
    return result;
}

bool testListExists(){
    bool result = true;

    List<int,CompareInt> list((CompareInt()));
    for(int i = 0; i < 10; i++){
        list.insert(i);
    }

    for(int i = 0; i < 10; i++){
        ASSERT_TEST(list.exists(i) == true, returnLabel);
    }

    ASSERT_TEST(list.exists(18) == false, returnLabel);
    ASSERT_TEST(list.exists(-15) == false, returnLabel);
    ASSERT_TEST(list.exists(100) == false, returnLabel);
returnLabel:
    return result;
}

bool testListCopyConstructorAssignment(){
    bool result = true;
    List<int, CompareInt> list((CompareInt()));
    for(int i = 0; i < 10; i++){
        list.insert(i);
    }

    List<int, CompareInt> list2 = list;
    List<int, CompareInt> list3(list);
    for(int i = 0; i < 10; i++){
        ASSERT_TEST(list2.exists(i) == true, returnLabel);
        ASSERT_TEST(list3.exists(i) == true, returnLabel);
    }
    list2.remove(3);
    list2.remove(6);
    list2 = list;

    for(int i = 0; i < 10; i++){
        ASSERT_TEST(list2.exists(i) == true, returnLabel);
    }

returnLabel:
    return result;
}

bool testListSortInt(){
    bool result = true;
    List<int, CompareInt> list((CompareInt()));
    list.insert(18);
    list.insert(28);
    list.insert(25);
    list.insert(16);

    ASSERT_TEST(list[0] == 16, returnLabel);
    ASSERT_TEST(list[1] == 18, returnLabel);
    ASSERT_TEST(list[2] == 25, returnLabel);
    ASSERT_TEST(list[3] == 28, returnLabel);

returnLabel:
    return result;
}


bool testListSortEvent(){
    bool result = true;
    List<TestEvent, CompareTestEvent> list((CompareTestEvent()));
    list.insert(TestEvent(6, "Small Event"));
    list.insert(TestEvent(6, "Big Event"));
    list.insert(TestEvent(4, "Small Event"));
    list.insert(TestEvent(9, "Big Event"));

    ASSERT_TEST(list[0].name == "Big Event", returnLabel);
    ASSERT_TEST(list[0].num == 6, returnLabel);
    ASSERT_TEST(list[1].name == "Big Event", returnLabel);
    ASSERT_TEST(list[1].num == 9, returnLabel);
    ASSERT_TEST(list[2].name == "Small Event", returnLabel);
    ASSERT_TEST(list[2].num == 4, returnLabel);
    ASSERT_TEST(list[3].name == "Small Event", returnLabel);
    ASSERT_TEST(list[3].num == 6, returnLabel);

returnLabel:
    return result;
} 

/* bool test(){
    bool result = true;

returnLabel:
    return result;
} */

bool (*tests[]) (void) = {
        testListCreateDestroy,
        testListInsertSize,
        testListSubscriptOperator,
        testListRemove,
        testListExists,
        testListCopyConstructorAssignment,
        testListSortInt,
        testListSortEvent       
};

const char* testNames[] = {
        "testListCreateDestroy",
        "testListInsertSize",
        "testListSubscriptOperator",
        "testListRemove",
        "testListExists",
        "testListCopyConstructorAssignment",
        "testListSortInt",
        "testListSortEvent"
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
