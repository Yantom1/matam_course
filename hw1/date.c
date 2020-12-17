#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "date.h"

#define MAX_DAY 30
#define MAX_MONTH 12
#define FIRST_DAY 1
#define FIRST_MONTH 1
#define DATES_EQUAL 0
#define DATE1_FIRST 1
#define DATE2_FIRST -1

// Struct decleration

struct Date_t {
    int day;
    int month;
    int year;
};

/*
* isDateLegal: checks whether the inserted legal
* return - bool value whether date is legal
*/
static bool isDateLegal(int day, int month , int year);

// Function implementations

Date dateCreate(int day, int month, int year)
{
    if(!isDateLegal(day, month, year)){
        return NULL;
    }
    
    Date date = malloc(sizeof(*date));
    if(date == NULL)
    {
        return NULL;
    }
    date->day = day;
    date->month = month;
    date->year = year;

    return date;
}

void dateDestroy(Date date)
{
    if(date == NULL)
    {
        return;
    }
    free(date);
}

Date dateCopy(Date date)
{
    if(date == NULL)
    {
        return NULL;
    }
    Date copy_date = dateCreate(date->day, date->month, date->year);
    if(copy_date == NULL)
    {
        return NULL;
    }
    return copy_date;
}

bool dateGet(Date date, int* day, int* month, int* year)
{
    if(date == NULL || day == NULL || month == NULL || year == NULL)
    {
        return false;
    }
    *day = date->day;
    *month = date->month;
    *year = date->year;
  
    return true;
}

int dateCompare(Date date1, Date date2)
{
    if(date1 == NULL || date2 == NULL)
    {
        return DATES_EQUAL;
    }
    
    if(date1->year > date2->year){
        return DATE1_FIRST;
    } else if(date1->year < date2->year){
        return DATE2_FIRST;
    } else {
        if(date1->month > date2->month){
            return DATE1_FIRST;
        } else if(date1->month < date2->month){
            return DATE2_FIRST;
        } else {
            if(date1->day > date2->day){
                return DATE1_FIRST;
            } else if(date1->day < date2->day){
                return DATE2_FIRST;
            } else {
                return DATES_EQUAL;
            }
        }
    }
}

void dateTick(Date date)
{
    if(date == NULL)
    {
        return;
    }
    if(date->day == MAX_DAY && date->month == MAX_MONTH)
    {
        date->year++;
        date->month = FIRST_MONTH;
        date->day = FIRST_DAY;
        return;
    }
    if(date->day == MAX_DAY)
    {
        date->day = FIRST_DAY;
        date->month++;
        return;
    }
    date->day++;
    return;
}

static bool isDateLegal(int day, int month , int year)
{
    if(day > MAX_DAY || day < FIRST_DAY || month > MAX_MONTH || month < FIRST_MONTH)
    {
        return false;
    }

    return true;
}