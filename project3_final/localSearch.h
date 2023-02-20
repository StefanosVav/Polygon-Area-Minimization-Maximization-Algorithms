//
// Created by stefanos on 12/10/22.
//

#ifndef POLYGONIZATION_ALGORITHM_WITH_MIN_MAX_AREA_PICK_EDGE_UPDATE_POLYGON_CHAIN_LOCALSEARCH_H
#define POLYGONIZATION_ALGORITHM_WITH_MIN_MAX_AREA_PICK_EDGE_UPDATE_POLYGON_CHAIN_LOCALSEARCH_H
#include "incremental.h"

double long localSearch(Polygon_2, const string&);
void move_V_to_e(size_t, size_t, size_t, std::vector<Point_2> & v);
bool segIntPol(Segment_2, vector<Segment_2>);

#endif //POLYGONIZATION_ALGORITHM_WITH_MIN_MAX_AREA_PICK_EDGE_UPDATE_POLYGON_CHAIN_LOCALSEARCH_H
