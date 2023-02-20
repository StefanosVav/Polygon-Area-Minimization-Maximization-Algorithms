//
// Created by nikos on 11/1/22.
//

#include "EdgeToClosestPoint.h"

EdgeToClosestPoint::EdgeToClosestPoint(const Point_2& p1, const Point_2& q1, int _x, int _y): segment(p1,q1),closestPoint(_x,_y) {}

EdgeToClosestPoint::EdgeToClosestPoint() {}
