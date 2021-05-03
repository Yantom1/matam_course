#ifndef TEST_UTILITIES_H_
#define TEST_UTILITIES_H_

#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>
using std::ofstream;


bool compareFiles(const std::string& p1, const std::string& p2) {
  std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
  std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

  if (f1.fail() || f2.fail()) {
    return false; //file problem
  }

  if (f1.tellg() != f2.tellg()) {
    return false; //size mismatch
  }

  //seek back to beginning and use std::equal to compare contents
  f1.seekg(0, std::ifstream::beg);
  f2.seekg(0, std::ifstream::beg);
  return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(f2.rdbuf()));
}

void clearFile(std::string file_name){
    std::ifstream ifs (file_name);
    ifs.close ();
    ofstream ofs(file_name, ofstream::out | ofstream::trunc);
    ofs.close();
}

/**
 * These macros are here to help you create tests more easily and keep them
 * clear.
 *
 * The basic idea with unit-testing is create a test function for every real
 * function and inside the test function declare some variables and execute the
 * function under test.
 *
 * Use the ASSERT_TEST and ASSERT_TEST_WITH_FREE to verify correctness of
 * values.
 */


/**
 * Evaluates expr and continues if expr is true.
 * If expr is false, ends the test by returning false and prints a detailed
 * message about the failure.
 */
 #define ASSERT_TEST(expr, goto_label)                                                         \
     do {                                                                          \
         if (!(expr)) {                                                            \
             printf("\nAssertion failed at %s:%d %s ", __FILE__, __LINE__, #expr); \
             result = false;                                                       \
             goto goto_label;                                                         \
         }                                                                         \
     } while (0)



/**
 * Macro used for running a test from the main function
 */
#define RUN_TEST(test, name)                  \
    do {                                 \
      printf("Running %s ... ", name);   \
        if (test()) {                    \
            printf("[OK]\n");            \
        } else {                         \
            printf("[Failed]\n");        \
        }                                \
    } while (0)

#endif /* TEST_UTILITIES_H_ */
