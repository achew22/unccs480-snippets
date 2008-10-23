#ifndef PATH_H
#define PATH_H

#include <vector>
#include <cmath>
#include "point.h"
#include "SDLLoader.h"


/**
 * A template class which allows one to define an arbitrary
 * path to be moved along over a given
 * time interval. To use, first define the start and end
 * times. Then add all of the points on your path,
 * evenly spaced out. If you want it to be cyclic,
 * use makeCyclic(), otherwise don't. Now, to find
 * out where it is on the path at the current time,
 * simply use getPoint().
 *
 * The type of path used can be any element for which
 * addition, subtraction, scalar multiplication, and
 * scalar division have been overloaded. Some examples
 * that would make good paths would include double,
 * Point2, Point3.
 */
template<typename Type>
class Path {
private:
    std::vector<Type> points; ///< The points on the path.
    int startTime;  ///< The time to start along the path.
    int endTime;    ///< The time to end along the path.
    bool isCyclic;  ///< Whether or not to repeat in a cycle.
    bool isFinished;    ///< Whether or not the path has finished.
public:
    Path(); ///< Default constructor.
    Path(int timeToStart, int timeToEnd);   ///< Constructor.
    Path operator =(Path toEqual);  ///< Overloaded equals operator.
    bool getIsFinished();  ///< Find out whether or not the path has finished.
    void setPoints(std::vector<Type> toSet);  ///< Set the path points.
    void addPoint(Type toAdd);    ///< Add a path point to the end of the path.
    //void addPoint(double x, double y, double z);    ///< Add a path point to the end of the path.
    void setStartTime(int start);   ///< Set time to start.
    void setEndTime(int end);       ///< Set time to end.
    void makeCyclic();  ///< Make the path cyclic.
    void unMakeCyclic();    ///< Make the path non-cyclic.
    Type getPoint();  ///< Get the point at the current time.
    Type getPoint(int time);  ///< Get the point at the specified time.
};

/**
 * The default constructor sets startTime and endTime to zero,
 * and isCyclic to false.
 */
template<typename Type>
Path<Type>::Path(){
    startTime = endTime = 0;
    isCyclic = false;
    isFinished = false;
}

/**
 * Manually set the start and end times, which isCyclic is
 * by default set to false.
 *
 * @param timeToStart is the time that you want the path to start at.
 * @param timeToEnd is the time that you want the path to end at.
 */
template<typename Type>
Path<Type>::Path(int timeToStart, int timeToEnd) {
    startTime = timeToStart;
    endTime = timeToEnd;
    isCyclic = false;
    isFinished = false;
}

/**
 * @param toEqual is the Path that you want this path to be equal to.
 * @return returns the newly reset Path.
 */
template<typename Type>
Path<Type> Path<Type>::operator =(Path<Type> toEqual) {
    startTime = toEqual.startTime;
    endTime = toEqual.endTime;
    isCyclic = toEqual.isCyclic;
    isFinished = toEqual.isFinished;

    points.resize(toEqual.points.size());
    for (int i = 0; i < toEqual.points.size(); i++) {
        points[i] = toEqual.points[i];
    }

    return *this;
}

/**
 * @return returns a copy of isFinished.
 */
template<typename Type>
bool Path<Type>::getIsFinished() {
    return isFinished;
}

/**
 * Set all of the points in your path at once, as
 * opposed to adding them one at a time with addPoint().
 *
 * @param toSet is the vector of points to add.
 */
template<typename Type>
void Path<Type>::setPoints(std::vector<Type> toSet) {
    points = toSet;
}

/**
 * Add a point to the list of points that constitute your
 * path.
 *
 * @param toAdd is the point to add.
 */
template<typename Type>
void Path<Type>::addPoint(Type toAdd) {
    points.push_back(toAdd);
}

/**
 * @param start is the time to start at.
 */
template<typename Type>
void Path<Type>::setStartTime(int start) {
    startTime = start;
}

/**
 * @param end is the time to end at.
 */
template<typename Type>
void Path<Type>::setEndTime(int end) {
    endTime = end;
}

/**
 * This function will cause the path to repeat itself,
 * so that when it reaches the end of the path, it
 * starts over at the beginning.
 */
template<typename Type>
void Path<Type>::makeCyclic() {
    isCyclic = true;
    if (isFinished) {
        isFinished = false;
    }
}

/**
 * This function will cause the path to be non-repeating,
 * so that once endTime is reached, the point stops moving.
 * This is the default behaviour of the class.
 */
template<typename Type>
void Path<Type>::unMakeCyclic() {
    isCyclic = false;
}

/**
 * Use this function to find out where in the path
 * your point currently lies (at the current time).
 *
 * @return returns the current location along the path.
 */
template<typename Type>
Type Path<Type>::getPoint() {
    return getPoint(SDLLoader::getTime());
}

/**
 * Use this function to find out where in the path
 * your point lies (at the given time).
 *
 * @param time is the time at which you wish to find the points location.
 * @return returns the location along the path at time.
 */
template<typename Type>
Type Path<Type>::getPoint(int time) {
    if (points.size() == 0) {
        return Type();
    }

    int timeRange = endTime - startTime;
    int timeLocation = 0;

    if (isCyclic) {
        timeLocation = (time - startTime) % timeRange;
    } else {
        timeLocation = (time - startTime);
    }

    double timeStep = (double)timeRange/(points.size() - 1*(!isCyclic));

    if (timeLocation < 0) {
        return Type();
    } else if (timeLocation > endTime) {
        printf("Changed the stated of isFinished\n");
        isFinished = true;
        return Type();
    } else {
        isFinished = false;
        double numSteps = (double)timeLocation / timeStep;
        int beforeIndex = floor(numSteps);
        int afterIndex = ceil(numSteps);
        if (afterIndex >= points.size()) {
            afterIndex = 0;
        }
        return points[beforeIndex] + (points[afterIndex] - points[beforeIndex])*(numSteps - beforeIndex);
    }
}

#endif //PATH_H
