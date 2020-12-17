#include <stdlib.h>
#include "test_utilities.h"
#include "../priority_queue.h"


#define PQ PriorityQueue
#define NUMBER_TESTS 1
static PQElementPriority copyIntGeneric(PQElementPriority n) {
    if (!n) {
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(int *) n;
    return copy;
}

static void freeIntGeneric(PQElementPriority n) {
    free(n);
}

static int compareIntsGeneric(PQElementPriority n1, PQElementPriority n2) {
    return (*(int *) n1 - *(int *) n2);
}

static bool equalIntsGeneric(PQElementPriority n1, PQElementPriority n2) {
    return *(int *) n1 == *(int *) n2;
}


int *randInt() {
    int *num = malloc(sizeof(int));
    *num = rand() % 50000;
    return num;
}

PriorityQueue createPQ() {
    PriorityQueue pq = pqCreate(copyIntGeneric, freeIntGeneric, equalIntsGeneric, copyIntGeneric, freeIntGeneric,
                                compareIntsGeneric);
    return pq;
}

PriorityQueue getSingleElementPQ() {
    PriorityQueue pq = createPQ();
    int *element = randInt();
    int *priority = randInt();
    pqInsert(pq, element, priority);
    free(element);
    free(priority);
    return pq;
}

PriorityQueue getMultipleElementPQ() {
    PriorityQueue pq = createPQ();
    for (int i = 0; i < 10; i++) {
        int *element = randInt();
        int *priority = randInt();
        pqInsert(pq, element, priority);
        free(element);
        free(priority);
    }
    return pq;
}

bool testPQCreateDestroy() {
    bool result = true;

    PriorityQueue pq = pqCreate(copyIntGeneric, freeIntGeneric, equalIntsGeneric, copyIntGeneric, freeIntGeneric, compareIntsGeneric);
    ASSERT_TEST(pq != NULL, returnPQCreateDestroy);
    ASSERT_TEST(pqGetSize(pq) == 0,destroyPQCreateDestroy);
    ASSERT_TEST(pqGetFirst(pq) == NULL,destroyPQCreateDestroy);

destroyPQCreateDestroy:
    pqDestroy(pq);
returnPQCreateDestroy:
    return result;
}

bool testPQInsertAndSize() {
    bool result = true;
    PriorityQueue pq = pqCreate(copyIntGeneric, freeIntGeneric, equalIntsGeneric, copyIntGeneric, freeIntGeneric, compareIntsGeneric);
    ASSERT_TEST(pqGetSize(pq) == 0,destroyPQInsertAndSize);
    int to_add = 1;
    ASSERT_TEST(pqInsert(pq, &to_add, &to_add) == PQ_SUCCESS,destroyPQInsertAndSize);
    ASSERT_TEST(pqGetSize(pq) == 1,destroyPQInsertAndSize);

destroyPQInsertAndSize:
    pqDestroy(pq);
    return result;
}

bool testPQGetFirst() {
    bool result = true;
    PriorityQueue pq = pqCreate(copyIntGeneric, freeIntGeneric, equalIntsGeneric, copyIntGeneric, freeIntGeneric, compareIntsGeneric);
    ASSERT_TEST(pqGetSize(pq) == 0, destroyPQGetFirst);
    int to_add = 1;
    ASSERT_TEST(pqInsert(pq, &to_add, &to_add) == PQ_SUCCESS, destroyPQGetFirst);
    int* first_value = pqGetFirst(pq);
    ASSERT_TEST(first_value != NULL, destroyPQGetFirst);
    ASSERT_TEST(*first_value == to_add, destroyPQGetFirst);

destroyPQGetFirst:
    pqDestroy(pq);
    return result;
}

bool testPQIterator() {
    bool result = true;
    PriorityQueue pq = pqCreate(copyIntGeneric, freeIntGeneric, equalIntsGeneric, copyIntGeneric, freeIntGeneric, compareIntsGeneric);

    int max_value = 10;

    for(int i=0; i< max_value; i++){
        ASSERT_TEST(pqInsert(pq, &i, &i) == PQ_SUCCESS, destroyPQIterator);
    }

    int i = 0;
    PQ_FOREACH(int*, iter, pq) {
        if (i != max_value) {
            ASSERT_TEST(iter != NULL,destroyPQIterator);
        } else {
            ASSERT_TEST(iter == NULL,destroyPQIterator);
        }
        i++;
    }

destroyPQIterator:
    pqDestroy(pq);
    return result;
}

bool testPQCopyMultipleElement() {
    bool result = true;
    PQ pq = getMultipleElementPQ();
    PQ new_pq = pqCopy(pq);
    ASSERT_TEST(new_pq != NULL, destroy);

    int *new_pq_current_elem = pqGetFirst(new_pq);
    PQ_FOREACH(int *, pq_current_elem, pq) {
        ASSERT_TEST(compareIntsGeneric(pq_current_elem, new_pq_current_elem) == 0, destroy);
        ASSERT_TEST(pq_current_elem != new_pq_current_elem,
                    destroy); // Copy should create a new copy so the pointers shouldn't be pointing to the same address
        new_pq_current_elem = pqGetNext(new_pq);
    }

    destroy:
    pqDestroy(pq);
    pqDestroy(new_pq);
    return result;
}

bool testPQContainsFalseAfterRemove() {
    bool result = true;
    PQ pq = createPQ();

    int *elem = randInt();
    pqInsert(pq, elem, elem);
    ASSERT_TEST(pqContains(pq, elem) == true, destroy);

    pqRemove(pq);
    ASSERT_TEST(pqContains(pq, elem) == false, destroy);

    destroy:
    free(elem);
    pqDestroy(pq);
    return result;
}

bool testPQChangePriorityNonExistentElement() {
    bool result = true;
    PQ pq = getMultipleElementPQ();

    int elem1 = 10;
    int old_prio = 20;
    int new_prio = 15;
    ASSERT_TEST(pqChangePriority(pq, &elem1, &old_prio, &new_prio) == PQ_ELEMENT_DOES_NOT_EXISTS, destroy);

    pqInsert(pq, &elem1, &old_prio);
    int wrong_old_prio = 1001;
    ASSERT_TEST(pqChangePriority(pq, &elem1, &wrong_old_prio, &new_prio) == PQ_ELEMENT_DOES_NOT_EXISTS, destroy);

    int wrong_elem = 1001;
    ASSERT_TEST(pqChangePriority(pq, &wrong_elem, &old_prio, &new_prio) == PQ_ELEMENT_DOES_NOT_EXISTS, destroy);

    destroy:
    pqDestroy(pq);
    return result;
}

bool testPQChangePriorityWOnlyChangesFirstOfMultipleSameElementsAndReinsertsIt() {
    bool result = true;
    PQ pq = createPQ();

    int prio = 1;
    int elem = 1;
    pqInsert(pq, &elem, &prio);
    pqInsert(pq, &elem, &prio);
    pqInsert(pq, &elem, &prio);

    int mid_elem = 5;
    int mid_prio = 5;
    pqInsert(pq, &mid_elem, &mid_prio);

    int elem2 = 10;
    int prio2 = 10;
    pqInsert(pq, &elem2, &prio2);

    int new_prio = prio2;
    ASSERT_TEST(pqChangePriority(pq, &elem, &prio, &new_prio) == PQ_SUCCESS, destroy);

    ASSERT_TEST((*(int *) pqGetFirst(pq)) == elem2, destroy); // Original elem2
    ASSERT_TEST((*(int *) pqGetNext(pq)) == elem, destroy); // Reinserted change prio
    ASSERT_TEST((*(int *) pqGetNext(pq)) == mid_elem, destroy); // Mid Elem
    ASSERT_TEST((*(int *) pqGetNext(pq)) == elem, destroy); // Left overs
    ASSERT_TEST((*(int *) pqGetNext(pq)) == elem, destroy);

    destroy:
    pqDestroy(pq);
    return result;
}
    


bool (*tests[]) (void) = {
        /* testPQCreateDestroy,
        testPQInsertAndSize,
        testPQGetFirst,
        testPQIterator,
        testPQCopyMultipleElement,
        testPQContainsFalseAfterRemove,
        testPQChangePriorityNonExistentElement, */
        testPQChangePriorityWOnlyChangesFirstOfMultipleSameElementsAndReinsertsIt
};

const char* testNames[] = {
        /* "testPQCreateDestroy",
        "testPQInsertAndSize",
        "testPQGetFirst",
        "testPQIterator",
        "testPQCopyMultipleElement",
        "testPQContainsFalseAfterRemove",
        "testPQChangePriorityNonExistentElement", */
        "testPQChangePriorityWOnlyChangesFirstOfMultipleSameElementsAndReinsertsIt"
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
