#include "test_utilities.h"
#include "../partA/date_wrap.h"
#include "../partA/exceptions.h"
#include <iostream>
#include <fstream>
#include <string>
using std::string;
using std::cout;
using std::ofstream;
using std::endl;
using namespace mtm;

#define NUMBER_TESTS 5

bool testDateWrapCreateDestroy() {
    bool result = true;
    int day = 11, day2 = 25 , month = 1, month2 = 8, year = 2021, year2 = 1990;
    DateWrap date(day,month, year);
    DateWrap date2(date);
    DateWrap date3(day2, month2, year2);

    ASSERT_TEST(date.day() == day, returnLabel);
    ASSERT_TEST(date.month() == month, returnLabel);
    ASSERT_TEST(date.year() == year, returnLabel);
    
    ASSERT_TEST(date2.day() == day, returnLabel);
    ASSERT_TEST(date2.month() == month, returnLabel);
    ASSERT_TEST(date2.year() == year, returnLabel);

    date2 = date3;
    ASSERT_TEST(date2.day() == day2, returnLabel);
    ASSERT_TEST(date2.month() == month2, returnLabel);
    ASSERT_TEST(date2.year() == year2, returnLabel);
    DateWrap date4(1,1, 20);
    DateWrap date5(1,1, 20);
    ASSERT_TEST(date5++ == date4, returnLabel);


returnLabel:
    return result;
}

bool testLegalDate(){
    bool result = true, passed = true;
    
    try{
        passed = false;
        DateWrap date(-3, 12, 4000);    
    } catch(mtm::InvalidDate& e){
        passed = true;
    } 
    ASSERT_TEST(passed, returnLabel);

    try{
        passed = false;
        DateWrap date(40, 12, 4000);    
    } catch(mtm::InvalidDate& e){
        passed = true;
    } 
    ASSERT_TEST(passed, returnLabel);

    try{
        passed = false;
        DateWrap date(1, -8, 4000);    
    } catch(mtm::InvalidDate& e){
        passed = true;
    } 
    ASSERT_TEST(passed, returnLabel);

    try{
        passed = false;
        DateWrap date(1, 15, 4000);    
    } catch(mtm::InvalidDate& e){
        passed = true;
    } 
    ASSERT_TEST(passed, returnLabel);
returnLabel:
    return result;
}

bool testDateComparison(){
    bool result = true;
    DateWrap date1(20,12,2020);
    DateWrap date2(30,2,2021);

    ASSERT_TEST(date1 == date1, returnLabel);
    ASSERT_TEST(date1 != date2, returnLabel);

    ASSERT_TEST(date1 < date2, returnLabel);
    ASSERT_TEST(date1 <= date2, returnLabel);
    ASSERT_TEST(date2 > date1, returnLabel);
    ASSERT_TEST(date2 >= date1, returnLabel);

    ASSERT_TEST(!(date1 == date2), returnLabel);
    ASSERT_TEST(!(date1 == date2), returnLabel);

    ASSERT_TEST(!(date1 >= date2), returnLabel);
    ASSERT_TEST(!(date1 > date2), returnLabel);
    ASSERT_TEST(!(date2 <= date1), returnLabel);
    ASSERT_TEST(!(date2 < date1), returnLabel);

returnLabel:
    return result;
}

bool testDateIncrement(){
    bool result = true, passed = true;
    int day = 30, month = 12, year = 2010;
    DateWrap date(day, month, year);
    DateWrap date2;

    date++;
    ASSERT_TEST(date.day() == 1, returnLabel);
    ASSERT_TEST(date.month() == 1, returnLabel);
    ASSERT_TEST(date.year() == 2011, returnLabel);

    try{
        passed = false;
        date += -3; 
    } catch(mtm::NegativeDays& e){
        passed = true;
    } 
    ASSERT_TEST(passed, returnLabel);

    date += 3;

    ASSERT_TEST(date.day() == 4, returnLabel);
    ASSERT_TEST(date.month() == 1, returnLabel);
    ASSERT_TEST(date.year() == 2011, returnLabel);

    date2 = date + 10;
    ASSERT_TEST(date2.day() == 14, returnLabel);
    ASSERT_TEST(date2.month() == 1, returnLabel);
    ASSERT_TEST(date2.year() == 2011, returnLabel);

    date2 = 30 + date;
    ASSERT_TEST(date2.day() == 4, returnLabel);
    ASSERT_TEST(date2.month() == 2, returnLabel);
    ASSERT_TEST(date2.year() == 2011, returnLabel);


returnLabel:
    return result;
}

bool testDatePrint(){
    bool result = true;
    string expected = "../tests/outputs/date_output.txt";
    string tested = "../tests/outputs/output.txt";
    DateWrap date(4,11,2011);
    clearFile(tested);
    ofstream output_file(tested);
    output_file << date;
    output_file.close();
    ASSERT_TEST(compareFiles(tested, expected), returnLabel);

returnLabel:
    return result;
}

/* bool test(){
    bool result = true;

returnLabel:
    return result;
} */

bool (*tests[]) (void) = {
        testDateWrapCreateDestroy,
        testLegalDate,
        testDateComparison,
        testDateIncrement,
        testDatePrint
};

const char* testNames[] = {
        "testDateWrapCreateDestroy",
        "testLegalDate",
        "testDateComparison",
        "testDateIncrement",
        "testDatePrint"
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
