#include <cmath>
#include <GL/glut.h>
#include <stdio.h>

#include <vector>

#include "stopFunctor.h"

//Default constructor
StopFunctor::StopFunctor()
{
    cacheValue = 0;
    functor = new Delta_Functor();
}

//Constructor. Pass in the start and end times for the function, as well as the beginning and ending values.
//The exp value is a control over how it varies: if exp > 1, the functor starts slow and ends fast. If
//0 < exp < 1, the functor starts fast and ends slow. exp = 1 is linear.
StopFunctor::StopFunctor(int startTime, int endTime, double startValue, double endValue, double exp)
{
    cacheValue = 0;
    functor = new Delta_Functor(startTime, endTime, startValue, endValue, exp);
}

//Add a functor to this one
StopFunctor StopFunctor::addFunctor(const Delta_Functor toAdd)
{
    functor->addFunctor(toAdd);
    return *this;
}

//Add a stop functor to this one
StopFunctor StopFunctor::addFunctor(const StopFunctor toAdd)
{
    //functor->addFunctor(toAdd->functor);
    return *this;
}

StopFunctor StopFunctor::operator +(const Delta_Functor& toAdd) {
    return addFunctor(toAdd);
}

StopFunctor StopFunctor::operator +(const StopFunctor toAdd) {
    return addFunctor( *(toAdd.functor));
}

//Set one functor equal to another
StopFunctor StopFunctor::operator =(const StopFunctor& toEqual)
{
    functor = toEqual.functor;
    return *this;
}

//Reset this functor so that starts over from the current time
void StopFunctor::reset()
{
    functor->reset();
}

//Make the functor have a small bounce back (not yet working)
void StopFunctor::makeBounceBack()
{
    functor->makeBounceBack();
}

//Stop the bounce back
void StopFunctor::unMakeBounceBack()
{
    functor->unMakeBounceBack();
}

//Make a functor cyclic (repeating) (not yet working)
void StopFunctor::makeCyclic()
{
    functor->makeCyclic();
}

//Make a functor not cyclic
void StopFunctor::unMakeCyclic()
{
    functor->unMakeCyclic();
}

//Shift the beginning and end times for the functor and all children
void StopFunctor::shiftTime(int toShift) {
    functor->shiftTime(toShift);
}

//Concatenate two functors
StopFunctor StopFunctor::cat(const StopFunctor toCat)
{
    //Move the time so that toCat's time begins when this functor ends
    functor->cat(*(toCat.functor));
    return * this;
}

//Concatenate two functors
StopFunctor StopFunctor::cat(const Delta_Functor toCat)
{
    //Move the time so that toCat's time begins when this functor ends
    functor->cat(toCat);
    return * this;
}

//Reverse the current functor (DOESN'T WORK)
void StopFunctor::reverse() {
    functor->reverse();
}

//Returns the current value of the functor, based on the current time
double StopFunctor::getValue()
{
    if (cacheValue) {
        return cacheValue;
    }
    if (functor->isComplete()) {
        cacheValue = functor->getValue();
        //printf("%d", &(functor));
    }
    return functor->getValue();
}
