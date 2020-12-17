#ifndef MEMBER_H
#define MEMBER_H

typedef struct Member_t *Member;

/* memberCreate - creates a new member by his name and id
                return- NULL if dynamic allocation failed
                        else the new member created */
Member memberCreate(const int member_id, const char* member_name);

/*memberDestroy - frees the member sent to the function */
void memberDestroy(Member member);

/*memberCopy - makes a new copy of the given member
               return- NULL if dynamic allocation failed
                       else the copy of the member*/
Member memberCopy(Member member);

/*memberGetId - return the id number of the given member */
int* memberGetId(Member member);

/*memberGetName - return the name of the givem member */
char* memberGetName(Member member);

/*memberEqual - return TRUE if member1 equal to member2
                       FALSE if not  */
bool memberEqual(Member member1, Member member2);
#endif