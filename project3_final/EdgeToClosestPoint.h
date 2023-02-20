//
// Created by nikos on 11/1/22.
//

#ifndef TOPOLYGON_EDGETOCLOSESTPOINT_H
#define TOPOLYGON_EDGETOCLOSESTPOINT_H



#include "utils.h"
class EdgeToClosestPoint {
private:
    Segment_2 segment;
    Point_2 closestPoint;
public:
    EdgeToClosestPoint(const Point_2&, const Point_2&, int, int);

    EdgeToClosestPoint();

    // Getters Setters
    const Segment_2& getSegment() const {return segment;};
    const Point_2& getClosestPoint() const {return closestPoint;};
};


#endif //TOPOLYGON_EDGETOCLOSESTPOINT_H
