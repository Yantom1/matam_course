#include "../partA/date_wrap.h"
#include "../partA/exceptions.h"
#include <iostream>
#include <fstream>


using mtm::DateWrap;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using mtm::Exception;

const std::string FILE_PATH = "../../provided/testOutputs/partA";

#define ASSERT_TEST(expr)                                                         \
     do {                                                                          \
         if (!(expr)) {                                                            \
             cout << "\nAssertion failed at"<< __FILE__<< __LINE__ << #expr << endl; \
             result = false;                                                       \
         }                                                                         \
     } while (0);

#define RUN_TEST(test, name)                  \
    do {                                 \
        cout << "+ Running" << name << "...";  \
        if (test()) {                    \
            cout << "[OK]\n" << endl;         \
        } else {                         \
            cout << "[Failed]\n\n <span>To see what the test does and why it failed, please check the link at the top of the page to the test file</span>" << endl;       \
        }                                \
    } while (0);

class FileFailed {
public:
    FileFailed() = default;
    ~FileFailed() = default;
};


bool matchFiles(std::string out, std::string  exp) {
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
        output >> std::noskipws >>c;
        char ex;
        expOutput >> std::noskipws >> ex;
        if (ex != c) {
            return false;
        }
    }
    return true;
}

#define OPEN_FILE(streamName, name) std::string fileName = name;\
std::ofstream streamName(fileName, std::ofstream::trunc | std::ofstream::in);\
if(!(streamName).is_open()){\
throw FileFailed();\
}

template <class T> void print(const T& x, ofstream& stream) { stream << x << endl; }

#define ASSERT(expr) ASSERT_TEST(expr)


/**________________________________________________*/
/** HERE START THE TESTS*/

bool testConstructor() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testConstructor.txt"))
    DateWrap d1(1, 1, 2000);
    DateWrap d2(2, 1, 2000);
    DateWrap d3(3, 1, 2000);
    print(d1, out);
    print(d2, out);
    print(d3, out);
    try {
        DateWrap d4(0, 1, 2000);
    }
    catch (mtm::InvalidDate&) {
        out << "InvalidDate" << endl;
    }
    try {
        DateWrap d4(1, 0, 2000);
    }
    catch (mtm::InvalidDate&) {
        out << "InvalidDate" << endl;
    }
    DateWrap d4(1, 1, 1);
    print(d4, out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testConstructor.txt")))
    return result;
}

bool testBooleanOperators()
{
    bool result = true;
    DateWrap d1(1, 1, 2000);
    DateWrap d2(1, 1, 2000);
    DateWrap d3(2, 1, 2000);
    ASSERT(d3 > d1)
    ASSERT(d1 >= d2)
    ASSERT(d1 <= d2)
    ASSERT(d1 == d2)
    ASSERT(d1 < d3)
    ASSERT(d1 != d3)
    return result;
}

bool testArithmeticOperators() {
    bool result = true;
    DateWrap d1(1, 1, 2000);
    DateWrap d2(1, 1, 2000);
    DateWrap d3(2, 1, 2000);
    d1 += 1;
    ASSERT(d1 == d3)
    DateWrap d4(30, 2, 3);
    d4 += 1;
    ASSERT(d4.day() == 1)
    d1++;
    ASSERT(d4.month() == 3)
    ASSERT(d1.year() == 2000)
    return result;
}

bool testExceptions()
{
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testExceptions.txt"))

    DateWrap d1(1, 1, 2000);
    DateWrap d2(2, 1, 2000);
    DateWrap d3(3, 1, 2000);
    try {
        d1 += -1;
    }
    catch (mtm::NegativeDays&) {
        out << "NegativeDays" << endl;
    }
    try {
        d2 = d1 + (-1);
        print(d2, out);
    }
    catch (mtm::NegativeDays&) {
        out << "NegativeDays" << endl;
    }
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testExceptions.txt")))
    return result;
}

bool testDateWrapCreateDestroy_CreaterYan() {
    bool result = true;
    int day = 11, day2 = 25, month = 1, month2 = 8, year = 2021, year2 = 1990;
    DateWrap date(day, month, year);
    DateWrap date2(date);
    DateWrap date3(day2, month2, year2);

    ASSERT(date.day() == day)
    ASSERT(date.month() == month)
    ASSERT(date.year() == year)

    ASSERT(date2.day() == day)
    ASSERT(date2.month() == month)
    ASSERT(date2.year() == year)

    date2 = date3;
    ASSERT(date2.day() == day2)
    ASSERT(date2.month() == month2)
    ASSERT(date2.year() == year2)
    DateWrap date4(1, 1, 20);
    DateWrap date5(1, 1, 20);
    ASSERT(date5++ == date4)
    return result;
}

bool testLegalDate_CreatorYan() {
    bool result = true, passed = true;
    try {
        passed = false;
        DateWrap date(-3, 12, 4000);
    }
    catch (mtm::InvalidDate& e) {
        passed = true;
    }
    ASSERT(passed);
    try {
        passed = false;
        DateWrap date(40, 12, 4000);
    }
    catch (mtm::InvalidDate& e) {
        passed = true;
    }
    ASSERT(passed);
    try {
        passed = false;
        DateWrap date(1, -8, 4000);
    }
    catch (mtm::InvalidDate& e) {
        passed = true;
    }
    ASSERT(passed);
    try {
        passed = false;
        DateWrap date(1, 15, 4000);
    }
    catch (mtm::InvalidDate& e) {
        passed = true;
    }
    ASSERT(passed);
    return result;
}

bool testDateComparison_CreatorYan() {
    bool result = true;
    DateWrap date1(20, 12, 2020);
    DateWrap date2(30, 2, 2021);

    ASSERT(date1 == date1)
    ASSERT(date1 != date2)

    ASSERT(date1 < date2)
    ASSERT(date1 <= date2)
    ASSERT(date2 > date1)
    ASSERT(date2 >= date1)

    ASSERT(!(date1 == date2))
    ASSERT(!(date1 == date2))

    ASSERT(!(date1 >= date2))
    ASSERT(!(date1 > date2))
    ASSERT(!(date2 <= date1))
    ASSERT(!(date2 < date1))
    return result;
}

bool testDateIncrement_CreatorYan() {
    bool result = true, passed = true;
    int day = 30, month = 12, year = 2010;
    DateWrap date(day, month, year);

    date++;
    ASSERT(date.day() == 1)
    ASSERT(date.month() == 1)
    ASSERT(date.year() == 2011)

    try {
        passed = false;
        date += -3;
    }
    catch (mtm::NegativeDays& e) {
        passed = true;
    }
    ASSERT(passed)

    date += 3;

    ASSERT(date.day() == 4)
    ASSERT(date.month() == 1)
    ASSERT(date.year() == 2011)

    DateWrap date2 = date + 10;
    ASSERT(date2.day() == 14)
    ASSERT(date2.month() == 1)
    ASSERT(date2.year() == 2011)

    date2 = 30 + date;
    ASSERT(date2.day() == 4)
    ASSERT(date2.month() == 2)
    ASSERT(date2.year() == 2011)
    return result;
}


bool testDatePrint_CreatorYan() {
    bool result = true;
    OPEN_FILE(out, FILE_PATH + std::string("/your_outputs/testDatePrint_CreatorYan.txt"))
    DateWrap date(4, 11, 2011);
    print(date, out);
    out.close();
    ASSERT(matchFiles(fileName, FILE_PATH + std::string("/expected/testDatePrint_CreatorYan.txt")))
    return result;
}
const int NUMBER_OF_TESTS = 9;

#define TEST_NAMES \
    X(testConstructor) \
    X(testBooleanOperators) \
    X(testArithmeticOperators) \
    X(testExceptions)  \
    X(testDateWrapCreateDestroy_CreaterYan) \
    X(testLegalDate_CreatorYan)\
    X(testDateComparison_CreatorYan)        \
    X(testDateIncrement_CreatorYan)               \
    X(testDatePrint_CreatorYan)



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
