#include <cmath>

#include "SDLLoader.h"
#include "path.h"

/**
 * The default constructor sets startTime and endTime to zero,
 * and isCyclic to false.
 */
Path::Path(){
    startTime = endTime = 0;
    isCyclic = false;
}

/**
 * Manually set the start and end times, which isCyclic is
 * by default set to false.
 *
 * @param timeToStart is the time that you want the path to start at.
 * @param timeToEnd is the time that you want the path to end at.
 */
Path::Path(int timeToStart, int timeToEnd) {
    startTime = timeToStart;
    endTime = timeToEnd;
    isCyclic = false;
}

/**
 * Set all of the points in your path at once, as
 * opposed to adding them one at a time with addPoint().
 *
 * @param toSet is the vector of points to add.
 */
void Path::setPoints(std::vector<Point3> toSet) {
    points = toSet;
}

/**
 * Add a point to the list of points that constitute your
 * path.
 *
 * @param toAdd is the point to add.
 */
void Path::addPoint(Point3 toAdd) {
    points.push_back(toAdd);
}

/**
 * @param start is the time to start at.
 */
void Path::setStartTime(int start) {
    startTime = start;
}

/**
 * @param end is the time to end at.
 */
void Path::setEndTime(int end) {
    endTime = end;
}

/**
 * This function will cause the path to repeat itself,
 * so that when it reaches the end of the path, it
 * starts over at the beginning.
 */
void Path::makeCyclic() {
    isCyclic = true;
}

/**
 * This function will cause the path to be non-repeating,
 * so that once endTime is reached, the point stops moving.
 * This is the default behaviour of the class.
 */
void Path::unMakeCyclic() {
    isCyclic = false;
}

/**
 * Use this function to find out where in the path
 * your point currently lies (at the current time).
 *
 * @return returns the current location along the path.
 */
Point3 Path::getPoint() {
    return getPoint(SDLLoader::getTime());
}

/**
 * Use this function to find out where in the path
 * your point lies (at the given time).
 *
 * @param time is the time at which you wish to find the points location.
 * @return returns the location along the path at time.
 */
Point3 Path::getPoint(int time) {
    if (points.size() == 0) {
        return Point3(0,0,0);
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
        return points[0];
    } else if (timeLocation > endTime) {
        return points[points.size() - 1];
    } else {
        double numSteps = (double)timeLocation / timeStep;
        int beforeIndex = floor(numSteps);
        int afterIndex = ceil(numSteps);
        if (afterIndex >= points.size()) {
            afterIndex = 0;
        }
        return points[beforeIndex] + (points[afterIndex] - points[beforeIndex])*(numSteps - beforeIndex);
    }
}
