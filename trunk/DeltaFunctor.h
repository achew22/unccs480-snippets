#ifndef DELTA_FUNCTOR_H
#define DELTA_FUNCTOR_H

#include <list>

//The functor class allows for a generic change in a variable to take place over
//a certain length of time. So if I want to say that at time 2000, theta is 0 degrees
//and at 4000, theta is 360 degrees, then this would make it possible.
//Use GetValue() to get the value for the current time.
//Note that the functor returns beginValue if it is before startTime
//and endValue if it is after endTime. This should be changed so that
//both of these calls are 0, since this would be nice for extension
//reasons.
class Delta_Functor
{
private:
    int totalStartTime;
    int totalEndTime;
    int totalTimeRange;
    int startTime;
    int endTime;
    double timeRange;
    double startValue;
    double endValue;
    double valueRange;
    double exponent;

    //Bools to tell what kind of functor it is
    bool isCyclic;
    bool isBounceBack;

    std::list<Delta_Functor> functors;
public:
    Delta_Functor();
    Delta_Functor(int start_time, int end_time, double start_value, double end_value, double exp);
    Delta_Functor operator=(const Delta_Functor toEqual);

    Delta_Functor addFunctor(const Delta_Functor toAdd);
	Delta_Functor operator +(const Delta_Functor& toAdd);

	bool operator >(const Delta_Functor&);
	bool operator <(const Delta_Functor&);
	//bool operator ==(Delta_Functor);

    void reverse();

    void shiftTime(int toShift);
    void reset();
    void makeBounceBack();
    void unMakeBounceBack();
    void makeCyclic();
    void unMakeCyclic();
    Delta_Functor cat(const Delta_Functor toCat);
    double getValue();
};

#endif //DELTA_FUNCTOR_H
