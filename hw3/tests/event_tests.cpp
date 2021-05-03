#include <iostream>
#include <string>

#include "test_utilities.h"
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
 
using std::string;
using std::cout;
using std::endl;
using namespace mtm;

#define NUMBER_TESTS 11

template <class T> void print(const T& x) { cout << x << endl; }

struct StudentFilter {
    bool operator()(int student) {
        return student == 1 || student == 3 || student == 20000 || student == 60 || student == 150000;
    }
};



bool testEventCreateDestroy() {
    bool result = true;

    string name = "Hot event";
    OpenEvent event(DateWrap(), name);
    ClosedEvent event2(DateWrap(), name);
    CustomEvent<StudentFilter> event3(DateWrap(), name, StudentFilter());
    
    return result;
}

bool testOpenEventGeneral(){
    bool result = true;
    string name = "Hot event";
    DateWrap date(8,1,2021);
    OpenEvent event(date, name);
    OpenEvent event2 = *static_cast<OpenEvent *>(event.clone());

    ASSERT_TEST(event.getDate() == date, returnLabel);
    ASSERT_TEST(event.getName() == name, returnLabel);
    
    ASSERT_TEST(event2.getDate() == date, returnLabel);
    ASSERT_TEST(event2.getName() == name, returnLabel);

    ASSERT_TEST(event == event2, returnLabel);

returnLabel:
    return result;
}

bool testOpenEventRegister(){
    bool result = true;
    bool passed = true;
    string name = "Hot event";
    DateWrap date(8,1,2021);
    OpenEvent event(date, name);

    event.registerParticipant(6);
    event.registerParticipant(18);


    ASSERT_TEST(event.participantRegistered(6) == true, returnLabel);
    ASSERT_TEST(event.participantRegistered(18) == true, returnLabel);
    ASSERT_TEST(event.participantRegistered(24) == false, returnLabel);

    try{
        passed = false;
        event.registerParticipant(6);
    } catch(mtm::AlreadyRegistered& e){
        passed = true;
    } 
    ASSERT_TEST(passed, returnLabel);

    try{
        passed = false;
        event.registerParticipant(-1);
    } catch(mtm::InvalidStudent& e){
        passed = true;
    } 
    ASSERT_TEST(passed, returnLabel);
    

    try{
        passed = false;
        event.registerParticipant(1234567891);
    } catch(mtm::InvalidStudent& e){
        passed = true;
    } 
    ASSERT_TEST(passed, returnLabel);


returnLabel:
    return result;
}

bool testOpenEventUnregister(){
    bool result = true;
    bool passed = true;
    string name = "Hot event";
    DateWrap date(8,1,2021);
    OpenEvent event(date, name);

    event.registerParticipant(6);
    event.registerParticipant(18);

    event.unregisterParticipant(6);

    ASSERT_TEST(event.participantRegistered(6) == false, returnLabel);
    ASSERT_TEST(event.participantRegistered(18) == true, returnLabel);
    ASSERT_TEST(event.participantRegistered(24) == false, returnLabel);


    try{
        passed = false;
        event.unregisterParticipant(6);
    } catch(mtm::NotRegistered& e){
        passed = true;
    } 
    ASSERT_TEST(passed, returnLabel);


    try{
        passed = false;
        event.unregisterParticipant(-1);
    } catch(mtm::InvalidStudent& e){
        passed = true;
    } 
    ASSERT_TEST(passed, returnLabel);

    try{
        passed = false;
        event.unregisterParticipant(1234567891);
    } catch(mtm::InvalidStudent& e){
        passed = true;
    } 
    ASSERT_TEST(passed, returnLabel);

returnLabel:
    return result;
}

bool testClosedEventGeneral(){
    bool result = true;

    string name = "Cold event";
    DateWrap date(8,1,2021);
    ClosedEvent event(date, name);
    ClosedEvent event2 = *static_cast<ClosedEvent *>(event.clone());

    ASSERT_TEST(event.getDate() == date, returnLabel);
    ASSERT_TEST(event.getName() == name, returnLabel);
    
    ASSERT_TEST(event2.getDate() == date, returnLabel);
    ASSERT_TEST(event2.getName() == name, returnLabel);
    
returnLabel:
    return result;
}

bool testClosedEventRegister(){
    bool result = true;
    bool passed; // To check exceptions
    string name = "Cold event";
    DateWrap date(8,1,2021);
    ClosedEvent event(date, name);

    // Invitee tests
    for(int i = 1; i < 10; i++){
        event.addInvitee(i);
    }
    
    for(int i = 1; i < 10; i++){
        ASSERT_TEST(event.isInvited(i), returnLabel);
    }
    
    try{
        passed = false;
        event.addInvitee(2);
    } catch(mtm::AlreadyInvited& e){
        passed = true;
    }
    ASSERT_TEST(passed, returnLabel);

    try{
        passed = false;
        event.addInvitee(-20);
    } catch(mtm::InvalidStudent&){
        passed = true;
    }
    ASSERT_TEST(passed, returnLabel);

    try{
        passed = false;
        event.addInvitee(1234567891);
    } catch(mtm::InvalidStudent&){
        passed = true;
    }
    ASSERT_TEST(passed, returnLabel);
    

    
    // Participants tests

    for(int i = 1; i < 10; i++){
        event.registerParticipant(i);
    }



    for(int i = 1; i < 10; i++){
        ASSERT_TEST(event.participantRegistered(i), returnLabel);
    }

    try{
        passed = false;
        event.registerParticipant(3);
    } catch(mtm::AlreadyRegistered& e){
        passed = true;
    }
    ASSERT_TEST(passed, returnLabel);

    try{
        event.registerParticipant(200);
        passed = false;
    } catch(mtm::RegistrationBlocked& e){
        passed = true;
    }
    ASSERT_TEST(passed, returnLabel);

    try{
        event.registerParticipant(1234567891);
        passed = false;
    } catch(mtm::InvalidStudent& e){
        passed = true;
    }
    ASSERT_TEST(passed, returnLabel);

    try{
        event.registerParticipant(-20);
        passed = false;
    } catch(mtm::InvalidStudent& e){
        passed = true;
    }
    ASSERT_TEST(passed, returnLabel);




returnLabel:
    return result;
} 

bool testCustomEventGeneral(){
    bool result = true;
    string name = "Lukewarm event";
    DateWrap date(5,5,2021);
    CustomEvent<StudentFilter> event(date, name, StudentFilter());
    CustomEvent<StudentFilter> event2 = *static_cast<CustomEvent<StudentFilter> *>(event.clone());

    ASSERT_TEST(event.getDate() == date, returnLabel);
    ASSERT_TEST(event.getName() == name, returnLabel);
    
    ASSERT_TEST(event2.getDate() == date, returnLabel);
    ASSERT_TEST(event2.getName() == name, returnLabel);

    ASSERT_TEST(event == event2, returnLabel);


returnLabel:
    return result;
}

bool testCustomEventRegister(){
    bool result = true;
    bool passed = true;
    string name = "Lukewarm event";
    DateWrap date(5,5,2021);
    CustomEvent<StudentFilter> event(DateWrap(), name, StudentFilter());

    event.registerParticipant(1);
    event.registerParticipant(3);
    event.registerParticipant(20000);
    

    ASSERT_TEST(event.participantRegistered(1), returnLabel);
    ASSERT_TEST(event.participantRegistered(3), returnLabel);
    ASSERT_TEST(event.participantRegistered(20000), returnLabel);
    ASSERT_TEST(!event.participantRegistered(1050), returnLabel);

    try{
        event.registerParticipant(55);
        passed = false;
    } catch(mtm::RegistrationBlocked& e){
        passed = true;
    }
    ASSERT_TEST(passed, returnLabel);

    try{
        event.registerParticipant(1);
        passed = false;
    } catch(mtm::AlreadyRegistered& e){
        passed = true;
    }
    ASSERT_TEST(passed, returnLabel);

    try{
        event.registerParticipant(1234567891);
        passed = false;
    } catch(mtm::InvalidStudent& e){
        passed = true;
    }
    ASSERT_TEST(passed, returnLabel);

    try{
        event.registerParticipant(-20);
        passed = false;
    } catch(mtm::InvalidStudent& e){
        passed = true;
    }
    ASSERT_TEST(passed, returnLabel);


returnLabel:
    return result;
}

bool testOpenEventPrints(){
    bool result = true;
    string expected = "../tests/outputs/event_output1_short.txt";
    string expected2 = "../tests/outputs/event_output1_long.txt";
    string tested = "../tests/outputs/output.txt";
    string tested2 = "../tests/outputs/output2.txt";
    clearFile(tested);
    clearFile(tested2);
    ofstream output_file(tested);
    ofstream output_file2(tested2);

    string name = "Hot event";
    DateWrap date(8,1,2021);
    OpenEvent event(date, name);
    event.registerParticipant(6);
    event.registerParticipant(18);

    event.printShort(output_file);
    output_file.close();
    ASSERT_TEST(compareFiles(tested, expected), returnLabel);

    event.printLong(output_file2);
    output_file2.close();
    ASSERT_TEST(compareFiles(tested2, expected2), returnLabel);

    
returnLabel:
    return result;
}

bool testClosedEventPrints(){
    bool result = true;
    string expected = "../tests/outputs/event_output2_short.txt";
    string expected2 = "../tests/outputs/event_output2_long.txt";
    string tested = "../tests/outputs/output.txt";
    string tested2 = "../tests/outputs/output2.txt";
    clearFile(tested);
    clearFile(tested2);
    ofstream output_file(tested);
    ofstream output_file2(tested2);

    string name = "Mediocore event";
    DateWrap date(8,1,2021);

    ClosedEvent event(date, name);
    for(int i = 100; i > 0; i--){
        event.addInvitee(i*i);
        event.registerParticipant(i*i);
    }

    event.printShort(output_file);
    output_file.close();
    ASSERT_TEST(compareFiles(tested, expected), returnLabel);

    event.printLong(output_file2);
    output_file2.close();
    ASSERT_TEST(compareFiles(tested2, expected2), returnLabel);

returnLabel:
    return result;
}

bool testCustomEventPrints(){
    bool result = true;
    string expected = "../tests/outputs/event_output3_short.txt";
    string expected2 = "../tests/outputs/event_output3_long.txt";
    string tested = "../tests/outputs/output.txt";
    string tested2 = "../tests/outputs/output2.txt";
    clearFile(tested);
    clearFile(tested2);
    ofstream output_file(tested);
    ofstream output_file2(tested2);

    string name = "Very interesting event";
    DateWrap date(8,1,2021);

    CustomEvent<StudentFilter> event(date, name, StudentFilter());
    event.registerParticipant(20000);
    event.registerParticipant(1);
    event.registerParticipant(3);
    event.registerParticipant(60);
    event.registerParticipant(150000);

    event.printShort(output_file);
    output_file.close();
    ASSERT_TEST(compareFiles(tested, expected), returnLabel);

    event.printLong(output_file2);
    output_file2.close();
    ASSERT_TEST(compareFiles(tested2, expected2), returnLabel);

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
        testEventCreateDestroy,
        testOpenEventGeneral,
        testOpenEventRegister,
        testOpenEventUnregister,
        testClosedEventGeneral,
        testClosedEventRegister,
        testCustomEventGeneral,
        testCustomEventRegister,
        testOpenEventPrints,
        testClosedEventPrints,
        testCustomEventPrints     
};

const char* testNames[] = {
        "testEventCreateDestroy",
        "testOpenEventGeneral",
        "testOpenEventRegisterUnregister",
        "testOpenEventUnregister",
        "testClosedEventGeneral",
        "testClosedEventRegister",
        "testCustomEventGeneral",
        "testCustomEventRegister",
        "testOpenEventPrints",
        "testClosedEventPrints",
        "testCustomEventPrints"
        
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
