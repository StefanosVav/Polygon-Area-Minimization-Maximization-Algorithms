//
// Created by stefanos on 12/10/22.
//

#ifndef POLYGONIZATION_ALGORITHM_WITH_MIN_MAX_AREA_PICK_EDGE_UPDATE_POLYGON_CHAIN_INCREMENTAL_H
#define POLYGONIZATION_ALGORITHM_WITH_MIN_MAX_AREA_PICK_EDGE_UPDATE_POLYGON_CHAIN_INCREMENTAL_H
#include "utils.h"

using namespace std;

Polygon_2 Incremental(string inputFile, string edge_selection);
Polygon_2 executeIncremental(vector<Point_2> p, string edge_selection);
bool isVisible(Segment_2 seg, Point_2 point, vector<Segment_2> segmentsPL);
bool points_by_x_asc(Point_2 p1, Point_2 p2);


#endif //POLYGONIZATION_ALGORITHM_WITH_MIN_MAX_AREA_PICK_EDGE_UPDATE_POLYGON_CHAIN_INCREMENTAL_H
