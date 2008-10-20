#ifndef PATH_H
#define PATH_H

#include <vector>
#include "point.h"

/**
 * A class which allows one to define an arbitrary
 * path in space to be moved along over a given
 * time interval (similar to the way Delta_Functor
 * works, but with a Point3 instead of simply a
 * double). To use, first define the start and end
 * times. Then add all of the points on your path,
 * evenly spaced out. If you want it to be cyclic,
 * use makeCyclic(), otherwise don't. Now, to find
 * out where it is on the path at the current time,
 * simply use getPoint().
 */
class Path {
private:
    std::vector<Point3> points; ///< The points on the path.
    int startTime;  ///< The time to start along the path.
    int endTime;    ///< The time to end along the path.
    bool isCyclic;  ///< Whether or not to repeat in a cycle.
public:
    Path(); ///< Default constructor.
    Path(int timeToStart, int timeToEnd);   ///< Constructor.
    void setPoints(std::vector<Point3> toSet);  ///< Set the path points.
    void addPoint(Point3 toAdd);    ///< Add a path point to the end of the path.
    void setStartTime(int start);   ///< Set time to start.
    void setEndTime(int end);       ///< Set time to end.
    void makeCyclic();  ///< Make the path cyclic.
    void unMakeCyclic();    ///< Make the path non-cyclic.
    Point3 getPoint();  ///< Get the point at the current time.
    Point3 getPoint(int time);  ///< Get the point at the specified time.
};

#endif //PATH_H
