#ifndef STOPFUNCTOR_H_INCLUDED
#define STOPFUNCTOR_H_INCLUDED

#include "DeltaFunctor.h"

class StopFunctor {
private:
    Delta_Functor * functor;
    double cacheValue;
public:
    StopFunctor();
    StopFunctor(int startTime, int endTime, double startValue, double endValue, double exp);
    StopFunctor operator=(const Delta_Functor& toEqual);
    StopFunctor operator=(const StopFunctor& toEqual);

    StopFunctor addFunctor(const Delta_Functor toAdd);
	StopFunctor operator +(const Delta_Functor& toAdd);
    StopFunctor addFunctor(const StopFunctor toAdd);
	StopFunctor operator +(const StopFunctor toAdd);

    //Return the value
    //double operator()();
    double getValue();

    void reverse();

    void shiftTime(int toShift);
    void reset();
    void makeBounceBack();
    void unMakeBounceBack();
    void makeCyclic();
    void unMakeCyclic();

    bool isComplete();

    StopFunctor cat(const Delta_Functor toCat);
    StopFunctor cat(const StopFunctor toCat);


};

#endif // STOPFUNCTOR_H_INCLUDED
