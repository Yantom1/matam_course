#include <iostream>
#include "date_wrap.h"
#include "exceptions.h"

extern "C"
{
  #include "date.h"  
}

using mtm::DateWrap;
using std::ostream;

/*
    isDateLegal - returns true or false whether date is legal
*/
bool DateWrap::isDateLegal(int day, int month , int year)
{
    if(day > max_day || day < min_day || month > max_month || month < min_month)
    {
        return false;
    }

    return true;
}

DateWrap::DateWrap(int day, int month, int year){
    if(!isDateLegal(day, month, year)){
        throw InvalidDate();
    }
    date = dateCreate(day, month, year);
}

DateWrap::~DateWrap(){
    dateDestroy(date);
}

DateWrap::DateWrap(const DateWrap& date2){
    date = dateCreate(date2.day(), date2.month(), date2.year());
}

DateWrap& DateWrap::operator=(const DateWrap& date_wrap){
    if(*this == date_wrap){
        return *this;
    }
    dateDestroy(date);
    date = dateCreate(date_wrap.day(), date_wrap.month(), date_wrap.year());

    return *this;
}

int DateWrap::day() const{
    int day, month, year;
    dateGet(date, &day, &month, &year);
    return day;
}

int DateWrap::month() const{
    int day, month, year;
    dateGet(date, &day, &month, &year);
    return month;
}

int DateWrap::year() const{
    int day, month, year;
    dateGet(date, &day, &month, &year);
    return year;
}

ostream& mtm::operator<<(ostream& os, const DateWrap& date){
    return os << date.day() << "/" << date.month() << "/" << date.year();
}

bool DateWrap::operator==(const DateWrap& date1) const{
    return dateCompare(this->date, date1.date) == 0;
}

bool mtm::operator!=(const DateWrap& date1, const DateWrap& date2){
    return ! (date1 == date2);
}

bool mtm::operator<=(const DateWrap& date1, const DateWrap& date2){
    return date2 > date1 || date1 == date2;
}

bool DateWrap::operator>(const DateWrap& date1) const{
    return dateCompare(this->date, date1.date) > 0;
}

bool mtm::operator>=(const DateWrap& date1, const DateWrap& date2){
    return date1 > date2 || date1 == date2;
}

bool mtm::operator<(const DateWrap& date1, const DateWrap& date2){
    return date2 > date1;
}

DateWrap DateWrap::operator++(int){
    DateWrap pervious_date(*this);
    dateTick(date);
    return pervious_date; 
}

DateWrap& DateWrap::operator+=(int num){

    if(num < 0){
        throw NegativeDays();
    }
    for(int i = 0; i < num; i++){
        dateTick(date);
    }
    return *this;
}

DateWrap mtm::operator+(DateWrap& date ,int num){
    DateWrap new_date = date;
    return new_date += num;
}

DateWrap mtm::operator+(int num, DateWrap& date){
    return date + num;
}