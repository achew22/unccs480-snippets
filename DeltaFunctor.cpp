#include <cmath>
#include <GL/glut.h>
#include <stdio.h>

#include "DeltaFunctor.h"

//Default constructor
Delta_Functor::Delta_Functor()
{
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
    startValue = start_value;
    endValue = end_value;
    valueRange = endValue - startValue;
    exponent = exp;
//    printf("Delta Functor initialized, startTime = %i, endTime = %i, exponent = %i\n", startTime, endTime, exponent);

    isCyclic = false;
    isBounceBack = false;
}

//Add a functor to this one (does not yet work)
void Delta_Functor::addFunctor(const Delta_Functor toAdd)
{
    functors.push_back(toAdd);
}

//Set one functor equal to another
Delta_Functor Delta_Functor::operator =(const Delta_Functor toEqual)
{
    startTime = toEqual.startTime;
    endTime = toEqual.endTime;
    timeRange = toEqual.timeRange;
    startValue = toEqual.startValue;
    endValue = toEqual.endValue;
    valueRange = toEqual.valueRange;
    exponent = toEqual.exponent;
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

//Concatenate two functors (not yet working)
Delta_Functor Delta_Functor::cat(Delta_Functor toCat)
{
    toCat.startTime = endTime;
    toCat.endTime = toCat.startTime + toCat.timeRange;
    //Finish here
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
    else if(startTime >= currentTime)
    {
        toReturn += startValue;
    }
    else
    {
        toReturn += endValue;
    }

    //Add on any other functors that are contained within this one
    for (int i = 0; i < functors.size(); i++)
    {
        toReturn += functors[i].GetValue();
    }
    return toReturn;
}
