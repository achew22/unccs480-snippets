#include <cmath>
#include <GL/glut.h>
#include <stdio.h>

#include "DeltaFunctor.h"

//Default constructor
Delta_Functor::Delta_Functor()
{
    totalStartTime = 0;
    totalEndTime = 0;
    totalTimeRange = 1; //We divide by this and it doesn't matter so to prevent error, set != 0
    startTime = 0;
    endTime = 0;
    timeRange = 1;
    startValue = 0;
    endValue = 0;
    valueRange = 1;
    exponent = 1;

    isCyclic = false;
    isBounceBack = false;
}

//Constructor. Pass in the start and end times for the function, as well as the beginning and ending values.
//The exp value is a control over how it varies: if exp > 1, the functor starts slow and ends fast. If
//0 < exp < 1, the functor starts fast and ends slow. exp = 1 is linear.
Delta_Functor::Delta_Functor(int start_time, int end_time, double start_value, double end_value, double exp)
{
    startTime = start_time;
    endTime = end_time;
    timeRange = endTime - startTime;
    totalStartTime = start_time;
    totalEndTime = end_time;
    totalTimeRange = endTime - startTime;
    startValue = start_value;
    endValue = end_value;
    valueRange = endValue - startValue;
    exponent = exp;
//    printf("Delta Functor initialized, startTime = %i, endTime = %i, exponent = %i\n", startTime, endTime, exponent);

    isCyclic = false;
    isBounceBack = false;
}

//Add a functor to this one
Delta_Functor Delta_Functor::addFunctor(const Delta_Functor toAdd)
{
    if (toAdd.totalStartTime < this->totalStartTime) {
        this->totalStartTime = toAdd.totalStartTime;
    }
    if (toAdd.totalEndTime > this->totalEndTime) {
        this->totalEndTime = toAdd.totalEndTime;
    }
    this->totalTimeRange = this->totalEndTime - this->totalStartTime;
    if (totalTimeRange == 0) {
        totalTimeRange = 1; //Don't divide by zero
    }
    functors.push_back(toAdd);
    return * this;
}

Delta_Functor Delta_Functor::operator +(const Delta_Functor& toAdd) {
    return addFunctor(toAdd);
}

//Used for linked list
bool Delta_Functor::operator <(const Delta_Functor& compare) {
    return this->totalStartTime < compare.totalStartTime;
}

//Used for linked list
bool Delta_Functor::operator >(const Delta_Functor& compare) {
    return this->totalStartTime > compare.totalStartTime;
}

/*
//Used for linked list
bool Delta_Functor::operator ==(Delta_Functor compare) {

    return
        startTime      == compare.startTime      &&
        endTime        == compare.endTime        &&
        timeRange      == compare.timeRange      &&
        totalStartTime == compare.totalStartTime &&
        totalEndTime   == compare.totalEndTime   &&
        totalTimeRange == compare.totalTimeRange &&
        startValue     == compare.startValue     &&
        endValue       == compare.endValue       &&
        valueRange     == compare.valueRange     &&
        exponent       == compare.exponent       &&
        functors       == compare.functors;

}
*/

//Set one functor equal to another
Delta_Functor Delta_Functor::operator =(const Delta_Functor toEqual)
{
    startTime = toEqual.startTime;
    endTime = toEqual.endTime;
    timeRange = toEqual.timeRange;
    totalStartTime = toEqual.totalStartTime;
    totalEndTime = toEqual.totalEndTime;
    totalTimeRange = toEqual.totalTimeRange;
    startValue = toEqual.startValue;
    endValue = toEqual.endValue;
    valueRange = toEqual.valueRange;
    exponent = toEqual.exponent;
    functors = toEqual.functors;
    return *this;
}

//Reset this functor so that starts over from the current time
void Delta_Functor::reset()
{
    startTime = glutGet(GLUT_ELAPSED_TIME);
    endTime = startTime + timeRange;

    isCyclic = false;
    isBounceBack = false;
}

//Make the functor have a small bounce back (not yet working)
void Delta_Functor::makeBounceBack()
{
    isBounceBack = true;
}

//Stop the bounce back
void Delta_Functor::unMakeBounceBack()
{
    isBounceBack = false;
}

//Make a functor cyclic (repeating) (not yet working)
void Delta_Functor::makeCyclic()
{
    isCyclic = true;
}

//Make a functor not cyclic
void Delta_Functor::unMakeCyclic()
{
    isCyclic = false;
}

//Shift the beginning and end times for the functor and all children
void Delta_Functor::shiftTime(int toShift) {

    startTime += toShift;
    endTime += toShift;

    totalStartTime += toShift;
    totalEndTime += toShift;

    for(std::list<Delta_Functor>::iterator i = functors.begin(); i != functors.end(); i++) {
        i->shiftTime(toShift);
    }
}

//Concatenate two functors
Delta_Functor Delta_Functor::cat(Delta_Functor toCat)
{
    //Move the time so that toCat's time begins when this functor ends
    toCat.shiftTime( -(toCat.totalStartTime - totalEndTime) );

    *this = *this + toCat;
    return * this;
}

//Reverse the current functor (DOESN'T WORK)
void Delta_Functor::reverse() {

    Delta_Functor * toReturn = new Delta_Functor();
    toReturn->cat(*this);

    /* This doesn't work! THIS IS PAIN * /
    functors.reverse();
    for (std::list<Delta_Functor>::iterator i = functors.begin(); i != functors.end(); i++) {
        int tmp = i->startValue;
        i->startValue = i->endValue;
        i->endValue = tmp;
        toReturn->cat(*i);
    }

    /**/

    *this = *toReturn;
    delete toReturn;
}

//Returns the current value of the functor, based on the current time
double Delta_Functor::getValue()
{
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    double toReturn = 0;
    if (startTime <= currentTime && endTime >= currentTime)
    {
        toReturn += pow((double)(currentTime - startTime)/(double)timeRange, exponent)*valueRange + startValue;
    }
    else if (isCyclic)
    {
        int cycleTime = (currentTime - startTime /*interval*/) % (int)timeRange;
        toReturn += pow((double)(cycleTime - startTime)/(double)timeRange, exponent)*valueRange + startValue;
    }
    else
    {
        toReturn = 0;
    }

    //Add on any other functors that are contained within this one
    for (std::list<Delta_Functor>::iterator i = functors.begin(); i != functors.end(); i++)
    {
        toReturn += i->getValue();
    }
    return toReturn;
}