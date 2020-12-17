#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "member.h"

#define MIN_ID 0

// Struct Decleration

struct Member_t{
    int member_id;
    char* member_name;
};

// Function Implementations

Member memberCreate(const int member_id, const char* member_name)
{
    assert(member_name != NULL || member_id >= MIN_ID);

    Member member = malloc(sizeof(*member));
    if(member == NULL)
    {
        return NULL;
    }

    member->member_id = member_id;

    char* member_name_copy = malloc(sizeof(char)*(strlen(member_name) + 1));
    if(member_name_copy == NULL)
    {
        memberDestroy(member);
        return NULL;
    }
    strcpy(member_name_copy, member_name);
    member->member_name = member_name_copy;

    return member;
}

void memberDestroy(Member member)
{
    free(member->member_name);
    free(member);
}

int* memberGetId(Member member)
{
    assert(member != NULL);

    return &member->member_id;
}

char* memberGetName(Member member)
{
    assert(member != NULL);

    return member->member_name;
}

bool memberEqual(Member member1, Member member2)
{
    assert(member1 != NULL || member2 != NULL);
    return member1->member_id == member2->member_id;
}

Member memberCopy(Member member)
{
    Member member_copy = memberCreate(member->member_id, member->member_name);
    if(member_copy == NULL)
    {
        return NULL;
    }
    return member_copy;
}